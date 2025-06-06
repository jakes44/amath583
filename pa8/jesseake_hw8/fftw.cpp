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
#include <fftw3.h>
// g++ -c -std=c++17 -m64 -I/gscratch/amath/roche-libs/include fftw.cpp
//  g++ -o fftw fftw.o -L/gscratch/amath/roche-libs/lib -lfftw3 -I m

const double PI = 3.14159265359;

int main()
{

    // timer foo
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    long double elapsed_time = 0.L;
    long double avg_time;
    const int ntrials = 3;

    const double lx = 1.0;
    const double ly = 1.0;
    const double lz = 1.0;

    // Loop on Problem Size
    for (int n = 16; n <= 256; n *= 2)
    {
        // Number of points
        int nxyz = n *n *n;
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

            for(int ix = 0; ix < n; ix++)
            {
                xx[ix] = dx * static_cast<double>(ix) - center_x * dx;
            }

            for(int iy = 0; iy < n; iy++)
            {
                yy[iy] = dy * static_cast<double>(iy) - center_y * dy;
            }
            for(int iz = 0; iz < n; iz++)
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
        fftw_plan plan_f = fftw_plan_dft_3d(n, n, n,
                                            reinterpret_cast<fftw_complex *>(&wave[0]),
                                            reinterpret_cast<fftw_complex *>(&fft_3[0]),
                                            FFTW_FORWARD,
                                            FFTW_MEASURE);

        std::vector<std::complex<double>> d_tmp(nxyz);
        fftw_plan plan_b = fftw_plan_dft_3d(n, n, n,
                                            reinterpret_cast<fftw_complex *>(&fft_3[0]),
                                            reinterpret_cast<fftw_complex *>(&d_tmp[0]),
                                            FFTW_BACKWARD,
                                            FFTW_MEASURE);

        std::vector<std::complex<double>> d_dx(nxyz);
        std::vector<std::complex<double>> d_dy(nxyz);
        std::vector<std::complex<double>> d_dz(nxyz);

        // Now we can compute the gradient
        // perform an experiment
        for (int t = 0; t < ntrials; t++)
        {
            start = std::chrono::high_resolution_clock::now();

            fftw_execute(plan_f);
            for (int j = 0; j < nxyz; j++)
            {
                wave[j] = fft_3[j] / static_cast<double>(nxyz);
                // d_dx
                fft_3[j] = wave[j] * std::complex<double>(0.0, kx[j]);
            }
            fftw_execute(plan_b);
            for (int j = 0; j < nxyz; j++)
            {
                d_dx[j] = d_tmp[j] / static_cast<double>(nxyz);
            }

            for (int j = 0; j < nxyz; j++)
            {
                // d_dy
                fft_3[j] = wave[j] * std::complex<double>(0.0, ky[j]);
            }

            fftw_execute(plan_b);
            for (int j = 0; j < nxyz; j++)
            {
                d_dy[j] = d_tmp[j] / static_cast<double>(nxyz);
            }

            for (int j = 0; j < nxyz; j++)
            {
                // d_dy
                fft_3[j] = wave[j] * std::complex<double>(0.0, kz[j]);
            }

            fftw_execute(plan_b);
            for (int j = 0; j < nxyz; j++)
            {
                d_dz[j] = d_tmp[j] / static_cast<double>(nxyz);
            }
            stop = std::chrono::high_resolution_clock::now();
            duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
            elapsed_time += (duration.count() * 1.e-9); // Convert duration to seconds
        }
        long n_3 = pow(n, 3);
        long ops = 24 * n_3 * log(n) + 9 * n_3;
        avg_time = elapsed_time / static_cast<long double>(ntrials);
        // save or report findings
        std::cout << n << "," << ops / avg_time << "\n";
        // zero time again
        elapsed_time = 0.L;
        fftw_destroy_plan(plan_f);
        fftw_destroy_plan(plan_b);
    }
}