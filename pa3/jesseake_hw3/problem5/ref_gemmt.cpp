#include "ref_gemmt.hpp"
#include <stdexcept>

template < typename T >
void gemm(T a, const std::vector<std::vector<T>> &A, const std::vector<std::vector<T>> &B, T b, std::vector<std::vector<T>> & C) {
    // Check if the sizes of A, B, and C are compatible
    // Columns of A must equal rows of B for multiplication
    // Result of that must be same dimensions of C
    for (size_t i = 0; i < C.size(); i++) {
        for (size_t j = 0; j < C[i].size(); j++) {
            C[i][j] *= b; // Scale C by b
            for (size_t p = 0; p < A[i].size(); p++) {
                if (A[p].size() != B.size() || B[p].size() != C[i].size()  || A.size() != C.size()) {
                    throw std::invalid_argument("Incompatible dimensions for Matrix-Matrix Multiplication (Columns of A must = Rows of B).");
                }
                C[i][j] += a * A[i][p] * B[p][j];
            }
        }
    }
}

template void gemm(int a, const std::vector<std::vector<int>> &A, const std::vector<std::vector<int>> &B, int b, std::vector<std::vector<int>> & C);
template void gemm(float a, const std::vector<std::vector<float>> &A, const std::vector<std::vector<float>> &B, float b, std::vector<std::vector<float>> & C);
template void gemm(double a, const std::vector<std::vector<double>> &A, const std::vector<std::vector<double>> &B, double b, std::vector<std::vector<double>> & C);
