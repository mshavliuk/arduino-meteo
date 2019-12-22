
#include "Menu.h"
#include "pages/MainPage.h"

Menu::Menu(Display *display, Sensors *sensors) :
        display(display), currentPageIndex(0), timer(MS, 50), logger(F("Menu")) {
    this->addPage(new MainPage(sensors));
//    this->addPage(new PlotPage());
//    this->addPage(new PlotPage());
//    this->addPage(new PlotPage());
//    this->addPage(new SettingPage());
}

void Menu::show() {
    this->pages.at(this->currentPageIndex)->draw(this->display);
}

bool Menu::tick() {
    this->pages.at(this->currentPageIndex)->tick(this->display);

    if (!this->timer.isReady()) {
        return false;
    }
}

void Menu::addPage(Page *page) {
    this->pages.push_back(page);
}

void Menu::showPage(size_t index) {
    if (index >= this->pages.size()) {
        this->logger.error(F("Out of range error"));
        return;
    }

    this->pages.at(index)->draw(this->display);
    this->currentPageIndex = index;
}

void Menu::nextPage() {
    this->currentPageIndex = this->currentPageIndex + 1 % this->pages.size();
}

unsigned int Menu::getCurrentPageIndex() {
    return this->currentPageIndex;
}
