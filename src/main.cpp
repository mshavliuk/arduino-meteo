#pragma once

#define MHZ_RX 2
#define MHZ_TX 3

#include <Wire.h>

#include "logger.h"
#include "display.h"
#include "Menu.h"
#include "sensors.h"
#include "storage.h"


Sensors *sensors;
Display *display;
Logger *logger;
Menu *menu;

void setup() {
    logger = new Logger(F("main"));
//    logger->printMemoryDump();

    logger->info(F("Starting setup"));
    sensors = new Sensors();
    display = new Display();
    menu = new Menu(display, sensors);
    menu->show();
    logger->info(F("Setup is finished"));
}

void loop() {
    sensors->tick();
    menu->tick();
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