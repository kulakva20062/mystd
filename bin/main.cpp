#include "../lib/allocators/pool_allocator.hpp"
#include <chrono>
#include <iostream>

namespace gg {
    class Test {
        public:
            Test(int index) : index(index) {}

            // friend std::ostream& operator<<(std::ostream& os, const Test& test);

        private:
            int index;
    };

    std::ostream& operator<<(std::ostream& os, const Test& test) {
        os << 4;
        return os;
    }
}


int main() {
    gg::Test test(1);
    std::cout << test;
    return 0;
}
