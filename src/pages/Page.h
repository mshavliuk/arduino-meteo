#pragma once

#include "../display.h"
#define LOG_LEVEL Logger::ERROR

class Page {
public:
    Page(const __FlashStringHelper *name) : logger(new Logger(name, LOG_LEVEL)) {
        Display::get().clear();
    };

    ~Page() {
        this->logger->info(F("destruct"));
        delete this->logger;
    }

    virtual void draw() = 0;
    virtual bool tick() = 0;

protected:
    Logger *logger;
};