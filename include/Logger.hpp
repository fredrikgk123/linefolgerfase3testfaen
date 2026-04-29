#pragma once
#include <Arduino.h>

class Logger {
public:
    static const uint16_t CAPACITY = 1500;  // To buffere i RAM - 1500 samples hver

    struct Sample {
        uint32_t t_ms;
        int16_t  error;
        int16_t  correction;
    };

    struct EncoderSample {
        uint32_t t_ms;
        long     leftPulses;
        long     rightPulses;
        float    leftRPM;
        float    rightRPM;
    };

    void start();
    void stop();
    void reset();
    void record(int16_t error, int16_t correction);
    void recordEncoder(long leftPulses, long rightPulses, float leftRPM, float rightRPM);

    bool     isLogging()          const { return logging; }
    uint16_t count()              const { return index; }
    uint16_t encoderCount()       const { return encoderIndex; }
    uint16_t capacity()           const { return CAPACITY; }
    bool     isFull()             const { return index >= CAPACITY; }

    const Sample&        at(uint16_t i)        const { return buffer[i]; }
    const EncoderSample& encoderAt(uint16_t i) const { return encoderBuffer[i]; }

private:
    Sample        buffer[CAPACITY];
    EncoderSample encoderBuffer[CAPACITY];
    uint16_t      index        = 0;
    uint16_t      encoderIndex = 0;
    bool          logging      = false;
    uint32_t      startTime    = 0;
};