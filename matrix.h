#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

class Matrix {
    private:
        unsigned m_rowSize;
        unsigned m_colSize;
        std::vector<std::vector<int> > m_matrix;

    public:
        Matrix(unsigned, unsigned, int); // rowsize, colsize, initial value for every cell
        Matrix(const char *);            // make matrix from file
        ~Matrix();
        
        int& operator()(const unsigned &, const unsigned &);
        std::vector<int>& operator[](const unsigned &);
        void print() const;
        unsigned getRows() const;
        unsigned getCols() const;
        std::vector<std::vector<int>> getMatrix() const;
};