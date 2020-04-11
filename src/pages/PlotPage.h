#pragma once

#include "Page.h"

class PlotPage : public Page {
public:
    PlotPage(int *(*fcnPtr)(), char* name) : Page(F("PlotPage")), dataGetter(fcnPtr), name(name) {}

    void draw() override {
        this->logger->info(F("draw PlotPage"));
        Display::get().write(this->name, 0, 0);
        for(auto i = 0; i < 3; ++i) {
            this->logger->info(this->dataGetter()[i]);
        }
    };

    bool tick() override {
        this->draw();
        return false;
    }

private:
    int* (*dataGetter)();
    char* name;
};