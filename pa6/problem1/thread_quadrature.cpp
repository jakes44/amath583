#include <thread>
#include <iostream>
#include <cmath>
#include <mutex>
#include <vector>
#include <chrono>

double f(double x) {
    return std::sqrt(1.0 + std::pow(((1.0/x) - (1.0/4.0)*x),2));
}

void compute_partial_sums(double a, double h, double &sum, std::mutex &mutex, int n, int num_threads, int i) {
    double local_sum = 0.0;
    int start = i * (n / num_threads);
    int end = (i == num_threads - 1) ? n : (i + 1) * (n / num_threads);
    for (int j = start; j < end; j++) {
        double x = a + (j * h);
        local_sum += f(x);
    }
    local_sum *= h;

    mutex.lock();
    sum += local_sum;
    mutex.unlock();
}

double parallel_reimann_sum(double a, double b, int n, int num_threads) {
    double sum = 0.0;
    double h = (b - a) / n;

    // Protect Sum Variable
    std::mutex mutex;
    std::vector<std::thread> threads(num_threads);

    //Spawn threads
   for (int i = 0; i < num_threads; i++) {
        threads[i] = std::thread(compute_partial_sums, a, h, std::ref(sum), std::ref(mutex), n, num_threads, i);
    }

    // Join threads
    for (int i = 0; i < num_threads; i++) {
        threads[i].join();
    }

    return sum;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " n_points n_threads" << std::endl;
        return 1;
    }

    int num_points = std::atoi(argv[1]);
    int num_threads = std::atoi(argv[2]);

    const double a = 1.0;
    const double b = 6.0;

    // Timer Fooo foo
    auto start = std::chrono::high_resolution_clock::now();
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);

    std::cout << "Using " << num_threads << " threads to compute the Riemann sum with " << num_points << " points. ";
    std::cout << "On Interval [" << a << ", " << b << "]" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    double par_sum = parallel_reimann_sum(a, b, num_points, num_threads);
    stop = std::chrono::high_resolution_clock::now();

    duration = std::chrono::duration_cast<std::chrono::nanoseconds>(stop - start);
    auto elapsed = (duration.count() * 1e-9); 
    std::cout << "Parallel Riemann Sum: " << par_sum << " Elapsed time: " << elapsed << " seconds" <<std::endl;

    std::cerr << par_sum <<","<<  elapsed << "," << num_points << "," << num_threads << std::endl;
    return 0;
}