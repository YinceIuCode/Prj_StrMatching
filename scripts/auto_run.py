import subprocess
import os
import re
import time
import random
import string
import urllib.request

ALG_CONFIG = {
    "bf":  {"file": "BruteForce.cpp",        "func": "searchNaive"},
    "kmp": {"file": "KnuthMorrisPratt.cpp",  "func": "searchKMP"},
    "bm":  {"file": "BoyerMoore.cpp",        "func": "searchBM"},
    "rk":  {"file": "RabinKarp.cpp",         "func": "searchRK"},
    "aho": {"file": "AhoCorasick.cpp",   	 "func": "searchAho"}
}

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPT_DIR)

SRC_DIR = os.path.join(PROJECT_ROOT, "src")
INPUT_DIR = os.path.join(PROJECT_ROOT, "inputs")
OUTPUT_DIR = os.path.join(PROJECT_ROOT, "outputs")

EXECUTABLE = os.path.join(PROJECT_ROOT, "crossword.exe" if os.name == "nt" else "./crossword")
REPORT_DIR = os.path.join(PROJECT_ROOT, "reports")
REPORT_FILE = os.path.join(REPORT_DIR, "benchmark.txt")

SCENARIO_1_GRIDS = [10, 30, 50, 100, 300, 500]
SCENARIO_1_K = 3

SCENARIO_2_K = [10, 30, 50, 100, 300, 500]
SCENARIO_2_GRIDS = 100

WORD_LIST_FILE = os.path.join(PROJECT_ROOT, "english_words.txt")
WORD_LIST_URL = "https://raw.githubusercontent.com/first20hours/google-10000-english/master/google-10000-english-no-swears.txt"

def load_english_dictionary():
	if not os.path.exists(WORD_LIST_FILE):
		print("⬇️ Đang tải thư viện 10.000 từ vựng tiếng Anh (chỉ tải 1 lần duy nhất)...")
		try:
			urllib.request.urlretrieve(WORD_LIST_URL, WORD_LIST_FILE)
			print("✅ Tải từ điển thành công!\n")
		except Exception as e:
			print(f"❌ Lỗi khi tải từ điển: {e}. Vui lòng kiểm tra mạng.")
			return ["error", "network", "offline", "fallback", "words"]

	with open(WORD_LIST_FILE, 'r', encoding = "utf-8") as f:
		words = [line.strip() for line in f if len(line.strip()) >= 4]

	return words

ENGLISH_DICT = load_english_dictionary()

def generate_random_keywords(k):
	actual_k = min(k, len(ENGLISH_DICT))
	return random.sample(ENGLISH_DICT, actual_k)

def generate_puzzle(rows, cols, keywords, filepath):
	grid = [[random.choice(string.ascii_lowercase) for _ in range(cols)] for _ in range(rows)]

	PLANT_RATIO = 1.0
	num_plant = max(1, int(len(keywords) * PLANT_RATIO))
	words_to_plant = random.sample(keywords, num_plant)
	base_size = min(rows, cols)
	max_k = max(2, int(base_size * 0.1))

	for word in words_to_plant:
		if len(word) > min(rows, cols): continue

		occurrences = random.randint(1, max_k)

		for _ in range(occurrences):
			direction = random.choice(["horizontal", "vertical"])
			word_len = len(word)

			if direction == "horizontal" and cols >= word_len:
				r = random.randint(0, rows - 1)
				c = random.randint(0, cols - word_len)
				for i in range(word_len): grid[r][c + i] = word[i]

			elif direction == "vertical" and rows >= word_len:
				r = random.randint(0, rows - word_len)
				c = random.randint(0, cols - 1)
				for i in range(word_len): grid[r + i][c] = word[i]

	with open(filepath, 'w', encoding = "utf-8") as f:
		f.write(f"{rows} {cols}\n")
		for row in grid:
			f.write(" ".join(row) + "\n")
		f.write(f"{len(keywords)}\n")
		for word in keywords:
			f.write(word + "\n")

def parse_results(filepath):
	time_ms, comparisons = "N/A", "N/A"
	try:
		with open(filepath, 'r', encoding = "utf-8") as f:
			content = f.read()
			comp_match = re.search(r"Comparisons:\s*(\d+)", content)
			time_match = re.search(r"Execution Time:\s*([\d.]+)", content)

			if comp_match: comparisons = comp_match.group(1)
			if time_match: time_ms = time_match.group(1)
	except Exception:
		pass
	return time_ms, comparisons

def is_implemented(alg):
	config = ALG_CONFIG.get(alg)
	if not config: return False

	filepath = os.path.join(SRC_DIR, config["file"])
	func_name = config["func"]

	try:
		with open(filepath, 'r', encoding = 'utf-8') as f:
			content = f.read()

		start_idx = content.find(func_name)
		if start_idx == -1: return False

		brace_start = content.find('{', start_idx)
		if brace_start == -1: return False

		brace_count = 1
		i = brace_start + 1
		while i < len(content) and brace_count > 0:
			if content[i] == '{': brace_count += 1
			elif content[i] == '}': brace_count -= 1
			i += 1

		body = content[brace_start + 1 : i - 1]
		body = re.sub(r'//.*', '', body)
		body = re.sub(r'/\*[\s\S]*?\*/', '', body)

		return len(body.strip()) > 0
	except FileNotFoundError:
		return False

def compile_cpp():
	print("⏳ Đang biên dịch mã nguồn C++...");

	cpp_files = [os.path.join(SRC_DIR, "main.cpp")]
	for c in ALG_CONFIG.values():
		filepath = os.path.join(SRC_DIR, c["file"])
		if os.path.exists(filepath):
			cpp_files.append(filepath)

	print(f"📁 Các file tìm thấy: {', '.join(cpp_files)}")

	compile_cmd = ["g++"] + cpp_files + ["-o", EXECUTABLE.replace("./", "")]

	try:
		result = subprocess.run(compile_cmd, capture_output = True, text = True)
		if result.returncode == 0:
			print("✅ Biên dịch thành công!\n")
			return True
		else:
			print("❌ Lỗi biên dịch:")
			print(result.stderr)
			return False
	except FileNotFoundError:
   		print("❌ Không tìm thấy trình biên dịch 'g++'. Hãy chắc chắn bạn đã cài đặt MinGW hoặc GCC.")
   		return False

def run_benchmark():
	if not os.path.exists(INPUT_DIR): os.makedirs(INPUT_DIR)
	if not os.path.exists(OUTPUT_DIR): os.makedirs(OUTPUT_DIR)
	if not os.path.exists(REPORT_DIR): os.makedirs(REPORT_DIR)

	valid_algs = [alg for alg in ALG_CONFIG.keys() if is_implemented(alg)]
	if not valid_algs:
		print("⚠️ Không có thuật toán nào được triển khai (các hàm đang trống)!")
		return

	print("🚀 BẮT ĐẦU CHẠY BENCHMARK...")

	with open(REPORT_FILE, 'w', encoding = 'utf-8') as report:

		# --- SCENARIO 1 ---
		print("\n--- Chạy Scenario 1 (Impact of Grid Size) ---")
		report.write("Scenario 1\n")

		keywords = generate_random_keywords(SCENARIO_1_K)

		for size in SCENARIO_1_GRIDS:
			print(f" ⚙️ Đang xử lý Grid: {size}x{size} ...")
			report.write(f"{size} {size}\n")

			input_file = os.path.join(INPUT_DIR, f"puzzle_S1_{size}x{size}.txt")
			generate_puzzle(size, size, keywords, input_file)

			for alg in valid_algs:
				out_file = os.path.join(OUTPUT_DIR, f"solution_{alg}_S1_{size}x{size}.txt")
				subprocess.run([EXECUTABLE, "-a", alg, "-i", input_file, "-o", out_file])

				time_ms, comps = parse_results(out_file)
				report.write(f"{alg} | {time_ms}, {comps}\n")

		# --- SCENARIO 2 ---
		print("\n--- Chạy Scenario 2 (Impact of Dictionary Size) ---")
		report.write("Scenario 2\n")

		for k in SCENARIO_2_K:
			print(f" ⚙️ Đang xử lý Keywords: {k} từ ...")
			report.write(f"{k}\n")

			keywords = generate_random_keywords(k)

			input_file = os.path.join(INPUT_DIR, f"puzzle_S2_k{k}.txt")
			generate_puzzle(SCENARIO_2_GRIDS, SCENARIO_2_GRIDS, keywords, input_file)

			for alg in valid_algs:
				out_file = os.path.join(OUTPUT_DIR, f"solution_{alg}_S2_k{k}.txt")
				subprocess.run([EXECUTABLE, "-a", alg, "-i", input_file, "-o", out_file])

				time_ms, comps = parse_results(out_file)
				report.write(f"{alg} | {time_ms}, {comps}\n")

	print("\n🎉 HOÀN THÀNH BENCHMARK!")
	print(f"📄 Kết quả tổng hợp đã lưu tại: {REPORT_FILE}")
	print(f"📂 Xem các file ma trận test case trong thư mục: {INPUT_DIR}/")

if __name__ == "__main__":
	if compile_cpp():
		run_benchmark()