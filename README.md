# ROMS - Restaurant Order Management System
DSA group project 
Zainab Fatima - 2025893
Zarghoon Shaukat - 2025906
Maraim Manan - 2025385 
Ahmed Ali - 2025078

# Restaurant Management System: Kitchen, Reports & Documentation

Part of the group Data Structures project (CS221). This module covers the kitchen
order flow, waiting list, staff rotation, sales reporting, and documentation.

## Build & run
```bash
g++ -std=c++11 main.cpp -o kitchen
./kitchen
```

## Files
| File | Purpose |
|------|---------|
| `structures.h` | Kitchen queue, waiting list, staff rotation, table map |
| `report.h` | Sales report: sorting and `sales.txt` file handling |
| `main.cpp` | Menu-driven program |
| `sales.txt` | Created automatically when an order is served (`item,qty,price`) |

## Order status flow
`Pending → Preparing → Ready → Served`

1. New order goes into the kitchen queue (Pending).
2. "Start preparing" dequeues the oldest order (Preparing), assigned to the next cook.
3. "Mark Ready", then "Mark Served". Serving appends the sale to `sales.txt`.

## Data structures and Big-O

n = number of elements currently stored.

### 1. Kitchen queue (linked list, FIFO)
| Operation | Time | Space |
|-----------|------|-------|
| enqueue (rear pointer) | O(1) | O(1) |
| dequeue | O(1) | O(1) |
| display | O(n) | O(1) |

### 2. Waiting list (circular array queue, capacity 10)
| Operation | Time | Space |
|-----------|------|-------|
| enqueue | O(1) | O(1) |
| dequeue | O(1) | O(1) |
| display | O(n) | O(1) |

The circular array reuses freed slots using `(index + 1) % CAP`, so nothing is shifted.

### 3. Staff rotation (circular linked list)
| Operation | Time | Space |
|-----------|------|-------|
| addStaff | O(1) | O(1) |
| nextOnDuty | O(1) | O(1) |
| display | O(n) | O(1) |

### 4. Sales report
| Operation | Time | Space |
|-----------|------|-------|
| addSale (linear search for item) | O(n) | O(1) |
| Bubble sort by quantity sold | O(n²) average/worst, O(n) best | O(1) |
| Load `sales.txt` (L lines) | O(L · n) | O(n) |
| Append sale to file | O(1) | O(1) |

### 5. Table map (graph, adjacency matrix)
| Operation | Time | Space |
|-----------|------|-------|
| connect two tables | O(1) | O(V²) |
| BFS distance from kitchen | O(V²) | O(V) |

## Notes
- Bubble sort is used because it is covered in the course; it is fine for a small menu.
- The graph and circular linked list are the optional parts from the task list.
- Capacity limits: waiting list 10, active orders 50, menu items in report 100.
