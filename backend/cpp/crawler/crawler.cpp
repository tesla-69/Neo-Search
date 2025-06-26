#include "crawler.h"
#include <curl/curl.h>
#include <iostream>
#include <regex>

static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

Crawler::Crawler() {
    // You can initialize any internal state here if needed
}

std::string Crawler::fetchHtml(const std::string& url) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return response;
}

// ✅ Extract the page <title> from HTML
std::string Crawler::extractTitle(const std::string& html) {
    std::smatch match;
    std::regex titleRegex("<title>(.*?)</title>", std::regex::icase);
    if (std::regex_search(html, match, titleRegex)) {
        return match[1];
    }
    return "Untitled";
}

// ✅ Clean the HTML: remove <script>, <style>, tags and compress spaces
std::string Crawler::cleanHTML(const std::string& html) {
    std::string cleaned = html;

    // ✅ Replace dotall regex with workaround [\s\S]*?
    cleaned = std::regex_replace(cleaned,
        std::regex("<script[^>]*>[\\s\\S]*?</script>", std::regex::icase), "");

    cleaned = std::regex_replace(cleaned,
        std::regex("<style[^>]*>[\\s\\S]*?</style>", std::regex::icase), "");

    // Remove all remaining HTML tags
    cleaned = std::regex_replace(cleaned, std::regex("<[^>]+>"), "");

    // Normalize multiple spaces/newlines to single space
    cleaned = std::regex_replace(cleaned, std::regex("\\s+"), " ");

    return cleaned;
}

// (Deprecated — kept only if used elsewhere)
std::string Crawler::extractText(const std::string& html) {
    return std::regex_replace(html, std::regex("<[^>]*>"), " ");
}

std::vector<std::string> Crawler::extractLinks(const std::string& html) {
    std::vector<std::string> links;
    std::regex hrefRegex("<a\\s+(?:[^>]*?\\s+)?href=[\"']([^\"']+)[\"']");
    auto begin = std::sregex_iterator(html.begin(), html.end(), hrefRegex);
    auto end = std::sregex_iterator();

    for (auto i = begin; i != end; ++i) {
        std::string link = (*i)[1].str();
        if (link.find("http") == 0) {
            links.push_back(link);
        }
    }
    return links;
}

std::vector<std::pair<std::string, std::string>> Crawler::crawl(const std::string& startUrl, int maxPages) {
    std::vector<std::pair<std::string, std::string>> result;
    std::vector<std::string> toVisit = { startUrl };

    while (!toVisit.empty() && result.size() < maxPages) {
        std::string url = toVisit.back(); toVisit.pop_back();
        if (visited.count(url)) continue;
        visited.insert(url);

        std::string html = fetchHtml(url);
        if (html.empty()) continue;

        std::string title = extractTitle(html);      // ✅ Proper title
        std::string text = cleanHTML(html);          // ✅ Clean readable text

        result.push_back({title, text});

        auto links = extractLinks(html);
        for (const auto& link : links)
            toVisit.push_back(link);
    }

    return result;
}
