#ifndef JESSEAKE_REFDGEMV_HPP
#define JESSEAKE_REFDGEMV_HPP

#include <vector>

void dgemv(double a, const std::vector <std::vector<double>>& A,
    const std::vector<double>& x , double b, std::vector <double>& y);

#endif