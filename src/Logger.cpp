#include "Logger.hpp"

void Logger::start() {
    index = 0;
    startTime = millis();
    logging = true;
}

void Logger::stop() {
    logging = false;
}

void Logger::reset() {
    logging = false;
    index = 0;
}

void Logger::record(int16_t error, int16_t correction) {
    if (!logging) return;
    if (index >= CAPACITY) {
        logging = false;  // auto-stopp når full
        return;
    }
    buffer[index].t_ms       = millis() - startTime;
    buffer[index].error      = error;
    buffer[index].correction = correction;
    index++;
}