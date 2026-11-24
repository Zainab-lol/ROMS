// report.h - sales report: sorting + file handling (sales.txt)
#ifndef REPORT_H
#define REPORT_H

#include "structures.h"
#include <sstream>

struct SaleItem {
    string name;
    int qtySold;
    double revenue;
};

class SalesReport {
    static const int MAX = 100;
    SaleItem items[MAX];
    int n;

    int find(const string& name) const {   // O(n) linear search
        for (int i = 0; i < n; i++) if (items[i].name == name) return i;
        return -1;
    }
public:
    SalesReport() : n(0) {}

    // Add a sale to the in-memory totals: O(n)
    void addSale(const string& name, int qty, double price) {
        int i = find(name);
        if (i == -1) {
            if (n == MAX) return;
            items[n++] = {name, qty, qty * price};
        } else {
            items[i].qtySold += qty;
            items[i].revenue += qty * price;
        }
    }

    // Append one sale line to sales.txt: O(1)
    static void saveSale(const string& file, const Order& o) {
        ofstream out(file.c_str(), ios::app);
        if (!out) { cout << "Could not open " << file << "\n"; return; }
        out << o.item << "," << o.qty << "," << o.price << "\n";
    }

    // Read sales.txt into totals: O(L * n), L = lines in file
    void loadFromFile(const string& file) {
        n = 0;
        ifstream in(file.c_str());
        if (!in) return;   // no file yet = no sales yet
        string line;
        while (getline(in, line)) {
            stringstream ss(line);
            string name, q, p;
            if (getline(ss, name, ',') && getline(ss, q, ',') && getline(ss, p, ','))
                addSale(name, atoi(q.c_str()), atof(p.c_str()));
        }
    }

    // Bubble sort, most sold first: O(n^2) worst/average, O(n) best (already sorted)
    void sortByQty() {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++) {
                if (items[j].qtySold < items[j + 1].qtySold) {
                    SaleItem t = items[j]; items[j] = items[j + 1]; items[j + 1] = t;
                    swapped = true;
                }
            }
            if (!swapped) break;
        }
    }

    // O(n)
    void print() const {
        if (n == 0) { cout << "  No sales recorded yet.\n"; return; }
        double total = 0;
        cout << left << setw(5) << "  #" << setw(20) << "Item" << setw(8) << "Sold" << "Revenue\n";
        for (int i = 0; i < n; i++) {
            cout << "  " << left << setw(3) << i + 1 << setw(20) << items[i].name
                 << setw(8) << items[i].qtySold << fixed << setprecision(2) << items[i].revenue << "\n";
            total += items[i].revenue;
        }
        cout << "  Total revenue: " << fixed << setprecision(2) << total << "\n";
    }
};

#endif
