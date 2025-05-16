#include "strassen.cpp"
#include <iostream>

int main() {

    // Initialize a vector
    std::vector<std::vector<double>> A = {{1.0, 2.0, 3.0, 4.0}, {5.0, 6.0, 7.0, 8.0 }, {9.0,10.0,11.0, 12.0}, {13.0, 14.0, 15.0, 16.0}};
    std::vector<std::vector<double>> B = {{1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0}};

    std::vector<std::vector<double>> D = {{1.0, 2.0}, {3.0, 4.0}};
    std::vector<std::vector<double>> E = {{1.0, 1.0}, {1.0, 1.0}};

    // Print the vector
    std::cout << "Multiplying Matrices: " << std::endl;
    printMatrix(A);
    std::cout << "and" << std::endl;
    printMatrix(B);
    std::cout << "Result: " << std::endl;
    auto C = strassenMultiply(A, B);
    printMatrix(C);

    std::cout << "Multiplying Matrices: " << std::endl;
    printMatrix(D);
    std::cout << "and" << std::endl;
    printMatrix(E);
    std::cout << "Result: " << std::endl;
    auto F = strassenMultiply(D, E);
    printMatrix(F);

    // Wrrong dimensions
    std::cout << "Multiplying Matrices: " << std::endl;
    printMatrix(A);
    std::cout << "and" << std::endl;
    printMatrix(D);
    std::cout << "Result: " << std::endl;
    try {
        auto G = strassenMultiply(A, D);
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Non 2^n dimensions
    std::vector<std::vector<double>> H = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
    std::vector<std::vector<double>> I = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
    std::cout << "Multiplying Matrices: " << std::endl;
    printMatrix(H);
    std::cout << "and" << std::endl;
    printMatrix(I);
    std::cout << "Result: " << std::endl;
    try {
        auto J = strassenMultiply(H, I);
    } catch (const std::invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
