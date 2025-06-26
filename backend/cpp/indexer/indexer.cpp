#include "indexer.h"
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <cctype>

std::vector<std::string> Indexer::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!word.empty())
            tokens.push_back(word);
    }
    return tokens;
}

void Indexer::addDocument(int docId, const std::string& title , const std::string& text) {
     docTitles[docId] = title; // ✅ Store the title
    docTexts[docId] = text; 
    docCount++;
    std::unordered_map<std::string, int> freqMap;
    auto words = tokenize(text);

    for (const auto& word : words)
        freqMap[word]++;

    for (const auto& pair : freqMap) {
        invertedIndex[pair.first].push_back({docId, pair.second});
    }

    docLengths[docId] = words.size();
}

std::vector<Posting> Indexer::getPostings(const std::string& word) {
    return invertedIndex[word];
}
std::string Indexer::getDocText(int docId) {
    return docTexts[docId];
}

// ✅ New function: returns title of a document
std::string Indexer::getDocTitle(int docId) {
    return docTitles[docId];
}

int Indexer::getDocumentFrequency(const std::string& word) const {
    auto it = invertedIndex.find(word);
    if (it != invertedIndex.end()) {
        return it->second.size();
    }
    return 0;
}

int Indexer::totalDocuments() const {
    return docCount;
}
