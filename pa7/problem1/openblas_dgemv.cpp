#include <cblas.h>
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>

int main()
{
    // Seed the generator
    std::mt19937 generator(std::random_device{}());
    // Example with doubles between 0.0 and 1.0
    std::uniform_real_distribution<double> double_distribution(0.0, 100.0);

    // timer foo
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now ();
    auto duration = std::chrono::duration_cast < std::chrono::nanoseconds > (stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 3;
    // loop on problem size

    for ( int n = 2;n <= 4096; n *= 2)
    {
        // Random a
        double a = double_distribution(generator);
        // Random b
        double b = double_distribution(generator);
        
        // Vectors
        std::vector<double> x (n);
        std::vector<double> y  (n);

        // A Matrix
        std::vector<double> A (n*n);
        // Fill A with random values
        std::generate(A.begin(), A.end(), [&]() {
            return double_distribution(generator);
         });

        // Fill x and y with random values
        // Use std::generate to fill the vector with random numbers
        std::generate(x.begin(), x.end(), [&]() {
            return double_distribution(generator);
         });
         std::generate(y.begin(), y.end(), [&]() {
            return double_distribution(generator);
         });

        // perform an experiment
        for ( int t = 0; t < ntrials ; t ++)
        {
            start = std::chrono::high_resolution_clock::now();
            cblas_dgemv(CblasColMajor, CblasNoTrans, n, n, a, A.data(),n, x.data(), 1, b, y.data(),1);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast < std::chrono::nanoseconds > ( stop - start );
            elapsed_time += ( duration . count () * 1.e-9); // Convert duration to seconds
        }
        avg_time = elapsed_time / static_cast < long double >( ntrials );
        // save or report findings
        std::cout << n << "," << (2*n*n)/avg_time << "\n";
        // zero time again
        elapsed_time = 0.L;
    }
}