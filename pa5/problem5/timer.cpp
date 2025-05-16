#include <iostream>
#include <stdexcept>
#include <chrono>
#include <random>
#include <cmath>
#include "strassen.cpp"

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

    // n= 2...512
    for ( int i = 1; i <= 9; i++)
    {
        int n = pow(2,i);
        // Example with doubles between 0.0 and 1.0
        std::uniform_real_distribution<double> double_distribution(0.0, 100.0);

        // Initialize vectors
        std::vector<std::vector<double>> A(n, std::vector<double>(n));
        std::vector<std::vector<double>> B(n, std::vector<double>(n));
        // Fill x and y with random values
        // Use std::generate to fill the vector with random numbers
        std::generate(A.begin(), A.end(), [&]() {
            std::vector<double> row(n);
            std::generate(row.begin(), row.end(), [&]() {
                return double_distribution(generator);
            });
            return row;
        });

        std::generate(B.begin(), B.end(), [&]() {
            std::vector<double> row(n);
            std::generate(row.begin(), row.end(), [&]() {
                return double_distribution(generator);
            });
            return row;
        });

        // perform an experiment
        for ( int t = 0; t < ntrials ; t ++)
        {
            start = std::chrono::high_resolution_clock::now();
            auto C = strassenMultiply(A, B);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast < std::chrono::nanoseconds > ( stop - start );
            elapsed_time += ( duration . count () * 1.e-9); // Convert duration to seconds
        }
        avg_time = elapsed_time / static_cast < long double >( ntrials );
        long double flops = 7 * pow(n, 2.81);
        
        // save or report findings
        std::cout << n << "," << flops/avg_time << "\n";
        // zero time again
        elapsed_time = 0.L;
    }
}