#include "matrix/matrix.h"
#include "zvector/zvector.h"
#include "trie/trie.h"

void print_vector(std::vector<int> vector) {
    for (unsigned i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char * argv[]) {
    if (argc > 1)
    {
        std::string data = argv[1];
        std::cout << "file name = " << data << std::endl;
        Matrix matrix(data);
        ZVector zvector(matrix.getMatrix());
        Node trie(zvector.getIntVector());

        std::cout << "\nMatrix: " << std::endl;
        matrix.print();

        std::cout << "\nInt vector:" << std::endl;
        print_vector(zvector.getIntVector());

        std::cout << "\nZ-order vector:" << std::endl;
        print_vector(zvector.getVector());

        std::cout << "\nWorst case Entropy: " << zvector.getWorstCaseEntropy() << std::endl;

        std::cout << "\nGap vector:" << std::endl;
        print_vector(zvector.getGapVector());
        std::cout << "\nGap Entropy: " << zvector.getGapEntropy() << std::endl;
        std::cout << "\nH_0(Gap):" << zvector.get_H0_GapEntropy() << std::endl;

        std::cout << "\n\nRLE vector:" << std::endl;
        print_vector(zvector.getRLEVector());
        std::cout << "\nRLE Entropy: " << zvector.getRLEEntropy() << std::endl;
        std::cout << "\nH_0(RLE):" << zvector.get_H0_RLEEntropy() << std::endl;

        std::cout << "\nTrie Peorder: " << std::endl;
        trie.printPreorder(&trie, 0);
        std::cout << std::endl;
        std::cout << "Trie Entropy: " << trie.getTrieEntropy(zvector.getIntVector()) << std::endl;
    }
    else
    {
        std::cout << "No file name entered. error.";
        return -1;
    }
    return 0;
}

// 00011111110001101100