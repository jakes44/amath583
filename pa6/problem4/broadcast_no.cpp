#include "mpi.h"
#include <iostream>
#include <vector>
#include <chrono>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        std::cerr << "This program requires at least two processes." << std::endl;
        MPI_Finalize();
        return 1;
    } 

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " n_bytes" << std::endl;
        MPI_Finalize();
        return 1;
    }

    int n_bytes = std::atoi(argv[1]);
    std::vector<char> data(n_bytes, 'a'); // Initialize data with 'a'

    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    // Broadcast the data from root process to all other processes
    start = std::chrono::high_resolution_clock::now();
    
    MPI_Bcast(data.data(), n_bytes, MPI_CHAR, 0, MPI_COMM_WORLD);
    stop = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    auto elapsed = (duration.count() * 1e-9); 

    if (rank == 0) {
        std::cerr << n_bytes <<","<< size <<","<<  elapsed << std::endl;
    }
    
    MPI_Finalize();
    return 0;
}
