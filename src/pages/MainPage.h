#pragma once

#include <inttypes.h>

#include "../sensors.h"
#include "Page.h"

#define PRESS_WIDTH 3
#define PPM_WIDTH 4
#define TEMP_WIDTH 4
#define HUMID_WIDTH 2
#define LOG_LEVEL Logger::ERROR

class MainPage : public Page {
public:
    MainPage(Sensors *sensors);
    void draw(Display *) override;
    bool tick(Display *) override;

private:
    Sensors *sensors;
    GTimer timer;
    bool displayDots;
    char *itoa(uint16_t, uint8_t, char *, const char * = "", bool = false);
    char *ftoa(double, uint8_t, uint8_t, char *, const char * = "");
    uint16_t ipow(int16_t, uint8_t);
};