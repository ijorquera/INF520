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

        std::vector<unsigned __int128> getIntVectorFromFile(const std::string fileName, const std::string mode);

        // Worst case entropy
        int getWorstCaseEntropy() const;

        // Algoritmos de compresion
        // Gap(S)
        void getGapFromFile(const std::string, const std::string);      // make zvector from file
        void get_H0_GapEntropy() const;

        void getWorstCaseEntropyFromFile(const std::string, const std::string);

        // RLE
        void getRLEFromFile(const std::string, const std::string);
        void get_H0_RLEEntropy() const;

        // Trie
        void getTrieFromFile(const std::string, const std::string);

        void getTrieFromFile2(const std::string, const std::string);

        // R-Trie 
};

class RTrieNode {
    private:
        bool value;
        std::vector<unsigned __int128> z_order_vector;
        RTrieNode* up_left;
        RTrieNode* up_right;
        RTrieNode* down_left;
        RTrieNode* down_right;
        unsigned heigth;

    public:
        // RTrieNode(char*, int);
        RTrieNode(std::vector<unsigned __int128>, unsigned __int128, unsigned __int128);
        void printPreorder(RTrieNode *root);
        int getTrieHeigth();
        void makeZOrderVector();
        void makeZOrderVector(RTrieNode *node, std::vector<unsigned __int128> &z_order_vector);
        void saveZOrderVector(const std::string fileName, const std::string mode);
        std::vector<unsigned __int128> getZOrderVector() const;
        ~RTrieNode();
        RTrieNode* operator[](const int);

        std::vector<unsigned __int128> getVector() const;
        // unsigned getTrieEntropy(std::vector<int>) const;
        // unsigned getRTrieEntropy(std::vector<std::vector<int>>) const;
};