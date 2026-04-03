import os
import subprocess
import sys

def install_and_import(package_name, import_name):
    try:
        __import__(import_name)
    except ImportError:
        print(f"Đang cài đặt '{package_name}'...")
        subprocess.check_call([sys.executable, "-m", "pip", "install", package_name])

install_and_import('matplotlib', 'matplotlib')
install_and_import('numpy', 'numpy')

import matplotlib.pyplot as plt
import os
import re

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

if os.path.basename(SCRIPT_DIR) == "scripts":
	PROJECT_ROOT = os.path.dirname(SCRIPT_DIR)
else:
	PROJECT_ROOT = SCRIPT_DIR

REPORT_DIR = os.path.join(PROJECT_ROOT, "reports")
REPORT_FILE = os.path.join(REPORT_DIR, "benchmark.txt")

ALG_NAMES = {
    "bf": "Brute Force",
    "kmp": "Knuth-Morris-Pratt",
    "bm": "Boyer-Moore",
    "rk": "Rabin-Karp",
    "aho": "Aho-Corasick"
}

def plot_charts():
	if not os.path.exists(REPORT_FILE):
		print(f"❌ Không tìm thấy file {REPORT_FILE}. Bạn hãy chạy auto_run.py trước!")
		return

	print("📊 Đang đọc dữ liệu từ file báo cáo...")

	# Cấu trúc lưu dữ liệu: { "Thuật Toán": {"x": [], "y": []} }
	sc1_data = {}
	sc2_data = {}

	current_scenario = 0
	current_x = ""

	with open(REPORT_FILE, 'r', encoding = "utf-8") as f:
		for line in f:
			line = line.strip()
			if not line: continue

			if line == "Scenario 1":
				current_scenario = 1
				continue
			elif line == "Scenario 2":
				current_scenario = 2
				continue

			if "|" in line:
				parts = line.split("|")
				alg_code = parts[0].strip().lower()
				alg_name = ALG_NAMES.get(alg_code, alg_code.upper())

				metrics = parts[1].split(",")
				time_str = metrics[0].strip()
				if time_str == "N/A":
					continue

				time_ms = float(time_str)

				if current_scenario == 1:
					if alg_name not in sc1_data: sc1_data[alg_name] = {'x': [], 'y': []}
					sc1_data[alg_name]['x'].append(current_x)
					sc1_data[alg_name]['y'].append(time_ms)
				elif current_scenario == 2:
					if alg_name not in sc2_data: sc2_data[alg_name] = {'x': [], 'y': []}
					sc2_data[alg_name]['x'].append(current_x)
					sc2_data[alg_name]['y'].append(time_ms)
			else:
				if current_scenario == 1:
					dims = line.split()
					if len(dims) == 2:
						current_x = f"{dims[0]}x{dims[1]}"
				elif current_scenario == 2:
					current_x = line

	def draw_plot(data, title, xlabel, output_file):
		if not data: return

		if not os.path.exists(REPORT_DIR):
			os.makedirs(REPORT_DIR)

		plt.figure(figsize = (10, 6))

		for alg, coords in data.items():
			plt.plot(coords['x'], coords['y'], marker = 'o', linewidth = 2.5, markersize = 8, label = alg)

		plt.title(title, fontsize = 16, fontweight = 'bold', pad = 15)
		plt.xlabel(xlabel, fontsize=12, fontweight='bold')
		plt.ylabel("Execution Time (ms)", fontsize=12, fontweight='bold')
		plt.yscale('log')
		plt.grid(True, linestyle='--', alpha=0.6)
		plt.legend(fontsize=11, loc="upper left")

		plt.tight_layout()

		save_path = os.path.join(REPORT_DIR, output_file)
		plt.savefig(save_path, dpi = 300)
		print(f"✅ Đã xuất biểu đồ: {save_path}")

	draw_plot(sc1_data, "Scenario 1: Impact of Grid Size on Execution Time", "Grid Size (N x N)", "chart_scenario1_time.png")
	draw_plot(sc2_data, "Scenario 2: Impact of Dictionary Size on Execution Time", "Number of Keywords (K)", "chart_scenario2_time.png")

if __name__ == "__main__":
	plot_charts()