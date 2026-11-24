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
| `menu.h` | Menu management: item array, searching, sorting, category tree, `menu.txt` file handling |
| `menu_ui.h` | Menu-driven screen for menu management (`menuManagementScreen`) |
| `menu_demo.cpp` | Standalone test program for the menu module |
| `menu.txt` | Created when the menu is saved (`id,name,price,category`) |

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

## Menu Management (Mariam)

Build and run the standalone version:
```bash
g++ -std=c++11 menu_demo.cpp -o menu
./menu
```

n = number of menu items, V = number of categories, L = lines in `menu.txt`.

### 6. Menu items (array, capacity 100)
| Operation | Time | Space |
|-----------|------|-------|
| add item (append to end) | O(1) + category path | O(1) |
| remove item (search + shift left) | O(n) | O(1) |
| update price | O(n) | O(1) |
| display all | O(n) | O(1) |

### 7. Searching
| Operation | Time | Space |
|-----------|------|-------|
| linear search by ID | O(n) | O(1) |
| linear search by name | O(n) | O(1) |
| binary search by ID / name | O(log n) | O(1) |

Binary search needs a sorted array. If the array is not already sorted by the right key, it is merge-sorted first (O(n log n)), and later searches are O(log n).

### 8. Sorting (by price or name)
| Algorithm | Time | Space |
|-----------|------|-------|
| Bubble sort | O(n²) average/worst, O(n) best | O(1) |
| Insertion sort | O(n²) average/worst, O(n) best | O(1) |
| Merge sort (recursive) | O(n log n) all cases | O(n) |

### 9. Category tree (first-child / next-sibling, recursion)
Example: `Menu → Food → Fast Food → Burger`

| Operation | Time | Space |
|-----------|------|-------|
| add category path | O(d · c) | O(d) |
| print tree with items (recursive) | O(V · n) | O(depth) |
| count / delete tree (recursive) | O(V) | O(depth) |

d = depth of the path, c = children per node.

### 10. File handling (`menu.txt`)
| Operation | Time | Space |
|-----------|------|-------|
| save menu | O(n) | O(1) |
| load menu | O(L) plus category paths | O(n) |

Each line is `id,name,price,category path`. The category tree is rebuilt from the paths when the file is loaded.

## Notes
- Bubble sort is used because it is covered in the course; it is fine for a small menu.
- The graph and circular linked list are the optional parts from the task list.
- Capacity limits: waiting list 10, active orders 50, menu items in report 100.
- Menu capacity limit: 100 items. Commas in item names and categories are replaced with spaces so `menu.txt` stays valid.
- Three sorting algorithms are included (bubble, insertion, merge) so their speeds can be compared.
- Binary search by ID works because new IDs always increase, so the array stays sorted by ID after adding items.
