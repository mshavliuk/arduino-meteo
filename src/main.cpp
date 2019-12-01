#include <Arduino.h>


#define MHZ_RX 2
#define MHZ_TX 3

#include <RTClib.h>
#include "pages.cpp"
#include "sensors.cpp"
#include "display.cpp"

RTC_DS3231 rtc;

Sensors *sensors;
Display *display;

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
    Serial.begin(9600);

//    setup_sensors();

//    sensors = new Sensors();
    display = new Display();
    display->write("123456", 0, 0, Display::BIG);
//    setup_pages(display);
}


void loop() {
    delay(500);
//    sensors.tick();
//    looptest();
//    delay(5000);
}
