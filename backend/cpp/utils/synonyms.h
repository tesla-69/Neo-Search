#ifndef SYNONYMS_H
#define SYNONYMS_H

#include <string>
#include <unordered_map>
#include <vector>

class SynonymExpander {
public:
    SynonymExpander(const std::string& jsonPath = "assets/synonyms.json");

    std::vector<std::string> expand(const std::string& word);

private:
    std::unordered_map<std::string, std::vector<std::string>> synonymMap;
    void loadFromJSON(const std::string& jsonPath);
};

#endif
