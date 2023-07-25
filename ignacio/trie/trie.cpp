#include "trie.h"

Node::Node(char* value, int heigth)
{
    std::string str_value(value);
    this->value = str_value;
    this->left = nullptr;
    this->right = nullptr;
    this->heigth = heigth;
}

std::string toBinary(int n, unsigned n_bits)
{
    std::string r;

    while(n != 0) {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n = n / 2;
    }

    while (r.length() < n_bits) {
        r = "0" + r;
    }
    // std::cout << r << std::endl;
    return r;
}

Node::Node(std::vector<int> int_vector)
{
    unsigned i, j;
    this->value = "root";
    this->left = nullptr;
    this->right = nullptr;

    Node* aux = this;

    this->heigth = floor(log2(int_vector[int_vector.size() - 1])) + 1;
    
    for (i = 0; i < int_vector.size(); i ++) {
        aux = this;
        std::string binary_number = toBinary(int_vector[i], this->heigth);

        for (j = 0; j < this->heigth; j++) {
            if (binary_number[j] == '0') {
                if (!aux->left) {
                    aux->left = new Node((char*)"0", this->heigth);
                }
                aux = aux->left;
            }
            if (binary_number[j] == '1') {
                if (!aux->right) {
                    aux->right = new Node((char*)"1", this->heigth);
                }
                aux = aux->right;
            }
        }
        // std::cout << "int_vector[" << i << "]: " << int_vector[i] << " | -> " << toBinary(int_vector[i], heigth) << std::endl;
    }
}

Node::~Node()
{
}

Node *Node::operator[](const char *idx)
{
    if (idx == (char*)"0") {
        return this->left;
    }
    if (idx == (char*)"1") {
        return this->right;
    }
    return this;
}

// Function to print preorder traversal
void Node::printPreorder(Node* node, int len)
{
    int newLen = (len > 0) ? len : this->getTrieHeigth();
    if (!node) {
        return;
    }
    // Deal with the node
    std::cout << node->value;
    if (newLen != len) {
        std::cout << std::endl;
    }

    // Recur on left subtree
    printPreorder(node->left, newLen - 1);

    // Recur on right subtree
    printPreorder(node->right, newLen - 1);
}

int Node::getTrieHeigth()
{
    return this->heigth;
}

std::string toBinary(int n)
{
    std::string r;

    while(n != 0) {
        r = (n % 2 == 0 ? "0" : "1") + r;
        n = n / 2;
    }
    return r;
}

unsigned Node::getTrieEntropy(std::vector<int> int_vector) const
{
    int n_bits = floor(log2(int_vector[int_vector.size() - 1])) + 1;
    std::string current_bit_sec, aux, new_bit_sec;


    unsigned trie_entropy = 0;
    unsigned x_i, i, j, dif;
    bool flag = false;

    for (i = 0; i < int_vector.size(); i++) {
        flag = false;
        x_i = 0;

        if (i == 0) {
            current_bit_sec = toBinary(int_vector[i], n_bits);
            x_i = current_bit_sec.length();
        } else {
            aux = toBinary(int_vector[i]);
            new_bit_sec = "";
            dif = current_bit_sec.length() - aux.length();

            // Save the prefix of the original value
            for (j = 0; j < dif; j++) {
                new_bit_sec += current_bit_sec[j];
            }

            // Start adding bits after the prefix
            // if at some point the bit in both secs is different,
            // then the prefix is broken and start adding new bits to the sum
            for (j = dif; j < current_bit_sec.length(); j++) {
                if (current_bit_sec[j] == aux[j - dif]) {
                    new_bit_sec += current_bit_sec[j];
                } else { // Here is the new bit after breaking the prefix
                    flag = true;
                    new_bit_sec += aux[j - dif];
                }
                if (flag) {
                    x_i += 1;
                }
            }
            current_bit_sec = new_bit_sec;
        }
        // std::cout << "x_" << i<< ": " << x_i << std::endl;
        trie_entropy += x_i;
    }

    return trie_entropy;
}

