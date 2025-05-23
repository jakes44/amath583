#include <iostream>
#include <cmath>
#include <chrono>
#include "mpi.h"

double f(double x) {
    return std::sqrt(1.0 + std::pow(((1.0/x) - (1.0/4.0)*x),2));
}

double sequential_reimann_sum(double a, double b, int n) {
    double sum = 0.0;
    double h = (b - a) / n;

    for (int i = 0; i < n; i++) {
        double x = a + (i * h);
        sum += f(x) * h;
    }

    return sum;
}

int main(int argc, char *argv[]) {
    
    MPI_Init(&argc, &argv);

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " n_points" << std::endl;
        MPI_Finalize();
        return 1;
    }
    
    int rank, size;
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int num_points = std::atoi(argv[1]);

    const double a = 1.0;
    const double b = 6.0;

    double local_a = a + rank * (b - a) / size;
    double local_b = a + (rank + 1) * (b - a) / size;
    double local_n = num_points / size;

    start = std::chrono::high_resolution_clock::now();
    double local_sum = sequential_reimann_sum(local_a, local_b, local_n);

    double global_sum = 0.0;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    auto elapsed = (duration.count() * 1e-9); 

    if (rank == 0) {
        std::cout << "Number of points: " << num_points << std::endl;
        std::cout << "Riemann Sum " << a << " to " << b << ": " << global_sum << std::endl;
        std::cerr << global_sum <<","<<  elapsed << "," << num_points << "," << size << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}