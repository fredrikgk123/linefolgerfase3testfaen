#include "Wifi.hpp"
#include "Sensors.hpp"
#include <WiFi.h>
#include <WebServer.h>

static WebServer server(80);

RobotWifi::RobotWifi(bool& running, int& baseSpeed, int& regSpeed,
                     float& kp, float& ki, float& kd, Sensor& sensor)
    : running(running), baseSpeed(baseSpeed), regSpeed(regSpeed),
      kp(kp), ki(ki), kd(kd), sensor(sensor) {}

void RobotWifi::begin() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP("LinjefølgerG11", "12345678");
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
    setupRoutes();
    server.begin();
}

void RobotWifi::handle() {
    server.handleClient();
}

String RobotWifi::buildPage() {
    String p = "<!doctype html><html><head>";
    p += "<meta name='viewport' content='width=device-width,initial-scale=1'>";
    p += "<title>Linjefølger</title></head>";
    p += "<body style='font-family:sans-serif;max-width:480px;margin:20px'>";
    p += "<h2>Linjefølger</h2>";

    p += "<p>Status: <b id='status'>" + String(running ? "KJØRER" : "STOPPET") + "</b></p>";
    p += "<p>Kalibrering: <b id='kal'>" + String(calibrating ? "PÅGÅR..." : (sensor.isCalibrated() ? "FERDIG" : "IKKE KALIBRERT")) + "</b></p>";

    // Start / Stopp / Kalibrer
    p += "<p>";
    p += "<button onclick='fetch(\"/start\")' style='padding:10px 20px;font-size:16px;margin:4px'>START</button> ";
    p += "<button onclick='fetch(\"/stopp\")' style='padding:10px 20px;font-size:16px;margin:4px'>STOPP</button> ";
    p += "<button onclick='fetch(\"/kalibrer\")' style='padding:10px 20px;font-size:16px;margin:4px'>KALIBRER</button>";
    p += "</p><hr>";

    // Basehastighet
    p += "<h3>Basehastighet (0-255)</h3>";
    p += "<form action='/set'><input name='base' type='number' min='0' max='255' value='" + String(baseSpeed) + "'> ";
    p += "<input type='submit' value='Set'></form>";

    // Reguleringshastighet
    p += "<h3>Reguleringshastighet (0-255)</h3>";
    p += "<form action='/set'><input name='reg' type='number' min='0' max='255' value='" + String(regSpeed) + "'> ";
    p += "<input type='submit' value='Set'></form>";

    // PID
    p += "<hr><h3>PID</h3>";
    p += "<form action='/set'>";
    p += "Kp: <input name='kp' type='number' step='0.001' value='" + String(kp, 4) + "'><br><br>";
    p += "Ki: <input name='ki' type='number' step='0.0001' value='" + String(ki, 4) + "'><br><br>";
    p += "Kd: <input name='kd' type='number' step='0.001' value='" + String(kd, 4) + "'><br><br>";
    p += "<input type='submit' value='Oppdater PID'></form>";

    // JS: poll status kvart sekund
    p += "<script>"
         "async function poll(){"
         "  const r=await fetch('/status');"
         "  const s=await r.json();"
         "  document.getElementById('status').textContent=s.running?'KJØRER':'STOPPET';"
         "  document.getElementById('kal').textContent=s.calibrating?'PÅGÅR...':s.calibrated?'FERDIG':'IKKE KALIBRERT';"
         "}"
         "setInterval(poll,500);poll();"
         "</script>";

    p += "</body></html>";
    return p;
}

void RobotWifi::setupRoutes() {

    server.on("/status", HTTP_GET, [this]() {
        String json = "{\"running\":" + String(running ? "true" : "false") + ",";
        json += "\"calibrating\":" + String(calibrating ? "true" : "false") + ",";
        json += "\"calibrated\":" + String(sensor.isCalibrated() ? "true" : "false") + "}";
        server.send(200, "application/json", json);
    });

    server.on("/", HTTP_GET, [this]() {
        server.send(200, "text/html", buildPage());
    });

    server.on("/start", HTTP_GET, [this]() {
        running = true;
        Serial.println("START");
        server.sendHeader("Location", "/");
        server.send(302);
    });

    server.on("/stopp", HTTP_GET, [this]() {
        running = false;
        Serial.println("STOPP");
        server.sendHeader("Location", "/");
        server.send(302);
    });

    server.on("/kalibrer", HTTP_GET, [this]() {
        running = false;
        calibrating = true;
        server.send(200, "text/plain", "OK");
        sensor.calibrate(5000);
        calibrating = false;
    });

    server.on("/set", HTTP_GET, [this]() {
        if (server.hasArg("base")) baseSpeed = constrain(server.arg("base").toInt(), 0, 255);
        if (server.hasArg("reg"))  regSpeed  = constrain(server.arg("reg").toInt(),  0, 255);
        if (server.hasArg("kp"))   kp = server.arg("kp").toFloat();
        if (server.hasArg("ki"))   ki = server.arg("ki").toFloat();
        if (server.hasArg("kd"))   kd = server.arg("kd").toFloat();

        Serial.printf("Base:%d Reg:%d Kp:%.4f Ki:%.4f Kd:%.4f\n",
                      baseSpeed, regSpeed, kp, ki, kd);

        server.sendHeader("Location", "/");
        server.send(302);
    });
}