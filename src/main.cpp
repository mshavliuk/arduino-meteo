#include <Arduino.h>


#define MHZ_RX 2
#define MHZ_TX 3

#include <RTClib.h>
#include "pages.cpp"
#include "sensors.cpp"
#include "display.cpp"
#include "logger.cpp"

RTC_DS3231 rtc;

Sensors *sensors;
Display *display;
Logger *logger;

void setup_pages(Display *display) {
    Menu *m = new Menu(display);
    m->addPage(new MainPage());
    m->addPage(new PlotPage());
    m->addPage(new PlotPage());
    m->addPage(new PlotPage());
    m->addPage(new SettingPage());

    m->showPage(0);
}

void setup() {
    logger = new Logger("main");
    delay(100);
    logger->info("Starting setup");
    sensors = new Sensors();
    display = new Display();
    display->write("2 2:3 0", 0, 0, Display::BIG);
    setup_pages(display);
    logger->info("Setup is finished");
}


void loop() {
//    delay(500);
    sensors->tick();
//    looptest();
//    delay(5000);
}

#if !defined(ARDUINO)
int main() {
    setup();
//    while(true) {
        loop();
//    }
    return 0;
}
#endif