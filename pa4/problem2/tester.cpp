#include "ref_gemm.hpp"
#include <iostream>
#include <vector>

int main() {
    // Initialize vectors
    std::vector<double> A = {1.0, 3.0, 2.0, 4.0};
    std::vector<double> B = {1.0, 0.0, 0.0, 1.0};
    // C All ones
    std::vector<double> C = {1.0, 1.0, 1.0, 1.0};
    double a = 1.0;
    double b = 0.0;

    mm_jki(a, A, B, b, C, 2, 2, 2);

    // Print the result
    std::cout << "Resulting vector C -- mm_jki: " << std::endl;
    for (size_t i = 0; i < C.size(); i++) {
        std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    // Initialize vectors
    A = {1.0, 3.0, 2.0, 4.0};
    B = {1.0, 0.0, 0.0, 1.0};
    // C All ones
    C = {1.0, 1.0, 1.0, 1.0};
    a = 1.0;
    b = 0.0;

    mm_kij(a, A, B, b, C, 2, 2, 2);
    // Print the result
    std::cout << "Resulting vector C -- mm_kij: " << std::endl;
    for (size_t i = 0; i < C.size(); i++) {
       std::cout << C[i] << " ";
    }
    std::cout << std::endl;

    // // Check mismacthed dimension
    // std::vector<double> z = {1.0, 2.0};
    // std::cout << "Initial vector z: ";
    // for (const double& val : z) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;

    // std::cout << "Attempting aAX + bz: " << std::endl;
    // dgemv(a, A, x, b, z);


    // // Print initial vectors
    // std::cout << "Initial vector x: ";
    // for (const double& val : x) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "Initial vector y: ";
    // for (const double& val : y) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;
    
    // // Perform DAXPY operation
    // daxpy(a, x, y);

    // // Print the result
    // std::cout << "Resulting vector y: ";
    // for (const double& val : y) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;

    // // Check multiply
    // y = {4.0, 5.0, 6.0};
    // a = 2.0;
    
    // std::cout << "Initial vector y: ";
    // for (const double& val : y) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "Multiply by " << a << ": ";

    // daxpy(a, x, y);
    // std::cout << "Resulting vector y: ";
    // for (const double& val : y) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;

    // // Check that vectors of diff length fail
    // std::vector<double> z = {1.0, 2.0};
    // std::cout << "Initial vector z: ";
    // for (const double& val : z) {
    //     std::cout << val << " ";
    // }
    // std::cout << std::endl;
    // std::cout << "Attempting to add vector z to y: " << std::endl;
    // daxpy(a, z, y);



    return 0;
}