#pragma once
#include <Arduino.h>

class Logger {
public:
    static const uint16_t CAPACITY = 4000;  // ~24 kB RAM

    struct Sample {
        uint32_t t_ms;
        int16_t  error;
        int16_t  correction;
    };

    void start();
    void stop();
    void reset();
    void record(int16_t error, int16_t correction);  // kall i loop

    bool     isLogging()  const { return logging; }
    uint16_t count()      const { return index; }
    uint16_t capacity()   const { return CAPACITY; }
    bool     isFull()     const { return index >= CAPACITY; }

    const Sample& at(uint16_t i) const { return buffer[i]; }

private:
    Sample   buffer[CAPACITY];
    uint16_t index      = 0;
    bool     logging    = false;
    uint32_t startTime  = 0;
};