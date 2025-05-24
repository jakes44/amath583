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
    for (int n = 16; n <= 8192; n *= 2)
    {
        int ma = n, na = n;
        lapack_complex_double* a = (lapack_complex_double *)malloc(sizeof(lapack_complex_double) * ma * na);
        lapack_complex_double*  b = (lapack_complex_double *)malloc(sizeof(lapack_complex_double) * ma);
        lapack_complex_double* z = (lapack_complex_double *)malloc(sizeof(lapack_complex_double) * na);
        lapack_complex_double* B = (lapack_complex_double *)malloc(sizeof(lapack_complex_double) * ma);
        lapack_complex_double* A = (lapack_complex_double *)malloc(sizeof(lapack_complex_double) * ma * na);
        lapack_int* ipiv = (lapack_int *)malloc(sizeof(lapack_int) * n);

        srand(0);
        int k = 0;
        for (int j = 0; j < na; j++)
        {
            for (int i = 0; i < ma; i++)
            {
                a[k] = 0.5 - (double)rand() / (double)RAND_MAX + lapack_make_complex_double(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);
                if (i == j)
                    a[k] *= static_cast<double>(ma);
                k++;
            }
        }
        srand(1);
        for (int i = 0; i < ma; i++)
        {
            b[i] = 0.5 - (double)rand() / (double)RAND_MAX + lapack_make_complex_double(0, 1) * (0.5 - (double)rand() / (double)RAND_MAX);
        }

        memcpy(B, b, sizeof(lapack_complex_double) * ma);
        memcpy(A, a, sizeof(lapack_complex_double) * ma * na);
        LAPACKE_zgesv(LAPACK_ROW_MAJOR, 1, n, a, n, ipiv, b, n);
        memcpy(z, b, sizeof(lapack_complex_double) * na);

        // A*Z = B
        // Residual is ||A*Z - B||
        // Compute the residual
        // Norm of Z
        double normz = cblas_dznrm2(na, z, 1);
        // Inf norm of A
        double norma = cblas_izmax(na*ma, A, 1);

        // Gemv for matric vector multiplication of A and Z
        lapack_complex_double* a_z = (lapack_complex_double *)malloc(sizeof(lapack_complex_double) * na);
        double alpha = 1.0;
        double beta = 0.0;

        void* alpha_ptr = static_cast<void*>(&alpha);
        void* beta_ptr = static_cast<void*>(&beta);
        
        cblas_zgemv(CblasRowMajor, CblasNoTrans, ma, na,alpha_ptr, A, na, z, 1, beta_ptr, a_z, 1);
        // Error = ||A*Z - B||_2 / (||A||_inf * ||Z||)_2
        lapack_complex_double* a_z_b = (lapack_complex_double *)malloc(sizeof(lapack_complex_double) * na);
        memcpy(a_z_b, a_z, sizeof(lapack_complex_double) * na);
        
        double alpha_2 = -1.0;
        void* alpha_ptr_2 = static_cast<void*>(&alpha);
        cblas_zaxpy(na, alpha_ptr_2, B, 1, a_z_b, 1);
        double l2norm_r = cblas_dznrm2(na, a_z_b, 1);
        double normalized_error = l2norm_r / (norma * normz);

        std::cout << n << "," << l2norm_r<< "," << normalized_error << std::endl;

    }
    return 0;
}