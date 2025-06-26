#ifndef SEARCH_ENGINE_H
#define SEARCH_ENGINE_H

#include <string>
#include <vector>
#include <unordered_map>
#include "../indexer/indexer.h"

struct SearchResult {
    int docId;
    double score;
    std::string title;
    std::string snippet;
};

class SearchEngine {
public:
    SearchEngine(Indexer& indexer);

    // ✅ UPDATED: Now returns both results and the expanded terms used
    std::pair<std::vector<SearchResult>, std::vector<std::string>> search(const std::string& query, int topK = 5);

    std::vector<std::string> tokenize(const std::string& text);

private:
    Indexer& indexer;  // ✅ Moved to private
    std::string generateSnippet(const std::string& content, const std::vector<std::string>& queryTokens); // ✅ Moved to private
};

#endif
