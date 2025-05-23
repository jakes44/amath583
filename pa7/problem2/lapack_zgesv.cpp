#include <iostream>
#include <complex>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <chrono>
#include <limits>
#include <cblas.h>
#include <lapacke.h>

int main()
{
    // Plot normalized error vs problem size
    for (int n = 16; n <= 8192; n *= 2)
    {
        int ma = n;
        int na = n;

        int *ipiv = (int *)malloc(sizeof(int) * ma);
        auto a = (std ::complex<double> *)malloc(sizeof(std ::complex<double>) * ma * na);
        auto b = (std ::complex<double> *)malloc(sizeof(std ::complex<double>) * ma);
        auto z = (std ::complex<double> *)malloc(sizeof(std ::complex<double>) * na);

        srand(0);
        int k = 0;
        for (int j = 0; j < na; j++)
        {
            for (int i = 0; i < ma; i++)
            {
                a[k] = 0.5 - (double)rand() / (double)RAND_MAX + std ::complex<double>(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);
                if (i == j)
                    a[k] *= static_cast<double>(ma);
                k++;
            }
        }
        srand(1);
        for (int i = 0; i < ma; i++)
        {
            b[i] = 0.5 - (double)rand() / (double)RAND_MAX + std ::complex<double>(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);
        }

        LAPACKE_zgesv(LAPACK_ROW_MAJOR, n, 1, a, n, ipiv, b, n);

        // Compute Az
        std::vector<std::complex<double>> Az(n, 0.0);
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                Az[i] += a[i * n + j] * z[j];
            }
        }

        // Compute residual r = b - Az
        std::vector<std::complex<double>> r(n);
        for (int i = 0; i < n; i++) {
            r[i] = b[i] - Az[i];
        }

        // Compute l2 norms
        double l2norm_r = 0.0, l2norm_z = 0.0;
        double norm_a = 0.0;
        for (int i = 0; i < n; i++) {
            norm_a += std::norm(a[i * n + i]);
        }
        norm_a = std::sqrt(norm_a);
        for (int i = 0; i < n; i++) {
            l2norm_r += std::norm(r[i]);
            l2norm_z += std::norm(z[i]);
        }
        l2norm_r = std::sqrt(l2norm_r);
        l2norm_z = std::sqrt(l2norm_z);

        double normalized_error = l2norm_r / (norm_a * l2norm_z);

        std::cout << "n = " << n
                  << ", residual = " << l2norm_r
                  << ", normalized error = " << normalized_error << std::endl;

        free(a);
        free(b);
        free(z);
    }
}