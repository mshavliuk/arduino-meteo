#pragma once

#include <inttypes.h>
#include <vector>

#include "sensors.h"
#include "pages/Page.h"
#include "display.h"


class Menu {
public:
    explicit Menu(Display *, Sensors *);
    void show();
    bool tick();
    void addPage(Page *);
    void showPage(size_t index);
    void nextPage();
    unsigned int getCurrentPageIndex();

private:
    Logger logger;
    std::vector<Page *> pages;
    unsigned int currentPageIndex;
    Display *display;
    GTimer timer;
};