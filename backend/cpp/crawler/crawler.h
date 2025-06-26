#ifndef CRAWLER_H
#define CRAWLER_H

#include <string>
#include <vector>
#include <unordered_set>

class Crawler {
public:
    Crawler();

    // Crawl and return {title, cleaned_text} pairs
    std::vector<std::pair<std::string, std::string>> crawl(const std::string& startUrl, int maxPages);

private:
    std::string fetchHtml(const std::string& url);
    std::string extractTitle(const std::string& html);   // ✅ Added
    std::string cleanHTML(const std::string& html);      // ✅ Added
    std::vector<std::string> extractLinks(const std::string& html);
    
    // ❌ Optional: remove if no longer used in cpp
    std::string extractText(const std::string& html);    

    std::unordered_set<std::string> visited;
};

#endif
