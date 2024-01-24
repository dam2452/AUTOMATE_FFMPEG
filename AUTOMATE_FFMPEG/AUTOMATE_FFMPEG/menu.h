#ifndef MENU_H
#define MENU_H

#include <vector>
#include <string>
#include <functional>
#include <memory>

class Menu {
public:

    Menu(const std::string& menuName);

    void addItem(const std::string& itemName, std::function<void()> action);
    void addSubMenu(const std::string& subMenuName, std::shared_ptr<Menu> subMenu);
    void display() const;
    void run();
    static void clearScreen();

private:

    std::string menuName;

    struct MenuItem {
        std::string name;
        std::function<void()> action;
        std::shared_ptr<Menu> subMenu;
    };

    std::vector<MenuItem> items;
};

#endif // MENU_H
