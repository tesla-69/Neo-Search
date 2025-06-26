#include <iostream>
#include "crawler/crawler.h"
#include "indexer/indexer.h"
#include "search/search_engine.h"
#include "suggest/trie.h"
#include "utils/tokenizer.h"

int main(int argc, char* argv[]) {
    std::string query;
    bool interactive = true;

    if (argc >= 2) {
        query = argv[1];
        interactive = false;  // Running in API mode
    }

    // 🔁 Crawl from multiple domains
    Crawler crawler;
    std::vector<std::pair<std::string, std::string>> allPages;
    std::vector<std::string> seedUrls = {
        "https://en.wikipedia.org/wiki/Artificial_intelligence",
        "https://openai.com/research",
        "https://en.wikipedia.org/wiki/Machine_learning"
    };

    for (const std::string& url : seedUrls) {
        auto pages = crawler.crawl(url, 5);
        allPages.insert(allPages.end(), pages.begin(), pages.end());
    }

    // ✅ Index documents
    Indexer indexer;
    int docId = 0;
    for (const auto& page : allPages) {
        std::string url = page.first;
        std::string text = page.second;

        std::string title = url;
        size_t pos = text.find('\n');
        if (pos != std::string::npos && pos < 100)
            title = text.substr(0, pos);

        if (interactive)
            std::cout << "Indexing: " << url << std::endl;

        indexer.addDocument(docId++, title, text);
    }

    // ✅ Initialize search engine + trie
    SearchEngine engine(indexer);
    Trie trie;

    for (const auto& page : allPages) {
        auto words = engine.tokenize(page.second);
        for (const std::string& word : words) {
            trie.insert(word);
        }
    }

    // ✅ Read search query
    if (interactive) {
        std::cout << "\n🔍 Enter search query: ";
        std::getline(std::cin, query);
    }

    // ✅ Modified: capture results + synonyms
    auto [results, expandedTerms] = engine.search(query);

    // ✅ Print synonyms (for API mode)
    if (!interactive) {
        for (const auto& term : expandedTerms) {
            std::cout << "@@SYNONYM: " << term << std::endl;
        }
    }

    // ✅ Print results
    if (interactive) {
        std::cout << "\n📄 Top Search Results:\n";
        for (const auto& res : results) {
            std::cout << "Doc #" << res.docId << " | Score: " << res.score << " | Title: " << res.title << "\n";
            std::cout << "Snippet: " << res.snippet << "\n\n";
        }
    } else {
        for (const auto& res : results) {
            std::cout << res.docId << "|" << res.score << "|" << res.title << "|" << res.snippet << std::endl;
        }
    }

    // ✅ Trie Suggestion
    if (interactive) {
        std::string prefix;
        std::cout << "\n⌨️  Enter prefix for suggestion: ";
        std::getline(std::cin, prefix);

        auto suggestions = trie.suggest(prefix);
        std::cout << "\n🔮 Suggestions:\n";
        for (const std::string& s : suggestions) {
            std::cout << "- " << s << "\n";
        }
    }

    return 0;
}
