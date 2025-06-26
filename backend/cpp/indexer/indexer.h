#ifndef INDEXER_H
#define INDEXER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>

struct Posting {
    int docId;
    int termFrequency;
};

class Indexer {
public:
    void addDocument(int docId, const std::string& title, const std::string& text);

    std::vector<Posting> getPostings(const std::string& word);
    int getDocumentFrequency(const std::string& word) const;
    int totalDocuments() const;

    std::string getDocText(int docId);
    std::string getDocTitle(int docId);

private:
    std::unordered_map<std::string, std::vector<Posting>> invertedIndex;
    std::unordered_map<int, int> docLengths;
    int docCount = 0;

    std::map<int, std::string> docTitles;
    std::map<int, std::string> docTexts;

    std::vector<std::string> tokenize(const std::string& text);
};

#endif
