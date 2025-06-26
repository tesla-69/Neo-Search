#include "tokenizer.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <unordered_set>

std::vector<std::string> Tokenizer::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        // Remove punctuation
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        // Convert to lowercase
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!word.empty())
            tokens.push_back(word);
    }

    return tokens;
}
