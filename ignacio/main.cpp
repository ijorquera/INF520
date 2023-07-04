#include "matrix/matrix.h"
#include "zvector/zvector.h"

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

        std::cout << "\nMatrix: " << std::endl;
        matrix.print();

        std::cout << "\nInt vector:" << std::endl;
        print_vector(zvector.getIntVector());

        std::cout << "\nZ-order vector:" << std::endl;
        print_vector(zvector.getVector());

        std::cout << "\nWorst case Entropy: " << zvector.getWorstCaseEntropy() << std::endl;

        std::cout << "\nGap vector/Delta values? pag 233:" << std::endl;
        print_vector(zvector.getGapVector());
        std::cout << "Gap Entropy: " << zvector.getGapEntropy() << std::endl;

        std::cout << "\nRLE vector:" << std::endl;
        print_vector(zvector.getRLEVector());
        std::cout << "RLE Entropy: " << zvector.getRLEEntropy() << std::endl;
    }
    else
    {
        std::cout << "No file name entered. error.";
        return -1;
    }
    return 0;
}