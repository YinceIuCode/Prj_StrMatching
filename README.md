# 🧩 DSA Project: 2D String Matching Tool
### Group 02 - Class 25C11 | VNU-HCM University of Science

## 📝 Project Overview
This project is a command-line application designed to solve the **String Matching** problem in a two-dimensional grid of characters. The tool identifies keywords within a grid by applying various fundamental and advanced algorithms, supporting both Horizontal and Vertical search directions.

### Key Features:
* **Search Directions**: Horizontal (Left-to-Right) and Vertical (Top-to-Bottom).
* **Core Algorithms**:
    * **Naïve (Brute-Force)**: The standard baseline approach.
    * **Rabin-Karp**: Hashing-based pattern matching.
    * **Knuth-Morris-Pratt (KMP)**: Optimized matching using the Failure Function.
    * **Boyer-Moore**: Efficient matching using shift rules.
* **Bonus (Advanced)**: Implementation of the **Aho-Corasick** algorithm for multi-pattern matching.

---

## 🚀 Getting Started

### Prerequisites
* **C++ Compiler**: GNU `g++` compiler (C++17 standard is required).
* **Python Environment** *(For Automated Tools)*: Python 3.x installed.
* **Python Libraries** *(For Plotting)*: `pip install matplotlib`

## 🛠️ Method 1: Manual Execution (C++ CLI)
Use this method if you want to manually test a specific puzzle file with a specific algorithm.

**1. Compilation**
Since the source files are neatly organized in the `src/` folder, run the following command in the root directory to compile the project:
```bash
g++ src/main.cpp src/BruteForce.cpp src/RabinKarp.cpp src/KMP.cpp src/BoyerMoore.cpp src/AhoCorasick.cpp -o crossword -std=c++17
```

**2. Usage**
Run the tool using the following command-line flags:
```bash
./crossword -a [algorithm] -i [input_file] -o [output_file]
```
* **Algorithms(`-a`)**: `bf`, `rk`, `kmp`, `bm`, `aho`.
* **Input(`-i`)**: Path to the grid file (e.g., `inputs/puzzle.txt`).
* **Output(`-o`)**: Path to the result file (e.g., `outputs/solution.txt`).

## 🤖 Method 2: Automated Benchmarking Tool (Python)
Use this method to automatically generate massive test cases, run all algorithms, and generate performance charts. **You do NOT need to compile the C++ code manually; the script handles it.**

**1. Run the Benchmark Script**
This script will auto-compile the C++, download an English dictionary, generate random 2D grids, and run multiple scenarios (varying Grid Size and Dictionary Size).
```bash
python scripts/auto_run.py
```
*After running, check the `inputs/` and `outputs/` folders for the generated files, and `reports/benchmark.txt` for the raw data.*

**2. Generate Performance Charts**
Once the benchmark is complete, run the plotting script to visualize the execution times on a Logarithmic scale:
```bash
python scripts/plot_charts.py
```
*The generated charts (`.png`) will be saved in the `reports/` folder.*

## 📂 Directory Structure
After use tool python
```text
Group02/
├── inputs/             # Auto-generated test cases (puzzle files)
├── outputs/            # Algorithm output results
├── reports/            # Benchmark raw data (.txt) and generated charts (.png)
├── scripts/            # Python automation tools
│   ├── auto_run.py     # Auto-compiler & Benchmarker
│   └── plot_charts.py  # Data visualization script
├── src/                # C++ Source Code
│   ├── main.cpp        # I/O handling and performance tracking
│   ├── Algorithms.h    # Shared header for data structures
│   ├── BruteForce.cpp  # Brute-Force implementation
│   ├── RabinKarp.cpp   # Rabin-Karp implementation
│   ├── KMP.cpp         # KMP implementation
│   ├── BoyerMoore.cpp  # Boyer-Moore implementation
│   └── AhoCorasick.cpp # Aho-Corasick implementation
├── english_words.txt   # Downloaded 10,000 English word dictionary
├── crossword.exe       # Compiled executable
└── README.md           # Project documentation
```

## 📊 Performance Metrics
The tool reports the following mandatory metrics at the end of each search:
1. **Comparisons**: Exact count of character-to-character comparisons (e.g., `if (grid[r][c] == word[k])`).
2. **Execution time**: The search algorithm duration measured in milliseconds (ms).

## 👥 Contributors (Group 02)
| Student Name | Student ID | Main Contribution |
| :--- | :--- | :--- |
| **Hoàng Đức Vinh** | 25127553 | Leader, Framework, Final Review, Aho-Corasick |
| **Võ Lê Hoàng** | 25127193 | Naïve Algorithm |
| **Đỗ Nguyễn Minh Trung** | 25127162 | Rabin-Karp Algorithm |
| **Trần Ngọc Nghĩa** | 25127431 | KMP Algorithm |
| **Nguyễn Duy Khang** | 25127367 | Boyer-Moore Algorithm |
