// main.cpp - Kitchen, Reports & Documentation module
// Compile: g++ -std=c++11 main.cpp -o kitchen
#include "structures.h"
#include "report.h"
#include <cstdlib>
#include <limits>

const string SALES_FILE = "sales.txt";

// small helpers
int readInt(const string& prompt) {
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
string readLine(const string& prompt) {
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

// orders being cooked / waiting to be served (status Preparing or Ready)
const int MAX_ACTIVE = 50;
Order active[MAX_ACTIVE];
int activeCount = 0;

int findActive(int id) {
    for (int i = 0; i < activeCount; i++) if (active[i].id == id) return i;
    return -1;
}
void removeActive(int idx) {
    for (int i = idx; i < activeCount - 1; i++) active[i] = active[i + 1];
    activeCount--;
}

int main() {
    KitchenQueue kitchen;
    WaitingList waiting;
    StaffRotation staff;
    TableMap map;
    int nextId = 1;

    staff.addStaff("Ali");
    staff.addStaff("Sara");
    staff.addStaff("Usman");
    // sample floor plan: 0 = kitchen
    map.connect(0, 1); map.connect(0, 2); map.connect(1, 3);
    map.connect(2, 4); map.connect(3, 5); map.connect(4, 6); map.connect(5, 7);

    int choice;
    do {
        cout << "\n===== KITCHEN & REPORTS =====\n"
             << "1.  Add order to kitchen queue\n"
             << "2.  View pending orders\n"
             << "3.  Start preparing next order\n"
             << "4.  Mark order Ready\n"
             << "5.  Mark order Served (saves to sales.txt)\n"
             << "6.  View orders in progress\n"
             << "7.  Add customer to waiting list\n"
             << "8.  Seat next customer\n"
             << "9.  View waiting list\n"
             << "10. Show kitchen staff rotation / next on duty\n"
             << "11. Daily sales report (most sold first)\n"
             << "12. Table map: distance from kitchen\n"
             << "0.  Exit\n";
        choice = readInt("Choice: ");

        if (choice == 1) {
            Order o;
            o.id = nextId++;
            o.table = readInt("Table number: ");
            o.item = readLine("Item name: ");
            o.qty = readInt("Quantity: ");
            cout << "Price per item: ";
            cin >> o.price; cin.ignore(numeric_limits<streamsize>::max(), '\n');
            o.status = PENDING;
            kitchen.enqueue(o);
            cout << "Order #" << o.id << " added (Pending).\n";
        }
        else if (choice == 2) { kitchen.display(); }
        else if (choice == 3) {
            if (kitchen.isEmpty()) cout << "No pending orders.\n";
            else if (activeCount == MAX_ACTIVE) cout << "Too many orders in progress.\n";
            else {
                Order o = kitchen.dequeue();
                o.status = PREPARING;
                active[activeCount++] = o;
                cout << "Order #" << o.id << " (" << o.item << ") is now Preparing. Cook: "
                     << staff.nextOnDuty() << "\n";
            }
        }
        else if (choice == 4) {
            int i = findActive(readInt("Order id: "));
            if (i == -1 || active[i].status != PREPARING) cout << "No such order in Preparing.\n";
            else { active[i].status = READY; cout << "Order #" << active[i].id << " is Ready.\n"; }
        }
        else if (choice == 5) {
            int i = findActive(readInt("Order id: "));
            if (i == -1 || active[i].status != READY) cout << "No such order in Ready.\n";
            else {
                active[i].status = SERVED;
                SalesReport::saveSale(SALES_FILE, active[i]);
                cout << "Order #" << active[i].id << " Served and recorded.\n";
                removeActive(i);
            }
        }
        else if (choice == 6) {
            if (activeCount == 0) cout << "  (nothing in progress)\n";
            for (int i = 0; i < activeCount; i++)
                cout << "  #" << active[i].id << " | Table " << active[i].table << " | "
                     << active[i].item << " x" << active[i].qty << " | "
                     << statusName(active[i].status) << "\n";
        }
        else if (choice == 7) {
            Customer c;
            c.name = readLine("Customer name: ");
            c.partySize = readInt("Party size: ");
            if (waiting.enqueue(c)) cout << "Added to waiting list.\n";
            else cout << "Waiting list is full.\n";
        }
        else if (choice == 8) {
            Customer c;
            if (waiting.dequeue(c)) cout << "Seat " << c.name << " (party of " << c.partySize << ").\n";
            else cout << "Nobody is waiting.\n";
        }
        else if (choice == 9) { waiting.display(); }
        else if (choice == 10) { staff.display(); }
        else if (choice == 11) {
            SalesReport r;
            r.loadFromFile(SALES_FILE);
            r.sortByQty();
            r.print();
        }
        else if (choice == 12) { map.distancesFromKitchen(); }
        else if (choice != 0) cout << "Invalid choice.\n";
    } while (choice != 0);

    cout << "Goodbye.\n";


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
