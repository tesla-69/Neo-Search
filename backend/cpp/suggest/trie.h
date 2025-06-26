#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <vector>
#include <string>

struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    bool isEnd = false;
    int frequency = 0;
};

class Trie {
public:
    Trie();
    void insert(const std::string& word);
    std::vector<std::string> suggest(const std::string& prefix, int maxSuggestions = 5);

private:
    TrieNode* root;
    void dfs(TrieNode* node, std::string path, std::vector<std::pair<std::string, int>>& results);
};

#endif
