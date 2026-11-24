// menu_demo.cpp - standalone test program for Menu Management (Mariam)
// Compile: g++ -std=c++11 menu_demo.cpp -o menu
// (Ahmed's main.cpp will call menuManagementScreen() instead of this main.)
#include "menu_ui.h"

int main() {
    MenuManager menu;

    if (!menu.loadFromFile(MENU_FILE)) {   // first run: add some sample items
        menu.addItem("Zinger Burger", 450, "Food/Fast Food/Burger");
        menu.addItem("Beef Burger", 550, "Food/Fast Food/Burger");
        menu.addItem("Pepperoni Pizza", 1200, "Food/Fast Food/Pizza");
        menu.addItem("Chicken Biryani", 400, "Food/Desi");
        menu.addItem("Cola", 120, "Drinks/Cold Drinks");
        menu.addItem("Green Tea", 100, "Drinks/Hot Drinks");
        menu.addItem("Gulab Jamun", 150, "Desserts");
    }

    menuManagementScreen(menu);
    menu.saveToFile(MENU_FILE);
    cout << "Menu saved. Goodbye.\n";
    return 0;
}
