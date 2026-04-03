#include "Algorithms.h"

using namespace std;

void BadCharacterRule(string pattern, int BadCharacterHeuristic[], int pattern_size){
    for(int i = 0; i < 256; i++){
        BadCharacterHeuristic[i] = pattern_size;
    }
    for(int i = 0; i < pattern_size-1; i++){
        BadCharacterHeuristic[(unsigned char)pattern[i]] = pattern_size-i-1;
    }
}

void FindSuffixes(string pattern, int suffixes[], int pattern_size){
    int g,f;
    suffixes[pattern_size-1] = pattern_size; //the suffix is the equivalent of the entire pattern;

    g = pattern_size-1;
    f = pattern_size-1;
    for(int pointer = pattern_size-2; pointer >= 0; pointer--){
        if(pointer > g && suffixes[pattern_size-1-(f-pointer)] < pointer-g){
            suffixes[pointer] = suffixes[pattern_size-1-(f-pointer)] ;
        }
        else{
            if(pointer < g){
                g = pointer;
            }
            f = pointer;
            while(g >= 0 && pattern[g+pattern_size-1-f] == pattern[g]){
                g--;
            }
            suffixes[pointer] = f-g;
        }
    }
}

void GoodSuffixRule(string pattern, int GoodSuffixHeuristic[], int pattern_size){
    int suffixes[pattern_size]; //the amount of suffix matched of a substring that ended at i
    FindSuffixes(pattern, suffixes, pattern_size);

    for(int i = 0; i < pattern_size; i++){
        GoodSuffixHeuristic[i] = pattern_size;
    }

    //Case 1
    int pointer_left = 0;
    for(int current_pointer = pattern_size-1; current_pointer >= -1; current_pointer--){
        if(current_pointer == -1 || current_pointer+1 == suffixes[current_pointer]){
            for(; pointer_left < pattern_size-1-current_pointer; pointer_left++){
                if(GoodSuffixHeuristic[pointer_left] == pattern_size){
                    GoodSuffixHeuristic[pointer_left] = pattern_size-1-current_pointer;
                }
            }

        }
    }

    //Case 2
    for(int current_pointer = 0; current_pointer <= pattern_size-2; current_pointer++){
        GoodSuffixHeuristic[pattern_size-1-suffixes[current_pointer]] = pattern_size-1-current_pointer;
    }
}

void searchBM(const ProjectData& data, vector<vector<Occurrence>>& results, long long& comparisons) {
    for (int k = 0; k < data.K; k++) {
        string word = data.keywords[k];
        int word_length = word.size();
        int BadCharacterHeuristic[256];
        int GoodSuffixHeuristic[word_length];
        BadCharacterRule(word, BadCharacterHeuristic, word_length);
        GoodSuffixRule(word,GoodSuffixHeuristic, word_length);

        int rows = data.R;
        int columns = data.C;
        vector<vector<char>> grid = data.grid;
        for(int i = 0; i < rows; i++){
            int shift = 0;
            while(shift <= (columns-word_length)){
                int current = word_length-1;
                while(current >= 0 && grid[i][shift+current] == word[current]){
                    comparisons++;
                    current--;
                }
                if(current == -1){
                    Occurrence Occurence_match = {i, shift, i, shift+word_length-1};
                    results[k].push_back(Occurence_match);
                    shift += GoodSuffixHeuristic[0];
                }
                else{
                    comparisons++;
                    int bad_character_value = BadCharacterHeuristic[(unsigned char)grid[i][shift+current]];
                    int real_bad_character_value = bad_character_value - (word_length - 1 - current);
                    shift += max({1,GoodSuffixHeuristic[current],real_bad_character_value});
                }
            }
        }

        for(int j = 0; j < columns; j++){
            int shift = 0;
            while(shift <= (rows-word_length)){
                int current = word_length-1;
                while(current >= 0 && grid[shift+current][j] == word[current]){
                    comparisons++;
                    current--;
                }
                if(current == -1){
                    Occurrence Occurence_match = {shift, j, shift+word_length-1, j};
                    results[k].push_back(Occurence_match);
                    shift += GoodSuffixHeuristic[0];
                }
                else{
                    comparisons++;
                    int bad_character_value = BadCharacterHeuristic[(unsigned char)grid[shift+current][j]];
                    int real_bad_character_value = bad_character_value - (word_length - 1 - current);
                    shift += max({1,GoodSuffixHeuristic[current],real_bad_character_value});
                }
            }
        }
    }
}


// void searchBM(const ProjectData& data, std::vector<std::vector<Occurrence>>& all_results, long long& comparisons) {
    
// }