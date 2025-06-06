#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include <algorithm>
#include <cuda_runtime.h>

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double elapsed_time = 0.L;

    for (long n = 1; n <= 2.5e9L; n *= 2)
    {
        // Random Char, each char is a byte, so can go up that way per bytes
        char* A = new char[n];
        for (long i = 0; i < n; i++)
        {
            A[i] = 'a' + (rand() % 26); 
        }

        // Allocate device memory
        char *d_A;
        cudaMalloc(&d_A, n * sizeof(char));

        start = std::chrono::high_resolution_clock::now();
        // Copy data to device
        cudaMemcpy(d_A, A, n * sizeof(char), cudaMemcpyHostToDevice);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        elapsed_time += (duration.count() * 1.e-9);

        long double bytes_per_sec_h2d = static_cast<long double>(n) / elapsed_time;
        
        start = std::chrono::high_resolution_clock::now();
        // Copy data to device
        cudaMemcpy(A, d_A, n * sizeof(char), cudaMemcpyDeviceToHost);
        stop = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
        elapsed_time += (duration.count() * 1.e-9);

        long double bytes_per_sec_d2h = static_cast<long double>(n) / elapsed_time;

        std::cout << n << "," << bytes_per_sec_h2d << "," << bytes_per_sec_d2h << "\n";

        // Free host memory
        delete[] A;
        // Free device memory
        cudaFree(d_A);
    }

    return 0;
}