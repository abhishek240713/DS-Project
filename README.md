# Library Management System in C

A complete, console-based **Library Management System** written entirely in **C (C99)** for a BTech 3rd-semester Data Structures project.

## Important

- **No login/authentication system** is included.
- No username/password is required.
- The program opens directly to the main menu.
- No database, external library, API, Python, C++, or GUI is required.

## Features

- Book Management: Add, Delete, Update, Display, View Details
- Member Management: Add, Delete, Update, Display, View Details
- Book Issue and Return
- Automatic due dates and fine calculation
- Book Reservation using FIFO Queue
- Transaction History
- Search by Book ID, Title, Author, Category, ISBN, and Member
- BST-based Book ID search
- Bubble, Selection, Insertion, Merge, and Quick Sort
- Recently Viewed Books using Stack
- Undo for supported Book/Member operations using Stack
- Reports and Library Statistics
- Persistent file storage using `.dat` text files
- Sample data initialization (50 books)
- Input validation and memory-allocation checks

## Data Structures

| Data Structure | Usage |
|---|---|
| Linked List | Books, Members, Transactions |
| Queue | Book reservations |
| Stack | Recently viewed books, Undo operations |
| Binary Search Tree | Book ID search |
| Arrays | Sorting and temporary processing |

## Algorithms

| Algorithm | Typical Complexity |
|---|---|
| Linear Search | O(n) |
| Binary Search | O(log n) when array is sorted |
| BST Search | O(log n) average, O(n) worst case |
| Bubble Sort | O(n²) |
| Selection Sort | O(n²) |
| Insertion Sort | O(n²) worst case |
| Merge Sort | O(n log n) |
| Quick Sort | O(n log n) average, O(n²) worst case |

## Project Structure

```text
LibraryManagementSystem/
├── .vscode/
│   └── tasks.json
├── main.c
├── books.c / books.h
├── members.c / members.h
├── transactions.c / transactions.h
├── reservations.c / reservations.h
├── stack.c / stack.h
├── queue.c / queue.h
├── bst.c / bst.h
├── sorting.c / sorting.h
├── search.c / search.h
├── reports.c / reports.h
├── file_manager.c / file_manager.h
├── utils.c / utils.h
├── build.bat
├── run.bat
├── build.sh
└── README.md
```

## Run in VS Code on Windows

### Requirements

Install **GCC/MinGW** and make sure `gcc` is available in the VS Code terminal:

```text
gcc --version
```

Install the Microsoft **C/C++** extension in VS Code for C language support/debugging.

### Method 1 — Build Task

1. Open the `LibraryManagementSystem` folder in VS Code.
2. Press `Ctrl + Shift + B`.
3. Select **Build Library Management System** if VS Code asks for a task.
4. Run the generated `library.exe` in the integrated terminal.

### Method 2 — Use the batch files

In the VS Code terminal:

```bat
build.bat
```

Then:

```bat
run.bat
```

Or simply double-click `run.bat` from Windows Explorer after GCC is installed.

## Run on Linux/macOS

```bash
chmod +x build.sh
./build.sh
./library
```

## GCC Build Command

```bash
gcc -std=c99 -Wall -Wextra -Wpedantic -O2 main.c books.c members.c transactions.c reservations.c stack.c queue.c bst.c sorting.c search.c file_manager.c reports.c utils.c -o library
```

On Windows, `build.bat` uses the same source files and produces `library.exe`.

## Data Files

The program stores data in the project folder:

```text
books.dat
members.dat
transactions.dat
reservations.dat
counters.dat
```

These files are created/updated automatically when the program saves data or exits.

## First Run

The program starts directly at the main menu. If no saved records exist, choose:

```text
14. Initialize Sample Data
```

Sample initialization is blocked when records already exist so duplicate sample data cannot be created accidentally.

## Main Menu

```text
1. Book Management
2. Member Management
3. Issue Book
4. Return Book
5. Book Reservation
6. Search
7. Sorting
8. Transaction History
9. Fine Management
10. Reports
11. Library Statistics
12. Recently Viewed Books
13. Undo Last Operation
14. Initialize Sample Data
15. Save Data
16. About Project
0. Exit
```

## Testing Completed

The project was compiled with GCC using:

```text
-std=c99 -Wall -Wextra -Wpedantic -O2
```

The main workflow was also exercised with runtime testing, including sample data loading, issuing and returning books, reservations, automatic reservation processing, BST search, sorting, persistence, and clean exit.

## Viva Topics

Be ready to explain:

- Structures and pointers
- Dynamic memory allocation
- Linked List insertion/deletion/traversal
- Queue and FIFO reservation handling
- Stack and LIFO undo/recent-history handling
- Binary Search Tree insertion/search/deletion
- Searching algorithms
- Sorting algorithms and complexity
- File handling and persistence
- Date and fine calculation
- Memory cleanup and error handling
