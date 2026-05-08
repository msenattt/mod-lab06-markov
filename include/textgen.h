#ifndef TEXTGEN_H
#define TEXTGEN_H

#include <string>
#include <deque>
#include <map>
#include <vector>
#include <random>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

typedef std::deque<std::string> prefix;
typedef std::map<prefix, std::vector<std::string>> statetab;

class TextGenerator {
private:
    statetab stateTable;
    int prefixSize;
    std::mt19937 rng;
    void add(const prefix& pref, const std::string& suffix);
    void build(std::istream& input);
    std::string generate(int maxWords);
    
public:
    TextGenerator(int npref = 2);
    bool loadFromFile(const std::string& filename);
    void loadFromString(const std::string& text);
    void addEntry(const prefix& pref, const std::string& suffix);
    bool generateToFile(const std::string& filename, int maxWords = 1000);
    std::string getGeneratedText(int maxWords = 1000);
    std::vector<std::string> getSuffixes(const prefix& pref) const;
    bool hasPrefix(const prefix& pref) const;
    static prefix makePrefix(const std::vector<std::string>& words);
    void reset();
    size_t getTableSize() const { return stateTable.size(); }
    prefix getFirstPrefix() const;
};

#endif