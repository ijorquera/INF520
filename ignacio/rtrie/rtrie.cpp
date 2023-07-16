#include "rtrie.h"

// RTrieNode::RTrieNode(char* value, int heigth)
// {
//     std::string str_value(value);
//     this->value = str_value;
//     this->up_left = nullptr;
//     this->up_right = nullptr;
//     this->heigth = heigth;
// }

// std::string toBinary(int n, unsigned n_bits)
// {
//     std::string r;

//     while(n != 0) {
//         r = (n % 2 == 0 ? "0" : "1") + r;
//         n = n / 2;
//     }

//     while (r.length() < n_bits) {
//         r = "0" + r;
//     }
//     // std::cout << r << std::endl;
//     return r;
// }

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

        // for (i = 0; i < int_vector.size(); i ++) {
        //     aux = this;
        //     std::string binary_number = toBinary(int_vector[i], this->heigth);

        //     for (j = 0; j < this->heigth; j++) {
        //         if (binary_number[j] == '0') {
        //             if (!aux->up_left) {
        //                 aux->up_left = new RTrieNode((char*)"0", this->heigth);
        //             }
        //             aux = aux->up_left;
        //         }
        //         if (binary_number[j] == '1') {
        //             if (!aux->up_right) {
        //                 aux->up_right = new RTrieNode((char*)"1", this->heigth);
        //             }
        //             aux = aux->up_right;
        //         }
        //     }
        //     // std::cout << "int_vector[" << i << "]: " << int_vector[i] << " | -> " << toBinary(int_vector[i], heigth) << std::endl;
        // }
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

// void zorder(std::vector<int> &vtr, std::vector<std::vector<int>> m, int y0, int x0, int size)
// {
//     if (size == 1) {
//         // Base case, just one cell
//         vtr.push_back(m[y0][x0]);
//         // std::cout << "y0: " << y0 << ", x0: " << x0 << ", m[y0][x0]: " << m[y0][x0] << std::endl;
//     } else {
//         // Recurse in Z-order
//         int h = size/2;
//         zorder(vtr, m, y0,   x0,   h); // top-left
//         zorder(vtr, m, y0,   x0+h, h); // top-right
//         zorder(vtr, m, y0+h, x0,   h); // bottom-left
//         zorder(vtr, m, y0+h, x0+h, h); // bottom-right
//     }
// }


// unsigned Node::getRTrieEntropy(std::vector<std::vector<int>> binary_matrix) const
// {
//     int n_bits = floor(log2(binary_matrix[0].size()));
//     std::string current_bit_sec, aux, new_bit_sec;


//     unsigned trie_entropy = 0;
//     unsigned x_i, i, j, dif;
//     bool flag = false;

//     for (i = 0; i < int_vector.size(); i++) {
//         flag = false;
//         x_i = 0;

//         if (i == 0) {
//             current_bit_sec = toBinary(int_vector[i], n_bits);
//             x_i = current_bit_sec.length();
//         } else {
//             aux = toBinary(int_vector[i]);
//             new_bit_sec = "";
//             dif = current_bit_sec.length() - aux.length();

//             // Save the prefix of the original value
//             for (j = 0; j < dif; j++) {
//                 new_bit_sec += current_bit_sec[j];
//             }

//             // Start adding bits after the prefix
//             // if at some point the bit in both secs is different,
//             // then the prefix is broken and start adding new bits to the sum
//             for (j = dif; j < current_bit_sec.length(); j++) {
//                 if (current_bit_sec[j] == aux[j - dif]) {
//                     new_bit_sec += current_bit_sec[j];
//                 } else { // Here is the new bit after breaking the prefix
//                     flag = true;
//                     new_bit_sec += aux[j - dif];
//                 }
//                 if (flag) {
//                     x_i += 1;
//                 }
//             }
//             current_bit_sec = new_bit_sec;
//         }
//         // std::cout << "x_" << i<< ": " << x_i << std::endl;
//         trie_entropy += x_i;
//     }

//     return trie_entropy;
// }
