#include "../lib/vector.h"
#include <cstdio>

int main() {
    FILE* file = fopen("text", "wb+");
    for (size_t index = 0; index < 1580000000; ++index) {
        fwrite("s", sizeof(char), 1, file);
    }
    fclose(file);
}