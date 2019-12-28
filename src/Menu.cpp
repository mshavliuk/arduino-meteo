
#include "Menu.h"
#include "pages/MainPage.h"
#include "pages/PlotPage.h"

#define BTN_PIN 4

Menu::Menu() : currentPageIndex(0), timer(MS, 50), logger(new Logger(F("Menu"))), button(BTN_PIN, LOW_PULL, NORM_OPEN) {
    this->currentPage = new MainPage();
//    this->nextPage();
}

Menu &Menu::get() {
    static Menu instance;
    return instance;
}

bool Menu::tick() {
    this->button.tick();
    if(this->button.isClick()) {
        this->nextPage();
    }
    this->currentPage->tick();
}

void Menu::showPage(size_t index) {
//    if (index >= this->pages.size()) {
//        this->logger->error(F("Out of range error"));
//        return;
//    }

//    this->pages.at(index)->draw(this->display);
//    this->currentPageIndex = index;


//    this->currentPage->draw();
}

int * dataGetter() {
    static int data[] = {1, 2, 3};
    return data;
}

void Menu::nextPage() {
    this->logger->info(F("Next page"));
    this->currentPageIndex = (this->currentPageIndex + 1) % 2;
    delete this->currentPage;
    this->logger->info(F("after delete current page"));
    Display::get().clear();
    switch (this->currentPageIndex) {
        case 0:
            this->logger->info(F("switch to MainPage"));
            this->currentPage = new MainPage();
            break;
        case 1:
            this->logger->info(F("switch to PlotPage"));
            this->currentPage = new PlotPage(dataGetter);
            break;
        default:
            this->logger->error(F("Page with given index does not exist!"));
    }
}



unsigned int Menu::getCurrentPageIndex() {
    return this->currentPageIndex;
}
