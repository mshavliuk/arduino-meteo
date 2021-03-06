#pragma once

#include <Arduino.h>
#include <GyverTimer.h>
#include <MHZ19_uart.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <inttypes.h>

#include "storage.h"
#include "measures.h"

#define MHZ_RX 2
#define MHZ_TX 3
#define PASCAL_TO_MM_HG 0.00750062
#define TAKE_MEASURES_INTERVAL 5000


class Sensors {
public:
    static Sensors &get();

    bool isReady();

    bool isMeasuresReady();

    bool tick();

    Sensors(Sensors const &) = delete;

    void operator=(Sensors const &) = delete;

    Measures getCurrentMeasures();

private:
    Sensors();

    Measures measures;
    Logger *logger;
    GTimer takeMeasuresTimer;
    MHZ19_uart mhz19;
    Adafruit_BME280 bme;
    bool ready = false;
    bool measuresReady = false;

    void takeMeasures();
};