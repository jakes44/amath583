#include "ref_dgemv.hpp"
#include <iostream>
#include <stdexcept>

void dgemv(double a, const std::vector <std::vector<double>>& A,
    const std::vector<double>& x , double b, std::vector <double>& y) {
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
    for (int i = 0; i < A.size(); i++) {
        if (A[i].size() != x.size()) {
            throw std::invalid_argument("Incompatible dimensions for Matrix-Vector Addition (Columns of A must = Rows of X).");
        }
        y[i] *= b; // Scale y by b
        for (int j = 0; j < A[i].size(); j++) {
            y[i] += a * A[i][j] * x[j];
        }
    }
}