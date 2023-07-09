#include <vector>
#include <sstream>
#include <iostream>
#include <cmath>

class Node {
    private:
        std::string value;
        Node* left;
        Node* right;
        unsigned heigth;

    public:
        Node(char*, int);
        Node(std::vector<int>);
        void printPreorder(Node *root, int);
        int getTrieHeigth();
        ~Node();
        Node* operator[](const char *);

        std::vector<int> getVector() const;
        unsigned getTrieEntropy(std::vector<int>) const;
};