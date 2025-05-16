#ifndef MY_BROADCAST_HPP
#define MY_BROADCAST_HPP

#include <mpi.h>

template <typename T>
void my_broadcast (T* data, int count, int root, MPI_Comm comm) {
}

template <>
void my_broadcast (int* data, int count, int root, MPI_Comm comm) {
    MPI_Bcast(data, count, MPI_INT, root, comm);
}
template <>
void my_broadcast (double* data, int count, int root, MPI_Comm comm) {
    MPI_Bcast(data, count, MPI_DOUBLE, root, comm);
}
template <>
void my_broadcast (float* data, int count, int root, MPI_Comm comm) {
    MPI_Bcast(data, count, MPI_FLOAT, root, comm);
}
template <>
void my_broadcast (char* data, int count, int root, MPI_Comm comm) {
    MPI_Bcast(data, count, MPI_CHAR, root, comm);
}

#endif