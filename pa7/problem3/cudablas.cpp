#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cublas_v2.h>
#include <cuda_runtime.h>

int main()
{
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> double_distribution(0.0, 100.0);

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 3;

    cublasHandle_t handle;
    cublasCreate(&handle);

    for (int n = 2; n <= 16384; n *= 2)
    {
        double a = double_distribution(generator);
        double b = double_distribution(generator);
        int k = n;

        std::vector<double> A(n * n);
        std::vector<double> B(n * n);
        std::vector<double> C(n * n);

        std::generate(B.begin(), B.end(), [&]() { return double_distribution(generator); });
        std::generate(C.begin(), C.end(), [&]() { return double_distribution(generator); });
        std::generate(A.begin(), A.end(), [&]() { return double_distribution(generator); });

        // Allocate device memory
        double *d_A, *d_B, *d_C;
        cudaMalloc(&d_A, n * n * sizeof(double));
        cudaMalloc(&d_B, n * n * sizeof(double));
        cudaMalloc(&d_C, n * n * sizeof(double));

        // Copy data to device
        cudaMemcpy(d_A, A.data(), n * n * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_B, B.data(), n * n * sizeof(double), cudaMemcpyHostToDevice);
        cudaMemcpy(d_C, C.data(), n * n * sizeof(double), cudaMemcpyHostToDevice);

        for (int t = 0; t < ntrials; t++)
        {
            cudaDeviceSynchronize();
            start = std::chrono::high_resolution_clock::now();

            cublasDgemm(
                handle,
                CUBLAS_OP_N, CUBLAS_OP_N,
                n, n, k,
                &a,
                d_A, n,
                d_B, n,
                &b,
                d_C, n
            );

            cudaDeviceSynchronize();
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9);
        }
        avg_time = elapsed_time / static_cast<long double>(ntrials);
        long double flops = static_cast<long double>(2.0 * n * n * k) / avg_time;
        std::cout << n << "," << flops << "\n";
        elapsed_time = 0.L;

        cudaFree(d_A);
        cudaFree(d_B);
        cudaFree(d_C);
    }

    cublasDestroy(handle);
}