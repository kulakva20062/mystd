#include "containers/string.hpp"
#include "loggers/logger.hpp"
#include <iostream>

int main() {
    Logger Logger("log.txt");
    Logger.Log("Hello World!");

    Logger.Log("Hello World! 2");
}
