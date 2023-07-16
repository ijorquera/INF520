#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>

class RTrieNode {
    private:
        bool value;
        std::vector<int> z_order_vector;
        RTrieNode* up_left;
        RTrieNode* up_right;
        RTrieNode* down_left;
        RTrieNode* down_right;
        unsigned heigth;

    public:
        // RTrieNode(char*, int);
        RTrieNode(std::vector<int>, unsigned, unsigned);
        void printPreorder(RTrieNode *root);
        int getTrieHeigth();
        void makeZOrderVector();
        void makeZOrderVector(RTrieNode *node, std::vector<int> &z_order_vector);
        std::vector<int> getZOrderVector() const;
        ~RTrieNode();
        RTrieNode* operator[](const int);

        std::vector<int> getVector() const;
        // unsigned getTrieEntropy(std::vector<int>) const;
        // unsigned getRTrieEntropy(std::vector<std::vector<int>>) const;
};