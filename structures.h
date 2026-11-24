// structures.h - data structures for Kitchen, Reports (Member: Kitchen/Reports/Docs)
#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
using namespace std;


enum Status { PENDING, PREPARING, READY, SERVED };

inline string statusName(Status s) {
    switch (s) {
        case PENDING:   return "Pending";
        case PREPARING: return "Preparing";
        case READY:     return "Ready";
        default:        return "Served";
    }
}

struct Order {
    int id;
    int table;
    string item;
    int qty;
    double price;   
    Status status;
};

struct OrderNode {
    Order data;
    OrderNode* next;
};

class KitchenQueue {
    OrderNode* front;
    OrderNode* rear;
    int count;
public:
    KitchenQueue() : front(NULL), rear(NULL), count(0) {} //constructor
    ~KitchenQueue() { while (!isEmpty()) dequeue(); } //distructor

    bool isEmpty() const { return front == NULL; }
    int size() const { return count; }


    void enqueue(const Order& o) {
        OrderNode* n = new OrderNode{o, NULL};
        if (rear == NULL) front = rear = n;
        else { rear->next = n; rear = n; }
        count++;
    }

    Order dequeue() {
        OrderNode* t = front;
        Order o = t->data;
        front = front->next;
        if (front == NULL) rear = NULL;
        delete t;
        count--;
        return o;
    }


    void display() const {
        if (isEmpty()) { cout << "  (no pending orders)\n"; return; }
        for (OrderNode* c = front; c != NULL; c = c->next)
            cout << "  #" << c->data.id << " | Table " << c->data.table << " | "
                 << c->data.item << " x" << c->data.qty << " | "
                 << statusName(c->data.status) << "\n";
    }
};

// waiting list 
struct Customer {
    string name;
    int partySize;
};

class WaitingList {
    static const int CAP = 10;
    Customer arr[CAP];
    int front, rear, count;
public:
    WaitingList() : front(0), rear(-1), count(0) {}
    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == CAP; }


    bool enqueue(const Customer& c) {
        if (isFull()) return false;
        rear = (rear + 1) % CAP;
        arr[rear] = c;
        count++;
        return true;
    }

   
    bool dequeue(Customer& out) {
        if (isEmpty()) return false;
        out = arr[front];
        front = (front + 1) % CAP;
        count--;
        return true;
    }

  
    void display() const {
        if (isEmpty()) { cout << "  (waiting list empty)\n"; return; }
        for (int i = 0; i < count; i++) {
            const Customer& c = arr[(front + i) % CAP];
            cout << "  " << i + 1 << ". " << c.name << " (party of " << c.partySize << ")\n";
        }
    }
};

//  kitchen staff 
struct StaffNode {
    string name;
    StaffNode* next;
};

class StaffRotation {
    StaffNode* tail;   // tail->next is the head
    StaffNode* current;
public:
    StaffRotation() : tail(NULL), current(NULL) {}
    ~StaffRotation() {
        if (!tail) return;
        StaffNode* c = tail->next;
        tail->next = NULL;
        while (c) { StaffNode* t = c; c = c->next; delete t; }
    }

    void addStaff(const string& name) {
        StaffNode* n = new StaffNode{name, NULL};
        if (!tail) { n->next = n; tail = n; current = n; }
        else { n->next = tail->next; tail->next = n; tail = n; }
    }

    string nextOnDuty() {
        if (!current) return "(no staff)";
        string name = current->name;
        current = current->next;
        return name;
    }

    void display() const {
        if (!tail) { cout << "  (no staff)\n"; return; }
        StaffNode* c = tail->next;
        do { cout << "  " << c->name << (c == current ? "  <- next on duty" : "") << "\n"; c = c->next; }
        while (c != tail->next);
    }
};

//  restaurant table map 
class TableMap {
    static const int N = 8;
    bool adj[N][N];
public:
    TableMap() { for (int i = 0; i < N; i++) for (int j = 0; j < N; j++) adj[i][j] = false; }

    // table 0 = kitchen, tables 1..7
    void connect(int a, int b) { adj[a][b] = adj[b][a] = true; }

    // BFS from kitchen: O(V^2) with adjacency matrix
    void distancesFromKitchen() const {
        int dist[N], q[N], f = 0, r = 0;
        for (int i = 0; i < N; i++) dist[i] = -1;
        dist[0] = 0; q[r++] = 0;
        while (f < r) {
            int u = q[f++];
            for (int v = 0; v < N; v++)
                if (adj[u][v] && dist[v] == -1) { dist[v] = dist[u] + 1; q[r++] = v; }
        }
        for (int i = 1; i < N; i++) {
            cout << "  Table " << i << ": ";
            if (dist[i] == -1) cout << "unreachable\n"; else cout << dist[i] << " step(s) from kitchen\n";
        }
    }
};

#endif
