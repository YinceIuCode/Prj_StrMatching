#include "Algorithms.h"
#include <iostream>
#include <fstream>
#include <chrono>

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

void writeOutput(ofstream& out, const ProjectData& data, const std::vector<std::vector<Occurrence>> all_results, const long long total_comparisons, chrono::duration<double, std::milli> duration, const string alg) {
    for (int i = 0; i < data.K; ++i) {
        out << data.keywords[i] << ": ";
        
        if (all_results[i].empty()) {
            out << "not found" << endl;
        } else {
            for (size_t j = 0; j < all_results[i].size(); ++j) {
                out << "(" << all_results[i][j].r_start << "," << all_results[i][j].c_start << ") -> "
                    << "(" << all_results[i][j].r_end << "," << all_results[i][j].c_end << ")";
                if (j < all_results[i].size() - 1) out << "; ";
            }
            out << ";" << endl;
        }
    }

    out << "\n------------------------------\n";
    out << "Algorithm: " << alg << endl;
    out << "Comparisons: " << total_comparisons << endl;
    out << "Execution Time: " << duration.count() << " ms" << endl;
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
    if (!readInput(input_file, data)) {
        cerr << "Error: Cannot open input file!" << endl;
        return 1;
    }

    ofstream out(output_file);
    if (!out.is_open()) {
        cerr << "Error: Cannot open output file!" << endl;
        return 1;
    }

    long long total_comparisons = 0; 
    
    // Khởi tạo sẵn mảng 2 chiều lưu kết quả cho K từ khóa
    vector<vector<Occurrence>> all_results(data.K);
    
    // BẮT ĐẦU ĐO THỜI GIAN KHỐI XỬ LÝ LOGIC
    auto start_time = chrono::high_resolution_clock::now();
    
    if (alg == "bf") {
        searchNaive(data, all_results, total_comparisons);
    }
    else if (alg == "kmp") {
        searchKMP(data, all_results, total_comparisons);
    }
    else if (alg == "bm") {
        searchBM(data, all_results, total_comparisons);
    }
    else if (alg == "rk") {
        searchRK(data, all_results, total_comparisons);
    }
    else if (alg == "aho") {
        searchAho(data, all_results, total_comparisons);
    }
    
    // KẾT THÚC ĐO THỜI GIAN
    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end_time - start_time;

    // IN KẾT QUẢ RA FILE
    // for (int i = 0; i < data.K; ++i) {
    //     out << data.keywords[i] << ": ";
        
    //     if (all_results[i].empty()) {
    //         out << "not found" << endl;
    //     } else {
    //         for (size_t j = 0; j < all_results[i].size(); ++j) {
    //             out << "(" << all_results[i][j].r_start << "," << all_results[i][j].c_start << ") -> "
    //                 << "(" << all_results[i][j].r_end << "," << all_results[i][j].c_end << ")";
    //             if (j < all_results[i].size() - 1) out << "; ";
    //         }
    //         out << ";" << endl;
    //     }
    // }

    // // In thông số Performance
    // out << "\n------------------------------\n";
    // out << "Algorithm: " << alg << endl;
    // out << "Comparisons: " << total_comparisons << endl;
    // out << "Execution Time: " << duration.count() << " ms" << endl;

    writeOutput(out, data, all_results, total_comparisons, duration, alg);

    return 0;
}