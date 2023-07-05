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
    this->int_vector = this->newIntVector();
    this->gap_vector = this->newGapVector();
    this->rle_vector = this->newRLEVector();
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
    unsigned long int i = 0;

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

int ZVector::getWorstCaseEntropy() const
{
    // For an integer universe U = [0 ... u), ...
    // denote the class S \subseteq U such that |S| = n
    // We assume S = {x1, x2, ..., xn}, for 0 <= x1 <= ... <= xn < u
    // Worst case we need B(n, u) = ceil(lg(C(u, n))) bits
    // that means, n = amount of integers in our subset S
    //             u = biggest integer in our universe U, such that xn < u, xn \in S
    //
    // if we take the biggest C unsigned integer u = 4294967295 or 0xffffffff, we have that u >> n
    // and since u is too big we can use Stirling to approximate the convergence of the factorial
    //
    // such that the new equation for worst case is
    // B(n, u) = n lg(u/n) + n lg(e) = n (lg(u/n) + lg(e)), which is around O(lg(u)) bits since u >> n

    float u = (float)UINT_MAX;
    float n = (float)this->int_vector.size(); // amount of integers in our subset S

    return n * (log2(u/n) + M_LOG2E); // M_LOG2E equivale a log2(e) y tiene valor definido en cmath
}

std::vector<int> ZVector::newGapVector() const
{
    std::vector<int> new_gap_vector;
    unsigned i = 0;
    int gaps = 0;

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
    // int g_i;
    // int g_i_prev;
    std::cout << "\n";

    // for (i = 0; i < this->gap_vector.size(); i++) {
    for (i = 0; i < this->gap_vector.size() - 1; i++) {
        gapEntropy = gapEntropy + floor(log2(gap_vector[i])) + 1; // paper profe
        std::cout << "gap_vector[" << i << "]: " << this->gap_vector[i]<< " , log(g_" << i << "): " << log2(this->gap_vector[i]) << std::endl;
        // gapEntropy = gapEntropy + ceil(log2(gap_vector[i + 1] - gap_vector[i])); // information retrieval implementing and evaluating search engines, p215 (234 del pdf)
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
        if (this->zv_vector[i] != prev) {
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
    float z_i, l_i;

    std::cout << std::endl;
    // el ciclo asume que rle_vector siempre tendra largo par (secuencia de 0^z 1^l),
    for (i = 0; i < this->rle_vector.size(); i = i + 2) {

        z_i = floor(log2(this->rle_vector[i] - 1)) + 1;
        l_i = floor(log2(this->rle_vector[i + 1] - 1)) + 1;
        std::cout << "rle_v[" << i << "]: " << rle_vector[i] << " , z_" << i << ": " << z_i <<" | rle_v[" << i+1 << "]: " << rle_vector[i+1] << " , l_" << i << ": " << l_i << std::endl;

        rleZerosEntropy = rleZerosEntropy + z_i;
        rleOnesEntropy = rleOnesEntropy + l_i;
    }
    std::cout << "rleZerosEntropy: " << rleZerosEntropy << std::endl;
    std::cout << "rleOnesEntropy: " << rleOnesEntropy << std::endl;
    return rleZerosEntropy + rleOnesEntropy;
}
