#pragma once
#include <Arduino.h>
#include "Sensors.hpp"
#include "Logger.hpp"

class RobotWifi {
public:
    RobotWifi(bool& running, int& baseSpeed, int& regSpeed,
              float& kp, float& ki, float& kd, Sensor& sensor, Logger& logger);

    void begin();
    void handle();

private:
    bool&   running;
    int&    baseSpeed;
    int&    regSpeed;
    float&  kp;
    float&  ki;
    float&  kd;
    Sensor& sensor;
    Logger& logger;
    bool    calibrating = false;

    void setupRoutes();
    String buildPage();
};