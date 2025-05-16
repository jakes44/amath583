#include "ref_gemm.hpp"
#include <iostream>
#include <stdexcept>
#include <chrono>
#include <random>

int main()
{
    // Seed the generator
    std::mt19937 generator(std::random_device{}());
    // Example with doubles between 0.0 and 1.0
    std::uniform_real_distribution<float> float_distribution(0.0, 100.0);

    // timer foo
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now ();
    auto duration = std::chrono::duration_cast < std::chrono::nanoseconds > (stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 3;
    // loop on problem size

    for ( int i = 2; i <= 512; i ++)
    {
        // Initialize vectors
        std::vector<float> A (i*i);
        std::vector<float> B (i*i);
        // C All ones
        std::vector<float> C (i*i);
        // Fill A and B with random values
        float a = float_distribution(generator);
        float b = float_distribution(generator);
        // Fill x and y with random values
        // Use std::generate to fill the vector with random numbers
        std::generate(A.begin(), A.end(), [&]() {
            return float_distribution(generator);
         });
         std::generate(B.begin(), B.end(), [&]() {
            return float_distribution(generator);
         });
        std::generate(C.begin(), C.end(), [&]() {
            return float_distribution(generator);
        });

        // perform an experiment
        for ( int t = 0; t < ntrials ; t ++)
        {
            start = std::chrono::high_resolution_clock::now();
            mm_kij(a, A, B, b, C, i, i, i);
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast < std::chrono::nanoseconds > ( stop - start );
            elapsed_time += ( duration . count () * 1.e-9); // Convert duration to seconds
        }
        avg_time = elapsed_time / static_cast < long double >( ntrials );
        // save or report findings
        std::cout << i << "," << (i*i + i*i*i*3)/avg_time << "\n";
        // zero time again
        elapsed_time = 0.L;
    }
}