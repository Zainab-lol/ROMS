// menu_ui.h - menu-driven screen for Menu Management (Member: Mariam)
// Ahmed: call menuManagementScreen(menu) from the Admin menu in main.cpp.
#ifndef MENU_UI_H
#define MENU_UI_H

#include "menu.h"
#include <limits>

const string MENU_FILE = "menu.txt";

inline int menuReadInt(const string& prompt) {
    int x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}
inline double menuReadDouble(const string& prompt) {
    double x;
    cout << prompt;
    while (!(cin >> x)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter a number: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return x;
}
inline string menuReadLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

inline SortKey askSortKey() {
    int k = menuReadInt("Sort by (1 = price, 2 = name): ");
    return k == 2 ? BY_NAME : BY_PRICE;
}

inline void showResult(const MenuManager& m, int idx) {
    if (idx == -1) cout << "Not found.\n";
    else { cout << "Found:\n"; m.printItem(idx); }
}

inline void menuManagementScreen(MenuManager& m) {
    int choice;
    do {
        cout << "\n===== MENU MANAGEMENT =====\n"
             << "1.  Add menu item\n"
             << "2.  Remove menu item\n"
             << "3.  Update item price\n"
             << "4.  View all items\n"
             << "5.  View category tree (recursive)\n"
             << "6.  Add category\n"
             << "7.  Linear search by ID\n"
             << "8.  Linear search by name\n"
             << "9.  Binary search by ID\n"
             << "10. Binary search by name\n"
             << "11. Sort - Bubble sort\n"
             << "12. Sort - Insertion sort\n"
             << "13. Sort - Merge sort\n"
             << "14. Save to menu.txt\n"
             << "15. Reload from menu.txt\n"
             << "0.  Back\n";
        choice = menuReadInt("Choice: ");

        if (choice == 1) {
            string name = menuReadLine("Item name: ");
            double price = menuReadDouble("Price: ");
            string cat = menuReadLine("Category path (e.g. Food/Fast Food/Burger): ");
            int id = m.addItem(name, price, cat);
            if (id == -1) cout << "Could not add item (menu full or invalid input).\n";
            else cout << "Added with ID " << id << ".\n";
        }
        else if (choice == 2) {
            if (m.removeItem(menuReadInt("Item ID: "))) cout << "Removed.\n";
            else cout << "No such item.\n";
        }
        else if (choice == 3) {
            int id = menuReadInt("Item ID: ");
            if (m.updatePrice(id, menuReadDouble("New price: "))) cout << "Price updated.\n";
            else cout << "No such item or invalid price.\n";
        }
        else if (choice == 4) { m.display(); }
        else if (choice == 5) { m.displayTree(); }
        else if (choice == 6) {
            if (m.addCategory(menuReadLine("Category path: "))) cout << "Category added.\n";
            else cout << "Invalid category.\n";
        }
        else if (choice == 7) { showResult(m, m.linearSearchById(menuReadInt("Item ID: "))); }
        else if (choice == 8) { showResult(m, m.linearSearchByName(menuReadLine("Item name: "))); }
        else if (choice == 9) { showResult(m, m.binarySearchById(menuReadInt("Item ID: "))); }
        else if (choice == 10) {
            showResult(m, m.binarySearchByName(menuReadLine("Item name: ")));
            cout << "(note: the list is now sorted by name for binary search)\n";
        }
        else if (choice == 11) { m.sortBubble(askSortKey());    m.display(); }
        else if (choice == 12) { m.sortInsertion(askSortKey()); m.display(); }
        else if (choice == 13) { m.sortMerge(askSortKey());     m.display(); }
        else if (choice == 14) { if (m.saveToFile(MENU_FILE)) cout << "Saved.\n"; }
        else if (choice == 15) {
            if (m.loadFromFile(MENU_FILE)) cout << "Loaded " << m.size() << " item(s).\n";
            else cout << "menu.txt not found.\n";
        }
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0);
}

#endif
