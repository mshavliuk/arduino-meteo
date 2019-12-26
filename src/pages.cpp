//#pragma once
//
//#include <GyverTimer.h>
//#include <vector>
//#include <Wire.h>
//
//#include "display.h"
//#include "<logger.h>"
//#include "sensors.h"
//
//#define PRESS_WIDTH 3
//#define PPM_WIDTH 4
//#define TEMP_WIDTH 4
//#define HUMID_WIDTH 2
//#define LOG_LEVEL Logger::ERROR
//
//
//
//class PlotPage : public Page {
//public:
//    PlotPage() : Page(F("PlotPage")) {}
//
//    void draw(Display *display) override {
//        this->logger.info(F("draw PlotPage"));
//    };
//
//    bool tick(Display *display) override {
//        return false;
//    }
//};
//
//class SettingPage : public Page {
//public:
//    SettingPage() : Page(F("SettingPage")) {}
//
//    void draw(Display *display) override {
//        this->logger.info(F("draw SettingPage"));
//    }
//
//    bool tick(Display *display) override {
//        return false;
//    }
//};
