#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <vector>
#include <string>
#include <queue>
#include <algorithm>

// Cấu trúc lưu tọa độ kết quả
struct Occurrence {
    int r_start, c_start, r_end, c_end;
};

// Cấu trúc dữ liệu cho Grid và kết quả search
struct ProjectData {
    int R, C, K;
    std::vector<std::vector<char>> grid;
    std::vector<std::string> keywords;
    std::string algorithm;
};

// Khai báo các thuật toán hàm
void searchNaive(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons);
void searchRK(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons);
void searchKMP(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons);
void searchBM(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons);
void searchAho(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons);

#endif