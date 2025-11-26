#include "../lib/string.hpp"

int main() {
    mystd::String s(5, '9') ,s2(5, 'p');
    s2.Upper();
    (s+s2).Print();
}