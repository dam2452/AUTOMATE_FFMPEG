#include <iostream>
#include <string>
#include <vector>
#include "commands.h"
#include "menu.h"


void funkcja1() {
    std::cout << "Funkcja 1 wywolana\n";
}

void funkcja2() {
    std::cout << "Funkcja 2 wywolana\n";
}


int main()
{
    auto podMenu = std::make_shared<Menu>();
    podMenu->addItem("Podopcja 1", funkcja1);
    podMenu->addItem("Podopcja 2", funkcja2);

    Menu menu;
    menu.addItem("Opcja 1", funkcja1);
    menu.addSubMenu("Podmenu", podMenu);

    menu.run();

	return 0;
}

