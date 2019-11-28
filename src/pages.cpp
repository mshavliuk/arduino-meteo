#include <Arduino.h>
#include <vector>

class Page {
public:
    virtual void draw() = 0;
};

class MainPage : public Page {
    void draw() override {
        Serial.println("draw MainPage");
    }
};

class PlotPage : public Page {
    void draw() override {
        Serial.println("draw PlotPage");
    };
};

class SettingPage : public Page {
    void draw() override {
        Serial.println("draw SettingPage");
    }
};


class Menu {
public:
    void addPage(Page *page) {
        this->pages.push_back(page);
    }

    void showPage(size_t index) {
        if(index >= this->pages.size()) {
            Serial.println("Out of range error"); // TODO: use logger
            return;
        }

        this->pages.at(index)->draw();
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
};