#include "matrix/matrix.h"
#include "zvector/zvector.h"
#include "trie/trie.h"
#include "rtrie/rtrie.h"

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
        
        RTrieNode rtrie_node(zvector.getVector(), 0, zvector.getVector().size());
        rtrie_node.makeZOrderVector();
        ZVector zvector_rtrie(rtrie_node.getZOrderVector());
        Node rtrie(zvector_rtrie.getIntVector());

        std::cout << "\nMatrix: " << std::endl;
        matrix.print();

        std::cout << "\nInt vector:" << std::endl;
        print_vector(zvector.getIntVector());

        std::cout << "\nZ-order vector:" << std::endl;
        print_vector(zvector.getVector());

        std::cout << "\nr-Trie Z-order-vector: " << std::endl;
        print_vector(zvector_rtrie.getVector());
        std::cout << std::endl;

        std::cout << "\nr-Trie Int vector:" << std::endl;
        print_vector(zvector_rtrie.getIntVector());

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

        
        std::cout << "\nGap vector (r-trie):" << std::endl;
        print_vector(zvector_rtrie.getGapVector());
        std::cout << "\nGap Entropy (r-trie): " << zvector_rtrie.getGapEntropy() << std::endl;
        std::cout << "\nH_0(Gap) (r-trie):" << zvector_rtrie.get_H0_GapEntropy() << std::endl;

        std::cout << "\n\nRLE vector (r-trie):" << std::endl;
        print_vector(zvector_rtrie.getRLEVector());
        std::cout << "\nRLE Entropy (r-trie): " << zvector_rtrie.getRLEEntropy() << std::endl;
        std::cout << "\nH_0(RLE) (r-trie):" << zvector_rtrie.get_H0_RLEEntropy() << std::endl;

        std::cout << "\nTrie Peorder (r-trie): " << std::endl;
        rtrie.printPreorder(&rtrie, 0);
        std::cout << std::endl;
        std::cout << "Trie Entropy (r-trie): " << rtrie.getTrieEntropy(zvector_rtrie.getIntVector()) << std::endl;
    }
    else
    {
        std::cout << "No file name entered. error.";
        return -1;
    }
    return 0;
}

// 00011111110001101100