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
* **Compiler**: GNU g++ compiler.
* **Standard**: C++17 is required for compilation.

### Compilation
To build the project, run the following command in your terminal:
```bash
g++ main.cpp Naive.cpp RabinKarp.cpp KMP.cpp BoyerMoore.cpp -std=c++17 -o crossword
```

### Usage
Run the tool using the following command-line flags:
```bash
./crossword -a [algorithm] -i [input_file] -o [output_file]
```
* **Algorithms(`-a`)**: `bf`, `rk`, `kmp`, `bm`, `ac`.
* **Input(`-i`)**: Path to the grid file (e.g., `puzzle.txt`).
* **Output(`-o`)**: Path to the result file (e.g., `solution.txt`).

## 📂 Directory Structure
```text
Group02/
├── main.cpp          # I/O handling and performance tracking
├── Algorithms.h      # Shared header for data structures and prototypes
├── Naive.cpp         # Brute-Force implementation
├── RabinKarp.cpp     # Rabin-Karp implementation
├── KMP.cpp           # KMP implementation
├── BoyerMoore.cpp    # Boyer-Moore implementation
├── puzzle.txt        # Folder containing input cases
├── solution.txt        # Folder containing output cases
└── README.md         # Documentation and project guide
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