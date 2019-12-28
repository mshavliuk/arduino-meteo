#pragma once

#include "../display.h"
#define LOG_LEVEL Logger::INFO

class Page {
public:
    Page(const __FlashStringHelper *name) : logger(new Logger(name, LOG_LEVEL)) {};
    ~Page() {
        this->logger->info(F("destruct"));
    }

    virtual void draw() = 0;
    virtual bool tick() = 0;

protected:
    Logger *logger;
};