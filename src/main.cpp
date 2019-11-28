#include <Arduino.h>


#define MHZ_RX 2
#define MHZ_TX 3

#include <RTClib.h>
#include "pages.cpp"
#include "sensors.cpp"

RTC_DS3231 rtc;

Sensors *sensors;

float dispTemp;
byte dispHum;
int dispPres;

struct main_screen_data {
    mhz_data mhzData;
    float temp;
    float press;
    float humid;
};

void drawSensors(main_screen_data data) {
    // дисплей 2004
//  lcd.setCursor(0, 2);
//  lcd.print(String(dispTemp, 1));
//  lcd.write(223);
//  lcd.setCursor(6, 2);
//  lcd.print(" " + String(dispHum) + "%  ");
//
//  lcd.print(String(data.mhzData.co2) + " ppm");
//  if (dispCO2 < 1000) lcd.print(" ");
//
//  lcd.setCursor(0, 3);
//  lcd.print(String(dispPres) + " mm  rain ");
//  lcd.print(F("       "));
}

void setup_pages() {
    Menu *m = new Menu();
    m->addPage(new MainPage());
    m->addPage(new PlotPage());
    m->addPage(new PlotPage());
    m->addPage(new PlotPage());
    m->addPage(new SettingPage());

    m->showPage(0);
    m->showPage(100);
}

void setup() {
    Serial.begin(9600);

//    setup_sensors();

    setup_pages();
    sensors = new Sensors();
//
//    if(rtc.begin()) {
//        Serial.println("RTC ok");
//    } else {
//        Serial.println("RTC fail");
//    }
}

void looptest() {
    Serial.print("RTC_temp: ");
    Serial.println(rtc.getTemperature());
}

void loop() {
    sensors->tick();
//    looptest();
//    delay(5000);
}
