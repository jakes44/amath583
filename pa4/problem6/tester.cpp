#include "io.hpp"
#include "file_swaps.hpp"
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

    // Open a file for writing
    std::fstream write_file("test.bin", std::ios::out | std::ios::binary);
    if (!write_file) {
        std::cerr << "Error opening file for writing." << std::endl;
        return 1;
    }

    // Write the vector to the file
    write(A, write_file);

    // Close the file
    write_file.close();

    // Open the file for reading
    std::fstream open_file("test.bin", std::ios::in | std::ios::out | std::ios::binary);
    if (!open_file) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 1;
    }

    //Swap rows 0 and 1
    swapRowsInFile(open_file, m, n, 0, 1);
    // swapColsInFile(open_file, m, n, 0, 1);

    // Close the file
    open_file.close();

    // Open the file for reading
    std::fstream final_file("test.bin", std::ios::in | std::ios::binary);

    std::vector<double> C(m*n);

    // Read the vector from the file
    read(C, final_file);

    // Print the vector
    std::cout << "After Row Swap: ";
    for (const auto &elem : C) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
