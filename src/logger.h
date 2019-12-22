#pragma once

#include <Arduino.h>
#include <Wire.h>

#if !DEBUG
#define info(...) noop()
#endif

class Logger {
public:
    enum Levels {
        ERROR,
        INFO,
    };

    explicit Logger(const __FlashStringHelper *name, Levels level = INFO);

    template<typename... Args>
    void error(Args...);

#if DEBUG
    template<typename... Args>
    void info(Args...);
#endif

    void noop();

private:
    const __FlashStringHelper *name;
    Levels level;

    template<typename T, typename... Args>
    void print(T t, Args...);

    template<typename T>
    void print(T);
};