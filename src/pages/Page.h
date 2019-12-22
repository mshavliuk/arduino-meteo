#pragma once

#include "../display.h"

class Page {
public:
    Page(const __FlashStringHelper *name) : logger(name, LOG_LEVEL) {};

    virtual void draw(Display *display) = 0;
    virtual bool tick(Display *display) = 0;

protected:
    Logger logger;
};