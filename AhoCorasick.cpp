#include "Algorithms.h"

struct AhoNode {
    int next[26];
    int fail;
    bool is_end;
    
    AhoNode() {
        for(int i = 0; i < 26; ++i) next[i] = -1;
        fail = -1;
        is_end = false;
    }
};

void buildAhoAutomaton(const std::string& word, std::vector<AhoNode>& trie) {
    trie.clear();
    trie.emplace_back(); // Khởi tạo Root Node (index 0)

    // Xây dựng Trie
    int current = 0;
    for (char c : word) {
        int char_idx = c - 'a'; // Giả định grid và word đều là chữ thường
        if (trie[current].next[char_idx] == -1) {
            trie[current].next[char_idx] = trie.size();
            trie.emplace_back();
        }
        current = trie[current].next[char_idx];
    }
    trie[current].is_end = true;

    // Xây dựng Failure Links bằng BFS
    std::queue<int> q;
    for (int i = 0; i < 26; i++) {
        if (trie[0].next[i] != -1) {
            trie[trie[0].next[i]].fail = 0;
            q.push(trie[0].next[i]);
        } else {
            trie[0].next[i] = 0;
        }
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int i = 0; i < 26; i++) {
            if (trie[u].next[i] != -1) {
                int v = trie[u].next[i];
                int fail_state = trie[u].fail;
                
                while (fail_state != 0 && trie[fail_state].next[i] == -1) {
                    fail_state = trie[fail_state].fail;
                }
                
                if (trie[fail_state].next[i] != -1) {
                    trie[v].fail = trie[fail_state].next[i];
                } else {
                    trie[v].fail = 0;
                }
                
                q.push(v);
            }
        }
    }
}

// Hàm quét Automaton trên một đường thẳng (tia) của ma trận
void searchStreamAho(const ProjectData& data, const std::string& word, const std::vector<AhoNode>& trie, 
                     int start_r, int start_c, int dr, int dc, 
                     std::vector<Occurrence>& results, long long& comparisons) {
    int r = start_r, c = start_c;
    int current_state = 0;
    int len = word.length();

    while (r >= 0 && r < data.R && c >= 0 && c < data.C) {
        int char_idx = data.grid[r][c] - 'a';
        comparisons++; // Đếm phép so sánh trên lưới

        // Nhảy fail link nếu không khớp
        while (current_state != 0 && trie[current_state].next[char_idx] == -1) {
            current_state = trie[current_state].fail;
            comparisons++; // Đếm thêm phép so sánh khi quay lui
        }

        if (trie[current_state].next[char_idx] != -1) {
            current_state = trie[current_state].next[char_idx];
        } else {
            current_state = 0;
        }

        // Nếu chạm state kết thúc (tìm thấy từ)
        if (trie[current_state].is_end) {
            int start_match_r = r - (len - 1) * dr;
            int start_match_c = c - (len - 1) * dc;
            results.push_back({start_match_r, start_match_c, r, c});
        }

        r += dr;
        c += dc;
    }
}

void searchAho(const ProjectData& data, const std::string& word, std::vector<Occurrence>& results, long long& comparisons) {
	if (word.empty() || data.R == 0 || data.C == 0) return;

    // Khởi tạo và xây dựng cỗ máy trạng thái cho từ khóa
    std::vector<AhoNode> trie;
    buildAhoAutomaton(word, trie);

    // Quét ngang: Trái -> Phải
    for (int i = 0; i < data.R; i++) {
        searchStreamAho(data, word, trie, i, 0, 0, 1, results, comparisons); // Trái -> Phải
    }

    // Quét dọc: Trên -> Dưới
    for (int j = 0; j < data.C; j++) {
        searchStreamAho(data, word, trie, 0, j, 1, 0, results, comparisons); // Trên -> Dưới
    }
}