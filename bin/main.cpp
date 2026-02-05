#include "../lib/allocators/pool_allocator.hpp"
#include <chrono>
#include <iostream>
#include <cstdint>

int main() {
    auto start = std::chrono::steady_clock::now();

    uint32_t a = 0;
    for (size_t index = 0; index < 10000000000; ++index) {
        a=!a;
    }

    auto end = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << a << std::endl << sizeof(a);
    std::cout << "Time: " << elapsed << " ms" << std::endl;
}
