#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <utility>

template <typename T>
void write(std::vector<T> &A, std::fstream &file) {
    file.write(reinterpret_cast<char*>(&A[0]), sizeof(T) * A.size());
}

template <typename T>
std::vector<T> read(std::vector<T> &A, std::fstream &file){
    file.read(reinterpret_cast<char*>(&A[0]), sizeof(T) * A.size());
    return A;
}