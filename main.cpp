#include <iostream>
#include <fstream>
#include <chrono>
#include "Algorithms.h"

using namespace std;

// Hàm đọc file puzzle.txt
bool readInput(const string& filename, ProjectData& data) {
    ifstream file(filename);
    if (!file.is_open()) return false;

    file >> data.R >> data.C;
    data.grid.resize(data.R, vector<char>(data.C));
    for (int i = 0; i < data.R; ++i) {
        for (int j = 0; j < data.C; ++j) {
            file >> data.grid[i][j];
        }
    }

    file >> data.K;
    data.keywords.resize(data.K);
    for (int i = 0; i < data.K; ++i) {
        file >> data.keywords[i];
    }
    return true;
}

int main(int argc, char* argv[]) {
    // Xử lý tham số dòng lệnh: -a [alg] -i [input] -o [output]
    string input_file, output_file, alg;
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "-a") alg = argv[++i];
        else if (arg == "-i") input_file = argv[++i];
        else if (arg == "-o") output_file = argv[++i];
    }

    ProjectData data;
    if (!readInput(input_file, data)) return 1;

    ofstream out(output_file);

    // Trong thuật toán có phép so sánh thì thêm dòng total_comparisons++ vào
    long long total_comparisons = 0; 
    
    auto start_time = chrono::high_resolution_clock::now();
    
    for (const string& word : data.keywords) {
        out << word << ": ";
        vector<Occurrence> results; 
        
        if (alg == "bf") {
            searchNaive(data, word, results, total_comparisons);
        }
        else if (alg == "kmp") {
            searchKMP(data, word, results, total_comparisons);
        }
        else if (alg == "bm") {
            searchBM(data, word, results, total_comparisons);
        }
        else if (alg == "rk") {
            searchRK(data, word, results, total_comparisons);
        }
        
        if (results.empty()) {
            out << "not found" << endl;
        } else {
            for (size_t i = 0; i < results.size(); ++i) {
                out << "(" << results[i].r_start << "," << results[i].c_start << ") -> "
                    << "(" << results[i].r_end << "," << results[i].c_end << ")";
                if (i < results.size() - 1) out << "; ";
            }
            out << ";" << endl;
        }
    }
    // ---------------------------------------------------

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

    // In thông số Performance
    out << "\n------------------------------\n";
    out << "Algorithm: " << alg << endl;
    out << "Comparisons: " << total_comparisons << endl;
    out << "Execution Time: " << (double)duration.count() << " ms" << endl;

    return 0;
}