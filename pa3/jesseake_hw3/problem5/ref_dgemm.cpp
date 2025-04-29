#include "ref_dgemm.hpp"

void dgemm ( double a , const std :: vector < std :: vector < double > > &A ,
    const std :: vector < std :: vector < double > > &B , double b ,
    std :: vector < std :: vector < double > > & C ) {

    // Check if the sizes of A, B, and C are compatible
    // Columns of A must equal rows of B for multiplication
    // Result of that must be same dimensions of C
    for (int i = 0; i < C.size(); i++) {
        for (int j = 0; j < C[i].size(); j++) {
            C[i][j] *= b; // Scale C by b
            for (int p = 0; p < A[i].size(); p++) { // Use A[0].size() for columns of A
                if (A[p].size() != B.size() || B[p].size() != C[i].size()  || A.size() != C.size()) {
                    throw std::invalid_argument("Incompatible dimensions for Matrix-Matrix Multiplication (Columns of A must = Rows of B).");
                }
                C[i][j] += a * A[i][p] * B[p][j];
            }
        }
    }
}