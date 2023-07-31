#include "file_reader/file_reader.h"

std::string uint128ToString2(unsigned __int128 in)
{
    std::string value = "";
    if (in == 0) {
        value += "0";
    }

    while (in != 0) {
        value = std::to_string((int)(in % 10)) + value;
        in = in / 10;
    }

    return value;
}


int main(int argc, char * argv[]) {
    if (argc > 1) {
        FileReader *file_reader = new FileReader();

        // worstCase entropy
        std::cout << "worstCase entropy" << std::endl;
        file_reader -> getWorstCaseEntropyFromFile(argv[1], "z");
        file_reader -> getWorstCaseEntropyFromFile(argv[1], "col");
        file_reader -> getWorstCaseEntropyFromFile(argv[1], "row");

        // Gap things
        std::cout << "Gap" << std::endl;
        file_reader -> getGapFromFile(argv[1], "z");
        file_reader -> getGapFromFile(argv[1], "col");
        file_reader -> getGapFromFile(argv[1], "row");

        // // RLE things
        std::cout << "RLE" << std::endl;
        file_reader -> getRLEFromFile(argv[1], "z");
        file_reader -> getRLEFromFile(argv[1], "col");
        file_reader -> getRLEFromFile(argv[1], "row");

        // Trie things
        std::cout << "Trie" << std::endl;
        file_reader -> getTrieFromFile(argv[1], "z");
        file_reader -> getTrieFromFile(argv[1], "col");
        file_reader -> getTrieFromFile(argv[1], "row");
        
        // alternative trie, it only uses strings instead of reading values as int
        // in order to avoid overflow of big numbers
        // file_reader -> getTrieFromFile2(argv[1], "z");
        // file_reader -> getTrieFromFile2(argv[1], "col");
        // file_reader -> getTrieFromFile2(argv[1], "row");
        
        
        std::cout << "finished" << std::endl;
    } else {
        std::cout << "No file name entered. error.";
        return -1;
    }
    return 0;
}