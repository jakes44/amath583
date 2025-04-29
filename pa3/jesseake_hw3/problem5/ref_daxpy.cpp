#include "ref_daxpy.hpp"
#include <iostream> 
#include <stdexcept>

void daxpy(double a, const std::vector<double>& x, std::vector<double>& y) {
   // Check if the sizes of x and y are equal
    if (x.size() != y.size()) {
        throw std::invalid_argument("Vectors x and y must have the same size.");
    }

    // Perform the DAXPY operation
    // Loop n times, where n is the size of the vectors
    // Perform 2 flops per loop (mutiply and add)
    for (int i = 0;  i < x.size(); i++) {
        y[i] += a * x[i];
    }
}
