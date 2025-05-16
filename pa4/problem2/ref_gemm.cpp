#include "ref_gemm.hpp"

// Column Major Order
// M*N matrix elements
template <typename T>
void mm_jki(T a, const std::vector<T> &A, const std::vector<T> &B, T b, std::vector<T> & C, int m , int p , int n ) {
    // Check if the sizes of A, B, and C are compatible
    if (A.size() != m * p || B.size() != p * n || C.size() != m * n) {
        throw std::invalid_argument("Incompatible dimensions for Matrix-Matrix Multiplication (Columns of A must = Rows of B).");
    }

    for (int i = 0; i < m * n; i++) {
        C[i] *= b;
    }

    // Perform the matrix multiplication
    // C = a * A * B + b * C
    for (int j = 0; j < n; j++) {
        for (int k = 0; k < p; k++) {
            for (int i = 0; i < m; i++) {
                C[i * n + j] += a * A[i * p + k] * B[k * n + j];
            }
        }
    }
}

template <typename T>
void mm_kij(T a, const std::vector<T> &A, const std::vector<T> &B, T b, std::vector<T> & C, int m , int p , int n ) {
    // Check if the sizes of A, B, and C are compatible
    if (A.size() != m * p || B.size() != p * n || C.size() != m * n) {
        throw std::invalid_argument("Incompatible dimensions for Matrix-Matrix Multiplication (Columns of A must = Rows of B).");
    }

    for (int i = 0; i < m * n; i++) {
        C[i] *= b;
    }

    // Perform the matrix multiplication
    for (int k = 0; k < p; k++) {
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                C[i * n + j] += a * A[i * p + k] * B[k * n + j];
            }
        }
    }
}

template void mm_jki(int a, const std::vector<int> &A, const std::vector<int> &B, int b, std::vector<int> & C, int m , int p , int n );
template void mm_jki(float a, const std::vector<float> &A, const std::vector<float> &B, float b, std::vector<float> & C, int m , int p , int n );
template void mm_jki(double a, const std::vector<double> &A, const std::vector<double> &B, double b, std::vector<double> & C, int m , int p , int n );

template void mm_kij(int a, const std::vector<int> &A, const std::vector<int> &B, int b, std::vector<int> & C, int m , int p , int n );
template void mm_kij(float a, const std::vector<float> &A, const std::vector<float> &B, float b, std::vector<float> & C, int m , int p , int n );
template void mm_kij(double a, const std::vector<double> &A, const std::vector<double> &B, double b, std::vector<double> & C, int m , int p , int n );