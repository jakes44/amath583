#include "io.hpp"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <random>
#include <cmath>

int main()
{
    // Seed the generator
    std::mt19937 generator(std::random_device{}());
    // Example with doubles between 0.0 and 1.0
    std::uniform_real_distribution<double> double_distribution(0.0, 100.0);

    // timer foo
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast < std::chrono::nanoseconds > (stop - start);
    long double elapsed_time = 0.L;

    for ( int i = 5; i <= 14; i++)
    {
        int n = pow(2,i);
        // Initialize vectors
        std::vector<double> A (n*n);
        // Fill x and y with random values
        // Use std::generate to fill the vector with random numbers
        std::generate(A.begin(), A.end(), [&]() {
            return double_distribution(generator);
        });
       
        std::fstream out("out_"+ std::to_string(n) + ".bin", std::ios::out | std::ios::binary);
        start = std::chrono::high_resolution_clock::now();
        read(A, out);
        stop = std::chrono::high_resolution_clock::now();
        out.close();
        duration = std::chrono::duration_cast < std::chrono::nanoseconds > ( stop - start );
        elapsed_time += ( duration . count () * 1.e-9); // Convert duration to seconds
        
        long n_l = static_cast<long>(n);
        // save or report findings
        std::cout << n << "," << (n_l*n_l*8l)/elapsed_time << "\n";
        // zero time again
        elapsed_time = 0.L;
    }
}