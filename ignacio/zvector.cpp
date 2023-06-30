#include "zvector.h"

void zorder(std::vector<int> &vtr, std::vector<std::vector<int>> m, int y0, int x0, int size)
{
    if (size == 1) {
        // Base case, just one cell
        vtr.push_back(m[y0][x0]);
        // std::cout << "y0: " << y0 << ", x0: " << x0 << ", m[y0][x0]: " << m[y0][x0] << std::endl;
    } else {
        // Recurse in Z-order
        int h = size/2;
        zorder(vtr, m, y0,   x0,   h); // top-left
        zorder(vtr, m, y0,   x0+h, h); // top-right
        zorder(vtr, m, y0+h, x0,   h); // bottom-left
        zorder(vtr, m, y0+h, x0+h, h); // bottom-right
    }
}

ZVector::ZVector(std::vector<std::vector<int>> binary_matrix)
{
    zorder(this->zv_vector, binary_matrix, 0, 0, binary_matrix.size());
    this->gap_vector = this->newGapVector();
    this->rle_vector = this->newRLEVector();
    this->int_vector = this->newIntVector();
}

ZVector::~ZVector()
{
}

std::vector<int> ZVector::getVector() const
{
    return this->zv_vector;
}

int &ZVector::operator[](const unsigned &idx)
{
    return this->zv_vector[idx];
}

std::vector<int> ZVector::newIntVector() const
{
    std::vector<int> new_int_vector;
    int i = 0;

    for (i = 0; i < this->zv_vector.size(); i++) {
        if (this->zv_vector[i] == 1) {
            new_int_vector.push_back(i);
        }
    }

    return new_int_vector;
}

std::vector<int> ZVector::getIntVector() const
{
    return this->int_vector;
}

unsigned factorial(unsigned start, unsigned finish)
{
    if (start == 0 || start == 1 || start == finish)
        return 1;
    return start * factorial(start - 1, finish);
}

float combination(unsigned u, unsigned n)
{
    return (float)factorial(u, n) / (float)factorial(u-n, 0);
}

int ZVector::getWorstCaseEntropy() const
{
    // Worst case we need B(n, u) = ceil(lg(C(u, n))) bits
    unsigned u = this->zv_vector.size();
    unsigned n = this->int_vector.size();

    float comb = combination(u, n);

    return ceil(log2(comb));
}

std::vector<int> ZVector::newGapVector() const
{
    std::vector<int> new_gap_vector;
    unsigned i = 0;
    int gaps = 0;
    int prev_gap = -1;

    for (i = 0; i < this->zv_vector.size(); i++) {
        if (this->zv_vector[i] == 0) {
            gaps = gaps + 1;
        }
        else {
            new_gap_vector.push_back(gaps);
            gaps = 0;
        }
    }

    return new_gap_vector;
}

std::vector<int> ZVector::getGapVector() const
{
    return this->gap_vector;
}

float ZVector::getGapEntropy() const
{
    unsigned i;
    float gapEntropy = 0.0f;
    int g_i;
    // std::cout << "\n";

    for (i = 0; i < this->int_vector.size(); i++) {
        if (i == 0) {
            g_i = this->int_vector[i] - 1;
        }
        else {
            g_i = this->int_vector[i] - this->int_vector[i - 1] - 1;
        }
        // std::cout << "int_v[i] : " << this->int_vector[i]<< " , g_i: "  << g_i << " , log(g_i): " << log2(g_i) << std::endl;
        gapEntropy = gapEntropy + floor(log2(g_i)) + 1;
    }
    return gapEntropy;
}

std::vector<int> ZVector::newRLEVector() const
{
    std::vector<int> new_rle_vector;

    unsigned i;
    int count = 0;
    int prev = 0;

    for (i = 0; i <= this->zv_vector.size(); i++) {
        if (i == this->zv_vector.size()) {
            new_rle_vector.push_back(count);
        }
        else if (this->zv_vector[i] != prev) {
            new_rle_vector.push_back(count);
            count = 0;
            prev = this->zv_vector[i];
        }
        count = count + 1;
    }

    // Agregar 1 al final
    return new_rle_vector;
}

std::vector<int> ZVector::getRLEVector() const
{
    return this->rle_vector;
}

float ZVector::getRLEEntropy() const
{
    unsigned i;
    float rleZerosEntropy = 0.0f;
    float rleOnesEntropy = 0.0f;
    float rleEntropy = 0.0f;
    float z_i, l_i;

    std::cout << std::endl;
    for (i = 0; i < this->rle_vector.size(); i = i + 2) {

        z_i = floor(log2(this->rle_vector[i] - 1)) + 1;
        std::cout << "rle_v[i]: " << rle_vector[i] << " , z_i: " << z_i << std::endl;

        rleZerosEntropy = rleZerosEntropy + z_i;
    }
    std::cout << "rleZerosEntropy: " << rleZerosEntropy << std::endl;

    for (i = 1; i < this->rle_vector.size(); i = i + 2) {

        l_i = floor(log2(this->rle_vector[i] - 1)) + 1;
        std::cout << "rle_v[i]: " << rle_vector[i] << " , l_i: " << l_i << std::endl;

        rleOnesEntropy = rleOnesEntropy + l_i;
    }

    std::cout << "rleOnesEntropy: " << rleOnesEntropy << std::endl;
    return rleZerosEntropy + rleOnesEntropy;
}
