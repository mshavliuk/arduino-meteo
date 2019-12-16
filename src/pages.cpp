#pragma once

#include <GyverTimer.h>
#include <vector>
#include <Wire.h>

#include "display.cpp"
#include "logger.cpp"
#include "sensors.cpp"

#define PRESS_WIDTH 3
#define PPM_WIDTH 4
#define TEMP_WIDTH 4
#define HUMID_WIDTH 2

class Page {
public:
    Page(const __FlashStringHelper *name) : logger(name) {};

    virtual void draw(Display *display) = 0;
    virtual bool tick(Display *display) = 0;

protected:
    Logger logger;
};

class MainPage : public Page {
public:
    MainPage(Sensors *sensors) : Page(F("MainPage")), sensors(sensors), timer(MS, 500), displayDots(true) {}

    void draw(Display *display) override {
        this->logger.info(F("draw MainPage"));

        if(!sensors->isMeasuresReady()) {
            display->write("Waiting for sensors", 0, 1);
            return;
        }

        DateTime now = this->sensors->getDateTime();
        char timeString[] = {
                static_cast<char>(now.hour() / 10 + '0'),
                ' ',
                static_cast<char>(now.hour() % 10 + '0'),
                this->displayDots ? ':' : ' ',
                static_cast<char>(now.minute() / 10 + '0'),
                ' ',
                static_cast<char>(now.minute() % 10 + '0'),
                '\0'
        };
        display->write(timeString, 0, 0, Display::BIG);

        Sensors::Measures measures = this->sensors->getCurrentMeasures();
        char tempString[TEMP_WIDTH + sizeof("\xDF")];
        this->ftoa(measures.temp, 4, 1, tempString, "\xDF");
        display->write(tempString, 0, 2);

        char ppmString[PPM_WIDTH + sizeof("ppm")];
        this->itoa(measures.ppm, 4, ppmString, "ppm");
        display->write(ppmString, 0, 3);

        char humidityString[HUMID_WIDTH + sizeof("%")];
        itoa((uint8_t) measures.humidity, HUMID_WIDTH, humidityString, "%");
        display->write(humidityString, 8, 3);

        char pressureString[PRESS_WIDTH + sizeof("mm")];
        itoa(measures.pressHg, PRESS_WIDTH, pressureString, "mm");
        display->write(pressureString, 6, 2);
    }


    bool tick(Display *display) override {
        if(this->timer.isReady()) {
            this->displayDots = !this->displayDots;
            this->draw(display);
        }
    }
private:
    Sensors *sensors;
    GTimer timer;
    bool displayDots;

    char *itoa(uint16_t n, uint8_t numberMaxWidth, char *buf, const char *postfix = "", bool leadZero = false) {
        for(uint8_t i = 0; i < numberMaxWidth; ++i) {
            uint8_t digit = (n / (uint16_t) this->ipow(10, numberMaxWidth - i - 1)) % 10;
            if(!leadZero && digit == 0) {
                buf[i] = ' ';
            } else {
                buf[i] = static_cast<char>(digit + '0');
                leadZero = true;
            }
        }

        uint8_t i;
        for(i = 0; postfix[i] != '\0'; ++i) {
            buf[numberMaxWidth + i] = postfix[i];
        }

        buf[numberMaxWidth + i] = '\0';
        return buf;
    }

    char *ftoa(double n, uint8_t width, uint8_t precision, char *buf, const char *postfix = "") {
        this->itoa(n, width - precision - 1, buf, ".");
        double fraction = modf(n, &fraction);
        this->itoa(fraction * ipow(10, precision), precision, &buf[width - precision], postfix, true);
        return buf;
    }

    uint16_t ipow(int16_t base, uint8_t exponent) {
        if(exponent == 0) {
            return 1;
        }
        int16_t res = base;
        for(auto i = 1; i < exponent; ++i) {
            res *= base;
        }
        return res;
    }
};

class PlotPage : public Page {
public:
    PlotPage() : Page(F("PlotPage")) {}

    void draw(Display *display) override {
        this->logger.info(F("draw PlotPage"));
    };

    bool tick(Display *display) override {
        return false;
    }
};

class SettingPage : public Page {
public:
    SettingPage() : Page(F("SettingPage")) {}

    void draw(Display *display) override {
        this->logger.info(F("draw SettingPage"));
    }

    bool tick(Display *display) override {
        return false;
    }
};


class Menu {
public:
    explicit Menu(Display *display, Sensors *sensors) : display(display), currentPageIndex(0), timer(MS, 50), logger(F("Menu")) {
        this->addPage(new MainPage(sensors));
        this->addPage(new PlotPage());
        this->addPage(new PlotPage());
        this->addPage(new PlotPage());
        this->addPage(new SettingPage());
    }

    void show() {
        this->pages.at(this->currentPageIndex)->draw(this->display);
    }

    bool tick() {
        this->pages.at(this->currentPageIndex)->tick(this->display);

        if(!this->timer.isReady()) {
            return false;
        }
    }

    void addPage(Page *page) {
        this->pages.push_back(page);
    }

    void showPage(size_t index) {
        if (index >= this->pages.size()) {
            this->logger.error(F("Out of range error"));
            return;
        }

        this->pages.at(index)->draw(this->display);
        this->currentPageIndex = index;
    }

    void nextPage() {
        this->currentPageIndex = this->currentPageIndex + 1 % this->pages.size();
    }

    unsigned int getCurrentPageIndex() {
        return this->currentPageIndex;
    }

private:
    Logger logger;
    std::vector<Page *> pages;
    unsigned int currentPageIndex;
    Display *display;
    GTimer timer;
};