#pragma once

#include "Page.h"

class PlotPage : public Page {
public:
    PlotPage(int *(*fcnPtr)()) : Page(F("PlotPage")), dataGetter(fcnPtr) {}

    void draw() override {
        this->logger->info(F("draw PlotPage"));
        this->logger->info(F("Given data:"));
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
};