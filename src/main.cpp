#include "textgen.h"
#include <iostream>

int main() {
    const int NPREF = 2;
    const int MAXGEN = 1000;
    
    std::cout << "=== Markov Chain Text Generator ===" << std::endl;
    std::cout << "Prefix size: " << NPREF << " words" << std::endl;
    std::cout << "Max words: " << MAXGEN << std::endl;
    
    TextGenerator gen(NPREF);
    
    if (!gen.loadFromFile("input.txt")) {
        std::cout << "Failed to open input.txt" << std::endl;
        return 1;
    }
    
    std::cout << "Table entries: " << gen.getTableSize() << std::endl;
    
    if (gen.generateToFile("gen.txt", MAXGEN)) {
        std::cout << "Text saved to result/gen.txt" << std::endl;
        
        std::string preview = gen.getGeneratedText(40);
        std::cout << "\nPreview:\n" << preview << "..." << std::endl;
    } else {
        std::cout << "Generation failed" << std::endl;
        return 1;
    }
    
    return 0;
}