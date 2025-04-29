#include "ref_gemvt.hpp"
#include <stdexcept>

template<typename T>
void gemv(T a, const std::vector<std::vector<T>>& A,
    const std::vector<T>& x , T b, std::vector<T>& y) {
    // Check if the sizes of A, x, and y are compatible
    if (A.size() != y.size() ) {
        throw std::invalid_argument("Incompatible dimensions for Matrix-Vector Addition (Rows of A must = Columns of Y).");
    }

    // Perform the DGEMV operation
    // Loop m times, where m is the number of rows in A
    // Loop n times, where n is the number of columns in A
    // Perform 1 flop per outer loop (multiply)
    // Perform 3 flops per inner loop (2 multiply and add)
    // mn
    for (size_t i = 0; i < A.size(); i++) {
        if (A[i].size() != x.size()) {
            throw std::invalid_argument("Incompatible dimensions for Matrix-Vector Addition (Columns of A must = Rows of X).");
        }
        y[i] *= b; // Scale y by b
        for (size_t j = 0; j < A[i].size(); j++) {
            y[i] += a * A[i][j] * x[j];
        }
    }
}

template void gemv(int a, const std::vector<std::vector<int>>& A,
    const std::vector<int>& x , int b, std::vector<int>& y);
template void gemv(float a, const std::vector<std::vector<float>>& A,
    const std::vector<float>& x , float b, std::vector<float>& y);
template void gemv(double a, const std::vector<std::vector<double>>& A,
    const std::vector<double>& x , double b, std::vector<double>& y);