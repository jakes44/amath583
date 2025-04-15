#include <iostream>
#include <cmath>

int main() {
    // Compare SP
    float zero_sp = 0.0f;
    for (int i = 0; i < 32; ++i) {
        float out = (1.0f + 1.0f/std::pow(2.0f, float(i))) - 1.0f;
        std::cout << "Iteration: " << i << " SP: " << out << std::endl;
        if (out == 0.0f) {
            break;
        }
    }

    // Compare DP
    double zero_dp = 0.0;
    for (int i = 0; i < 64; ++i) {
        double out = (1.0 + 1.0/std::pow(2.0, double(i))) - 1.0;
        std::cout << "Iteration: " << i << " DP: " << out << std::endl;
        if (out == 0.0) {
            break;
        }
    }
}