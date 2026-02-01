#include "../lib/loggers/logger.hpp"

int main() {
    Logger logger;
    logger.Log("Hello, world!", 0);
    logger.Log("critical error", 20);
    return 0;
}
