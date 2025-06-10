#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <cstring>
#include <png.h>

void compute_grayscale(png_bytep* image, int width, int height, int channels, int numThreads, int i){
    // Parallel On rows, don't need to lock anything as long as threads only operate on indpendent rows
    int start = i * (height / numThreads);
    int end = (i == numThreads - 1) ? height: (i + 1) * (height / numThreads);

    for (int j = start; j < end; j++){
        png_bytep row = image[j];
        for(int x = 0; x < width; ++x){
            png_bytep px = &(row[x * channels]);
            // This assumes 3 channels, RGB
            uint8_t gray = static_cast<uint8_t>(0.3 * px[0] + 0.59 * px[1] + 0.11 * px[2]);
            px[0] = px[1] = px[2] = gray;
        }
    }
}

//threaded solution
void grayscaleThreaded(png_bytep* image, int width, int height, int channels, int numThreads) {
    
    std::vector<std::thread> threads(numThreads);
    //Spawn threads
    for (int i = 0; i < numThreads; i++) {
         threads[i] = std::thread(compute_grayscale, image, width, height, channels, numThreads, i);
    }

    // Join threads
    for (int i = 0; i < numThreads; i++) {
        threads[i].join();
    }
}
