CXX = g++
CXXFLAGS = -std=c++17
LDLIBS = -lcurl

SRC = main.cpp \
      crawler/crawler.cpp \
      indexer/indexer.cpp \
      search/search_engine.cpp \
      suggest/trie.cpp \
      utils/tokenizer.cpp \
	   utils/synonyms.cpp

HEADERS = $(wildcard crawler/*.h indexer/*.h search/*.h suggest/*.h utils/*.h)

NEO: $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o NEO $(LDLIBS)

clean:
	rm -f NEO
