import subprocess
import os
import time
import re

EXECUTABLE = "crossword.exe" if os.name == "nt" else "./crossword"
INPUT_FILE = "puzzle.txt"
OUTPUT_DIR = "outputs"

ALG_CONFIG = {
    "bf":  {"file": "Naive.cpp",             "func": "searchNaive"},
    "kmp": {"file": "KMP.cpp",               "func": "searchKMP"},
    "bm":  {"file": "BoyerMoore.cpp",        "func": "searchBM"},
    "rk":  {"file": "RabinKarp.cpp",         "func": "searchRK"},
    "aho": {"file": "AhoCorasickFull.cpp",   "func": "searchAho"}
}

def is_implemented(alg):
	config = ALG_CONFIG.get(alg)
	if not config: return False

	filepath = config["file"]
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

	cpp_files_to_compile = ["main.cpp"]
	for config in ALG_CONFIG.values():
		if os.path.exists(config["file"]):
			cpp_files_to_compile.append(config["file"])

	print(f"📁 Các file tìm thấy: {', '.join(cpp_files_to_compile)}")

	compile_cmd = ["g++"] + cpp_files_to_compile + ["-o", EXECUTABLE.replace("./", "")]

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

def run_tests():
	if not os.path.exists(OUTPUT_DIR):
		os.makedirs(OUTPUT_DIR)

	if not os.path.exists(INPUT_FILE):
		print(f"❌ Không tìm thấy file đầu vào: {INPUT_FILE}")
		return
	
	print(f"🚀 Bắt đầu chạy test với file: {INPUT_FILE}")
	print("-" * 50)

	valid_algs = [alg for alg in ALG_CONFIG.keys() if is_implemented(alg)]
	skipped_algs = [alg for alg in ALG_CONFIG.keys() if alg not in valid_algs]

	if skipped_algs:
		print(f"⏭️  Bỏ qua các hàm chưa code (hoặc chưa tạo file): {', '.join([alg.upper() for alg in skipped_algs])}\n")

	if not valid_algs:
		print("⚠️ Không có thuật toán nào được triển khai. Vui lòng code ít nhất 1 hàm!")
		return

	for alg in valid_algs:
		output_file = os.path.join(OUTPUT_DIR, f"solution_{alg}.txt")
		run_cmd = [EXECUTABLE, "-a", alg, "-i", INPUT_FILE, "-o", output_file]

		print(f"▶️ Đang chạy [{alg.upper()}]...")
		subprocess.run(run_cmd)
		print(f"   ↳ Lưu kết quả tại: {output_file}")

	print("-" * 50)
	print("🎉 TẤT CẢ TEST CASE ĐÃ CHẠY XONG!")

if __name__ == "__main__":
	if compile_cpp():
		run_tests()