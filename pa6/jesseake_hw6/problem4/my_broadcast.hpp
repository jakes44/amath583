#ifndef MY_BROADCAST_HPP
#define MY_BROADCAST_HPP

#include <mpi.h>

template <typename T>
void my_broadcast (T* data, int count, int root, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    if (rank == root) {
        for (int i = 0; i < size; ++i) {
            if (i != root) {
                MPI_Send(data, count, MPI_BYTE, i, 0, comm);
            }
        }
    } else {
        std::vector<char> recv_data(count);
        // All other processes receive data from the root process
        MPI_Recv(recv_data.data(), count, MPI_BYTE, root, 0, comm, MPI_STATUS_IGNORE);
    }
}

#endif