#include "Algorithms.h"

void searchNaive(const ProjectData& data, const string& word, vector<Occurrence>& results, long long& comparisons) {
    int len = word.length();

    for (int r = 0; r < data.R; ++r) {
        for (int c = 0; c < data.C; ++c) {
            
            // 1. Kiểm tra hướng Ngang (Left-to-Right) [cite: 21]
            if (c + len <= data.C) {
                bool match = true;
                for (int k = 0; k < len; ++k) {
                    comparisons++; // Đếm phép so sánh [cite: 56-57]
                    if (data.grid[r][c + k] != word[k]) {
                        match = false;
                        break;
                    }
                }
                if (match) results.push_back({r, c, r, c + len - 1});
            }

            // 2. Kiểm tra hướng Dọc (Top-to-Bottom) [cite: 21]
            if (r + len <= data.R) {
                bool match = true;
                for (int k = 0; k < len; ++k) {
                    comparisons++; // Đếm phép so sánh [cite: 56-57]
                    if (data.grid[r + k][c] != word[k]) {
                        match = false;
                        break;
                    }
                }
                if (match) results.push_back({r, c, r + len - 1, c});
            }
        }
    }
}