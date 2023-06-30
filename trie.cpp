#include "trie.h"

Trie::Trie()
{
    this->value = 0;
    this->left = nullptr;
    this->right = nullptr;
}

// Trie::Trie(std::vector<int> int_vector, int domain)
// {

//     std::stringstream bitvec_stream;
//     char bitvec[domain];
//     int bit;
//     unsigned i, j;

//     this->value = 0;
//     this->left = nullptr;
//     this->right = nullptr;


//     for (i = 0; i < int_vector.size(); i++) {
//         Trie *aux = this;
//         bitvec_stream << int_vector[i];
//         bitvec_stream.rdbuf()->pubsetbuf(bitvec, sizeof(bitvec));

//         for (j = 0; j < domain; j++) {
//             bit = (int)bitvec[j];
//             if (!aux[bit]) {
//                 Trie newTrie;
//                 aux[bit] = newTrie; 
//             }
//             aux = &newTrie; 
//         }
//     }
// }

Trie::Trie(std::vector<std::vector<int>> binary_matrix)
{
    this->value = 0;
    this->left = nullptr;
    this->right = nullptr;
}

Trie::~Trie()
{
}

Trie *Trie::operator[](const unsigned &idx)
{
    // TODO: insert return statement here
    if (idx == 0) {
        return this->left;
    }
    return this->right;
}

unsigned getEdgeCount(Trie* trie)
{

}

// Given a binary tree, print its nodes in preorder
// void printPreorder(Trie* trie)
// {
//     if (trie == nullptr)
//         return;
 
//     // First print data of node
//     printf("%d ", node->data);
 
//     // Then recur on left subtree
//     printPreorder(node->left);
 
//     // Now recur on right subtree
//     printPreorder(node->right);
// }

float Trie::trieEntropy() const
{
    float x_1 = 0.0f; // cantidad de bits que necesito 

    unsigned i;

    return 0.0f;
}
