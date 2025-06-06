#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <complex>
#include <iomanip>
#include <cuda.h>
#include <cuda_runtime.h>
#include <cufft.h>

const double PI = 3.14159265359;
// module load gcc cuda
// nvcc -c -arch sm_75 -I/gscratch/amath/roche-libs/include cufft.cu
// g++ -o cufft cufft.o -L/mmfs1/sw/cuda/12.2.2/lib64/ -lcudart -lcufft -lm

__global__ void kr_fast_scale_copy(cufftDoubleComplex *in, cufftDoubleComplex *out, int n, double scale_factor){
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < n){
        out[idx].x = scale_factor * in[idx].x;
        out[idx].y = scale_factor * in[idx].y;
    }
}

__global__ void kr_fast_scale(cufftDoubleComplex *in, int n, double *scale_factor) {
    int idx = threadIdx.x + blockIdx.x * blockDim.x;
    if (idx < n){
        in[idx].x *= scale_factor[idx];
        in[idx].y *= scale_factor[idx];
    }
}

int main()
{
    long double avg_time;
    const int ntrials = 3;

    const double lx = 1.0;
    const double ly = 1.0;
    const double lz = 1.0;

    // Loop on Problem Size
    for (int n = 16; n <= 256; n *= 2)
    {
        // Number of points
        int nxyz = n * n * n;
        const double dx = lx / n;
        const double dy = ly / n;
        const double dz = lz / n;

        std::vector<double> xa(nxyz);
        std::vector<double> ya(nxyz);
        std::vector<double> za(nxyz);

        double center_x = static_cast<double>(n) / 2.0;
        double center_y = static_cast<double>(n) / 2.0;
        double center_z = static_cast<double>(n) / 2.0;
        {
            std::vector<double> xx(n);
            std::vector<double> yy(n);
            std::vector<double> zz(n);

            for (int ix = 0; ix < n; ix++)
            {
                xx[ix] = dx * static_cast<double>(ix) - center_x * dx;
            }

            for (int iy = 0; iy < n; iy++)
            {
                yy[iy] = dy * static_cast<double>(iy) - center_y * dy;
            }
            for (int iz = 0; iz < n; iz++)
            {
                zz[iz] = dz * static_cast<double>(iz) - center_z * dz;
            }

            for (int ix = 0; ix < n; ix++)
            {
                for (int iy = 0; iy < n; iy++)
                {
                    for (int iz = 0; iz < n; iz++)
                    {
                        int index = iz + n * (iy + n * ix);
                        xa[index] = xx[ix];
                        ya[index] = yy[iy];
                        za[index] = zz[iz];
                    }
                }
            }
        }

        cufftDoubleReal *g_kxyz;
        cudaMalloc((void **)&g_kxyz, 3 * nxyz * sizeof(cufftDoubleReal));
        std::vector<double> kx(nxyz);
        std::vector<double> ky(nxyz);
        std::vector<double> kz(nxyz);
        {
            std::vector<double> kkx(n);
            std::vector<double> kky(n);
            std::vector<double> kkz(n);

            int i;
            kkx[0] = 0.;
            kky[0] = 0.;
            kkz[0] = 0.;

            for (i = 1; i <= n / 2 - 1; i++)
            {
                kkx[i] = 2.0 * PI / lx * static_cast<double>(i);
            }
            int j = -i;
            for (i = n / 2; i < n; i++)
            {
                kkx[i] = 2.0 * PI / lx * static_cast<double>(j);
                j++;
            }
            for (i = 1; i <= n / 2 - 1; i++)
            {
                kky[i] = 2.0 * PI / ly * static_cast<double>(i);
            }
            j = -i;
            for (i = n / 2; i < n; i++)
            {
                kky[i] = 2.0 * PI / ly * static_cast<double>(j);
                j++;
            }
            for (i = 1; i <= n / 2 - 1; i++)
            {
                kkz[i] = 2.0 * PI / lz * static_cast<double>(i);
            }
            j = -i;
            for (i = n / 2; i < n; i++)
            {
                kkz[i] = 2.0 * PI / lz * static_cast<double>(j);
                j++;
            }

            for (int ix = 0; ix < n; ix++)
            {
                for (int iy = 0; iy < n; iy++)
                {
                    for (int iz = 0; iz < n; iz++)
                    {
                        int index = iz + n * (iy + n * ix);
                        kx[index] = kkx[ix];
                        ky[index] = kky[iy];
                        kz[index] = kkz[iz];
                    }
                }
            }
        }

        // Copy Lattice to the GPU
        cudaMemcpy(g_kxyz, kx.data(), nxyz * sizeof(cufftDoubleReal), cudaMemcpyHostToDevice);
        cudaMemcpy(g_kxyz + nxyz, ky.data(), nxyz * sizeof(cufftDoubleReal), cudaMemcpyHostToDevice);
        cudaMemcpy(g_kxyz + 2 * nxyz, kz.data(), nxyz * sizeof(cufftDoubleReal), cudaMemcpyHostToDevice);

        // Setup Lattice Plane Wave
        std::vector<std::complex<double>> wave(nxyz);
        for (int ix = 0; ix < n; ix++)
        {
            for (int iy = 0; iy < n; iy++)
            {
                for (int iz = 0; iz < n; iz++)
                {
                    int index = iz + n * (iy + n * ix);
                    wave[index] = std::exp(std::complex<double>(0.0, 1.0) * (xa[index] * kx[index] + ya[index] * ky[index] + za[index] * kz[index]));
                }
            }
        }

        // Calc norm of wave function
        double norm = 0.0;
        for (int j = 0; j < nxyz; j++)
        {
            norm += std::norm(wave[j]);
        }

        // Normalize Wave Function
        double sqrtNorm = std::sqrt(norm);
        for (int j = 0; j < nxyz; j++)
        {
            wave[j] /= sqrtNorm;
        }

        // FFT Plans
        std::vector<std::complex<double>> fft_3(nxyz);
        std::vector<std::complex<double>> d_tmp(nxyz);
        std::vector<std::complex<double>> d_dx(nxyz);
        std::vector<std::complex<double>> d_dy(nxyz);
        std::vector<std::complex<double>> d_dz(nxyz);

        cufftDoubleComplex *g_fft3, *g_wfft;
        cudaMalloc((void **)&g_fft3, sizeof(cufftDoubleComplex) * nxyz);
        cudaMalloc((void **)&g_wfft, sizeof(cufftDoubleComplex) * nxyz);

        cufftHandle g_plan_f;
        cufftHandle g_plan_b;
        cufftCreate(&g_plan_f);
        cufftCreate(&g_plan_b);
        cufftPlan3d(&g_plan_f, n, n, n, CUFFT_C2C);
        cufftPlan3d(&g_plan_b, n, n, n, CUFFT_C2C);

        cufftDoubleComplex *g_ddx, *g_ddy, *g_ddz;
        cudaMalloc((void **)&g_ddx, sizeof(cufftDoubleComplex) * nxyz);
        cudaMalloc((void **)&g_ddy, sizeof(cufftDoubleComplex) * nxyz);
        cudaMalloc((void **)&g_ddz, sizeof(cufftDoubleComplex) * nxyz);

        // Determine Thread Block Count
        static int tpb = 512;
        int nblk = nxyz / tpb;
        if (nxyz % tpb != 0)
        {
            nblk++;
        }

        // Copy wave function to GPU
        cudaMemcpy(g_fft3, wave.data(), nxyz * sizeof(cufftDoubleComplex), cudaMemcpyHostToDevice);

        // Timer Vars
        cudaEvent_t g_start, g_stop;
        float elapsed_time = 0.0;
        cudaEventCreate(&g_start);
        cudaEventCreate(&g_stop);

        // Now we can compute the gradient
        // perform an experiment
        for (int t = 0; t < ntrials; t++)
        {
            cudaEventRecord(g_start, 0);

            if (cufftExecZ2Z(g_plan_f, g_fft3, g_fft3, CUFFT_FORWARD) != CUFFT_SUCCESS)
            {
                std::cout << "Error in cufftExecZ2Z -- Forward Failed" << std::endl;
            }

            // d_dx
            kr_fast_scale_copy<<<nblk, tpb>>>(g_fft3, g_wfft, nxyz, 1. / static_cast<double>(nxyz));
            kr_fast_scale<<<nblk, tpb>>>(g_wfft, nxyz, g_kxyz);
            if (cufftExecZ2Z(g_plan_b, g_wfft, g_wfft, CUFFT_INVERSE) != CUFFT_SUCCESS)
            {
                std::cout << "Error in cufftExecZ2Z -- Inverse Failed" << std::endl;
            }
            kr_fast_scale_copy<<<nblk, tpb>>>(g_wfft, g_ddx, nxyz, 1. / static_cast<double>(nxyz));

            // d_dy
            kr_fast_scale_copy<<<nblk, tpb>>>(g_fft3, g_wfft, nxyz, 1. / static_cast<double>(nxyz));
            kr_fast_scale<<<nblk, tpb>>>(g_wfft, nxyz, g_kxyz + nxyz);
            if (cufftExecZ2Z(g_plan_b, g_wfft, g_wfft, CUFFT_INVERSE) != CUFFT_SUCCESS)
            {
                std::cout << "Error in cufftExecZ2Z -- Inverse Failed" << std::endl;
            }
            kr_fast_scale_copy<<<nblk, tpb>>>(g_wfft, g_ddy, nxyz, 1. / static_cast<double>(nxyz));

            // d_dz
            kr_fast_scale_copy<<<nblk, tpb>>>(g_fft3, g_wfft, nxyz, 1. / static_cast<double>(nxyz));
            kr_fast_scale<<<nblk, tpb>>>(g_wfft, nxyz, g_kxyz + 2 * nxyz);
            if (cufftExecZ2Z(g_plan_b, g_wfft, g_wfft, CUFFT_INVERSE) != CUFFT_SUCCESS)
            {
                std::cout << "Error in cufftExecZ2Z -- Inverse Failed" << std::endl;
            }
            kr_fast_scale_copy<<<nblk, tpb>>>(g_wfft, g_ddz, nxyz, 1. / static_cast<double>(nxyz));

            cudaDeviceSynchronize();
            cudaEventRecord(g_stop, 0);
            cudaEventSynchronize(g_stop);
            float cuda_time;
            cudaEventElapsedTime(&cuda_time, g_start, g_stop);
            elapsed_time += (cuda_time * 1.e-3); // Convert duration to seconds
        }
        long n_3 = pow(n, 3);
        long ops = 24 * n_3 * log(n) + 9 * n_3;
        avg_time = elapsed_time / static_cast<float>(ntrials);
        // save or report findings
        std::cout << n << "," << ops / avg_time << "\n";
        // zero time again
        elapsed_time = 0.0;
    }
}