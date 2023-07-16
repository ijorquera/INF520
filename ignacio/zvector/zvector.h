#define _USE_MATH_DEFINES
#include <algorithm>
#include <vector>
#include <cmath>
#include <climits>
#include <iostream>

class ZVector {
    private:
        std::vector<int> zv_vector; 
        std::vector<int> int_vector; 
        std::vector<int> gap_vector; 
        std::vector<int> rle_vector; 

    public:
        ZVector(std::vector<std::vector<int>>);
        ZVector(std::vector<int>);
        ~ZVector();

        std::vector<int> getVector() const;
        int& operator[](const unsigned &);

        std::vector<int> newIntVector() const;
        std::vector<int> getIntVector() const;
        int getWorstCaseEntropy() const;

        // Algoritmos de compresion
        // Gap(S)
        std::vector<int> newGapVector() const;
        std::vector<int> getGapVector() const;
        float getGapEntropy() const;
        double get_H0_GapEntropy() const;

        // RunLen(S)
        std::vector<int> newRLEVector() const;
        std::vector<int> getRLEVector() const;
        float getRLEEntropy() const;
        float get_H0_RLEEntropy() const;
};