#include "matrix.h"

// Constructor for Any Matrix
Matrix::Matrix(unsigned rowSize, unsigned colSize, int initial){
    m_rowSize = rowSize;
    m_colSize = colSize;
    m_matrix.resize(rowSize);

    for (unsigned i = 0; i < m_matrix.size(); i++)
    {
        m_matrix[i].resize(colSize, initial);
    }
}

// Make a matrix from a file
Matrix::Matrix(const char * fileName){
    std::ifstream file_A(fileName); // input file stream to open the file A.txt

    // Task 1
    // Keeps track of the Column and row sizes
    int colSize = 0;
    int rowSize = 0;
    
    // read it as a vector
    std::string line_A;
    int idx = 0;
    int element_A;
    int *vector_A = nullptr;
    
    if (file_A.is_open() && file_A.good())
    {
        // std::cout << "File is open. \n";
        while (getline(file_A, line_A))
        {
            rowSize += 1;
            colSize = 0;
            std::stringstream stream_A(line_A);
            while (1)
            {
                stream_A >> element_A;
                if (!stream_A)
                    break;
                colSize += 1;
                int *tempArr = new int[idx + 1];
                std::copy(vector_A, vector_A + idx, tempArr);
                tempArr[idx] = element_A;
                vector_A = tempArr;
                idx += 1;
            }
        }
    }
    else
    {
        std::cout << "Failed to open file.  \n";
    }
    
    // std::cout << "Temporary matrix is complete. \n";
    
    unsigned j;
    idx = 0;
    m_matrix.resize(rowSize);

    for (unsigned i = 0; i < m_matrix.size(); i++) {
        m_matrix[i].resize(colSize);
    }

    // std::cout << "New Matrix size: " << m_matrix.size() << " x " << m_matrix[0].size() << "\n";

    // this->m_matrix_arr = new int *[rowSize];

    for (unsigned i = 0; i < rowSize; i++)
    {
        for (j = 0; j < colSize; j++)
        {
            
            // this->m_matrix_arr[i] = new int[colSize];
            // this->m_matrix_arr[i][j] = vector_A[idx];
            this->m_matrix[i][j] = vector_A[idx];
            idx++;
        }
    }
    this->m_colSize = colSize;
    this->m_rowSize = rowSize;


    delete [] vector_A; // Tying up loose ends
}

Matrix::~Matrix()
{
}

// Returns value of given location when asked in the form A(x,y)
int& Matrix::operator()(const unsigned &rowNo, const unsigned & colNo)
{
    return this->m_matrix[rowNo][colNo];
}

// Returns value of given location when asked in the form A[x]
std::vector<int>& Matrix::operator[](const unsigned &rowNo)
{
    return this->m_matrix[rowNo];
}

// returns # row
unsigned Matrix::getRows() const
{
    return this->m_rowSize;
}

// returns # col
unsigned Matrix::getCols() const
{
    return this->m_colSize;
}

std::vector<std::vector<int>> Matrix::getMatrix() const
{
    return this->m_matrix;
}

int countDigit(int n)
{
    if (n == 0)
        return 1;
    int count = 0;
    while (n != 0) {
        n = n / 10;
        ++count;
    }
    return count;
}

// Prints the matrix beautifully
void Matrix::print() const
{
    int biggest_width = 0;
    int new_width = 0;
    for (unsigned i = 0; i < m_rowSize; i++) {
        for (unsigned j = 0; j < m_colSize; j++) {
            new_width = countDigit(m_matrix[i][j]);
            biggest_width = new_width > biggest_width ? new_width : biggest_width;
        }
    }    
    for (unsigned i = 0; i < m_rowSize; i++) {
        for (unsigned j = 0; j < m_colSize; j++) {
            std::cout << " " << std::setw(biggest_width + 2)  << m_matrix[i][j];
        }
        std::cout << std::endl;
    }
}
