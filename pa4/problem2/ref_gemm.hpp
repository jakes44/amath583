#ifndef REF_GEMM_HPP
#define REF_GEMM_HPP
#include <vector>

template <typename T>
void mm_jki(T a, const std::vector<T>& A, const std::vector<T>& B, T b, std::vector<T>& C, int m, int p, int n);

template <typename T>
void mm_kij(T a, const std::vector<T>& A, const std::vector<T>& B ,T b , std::vector<T>& C , int m, int p, int n);

#endif