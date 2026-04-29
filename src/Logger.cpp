#include "Logger.hpp"

void Logger::start() {
    index        = 0;
    encoderIndex = 0;
    startTime    = millis();
    logging      = true;
}

void Logger::stop() {
    logging = false;
}

void Logger::reset() {
    logging      = false;
    index        = 0;
    encoderIndex = 0;
}

void Logger::record(int16_t error, int16_t correction) {
    if (!logging) return;
    if (index >= CAPACITY) {
        logging = false;
        return;
    }
    buffer[index].t_ms       = millis() - startTime;
    buffer[index].error      = error;
    buffer[index].correction = correction;
    index++;
}

void Logger::recordEncoder(long leftPulses, long rightPulses, float leftRPM, float rightRPM) {
    if (!logging) return;
    if (encoderIndex >= CAPACITY) return;

    encoderBuffer[encoderIndex].t_ms        = millis() - startTime;
    encoderBuffer[encoderIndex].leftPulses  = leftPulses;
    encoderBuffer[encoderIndex].rightPulses = rightPulses;
    encoderBuffer[encoderIndex].leftRPM     = leftRPM;
    encoderBuffer[encoderIndex].rightRPM    = rightRPM;
    encoderIndex++;
}