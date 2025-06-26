#include "trie.h"
#include <queue>
#include <algorithm>

Trie::Trie() {
    root = new TrieNode();
}

void Trie::insert(const std::string& word) {
    TrieNode* node = root;
    for (char c : word) {
        if (!node->children[c])
            node->children[c] = new TrieNode();
        node = node->children[c];
    }
    node->isEnd = true;
    node->frequency++;
}

void Trie::dfs(TrieNode* node, std::string path, std::vector<std::pair<std::string, int>>& results) {
    if (node->isEnd)
        results.emplace_back(path, node->frequency);

    for (auto& [ch, child] : node->children)
        dfs(child, path + ch, results);
}

std::vector<std::string> Trie::suggest(const std::string& prefix, int maxSuggestions) {
    TrieNode* node = root;
    for (char c : prefix) {
        if (!node->children[c]) return {};
        node = node->children[c];
    }

    std::vector<std::pair<std::string, int>> results;
    dfs(node, prefix, results);

    std::sort(results.begin(), results.end(), [](auto& a, auto& b) {
        return b.second < a.second; // sort by frequency desc
    });

    std::vector<std::string> suggestions;
    for (int i = 0; i < results.size() && i < maxSuggestions; ++i)
        suggestions.push_back(results[i].first);

    return suggestions;
}
