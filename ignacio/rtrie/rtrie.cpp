#include "rtrie.h"

RTrieNode::RTrieNode(std::vector<int> z_order_vector, unsigned start, unsigned end)
{
    unsigned i, j;
    this->value = true;
    this->up_left = nullptr;
    this->up_right = nullptr;
    this->down_left = nullptr;
    this->down_right = nullptr;

    this->heigth = (z_order_vector.size() / 4);

    if ((end - start) == 1) {
        this->value = z_order_vector[start] == 1;
    } else {
        i = start;
        j = i + ((end - start) / 4);

        // Primer cuadrante (up_left)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->up_left = new RTrieNode(z_order_vector, i, j);
        }

        i = j;
        j = i + ((end - start) / 4);

        // Segundo cuadrante (up_right)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->up_right = new RTrieNode(z_order_vector, i, j);
        }

        i = j;
        j = i + ((end - start) / 4);

        // Tercer cuadrante (down_left)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->down_left = new RTrieNode(z_order_vector, i, j);
        }

        i = j;
        j = i + ((end - start) / 4);

        // Cuarto cuadrante (down_right)
        for(auto it = z_order_vector.begin() + i; it != z_order_vector.begin() + j; it++) {
            this->down_right = new RTrieNode(z_order_vector, i, j);
        }

        if (!this->up_left->value && !this->up_right->value && !this->down_left->value && !this->down_right->value) {
            delete this->up_left;
            delete this->up_right;
            delete this->down_left;
            delete this->down_right;
            this->up_left = nullptr;
            this->up_right = nullptr;
            this->down_left = nullptr;
            this->down_right = nullptr;
            this->value = false;
        }
    }
}

RTrieNode::~RTrieNode()
{
}

RTrieNode *RTrieNode::operator[](const int idx)
{
    if (idx == 0) {
        return this->up_left;
    }
    if (idx == 1) {
        return this->up_right;
    }
    if (idx == 2) {
        return this->down_left;
    }
    if (idx == 3) {
        return this->down_right;
    }
    return this;
}

int RTrieNode::getTrieHeigth()
{
    return this->heigth;
}

void RTrieNode::makeZOrderVector() {
    makeZOrderVector(this, this->z_order_vector);
}
// Function to make a compressed z-order vector
void RTrieNode::makeZOrderVector(RTrieNode* node, std::vector<int> &z_order_vector)
{
    if (!node) {
        return;
    }
    // Deal with the node
    z_order_vector.push_back((int)node->value);

    // Recur on up_left subtree
    makeZOrderVector(node->up_left, z_order_vector);

    // Recur on up_right subtree
    makeZOrderVector(node->up_right, z_order_vector);

    // Recur on down_left subtree
    makeZOrderVector(node->down_left, z_order_vector);

    // Recur on down_right subtree
    makeZOrderVector(node->down_right, z_order_vector);
}

std::vector<int> RTrieNode::getZOrderVector() const
{
    return this->z_order_vector;
}

// Function to print preorder traversal
void RTrieNode::printPreorder(RTrieNode* node)
{
    if (!node) {
        return;
    }
    // Deal with the node
    std::cout << node->value << " ";

    // Recur on up_left subtree
    printPreorder(node->up_left);

    // Recur on up_right subtree
    printPreorder(node->up_right);

    // Recur on down_left subtree
    printPreorder(node->down_left);

    // Recur on down_right subtree
    printPreorder(node->down_right);
}
