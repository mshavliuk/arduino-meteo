#include <Wire.h>

#define MHZ_RX 2
#define MHZ_TX 3

#include "display.h"
#include "pages.cpp"
#include "sensors.cpp"
#include "logger.cpp"


Sensors *sensors;
Display *display;
Logger *logger;
Menu *menu;

void setup() {
    logger = new Logger(F("main"));
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
#endif