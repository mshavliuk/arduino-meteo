#pragma once

#include <inttypes.h>
#include <RTClib.h>

#include "../sensors.h"
#include "../storage.h"
#include "Page.h"

#define PRESS_WIDTH 3
#define PPM_WIDTH 4
#define TEMP_WIDTH 4
#define HUMID_WIDTH 2
#define LOG_LEVEL Logger::ERROR

class MainPage : public Page {
public:
    const char weekDays[7][3] = {"Su", "Mo", "Tu", "We", "Th", "Fr", "Sa"};
    explicit MainPage();
    ~MainPage();
    void draw() override;
    bool tick() override;

private:
    GTimer timer;
    bool displayDots;
    char *itoa(uint16_t, uint8_t, char *, bool = false);
    char *ftoa(double, uint8_t, uint8_t, char *);
    uint16_t ipow(int16_t, uint8_t);
};