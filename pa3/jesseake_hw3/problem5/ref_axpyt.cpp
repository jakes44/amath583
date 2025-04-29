#include "ref_axpyt.hpp"
#include <stdexcept>

template<typename T>
void axpy(T a, const std::vector<T> &x, std::vector<T> &y)
{
    // Check if the sizes of x and y are equal
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }

    for (size_t i = 0;  i < x.size(); i++) {
        y[i] += a * x[i];
    }
}

template void axpy(int a, const std::vector<int> &x, std::vector<int> &y);
template void axpy(float a, const std::vector<float> &x, std::vector<float> &y);
template void axpy(double a, const std::vector<double> &x, std::vector<double> &y);