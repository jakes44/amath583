#include "io.hpp"
#include <iostream>

int main() {

    // Initialize a vector
    std::vector<double> A = {1.0, 2.0, 3.0, 4.0, 5.0};

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
    std::fstream open_file("test.bin", std::ios::in | std::ios::binary);
    if (!open_file) {
        std::cerr << "Error opening file for reading." << std::endl;
        return 1;
    }

    // Read the vector from the file
    read(A, open_file);

    // Print the vector
    for (const auto &elem : A) {
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    return 0;
}
