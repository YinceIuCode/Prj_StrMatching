#include "Algorithms.h"

std::vector<int> computeLPS(std::string pattern)
{
    int n = (int)pattern.size();
    int j = 0;
    std::vector LPS(n, 0);

    for (int i = 1; i < n; ++i)
    {
        while (j > 0 && pattern[j] != pattern[i])
        {
            j = LPS[j - 1];
        }
        if (pattern[i] == pattern[j])
        {
            ++j;
            LPS[i] = j;
        }
    }
    return LPS;
}

void searchKMP(const ProjectData &data, std::vector<std::vector<Occurrence>> &all_results, long long &comparisons)
{
    std::string text;

    // Row:
    for (int r = 0; r < data.R; r++)
    {
        text = "";
        for (int c = 0; c < data.C; c++)
        {
            text += data.grid[r][c];
        }

        // Traversal through each keyword
        for (int k = 0; k < data.K; ++k)
        {
            std::string pattern = data.keywords[k];
            std::vector <int> LPS = computeLPS (pattern);

            int j = 0;
            for (int i = 0; i < (int)text.size(); ++i)
            {
                while (j > 0 && pattern[j] != text[i]){
                    j = LPS[j - 1];
                    comparisons++;
                }
                    comparisons++;
                if (pattern[j] == text[i])
                    ++j;

                if (j == (int)pattern.size())
                {
                    all_results[k].push_back({r, i - j + 1, r, i});
                    j = 0;
                }
            }
        }
    }


    // Col
    for (int c = 0; c < data.C; c++)
    {
        text = "";
        for (int r = 0; r < data.R; r++)
        {
            text += data.grid[r][c];
        }

        // Traversal through each keyword
        for (int k = 0; k < data.K; ++k)
        {
            std::string pattern = data.keywords[k];
            std::vector <int> LPS = computeLPS (pattern);

            int j = 0;
            for (int i = 0; i < (int)text.size(); ++i)
            {
                while (j > 0 && pattern[j] != text[i]){
                    j = LPS[j - 1];
                    ++comparisons;
                }
                ++comparisons;
                if (pattern[j] == text[i])
                    ++j;

                if (j == (int)pattern.size())
                {
                    all_results[k].push_back({i - j + 1, c, i, c});
                    j = 0;
                }
            }
        }
    }
}