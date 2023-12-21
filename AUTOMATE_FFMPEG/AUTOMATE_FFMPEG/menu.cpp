#include "menu.h"
#include <iostream>

void Menu::addItem(const std::string& itemName, std::function<void()> action) {
    items.push_back({ itemName, action, nullptr });
}

void Menu::addSubMenu(const std::string& subMenuName, std::shared_ptr<Menu> subMenu) {
    items.push_back({ subMenuName, nullptr, subMenu });
}

void Menu::display() const {
    std::cout << "Menu:\n";
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << (i + 1) << ". " << items[i].name << std::endl;
    }
}

void Menu::run() {
    while (true) {
        clearScreen();
        display();

        std::cout << "Wybierz opcje (0 aby wyjsc): ";
        int choice;
        std::cin >> choice;

        if (choice == 0) {
            break;
        }

        if (choice > 0 && choice <= static_cast<int>(items.size())) {
            auto& item = items[choice - 1];
            if (item.subMenu) {
                item.subMenu->run();
            }
            else if (item.action) {
                item.action();
                std::cout << "Nacisnij Enter, aby kontynuowac...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
            }
        }
        else {
            std::cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.get();
        }
    }
}


void Menu::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}