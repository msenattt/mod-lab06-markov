// Copyright 2024 <Copyright Owner>

#include <string>
#include <vector>
#include "textgen.h"

TextGenerator::TextGenerator(int npref) {
    prefixSize = npref;
    std::random_device rd;
    rng = std::mt19937(rd());
}

void TextGenerator::add(const prefix& pref, const std::string& suffix) {
    stateTable[pref].push_back(suffix);
}

prefix TextGenerator::makePrefix(const std::vector<std::string>& words) {
    prefix pref;
    for (int i = 0; i < words.size(); i++) {
        pref.push_back(words[i]);
    }
    return pref;
}

void TextGenerator::build(std::istream& input) {
    std::string word;
    prefix current;
    
    for (int i = 0; i < prefixSize; i++) {
        if (!(input >> word)) return;
        current.push_back(word);
    }
    
    while (input >> word) {
        add(current, word);
        current.pop_front();
        current.push_back(word);
    }
    
    add(current, "");
}

void TextGenerator::addEntry(const prefix& pref, const std::string& suffix) {
    add(pref, suffix);
}

std::string TextGenerator::generate(int maxWords) {
    if (stateTable.empty()) {
        return "";
    }
    
    prefix current = stateTable.begin()->first;
    
    std::string result;
    int wordCount = 0;
    
    for (int i = 0; i < current.size(); i++) {
        if (wordCount >= maxWords) break;
        if (wordCount > 0) result += " ";
        result += current[i];
        wordCount++;
    }
    
    if (wordCount >= maxWords) return result;
    
    while (wordCount < maxWords) {
        auto it = stateTable.find(current);
        if (it == stateTable.end() || it->second.empty()) {
            break;
        }
        
        const std::vector<std::string>& suffixes = it->second;
        std::uniform_int_distribution<int> sfxDist(0, suffixes.size() - 1);
        std::string nextWord = suffixes[sfxDist(rng)];
        
        if (nextWord.empty()) break;
        
        result += " " + nextWord;
        wordCount++;
        
        current.pop_front();
        current.push_back(nextWord);
    }
    
    return result;
}

bool TextGenerator::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    stateTable.clear();
    build(file);
    file.close();
    return true;
}

void TextGenerator::loadFromString(const std::string& text) {
    stateTable.clear();
    std::istringstream stream(text);
    build(stream);
}

bool TextGenerator::generateToFile(const std::string& filename, int maxWords) {
    std::string text = generate(maxWords);
    if (text.empty()) return false;
    
    std::ofstream file(filename);
    if (!file.is_open()) return false;
    
    file << text;
    file.close();
    return true;
}

std::string TextGenerator::getGeneratedText(int maxWords) {
    return generate(maxWords);
}

std::vector<std::string> TextGenerator::getSuffixes(const prefix& pref) const {
    auto it = stateTable.find(pref);
    if (it != stateTable.end()) {
        return it->second;
    }
    return std::vector<std::string>();
}

bool TextGenerator::hasPrefix(const prefix& pref) const {
    return stateTable.find(pref) != stateTable.end();
}

void TextGenerator::reset() {
    stateTable.clear();
}

prefix TextGenerator::getFirstPrefix() const {
    if (!stateTable.empty()) {
        return stateTable.begin()->first;
    }
    return prefix();
}
