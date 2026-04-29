#pragma once
#include <Arduino.h>

class Encoder {
public:
    // Venstre motor
    static const int A_LEFT  = 39;  // A_Motor_V
    static const int B_LEFT  = 36;  // B_Motor_V

    // Høyre motor
    static const int A_RIGHT = 35;  // A_Motor_H
    static const int B_RIGHT = 34;  // B_Motor_H

    // Juster etter encoder-spesifikasjon
    static const int PULSES_PER_REV = 20;

    void begin();
    void update();  // kall i loop for RPM-beregning

    long  getLeftPulses()  const { return leftPulses; }
    long  getRightPulses() const { return rightPulses; }
    float getLeftRPM()     const { return leftRPM; }
    float getRightRPM()    const { return rightRPM; }

    void resetPulses();

    // ISR-handlere (må være public for interrupt-callback)
    static void IRAM_ATTR isrLeftA();
    static void IRAM_ATTR isrRightA();

private:
    static volatile long leftPulses;
    static volatile long rightPulses;

    float leftRPM  = 0.0f;
    float rightRPM = 0.0f;

    long  lastLeftPulses  = 0;
    long  lastRightPulses = 0;
    unsigned long lastRPMTime = 0;
};