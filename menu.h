// menu.h - Menu Management (Member: Mariam)
// Arrays, linear/binary search, bubble/insertion/merge sort,
// category tree + recursion, menu.txt file handling.
// Compile with structures.h in the same folder (g++ -std=c++11)
#ifndef MENU_H
#define MENU_H

#include "structures.h"
#include <sstream>
#include <cstdlib>
#include <cctype>

// ---------- small helpers ----------
inline string toLower(string s) {                 // O(L)
    for (size_t i = 0; i < s.size(); i++) s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}
inline string trim(const string& s) {             // O(L)
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) a++;
    while (b > a && isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}
// commas would break the menu.txt format, so replace them
inline string noCommas(string s) {                // O(L)
    for (size_t i = 0; i < s.size(); i++) if (s[i] == ',') s[i] = ' ';
    return trim(s);
}

// ---------- menu item ----------
struct MenuItem {
    int id;
    string name;
    double price;
    string category;   // full path from root, e.g. "Food/Fast Food/Burger"
};

enum SortKey { BY_ID, BY_PRICE, BY_NAME, UNSORTED };

// =====================================================
// Category tree (first-child / next-sibling representation)
//   Menu
//   +-- Food
//   |   +-- Fast Food
//   |       +-- Burger
//   +-- Drinks
// Printing / counting / deleting use recursion.
// =====================================================
struct CategoryNode {
    string name;
    CategoryNode* firstChild;
    CategoryNode* nextSibling;
};

class CategoryTree {
    CategoryNode* root;

    static CategoryNode* makeNode(const string& name) {
        return new CategoryNode{name, NULL, NULL};
    }

    // find direct child by name (case-insensitive): O(c), c = children
    static CategoryNode* findChild(CategoryNode* parent, const string& name) {
        for (CategoryNode* c = parent->firstChild; c != NULL; c = c->nextSibling)
            if (toLower(c->name) == toLower(name)) return c;
        return NULL;
    }

    // recursive delete: O(V)
    static void destroy(CategoryNode* node) {
        if (node == NULL) return;
        destroy(node->firstChild);
        destroy(node->nextSibling);
        delete node;
    }

    // recursive count of nodes: O(V)
    static int countRec(const CategoryNode* node) {
        if (node == NULL) return 0;
        return 1 + countRec(node->firstChild) + countRec(node->nextSibling);
    }

    // recursive print: O(V * n) because every category scans the item array
    static void printRec(const CategoryNode* node, int depth, const string& path,
                         const MenuItem* items, int n) {
        for (const CategoryNode* c = node->firstChild; c != NULL; c = c->nextSibling) {
            string p = path.empty() ? c->name : path + "/" + c->name;
            cout << "  " << string(depth * 4, ' ') << "+-- " << c->name << "\n";
            for (int i = 0; i < n; i++)
                if (toLower(items[i].category) == toLower(p))
                    cout << "  " << string(depth * 4 + 4, ' ') << "- [" << items[i].id << "] "
                         << items[i].name << "  (" << fixed << setprecision(2) << items[i].price << ")\n";
            printRec(c, depth + 1, p, items, n);   // recursion into sub-categories
        }
    }

public:
    CategoryTree() { root = makeNode("Menu"); }
    ~CategoryTree() { destroy(root); }
    CategoryTree(const CategoryTree&) = delete;
    CategoryTree& operator=(const CategoryTree&) = delete;

    // Add a path like "Food/Fast Food/Burger", creating missing nodes.
    // Returns the cleaned path, or "" if the path is empty. O(d * c)
    string addPath(const string& path) {
        stringstream ss(path);
        string part, clean;
        CategoryNode* cur = root;
        while (getline(ss, part, '/')) {
            part = noCommas(part);
            if (part.empty()) continue;
            CategoryNode* child = findChild(cur, part);
            if (child == NULL) {
                child = makeNode(part);
                child->nextSibling = cur->firstChild;   // insert at front: O(1)
                cur->firstChild = child;
            }
            clean += (clean.empty() ? "" : "/") + child->name;
            cur = child;
        }
        return clean;
    }

    int countCategories() const { return countRec(root) - 1; }   // O(V), root not counted

    void print(const MenuItem* items, int n) const {
        cout << "  Menu\n";
        printRec(root, 1, "", items, n);
    }
};

// =====================================================
// Menu manager: array of items + searching + sorting + file
// =====================================================
class MenuManager {
public:
    static const int MAX = 100;
private:
    MenuItem items[MAX];
    int n;
    int nextId;
    SortKey sortedBy;      // how the array is currently ordered (needed for binary search)
    CategoryTree tree;

    // comparison used by all three sorts
    static bool before(const MenuItem& a, const MenuItem& b, SortKey k) {
        if (k == BY_ID) return a.id < b.id;
        if (k == BY_PRICE) {
            if (a.price != b.price) return a.price < b.price;
            return toLower(a.name) < toLower(b.name);
        }
        return toLower(a.name) < toLower(b.name);   // BY_NAME
    }

    // recursive merge sort on items[lo..hi]: O(n log n), stable
    void mergeRec(int lo, int hi, SortKey k, MenuItem* tmp) {
        if (lo >= hi) return;
        int mid = (lo + hi) / 2;
        mergeRec(lo, mid, k, tmp);
        mergeRec(mid + 1, hi, k, tmp);
        int i = lo, j = mid + 1, t = lo;
        while (i <= mid && j <= hi)
            tmp[t++] = before(items[j], items[i], k) ? items[j++] : items[i++];
        while (i <= mid) tmp[t++] = items[i++];
        while (j <= hi)  tmp[t++] = items[j++];
        for (int x = lo; x <= hi; x++) items[x] = tmp[x];
    }

    void ensureSorted(SortKey k) { if (sortedBy != k) sortMerge(k); }   // O(1) or O(n log n)

public:
    MenuManager() : n(0), nextId(1), sortedBy(BY_ID) {
        // default category skeleton so empty categories are visible too
        tree.addPath("Food/Fast Food/Burger");
        tree.addPath("Food/Fast Food/Pizza");
        tree.addPath("Food/Desi");
        tree.addPath("Drinks/Cold Drinks");
        tree.addPath("Drinks/Hot Drinks");
        tree.addPath("Desserts");
    }

    int size() const { return n; }
    const MenuItem& at(int i) const { return items[i]; }       // O(1)
    int categoryCount() const { return tree.countCategories(); }

    // ---------- category ----------
    bool addCategory(const string& path) { return tree.addPath(path) != ""; }   // O(d * c)

    // ---------- add / remove / update ----------
    // Add item to the end of the array: O(d*c) for category path, O(1) for the array
    // Returns new id, or -1 if the menu is full / input invalid.
    int addItem(const string& name, double price, const string& category) {
        string nm = noCommas(name);
        string cat = tree.addPath(category);
        if (n == MAX || nm.empty() || price < 0 || cat.empty()) return -1;
        items[n++] = {nextId, nm, price, cat};
        if (sortedBy != BY_ID) sortedBy = UNSORTED;   // ids always grow, so BY_ID order stays valid
        return nextId++;
    }

    // Remove by id: linear search + shift left: O(n)
    bool removeItem(int id) {
        int i = linearSearchById(id);
        if (i == -1) return false;
        for (int j = i; j < n - 1; j++) items[j] = items[j + 1];
        n--;
        return true;
    }

    // Change price: O(n)
    bool updatePrice(int id, double price) {
        int i = linearSearchById(id);
        if (i == -1 || price < 0) return false;
        items[i].price = price;
        if (sortedBy == BY_PRICE) sortedBy = UNSORTED;
        return true;
    }

    // ---------- searching (return index in array, or -1) ----------
    int linearSearchById(int id) const {                 // O(n)
        for (int i = 0; i < n; i++) if (items[i].id == id) return i;
        return -1;
    }
    int linearSearchByName(const string& name) const {   // O(n * L)
        string key = toLower(trim(name));
        for (int i = 0; i < n; i++) if (toLower(items[i].name) == key) return i;
        return -1;
    }
    // Binary search needs a sorted array. If it is not sorted by the right key,
    // it is merge-sorted first (O(n log n)); after that every search is O(log n).
    int binarySearchById(int id) {
        ensureSorted(BY_ID);
        int lo = 0, hi = n - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (items[mid].id == id) return mid;
            if (items[mid].id < id) lo = mid + 1; else hi = mid - 1;
        }
        return -1;
    }
    int binarySearchByName(const string& name) {
        ensureSorted(BY_NAME);
        string key = toLower(trim(name));
        int lo = 0, hi = n - 1;
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            string m = toLower(items[mid].name);
            if (m == key) return mid;
            if (m < key) lo = mid + 1; else hi = mid - 1;
        }
        return -1;
    }

    // ---------- sorting (key = BY_PRICE or BY_NAME, ascending) ----------
    // Bubble sort: O(n^2) average/worst, O(n) best (already sorted)
    void sortBubble(SortKey k) {
        for (int i = 0; i < n - 1; i++) {
            bool swapped = false;
            for (int j = 0; j < n - 1 - i; j++)
                if (before(items[j + 1], items[j], k)) {
                    MenuItem t = items[j]; items[j] = items[j + 1]; items[j + 1] = t;
                    swapped = true;
                }
            if (!swapped) break;
        }
        sortedBy = k;
    }

    // Insertion sort: O(n^2) worst/average, O(n) best
    void sortInsertion(SortKey k) {
        for (int i = 1; i < n; i++) {
            MenuItem key = items[i];
            int j = i - 1;
            while (j >= 0 && before(key, items[j], k)) { items[j + 1] = items[j]; j--; }
            items[j + 1] = key;
        }
        sortedBy = k;
    }

    // Merge sort (recursive): O(n log n) all cases, O(n) extra space
    void sortMerge(SortKey k) {
        MenuItem tmp[MAX];
        if (n > 1) mergeRec(0, n - 1, k, tmp);
        sortedBy = k;
    }

    // ---------- display ----------
    void printItem(int i) const {                          // O(1)
        cout << "  [" << items[i].id << "] " << items[i].name << " | "
             << fixed << setprecision(2) << items[i].price << " | " << items[i].category << "\n";
    }
    void display() const {                                 // O(n)
        if (n == 0) { cout << "  (menu is empty)\n"; return; }
        cout << left << setw(6) << "  ID" << setw(24) << "Name" << setw(10) << "Price" << "Category\n";
        for (int i = 0; i < n; i++)
            cout << "  " << left << setw(4) << items[i].id << setw(24) << items[i].name
                 << setw(10) << fixed << setprecision(2) << items[i].price << items[i].category << "\n";
    }
    void displayTree() const { tree.print(items, n); }     // O(V * n)

    // ---------- file handling: menu.txt (id,name,price,category) ----------
    // Save all items: O(n)
    bool saveToFile(const string& file) const {
        ofstream out(file.c_str());
        if (!out) { cout << "Could not open " << file << "\n"; return false; }
        for (int i = 0; i < n; i++)
            out << items[i].id << "," << items[i].name << "," << items[i].price << ","
                << items[i].category << "\n";
        return true;
    }

    // Load all items: O(L * d * c) (category path is added to the tree for each line)
    bool loadFromFile(const string& file) {
        ifstream in(file.c_str());
        if (!in) return false;   // no file yet = empty menu
        n = 0; nextId = 1;
        string line;
        while (getline(in, line) && n < MAX) {
            stringstream ss(line);
            string id, name, price, cat;
            if (getline(ss, id, ',') && getline(ss, name, ',') &&
                getline(ss, price, ',') && getline(ss, cat)) {
                cat = tree.addPath(cat);
                if (cat.empty()) continue;
                items[n++] = {atoi(id.c_str()), name, atof(price.c_str()), cat};
                if (items[n - 1].id >= nextId) nextId = items[n - 1].id + 1;
            }
        }
        sortMerge(BY_ID);   // makes binary search by id valid
        return true;
    }
};

#endif
