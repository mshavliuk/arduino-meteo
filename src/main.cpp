#pragma once

#define MHZ_RX 2
#define MHZ_TX 3

#include <Wire.h>

#include "logger.h"
#include "Menu.h"
#include "sensors.h"

Logger *logger;

void setup() {
    Serial.begin(9600);
    logger = new Logger(F("main"));
    logger->info(F("Setup is finished"));
}

void loop() {
    Sensors::get().tick();
    Menu::get().tick();
}


#undef main
#if !defined(ARDUINO)
#include <unistd.h>

int main() {
    setup();
    while(true) {
        loop();
        sleep(1);
    }
    return 0;
}
#else
int main(void) {
    init();

    initVariant();

#if defined(USBCON)
    USBDevice.attach();
#endif

    setup();

    while (true) {
        loop();
//        if (serialEventRun) serialEventRun();
    }

    return 0;
}
#endif