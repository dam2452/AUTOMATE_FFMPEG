#include "menu.h"
#include <iostream>
#include <iomanip>

Menu::Menu(const std::string& menuName) : menuName(menuName)
{
}

void Menu::addItem(const std::string& itemName, std::function<void()> action) {
    items.push_back({ itemName, action, nullptr });
}

void Menu::addSubMenu(const std::string& subMenuName, std::shared_ptr<Menu> subMenu) {
    items.push_back({ subMenuName, nullptr, subMenu });
}

void Menu::display() const {
    const size_t width = 40; // Width for displaying the title and options
    const std::string border(width, '-'); // Creating a separating line

    // Displaying the title
    std::cout << border << std::endl;
    size_t titleStartPos = (width - menuName.length()) / 2; // Calculating the start position for the title
    std::cout << std::setw(titleStartPos) << "" << menuName << std::endl; // Centering the title
    std::cout << border << std::endl;

    // Displaying options
    for (size_t i = 0; i < items.size(); ++i) {
        std::cout << std::setw(2) << (i + 1) << ". " << items[i].name << std::endl;
    }

    // Closing the menu with a separating line
    std::cout << border << std::endl;
}

void Menu::run() {
    while (true) {
        clearScreen();
        display();

        std::cout << "Choose an option (0 to exit): ";
        int choice;
        std::cin >> choice;

        // Clear the stream buffer before calling the action
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
                std::cout << "Press Enter to continue...";
                std::cin.get();
            }
        }
        else {
            std::cout << "Invalid selection. Try again.\n";
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