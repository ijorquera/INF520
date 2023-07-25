#define _USE_MATH_DEFINES
// #include <iomanip>
#include <string>
#include <string.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <climits>

class FileReader {
    private:

    public:
        FileReader();
        ~FileReader();

        // Worst case entropy
        int getWorstCaseEntropy() const;

        // Algoritmos de compresion
        // Gap(S)
        void getGapFromFile(const std::string, const std::string);      // make zvector from file
        void get_H0_GapEntropy() const;

        // RLE
        void getRLEFromFile(const std::string, const std::string);
        void get_H0_RLEEntropy() const;

        // Trie
        void getTrieFromFile(const std::string, const std::string);

        // R-Trie 
};