#include "Algorithms.h"

struct State {
	int next[26];
	int fail;
	std::vector<int> output;

	State() {
		std::fill(next, next + 26, -1);
		fail = -1;
	}
};

void buildAhoCorasick(const std::vector<std::string>& keywords, std::vector<State>& states) {
	states.clear();
	states.emplace_back();

	// Step 1: Build Trie
	for (int i = 0; i < keywords.size(); i++) {
		int current = 0;
		for (char c : keywords[i]) {
			int char_idx = c - 'a';
			if (states[current].next[char_idx] == -1) {
				states[current].next[char_idx] = states.size();
				states.emplace_back();
			}
			current = states[current].next[char_idx];
		}
		states[current].output.push_back(i);
	}

	// Step 2: Build Failure Links (BFS)
	std::queue<int> q;
	for (int i = 0; i < 26; i++) {
		if (states[0].next[i] != -1) {
			states[states[0].next[i]].fail = 0;
			q.push(states[0].next[i]);
		} else {
			states[0].next[i] = 0;
		}
	}

	while (!q.empty()) {
		int u = q.front();
		q.pop();

		for (int i = 0; i < 26; i++) {
			if (states[u].next[i] != -1) {
				int v = states[u].next[i];
				int fail_state = states[u].fail;

				while (fail_state != 0 && states[fail_state].next[i] == -1) {
					fail_state = states[fail_state].fail;
				}

				if (states[fail_state].next[i] != -1) {
					states[v].fail = states[fail_state].next[i];
				} else {
					states[v].fail = 0;
				}

				for (int out_idx : states[states[v].fail].output) {
					states[v].output.push_back(out_idx);
				}

				q.push(v);
			}
		}
	}
}

void searchStream(const ProjectData& data, const std::vector<State>& states,
				  int start_r, int start_c, int dr, int dc,
				  std::vector<std::vector<Occurrence>>& all_results, long long& comparisons) {
	int r = start_r, c = start_c;
	int current_state = 0;

	while (r >= 0 && r < data.R && c >= 0 && c < data.C) {
		int char_idx = data.grid[r][c] - 'a';
		comparisons++;

		while (current_state != 0 && states[current_state].next[char_idx] == -1) {
			current_state = states[current_state].fail;
			comparisons++;
		}

		if (states[current_state].next[char_idx] != -1) {
			current_state = states[current_state].next[char_idx];
		} else {
			current_state = 0;
		}

		for (int word_idx : states[current_state].output) {
			int len = data.keywords[word_idx].length();
			int start_match_r = r - (len - 1) * dr;
			int start_match_c = c - (len - 1) * dc;
			all_results[word_idx].push_back({start_match_r, start_match_c, r, c});
		}

		r += dr;
		c += dc;
	}
}

void searchAho(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons) {
	if (data.R == 0 || data.C == 0 || data.keywords.empty()) return;

	all_results.assign(data.K, std::vector<Occurrence>());

	std::vector<State> states;
	buildAhoCorasick(data.keywords, states);

	for (int i = 0; i < data.R; i++) {
		searchStream(data, states, i, 0, 0, 1, all_results, comparisons);
	}
	for (int j = 0; j < data.C; j++) {
		searchStream(data, states, 0, j, 1, 0, all_results, comparisons);
	}
}