#include "synonyms.h"
#include "json.hpp"  // nlohmann JSON

#include <fstream>
#include <iostream>

using json = nlohmann::json;

SynonymExpander::SynonymExpander(const std::string& jsonPath) {
    loadFromJSON(jsonPath);
}

void SynonymExpander::loadFromJSON(const std::string& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        std::cerr << "❌ Could not open synonym file: " << jsonPath << "\n";
        return;
    }

    json j;
    file >> j;

    for (auto it = j.begin(); it != j.end(); ++it) {
        std::string key = it.key();
        std::vector<std::string> synonyms = it.value().get<std::vector<std::string>>();
        synonymMap[key] = synonyms;
    }
}

std::vector<std::string> SynonymExpander::expand(const std::string& word) {
    std::vector<std::string> result = {word};
    if (synonymMap.count(word)) {
        result.insert(result.end(), synonymMap[word].begin(), synonymMap[word].end());
    }
    return result;
}
