#include "Encoder.hpp"

volatile long Encoder::leftPulses  = 0;
volatile long Encoder::rightPulses = 0;

void IRAM_ATTR Encoder::isrLeftA() {
    leftPulses++;
}

void IRAM_ATTR Encoder::isrRightA() {
    rightPulses++;
}

void Encoder::begin() {
    pinMode(A_LEFT,  INPUT);
    pinMode(B_LEFT,  INPUT);
    pinMode(A_RIGHT, INPUT);
    pinMode(B_RIGHT, INPUT);

    attachInterrupt(digitalPinToInterrupt(A_LEFT),  isrLeftA,  RISING);
    attachInterrupt(digitalPinToInterrupt(A_RIGHT), isrRightA, RISING);

    lastRPMTime = millis();
}

void Encoder::update() {
    unsigned long now = millis();
    float dt = (now - lastRPMTime) / 1000.0f;

    // Oppdater RPM hvert 100 ms
    if (dt >= 0.1f) {
        long currentLeft  = leftPulses;
        long currentRight = rightPulses;

        long deltLeft  = currentLeft  - lastLeftPulses;
        long deltRight = currentRight - lastRightPulses;

        // RPM = (pulser / pulser_per_omdreing) / tid_i_min
        leftRPM  = (deltLeft  / (float)PULSES_PER_REV) / (dt / 60.0f);
        rightRPM = (deltRight / (float)PULSES_PER_REV) / (dt / 60.0f);

        lastLeftPulses  = currentLeft;
        lastRightPulses = currentRight;
        lastRPMTime     = now;
    }
}

void Encoder::resetPulses() {
    leftPulses  = 0;
    rightPulses = 0;
    lastLeftPulses  = 0;
    lastRightPulses = 0;
    leftRPM  = 0.0f;
    rightRPM = 0.0f;
}