#include "file_swaps.hpp"
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

    // timer foo
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast < std::chrono::nanoseconds > (stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 3;

    for ( int i = 4; i <= 13; i++)
    {
        int n = pow(2,i);
        std::uniform_int_distribution<int> int_distribution(0,n-1);
        // Example with doubles between 0.0 and 1.0
        std::uniform_real_distribution<double> double_distribution(0.0, 100.0);
        int random_i = int_distribution(generator);
        int random_j = int_distribution(generator);

        // Initialize vectors
        std::vector<double> A (n*n);
        // Fill x and y with random values
        // Use std::generate to fill the vector with random numbers
        std::generate(A.begin(), A.end(), [&]() {
            return double_distribution(generator);
        });
        std::string filename = "out_"+ std::to_string(n) + ".bin";
        std::fstream out(filename, std::ios::out | std::ios::binary);
        write(A, out);
        out.close();

        // perform an experiment
        for ( int t = 0; t < ntrials ; t ++)
        {
            std::fstream swap(filename, std::ios::in | std::ios::out | std::ios::binary);
            if (!swap) {
                std::cerr << "Error opening file for reading/writing." << std::endl;
                return 1;
            }
            start = std::chrono::high_resolution_clock::now();
            // swapRowsInFile(swap, n, n, random_i, random_j);
            swapColsInFile(swap, n, n, random_i, random_j);
            stop = std::chrono::high_resolution_clock::now();
            swap.close();
            duration = std::chrono::duration_cast < std::chrono::nanoseconds > ( stop - start );
            elapsed_time += ( duration . count () * 1.e-9); // Convert duration to seconds
        }
        avg_time = elapsed_time / static_cast < long double >( ntrials );
        
        // save or report findings
        std::cout << n << "," << avg_time << "\n";
        // zero time again
        elapsed_time = 0.L;
        std::remove(filename.c_str());
    }
}