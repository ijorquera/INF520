#include <vector>
#include <sstream>
#include <iostream>

class Trie {
    private:
        int value; 
        Trie* left; 
        Trie* right; 

    public:
        Trie();
        Trie(std::vector<int>, int);
        Trie(std::vector<std::vector<int>>);
        ~Trie();

        Trie* operator[](const unsigned &);

        std::vector<int> getVector() const;
        float trieEntropy() const;

        // std::vector<int> newIntVector() const;
        // std::vector<int> getIntVector() const;
        // float getWorstCaseEntropy() const;

        // // Algoritmos de compresion
        // // Gap(S)
        // std::vector<int> newGapVector() const;
        // std::vector<int> getGapVector() const;
        // float getGapEntropy() const;

        // // RunLen(S)
        // std::vector<int> newRLEVector() const;
        // std::vector<int> getRLEVector() const;
        // float getRLEEntropy() const;

};