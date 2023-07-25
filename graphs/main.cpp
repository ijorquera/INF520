#include "file_reader/file_reader.h"

int main(int argc, char * argv[]) {
    if (argc > 1) {
        FileReader *file_reader = new FileReader();

        // Gap things
        std::cout << "Gap" << std::endl;
        file_reader -> getGapFromFile(argv[1], "z");
        file_reader -> getGapFromFile(argv[1], "col");
        file_reader -> getGapFromFile(argv[1], "row");

        // RLE things
        std::cout << "RLE" << std::endl;
        file_reader -> getRLEFromFile(argv[1], "z");
        file_reader -> getRLEFromFile(argv[1], "col");
        file_reader -> getRLEFromFile(argv[1], "row");

        // Trie things
        std::cout << "Trie" << std::endl;
        file_reader -> getTrieFromFile(argv[1], "z");
        file_reader -> getTrieFromFile(argv[1], "col");
        file_reader -> getTrieFromFile(argv[1], "row");
        
        // R-Trie things
        std::cout << "r-Trie" << std::endl;
        // Todo
        
        std::cout << "finished" << std::endl;
    } else {
        std::cout << "No file name entered. error.";
        return -1;
    }
    return 0;
}