#pragma once

#include <inttypes.h>

#include "GyverButton.h"

#include "sensors.h"
#include "pages/Page.h"
#include "display.h"

#include <vector>

class Menu {
public:
    static Menu &get();
    bool tick();
    void showPage(size_t index);
    void nextPage();
    unsigned int getCurrentPageIndex();
    Menu(Menu const &) = delete;

    void operator=(Menu const &) = delete;

private:
    explicit Menu();

    GButton button;
    Page *currentPage;
    Logger *logger;
    uint8_t currentPageIndex;
    std::vector<Page* (*)()> pages;
};