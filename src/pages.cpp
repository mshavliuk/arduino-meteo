#pragma once

#include <Arduino.h>
#include <vector>
#include "display.cpp"

class Page {
public:
    virtual void draw(Display *display) = 0;
};

class MainPage : public Page {
    void draw(Display *display) override {
        Serial.println("draw MainPage");

        display->write("12:40", 0,0, Display::BIG);

        // get sensors data
        // get time

    }
};

class PlotPage : public Page {
    void draw(Display *display) override {
        Serial.println("draw PlotPage");
    };
};

class SettingPage : public Page {
    void draw(Display *display) override {
        Serial.println("draw SettingPage");
    }
};


class Menu {
public:
    explicit Menu(Display* display): display(display), currentPageIndex(0) {
    }

    void tick() {
        this->pages.at(this->currentPageIndex)->draw(this->display);
    }

    void addPage(Page *page) {
        this->pages.push_back(page);
    }

    void showPage(size_t index) {
        if(index >= this->pages.size()) {
            Serial.println("Out of range error"); // TODO: use logger
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
    std::vector<Page *> pages;
    unsigned int currentPageIndex;
    Display *display;
};