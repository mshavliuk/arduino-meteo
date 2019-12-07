#pragma once

#include <vector>

#include "display.cpp"
#include "logger.cpp"

class Page {
public:
    Page(char* name): logger(name) {};

    virtual void draw(Display *display) = 0;

protected:
    Logger logger;
};

class MainPage : public Page {
public:
    MainPage() : Page("MainPage") {}

    void draw(Display *display) override {
        this->logger.info("draw MainPage");

        display->write("12:40", 0,0, Display::BIG);

        // get sensors data
        // get time

    }
};

class PlotPage : public Page {
public:
    PlotPage() : Page("PlotPage") {}
    void draw(Display *display) override {
        this->logger.info("draw PlotPage");
    };
};

class SettingPage : public Page {
public:
    SettingPage() : Page("SettingPage") {}

    void draw(Display *display) override {
        this->logger.info("draw SettingPage");
    }
};


class Menu {
public:
    explicit Menu(Display* display): display(display), currentPageIndex(0), logger("Menu") {
    }

    void tick() {
        this->pages.at(this->currentPageIndex)->draw(this->display);
    }

    void addPage(Page *page) {
        this->pages.push_back(page);
    }

    void showPage(size_t index) {
        if(index >= this->pages.size()) {
            this->logger.error("Out of range error");
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
};