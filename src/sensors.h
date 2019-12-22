#pragma once

#include <Arduino.h>
#include <GyverTimer.h>
#include <MHZ19_uart.h>
#include <Adafruit_BME280.h>
#include <RTClib.h>
#include <Wire.h>
#include <inttypes.h>

#include "storage.h"
#include "measures.h"

#define MHZ_RX 2
#define MHZ_TX 3
#define RESET_TIME false
#define PASCAL_TO_MM_HG 0.00750062
#define TAKE_MEASURES_INTERVAL 5000
#define SAVE_MEASURES_INTERVAL 5000



class Sensors {
public:
    Sensors();
    bool isReady();
    bool isMeasuresReady();
    bool tick();

    Measures getCurrentMeasures();

    DateTime getDateTime();

private:
    Measures measures;
    Logger logger;
    GTimer takeMeasuresTimer;
    GTimer saveMeasuresTimer;
    MHZ19_uart mhz19;
    Adafruit_BME280 bme;
    RTC_DS3231 rtc;
    bool ready = false;
    bool measuresReady = false;

    void takeMeasures();
    void saveMeasures() const;
};