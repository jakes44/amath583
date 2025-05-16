#include "matrix_class.hpp"

#include <stdexcept>
#include <iostream>

int main() {
    // Initialize a 2x2 matrix
    Matrix<double> A(2, 2);
    A(0, 0) = 1.0;
    A(0, 1) = 2.0;
    A(1, 0) = 3.0;
    A(1, 1) = 4.0;

    // Try with a 3x2
    Matrix<double> B(2,3);
    B(0, 0) = 1.0;
    B(0, 1) = 1.0;
    B(0, 2) = 2.0;
    B(1, 0) = 1.0;
    B(1, 1) = 1.0;
    B(1, 2) = 2.0;


    // Print the matrix
    std::cout << "Matrix A: " << std::endl;
    for (int i = 0; i < A.numRows(); i++) {
        for (int j = 0; j < A.numCols(); j++) {
            std::cout << A(i, j) << " ";
        }
        std::cout << std::endl;
    }

    // Print the matrix
    std::cout << "Matrix B: " << std::endl;
    for (int i = 0; i < B.numRows(); i++) {
        for (int j = 0; j < B.numCols(); j++) {
            std::cout << B(i, j) << " ";
        }
        std::cout << std::endl;
    }

    // // Multiply the two matrices
    Matrix<double> C = A * B;
    std::cout << "Multiplying A * B: " << std::endl;
    for (int i = 0; i < C.numRows(); i++) {
        for (int j = 0; j < C.numCols(); j++) {
            std::cout << C(i, j) << " ";
        }
        std::cout << std::endl;
    }

    // Check if failed on bad dimensions
    try {
        Matrix<double> D(1, 3);
        D(0, 0) = 1.0;
        D(0, 1) = 2.0;
        D(0, 2) = 3.0;
        Matrix<double> E = A * D; // This should throw an error
    } catch (const std::invalid_argument &e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    }

    // // Infinity Norm
    // double norm = A.infinityNorm();
    // std::cout << "Infinity Norm of A: " << norm << std::endl;

    // // Transpose
    // Matrix<double> C = A.transpose();
    // std::cout << "Transposed Matrix A: " << std::endl;
    // for (int i = 0; i < C.numRows(); i++) {
    //     for (int j = 0; j < C.numCols(); j++) {
    //         std::cout << C(i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // // Try with a 3x2
    // Matrix<double> B(3, 2);
    // B(0, 0) = 1.0;
    // B(0, 1) = 2.0;
    // B(1, 0) = 3.0;
    // B(1, 1) = 4.0;
    // B(2, 0) = 5.0;
    // B(2, 1) = 6.0;

    // // Print the matrix
    // std::cout << "Matrix B: " << std::endl;
    // for (int i = 0; i < B.numRows(); i++) {
    //     for (int j = 0; j < B.numCols(); j++) {
    //         std::cout << B(i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // // Transpose
    // Matrix<double> D =  B.transpose();
    // std::cout << "Transposed Matrix B: " << std::endl;
    // for (int i = 0; i < D.numRows(); i++) {
    //     for (int j = 0; j < D.numCols(); j++) {
    //         std::cout << D(i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }


    // // Initialize a 2x2 matrix
    // Matrix<double> B(2, 2);
    // B(0, 0) = 1.0;
    // B(0, 1) = 1.0;
    // B(1, 0) = 1.0;
    // B(1, 1) = 1.0;

    // // Add the two together and print
    // Matrix<double> C = A + B;
    // std::cout << "Adding A + B: " << std::endl;
    // for (int i = 0; i < A.numRows(); i++) {
    //     for (int j = 0; j < A.numCols(); j++) {
    //         std::cout << A(i, j) << " ";
    //     }
    //     std::cout << " ";
    //     for (int j = 0; j < B.numCols(); j++) {
    //         std::cout << B(i, j) << "  ";
    //     }
    //     std::cout << std::endl;
    // }

    // std::cout << "Resulting matrix C: " << std::endl;
    // for (int i = 0; i < C.numRows(); i++) {
    //     for (int j = 0; j < C.numCols(); j++) {
    //         std::cout << C(i, j) << " ";
    //     }
    //     std::cout << std::endl;
    // }

    // // Check if failed on bad dimensions
    // try {
    //     Matrix<double> D(2, 3);
    //     D(0, 0) = 1.0;
    //     D(0, 1) = 2.0;
    //     D(1, 0) = 3.0;
    //     D(1, 1) = 4.0;
    //     D(1, 2) = 5.0;
    //     Matrix<double> E = A + D; // This should throw an error
    // } catch (const std::invalid_argument &e) {
    //     std::cerr << "Caught exception: " << e.what() << std::endl;
    // }

    return 0;
}