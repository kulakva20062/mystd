#include <iostream>
#include "../lib/maths/mystdint.hpp"
#include <bitset>

int main() {
    mystd::UIntN_t<32> a(10U);
    mystd::UIntN_t<32> b(20U);
    a *= b;
    std::bitset<32> a_bits(a.Data());
    std::cout << a_bits << std::endl;
}