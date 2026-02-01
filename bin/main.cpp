#include "../lib/loggers/logger.hpp"

int main() {
    Logger logger;
    logger.Log("Hello, world!", 0);
    logger.Log("Hello, world!", 1);
    logger.CleanLogs(2, 2);
    return 0;
}
