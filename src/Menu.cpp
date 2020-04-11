
#include "Menu.h"
#include "pages/MainPage.h"
#include "pages/PlotPage.h"

#define BTN_PIN 4

int *dataGetter() {
    static int data[] = {1, 2, 3};
    return data;
}

Menu::Menu() : currentPageIndex(0), logger(new Logger(F("Menu"))), button(BTN_PIN, LOW_PULL, NORM_OPEN) {
    this->pages = {
            []() -> Page * { return new MainPage(); },
            []() -> Page * {
                // possible values range - 4*8=0..32
                return new PlotPage(dataGetter, "Pressure");
                },
    };
    this->showPage(this->currentPageIndex);
}

Menu &Menu::get() {
    static Menu instance;
    return instance;
}

bool Menu::tick() {
    this->button.tick();
    if (this->button.isClick()) {
        this->nextPage();
    }
    this->currentPage->tick();
}

void Menu::showPage(size_t index) {
    this->currentPage = this->pages.at(index)();
    this->currentPage->draw();
    this->currentPageIndex = index;
}


void Menu::nextPage() {
    this->logger->info(F("Next page"));
    delete this->currentPage;
    this->logger->info(F("after delete current page"));

    auto newPageIndex = (this->currentPageIndex + 1) % this->pages.size();
    this->showPage(newPageIndex);
}


unsigned int Menu::getCurrentPageIndex() {
    return this->currentPageIndex;
}
