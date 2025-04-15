#include <iostream>

int main() {
    
    unsigned int counter = 0;
    for (int i = 0; i < 3; ++ i) --counter;

    std::cout << "Counter: " << counter << std::endl;
    return 0;
}