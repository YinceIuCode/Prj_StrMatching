#include "Algorithms.h"

void searchNaive(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons) {
    all_results.clear();
    all_results.resize(data.K);
    comparisons = 0;
    for (int k = 0; k < data.K; ++k) {
        const std::string& word = data.keywords[k];
        int len = word.length();
        if (len == 0) continue;

        // 1. Tìm kiếm theo chiều NGANG (Horizontal: Left-to-Right)
        for (int r = 0; r < data.R; ++r) {
            for (int c = 0; c <= data.C - len; ++c) {
                bool match = true;
                for (int i = 0; i < len; ++i) {
                    comparisons++;
                    if (data.grid[r][c + i] != word[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    all_results[k].push_back({r, c, r, c + len - 1});
                }
            }
        }
        // 2. Tìm kiếm theo chiều DỌC (Vertical: Top-to-Bottom)
        for (int c = 0; c < data.C; ++c) {
            for (int r = 0; r <= data.R - len; ++r) {
                bool match = true;
                for (int i = 0; i < len; ++i) {
                    comparisons++;
                    if (data.grid[r + i][c] != word[i]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    all_results[k].push_back({r, c, r + len - 1, c});
                }
            }
        }
    }
}
