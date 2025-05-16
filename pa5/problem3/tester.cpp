#include "mem_swaps.hpp"
#include <iostream>

int main() {

    // Initialize a vector
    std::vector<double> A = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    int m = 2;
    int n = 3;

    // Print the vector
    std::cout << "Before Row Swap: ";
    for (const auto &elem : A) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    //Swap rows 0 and 1
    // swapRows(A, m, n, 0, 1);
    swapCols(A, m, n, 0, 1);

    // Print the vector
    std::cout << "After Row Swap: ";
    for (const auto &elem : A) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
