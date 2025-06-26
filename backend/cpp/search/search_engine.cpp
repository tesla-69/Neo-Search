#include "search_engine.h"
#include <cmath>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <iostream>
#include "../utils/synonyms.h"  // ✅ NEW: Include synonym expansion

SearchEngine::SearchEngine(Indexer& indexer) : indexer(indexer) {}

std::vector<std::string> SearchEngine::tokenize(const std::string& text) {
    std::vector<std::string> tokens;
    std::istringstream iss(text);
    std::string word;

    while (iss >> word) {
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        if (!word.empty()) tokens.push_back(word);
    }

    return tokens;
}

// ✅ UPDATED: query-aware snippet extractor
std::string SearchEngine::generateSnippet(const std::string& content, const std::vector<std::string>& queryTokens) {
    std::string loweredContent = content;
    std::transform(loweredContent.begin(), loweredContent.end(), loweredContent.begin(), ::tolower);

    for (const std::string& word : queryTokens) {
        size_t pos = loweredContent.find(word);
        if (pos != std::string::npos) {
            size_t start = (pos >= 50) ? pos - 50 : 0;
            size_t end = std::min(start + 150, content.size());
            return content.substr(start, end - start) + "...";
        }
    }

    return content.substr(0, 150) + "...";  // fallback snippet
}

// ✅ FINAL: Now returns both results + expanded terms used
std::pair<std::vector<SearchResult>, std::vector<std::string>> SearchEngine::search(const std::string& query, int topK) {
    std::unordered_map<int, double> docScores;
    std::unordered_map<std::string, bool> seen;
    int totalDocs = indexer.totalDocuments();

    auto words = tokenize(query);
    SynonymExpander expander;

    std::vector<std::string> expandedTerms;

    for (const auto& word : words) {
        auto exps = expander.expand(word);  // "ai" → ["ai", "artificial intelligence", ...]
        for (const auto& expanded : exps) {
            if (seen[expanded]) continue;
            seen[expanded] = true;
            expandedTerms.push_back(expanded);

            auto postings = indexer.getPostings(expanded);
            int df = indexer.getDocumentFrequency(expanded);
            if (df == 0) continue;

            double idf = log((double)(totalDocs + 1) / (df + 1));
            for (const auto& post : postings) {
                double tf = 1 + log(post.termFrequency);
                docScores[post.docId] += tf * idf;
            }
        }
    }

    std::vector<SearchResult> results;
    for (const auto& pair : docScores) {
        int docId = pair.first;
        double score = pair.second;

        std::string title = indexer.getDocTitle(docId);
        std::string fullText = indexer.getDocText(docId);
        std::string snippet = generateSnippet(fullText, words);  // ✅ uses original query words

        results.push_back({docId, score, title, snippet});
    }

    std::sort(results.begin(), results.end(), [](const SearchResult& a, const SearchResult& b) {
        return a.score > b.score;
    });

    if (results.size() > topK)
        results.resize(topK);

    return {results, expandedTerms};  // ✅ Also return the used synonym terms
}
