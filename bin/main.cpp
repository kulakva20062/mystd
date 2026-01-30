#include "../lib/file_manager/formats/file_txt.hpp"
#include <unistd.h>
#include <vector>
#include <iostream>
#include "../lib/loggers/logger.hpp"

int main() {
    Logger logger;
    logger.Log("Hello, world!");
    sleep(60);
    Logger logger2;
    logger2.Log("Hello, world!");
    return 0;
}
