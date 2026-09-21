#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "books.h"
#include "members.h"
#include "transactions.h"
#include "reservations.h"
#include "search.h"
#include "sorting.h"
#include "reports.h"
#include "file_manager.h"
#include "stack.h"
#include "queue.h"
#include "bst.h"

static Book *bookList = NULL;
static Member *memberList = NULL;
static Transaction *transList = NULL;
static ReservationQueue resQueue;
static BSTNode *bstRoot = NULL;
static ViewStackNode *viewStack = NULL;
static UndoNode *undoStack = NULL;
static IdCounters counters;

void displayMainMenu(void) {
    clearScreen();
    //printf("================================================\n");
    printf("           LIBRARY MANAGEMENT SYSTEM\n");
    //printf("================================================\n\n");
    printf(" 1. Book Management\n");
    printf(" 2. Member Management\n");
    printf(" 3. Issue Book\n");
    printf(" 4. Return Book\n");
    printf(" 5. Book Reservation\n");
    printf(" 6. Search\n");
    printf(" 7. Sorting\n");
    printf(" 8. Transaction History\n");
    printf(" 9. Fine Management\n");
    printf("10. Reports\n");
    printf("11. Library Statistics\n");
    printf("12. Recently Viewed Books\n");
    printf("13. Undo Last Operation\n");
    printf("14. Initialize Sample Data\n");
    printf("15. Save Data\n");
    printf("16. About Project\n");
    printf(" 0. Exit\n\n");
    printf("================================================\n");
}

void initializeSampleData(Book **bList, Member **mList, Transaction **tList, BSTNode **root, IdCounters *cntrs) {
    if (*bList || *mList || *tList) {
        printf("Sample data was not initialized because library data already exists.\n");
        printf("Use the existing records or remove the data files before starting fresh.\n");
        return;
    }

    /* 50 realistic sample books covering major academic and technical subjects. */
    static const char *titles[50] = {
        "Data Structures and Algorithms", "Operating System Concepts", "Database System Concepts",
        "Computer Networks", "Digital Logic Design", "Engineering Mathematics", "The C Programming Language",
        "Clean Code", "Design Patterns", "Artificial Intelligence", "Computer Organization and Architecture",
        "Software Engineering", "Object Oriented Programming with C++", "Python Crash Course",
        "Introduction to Algorithms", "Discrete Mathematics and Its Applications", "Compiler Design",
        "Web Technologies", "Computer Graphics", "Machine Learning", "Deep Learning", "Cloud Computing",
        "Cyber Security Essentials", "Cryptography and Network Security", "Data Mining",
        "Big Data Analytics", "Internet of Things", "Operating Systems: Three Easy Pieces",
        "Modern Control Engineering", "Signals and Systems", "Electronic Devices and Circuit Theory",
        "Microprocessors and Microcontrollers", "Power System Engineering", "Thermodynamics",
        "Engineering Physics", "Engineering Chemistry", "Object-Oriented Analysis and Design",
        "System Design Interview", "Head First Design Patterns", "The Pragmatic Programmer",
        "Effective Java", "Let Us C", "Programming in ANSI C", "C Programming: A Modern Approach",
        "Structure and Interpretation of Computer Programs", "Artificial Intelligence: A Modern Approach",
        "Hands-On Machine Learning", "Fundamentals of Database Systems", "Computer Architecture: A Quantitative Approach",
        "Pattern Recognition and Machine Learning"
    };

    static const char *authors[50] = {
        "Thomas H. Cormen", "Abraham Silberschatz", "Abraham Silberschatz", "Andrew S. Tanenbaum",
        "Morris Mano", "B.S. Grewal", "Brian W. Kernighan", "Robert C. Martin", "Erich Gamma",
        "Stuart Russell", "William Stallings", "Ian Sommerville", "Robert Lafore", "Eric Matthes",
        "Thomas H. Cormen", "Kenneth H. Rosen", "Alfred V. Aho", "Jeffrey C. Jackson", "Donald D. Hearn",
        "Tom M. Mitchell", "Ian Goodfellow", "Thomas Erl", "Charles J. Brooks", "William Stallings",
        "Jiawei Han", "Vijay Kotu", "Samuel Greengard", "Remzi H. Arpaci-Dusseau", "Katsuhiko Ogata",
        "Alan V. Oppenheim", "Boylestad & Nashelsky", "Krishna Kant", "C.L. Wadhwa", "Yunus A. Cengel",
        "P.K. Nag", "Jain & Bhattacharyya", "Martin Fowler", "Alex Xu", "Eric Freeman",
        "Andrew Hunt", "Joshua Bloch", "Yashavant Kanetkar", "E. Balagurusamy", "K.N. King",
        "Harold Abelson", "Stuart Russell", "Aurelien Geron", "Elmasri & Navathe", "John L. Hennessy",
        "Christopher M. Bishop"
    };

    static const char *categories[50] = {
        "Computer Science", "Computer Science", "Computer Science", "Computer Science", "Electronics",
        "Mathematics", "Programming", "Software Engineering", "Software Engineering", "AI/ML",
        "Computer Science", "Software Engineering", "Programming", "Programming", "Algorithms",
        "Mathematics", "Programming", "Web Development", "Computer Graphics", "AI/ML", "AI/ML",
        "Cloud Computing", "Cyber Security", "Cyber Security", "Data Science", "Data Science",
        "IoT", "Operating Systems", "Control Systems", "Electronics", "Electronics", "Microprocessors",
        "Electrical Engineering", "Mechanical Engineering", "Engineering", "Engineering Chemistry",
        "Software Engineering", "System Design", "Software Engineering", "Software Engineering",
        "Programming", "Programming", "Programming", "Programming", "Programming", "AI/ML",
        "AI/ML", "Databases", "Computer Architecture", "AI/ML"
    };

    static const char *publishers[50] = {
        "MIT Press", "Wiley", "McGraw-Hill", "Pearson", "Pearson", "Khanna Publishers", "Prentice Hall",
        "Prentice Hall", "Addison-Wesley", "Pearson", "Pearson", "Pearson", "SAMS Publishing", "No Starch Press",
        "MIT Press", "McGraw-Hill", "Pearson", "Addison-Wesley", "Pearson", "McGraw-Hill", "MIT Press",
        "CRC Press", "Wiley", "Pearson", "Morgan Kaufmann", "Packt", "Wiley", "Arpaci-Dusseau Books",
        "Wiley", "Prentice Hall", "Pearson", "McGraw-Hill", "New Age International", "McGraw-Hill",
        "Wiley", "New Age International", "Addison-Wesley", "Alex Xu", "O'Reilly Media", "Addison-Wesley",
        "Addison-Wesley", "Techmax", "Tata McGraw-Hill", "W.W. Norton", "MIT Press", "Pearson",
        "O'Reilly Media", "Pearson", "Morgan Kaufmann", "Springer"
    };

    const int copies[50] = {
        5,3,4,3,2,6,4,3,2,3,4,3,2,4,5,3,2,4,2,4,3,3,2,2,3,3,2,3,2,3,4,3,2,2,4,3,2,3,3,4,
        3,5,4,3,2,3,4,3,2,3
    };

    Book *sampleBooks[50] = {0};
    for (int i = 0; i < 50; ++i) {
        sampleBooks[i] = createBook(cntrs->nextBookId++, titles[i], authors[i], categories[i],
                                    "", publishers[i], copies[i]);
        if (!sampleBooks[i]) {
            printf("Failed to create sample book %d.\n", i + 1);
            continue;
        }

        /* Generate a unique demo ISBN without external dependencies. */
        snprintf(sampleBooks[i]->isbn, MAX_ISBN, "978-0000-%06d", sampleBooks[i]->id);
        *bList = addBookToList(*bList, sampleBooks[i]);
        *root = insertBST(*root, sampleBooks[i]->id, sampleBooks[i]);
    }

    Member *m1 = createMember(cntrs->nextMemberId++, "Rahul Sharma", "9876543210", "rahul@email.com");
    Member *m2 = createMember(cntrs->nextMemberId++, "Priya Patel", "9876543211", "priya@email.com");
    Member *m3 = createMember(cntrs->nextMemberId++, "Amit Kumar", "9876543212", "amit@email.com");
    Member *m4 = createMember(cntrs->nextMemberId++, "Sneha Gupta", "9876543213", "sneha@email.com");
    Member *m5 = createMember(cntrs->nextMemberId++, "Vikram Singh", "9876543214", "vikram@email.com");

    *mList = addMemberToList(*mList, m1);
    *mList = addMemberToList(*mList, m2);
    *mList = addMemberToList(*mList, m3);
    *mList = addMemberToList(*mList, m4);
    *mList = addMemberToList(*mList, m5);

    time_t now = time(NULL);
    time_t t1_issue = now - 10 * 86400;
    time_t t1_due = t1_issue + 14 * 86400;
    Transaction *t1 = createTransaction(cntrs->nextTransactionId++, m1->id, sampleBooks[0]->id, t1_issue, t1_due);
    sampleBooks[0]->availableCopies--; sampleBooks[0]->issuedCopies++; sampleBooks[0]->timesBorrowed++;
    m1->booksCurrentlyBorrowed++; m1->totalBooksBorrowed++;
    *tList = addTransactionToList(*tList, t1);

    time_t t2_issue = now - 5 * 86400;
    time_t t2_due = t2_issue + 14 * 86400;
    Transaction *t2 = createTransaction(cntrs->nextTransactionId++, m2->id, sampleBooks[2]->id, t2_issue, t2_due);
    sampleBooks[2]->availableCopies--; sampleBooks[2]->issuedCopies++; sampleBooks[2]->timesBorrowed++;
    m2->booksCurrentlyBorrowed++; m2->totalBooksBorrowed++;
    *tList = addTransactionToList(*tList, t2);

    time_t t3_issue = now - 3 * 86400;
    time_t t3_due = t3_issue + 14 * 86400;
    Transaction *t3 = createTransaction(cntrs->nextTransactionId++, m1->id, sampleBooks[6]->id, t3_issue, t3_due);
    sampleBooks[6]->availableCopies--; sampleBooks[6]->issuedCopies++; sampleBooks[6]->timesBorrowed++;
    m1->booksCurrentlyBorrowed++; m1->totalBooksBorrowed++;
    *tList = addTransactionToList(*tList, t3);

    printf("Sample data initialized successfully!\n");
    printf("Added 50 Books, 5 Members and 3 Transactions.\n");
}

void undoLastOperation(Book **bList, Member **mList, BSTNode **root, Transaction *transList, UndoNode **uStack) {
    if (isUndoStackEmpty(*uStack)) {
        printf("Nothing to undo.\n");
        return;
    }
    
    UndoNode *node = popUndo(uStack);
    if (!node) return;
    
    int targetId = node->id;
    
    switch (node->type) {
        case UNDO_ADD_BOOK: {
            Transaction *t = transList;
            while (t) {
                if (t->bookId == targetId && t->status == STATUS_ACTIVE) {
                    printf("Cannot undo Book ID %d: it has an active transaction.\n", targetId);
                    if (node->data) free(node->data);
                    free(node);
                    pressEnterToContinue();
                    return;
                }
                t = t->next;
            }
            Book *prev = NULL, *curr = *bList;
            while (curr) {
                if (curr->id == targetId) {
                    if (prev) prev->next = curr->next;
                    else *bList = curr->next;
                    *root = deleteBST(*root, targetId);
                    free(curr);
                    break;
                }
                prev = curr;
                curr = curr->next;
            }
            printf("Undo: Book ID %d addition reversed.\n", targetId);
            break;
        }
        case UNDO_DELETE_BOOK: {
            if (node->data) {
                Book *saved = (Book*)node->data;
                Book *newBook = createBook(saved->id, saved->title, saved->author, saved->category, saved->isbn, saved->publisher, saved->totalCopies);
                newBook->availableCopies = saved->availableCopies;
                newBook->issuedCopies = saved->issuedCopies;
                newBook->timesBorrowed = saved->timesBorrowed;
                *bList = addBookToList(*bList, newBook);
                *root = insertBST(*root, newBook->id, newBook);
                printf("Undo: Book ID %d deletion reversed.\n", targetId);
            }
            break;
        }
        case UNDO_UPDATE_BOOK: {
            if (node->data) {
                Book *oldData = (Book*)node->data;
                Book *curr = findBookById(*bList, targetId);
                if (curr) {
                    Book *nxt = curr->next;
                    memcpy(curr, oldData, sizeof(Book));
                    curr->next = nxt;
                    printf("Undo: Book ID %d update reversed.\n", targetId);
                }
            }
            break;
        }
        case UNDO_ADD_MEMBER: {
            Transaction *t = transList;
            while (t) {
                if (t->memberId == targetId && t->status == STATUS_ACTIVE) {
                    printf("Cannot undo Member ID %d: it has an active transaction.\n", targetId);
                    if (node->data) free(node->data);
                    free(node);
                    pressEnterToContinue();
                    return;
                }
                t = t->next;
            }
            Member *prev = NULL, *curr = *mList;
            while (curr) {
                if (curr->id == targetId) {
                    if (prev) prev->next = curr->next;
                    else *mList = curr->next;
                    free(curr);
                    break;
                }
                prev = curr;
                curr = curr->next;
            }
            printf("Undo: Member ID %d addition reversed.\n", targetId);
            break;
        }
        case UNDO_DELETE_MEMBER: {
            if (node->data) {
                Member *saved = (Member*)node->data;
                Member *newMember = createMember(saved->id, saved->name, saved->phone, saved->email);
                newMember->booksCurrentlyBorrowed = saved->booksCurrentlyBorrowed;
                newMember->totalBooksBorrowed = saved->totalBooksBorrowed;
                newMember->pendingFine = saved->pendingFine;
                *mList = addMemberToList(*mList, newMember);
                printf("Undo: Member ID %d deletion reversed.\n", targetId);
            }
            break;
        }
        case UNDO_UPDATE_MEMBER: {
            if (node->data) {
                Member *oldData = (Member*)node->data;
                Member *curr = findMemberById(*mList, targetId);
                if (curr) {
                    Member *nxt = curr->next;
                    memcpy(curr, oldData, sizeof(Member));
                    curr->next = nxt;
                    printf("Undo: Member ID %d update reversed.\n", targetId);
                }
            }
            break;
        }
    }
    
    if (node->data) {
        free(node->data);
    }
    free(node);
    pressEnterToContinue();
}

void aboutProject(void) {
    printHeader("About Project");
    printf("Project: Library Management System\n");
    printf("Language: C (C99)\n");
    printf("Type: BTech 3rd Semester - Data Structures Project\n\n");
    printf("Data Structures Used:\n");
    printf("  - Linked List (Books, Members, Transactions)\n");
    printf("  - Stack (Recently Viewed, Undo Operations)\n");
    printf("  - Queue (Book Reservations)\n");
    printf("  - Binary Search Tree (Book ID Search)\n");
    printf("  - Arrays (Sorting, Statistics)\n\n");
    printf("Algorithms Used:\n");
    printf("  - Linear Search O(n)\n");
    printf("  - Binary Search O(log n)\n");
    printf("  - BST Search O(log n) average\n");
    printf("  - Bubble Sort O(n^2)\n");
    printf("  - Selection Sort O(n^2)\n");
    printf("  - Insertion Sort O(n^2)\n");
    printf("  - Merge Sort O(n log n)\n");
    printf("  - Quick Sort O(n log n) average\n\n");
    printf("Features:\n");
    printf("  - Book Management (CRUD)\n");
    printf("  - Member Management (CRUD)\n");
    printf("  - Book Issue and Return\n");
    printf("  - Fine Calculation\n");
    printf("  - Reservation Queue (FIFO)\n");
    printf("  - Transaction History\n");
    printf("  - Search (Linear, Binary, BST)\n");
    printf("  - Sorting (5 algorithms, 5 criteria)\n");
    printf("  - Reports and Statistics\n");
    printf("  - File Persistence\n");
    printf("  - Undo Operations\n");
    printf("  - Recently Viewed Books\n");
    printf("  - Sample Data Initialization\n");
    pressEnterToContinue();
}

static void syncCountersWithLoadedData(IdCounters *c, Book *books, Member *members, Transaction *transactions) {
    int maxBookId = 1000;
    int maxMemberId = 100;
    int maxTransactionId = 0;

    for (Book *b = books; b; b = b->next)
        if (b->id > maxBookId) maxBookId = b->id;
    for (Member *m = members; m; m = m->next)
        if (m->id > maxMemberId) maxMemberId = m->id;
    for (Transaction *t = transactions; t; t = t->next)
        if (t->id > maxTransactionId) maxTransactionId = t->id;

    if (c->nextBookId <= maxBookId) c->nextBookId = maxBookId + 1;
    if (c->nextMemberId <= maxMemberId) c->nextMemberId = maxMemberId + 1;
    if (c->nextTransactionId <= maxTransactionId) c->nextTransactionId = maxTransactionId + 1;
}

int main(void) {
    initQueue(&resQueue);
    counters.nextBookId = 1001;
    counters.nextMemberId = 101;
    counters.nextTransactionId = 1;

    loadCounters(&counters);
    bookList = loadBooks();
    memberList = loadMembers();
    transList = loadTransactions();
    loadReservations(&resQueue);

    bstRoot = buildBSTFromList(bookList);

    syncCountersWithLoadedData(&counters, bookList, memberList, transList);
    printf("Library data loaded.\n");
    if (!bookList && !memberList && !transList) {
        printf("No saved records were found. Use option 14 to initialize sample data.\n");
    }

    int choice;
    do {
        displayMainMenu();
        choice = getIntInput("Enter your choice: ");
        
        switch (choice) {
            case 1:
                bookManagementMenu(&bookList, &bstRoot, &counters, transList, &undoStack, &viewStack);
                break;
            case 2:
                memberManagementMenu(&memberList, &counters, transList, &undoStack);
                break;
            case 3:
                transList = issueBook(transList, bookList, memberList, bstRoot, &counters, &resQueue);
                pressEnterToContinue();
                break;
            case 4:
                transList = returnBook(transList, bookList, memberList, &resQueue, &counters);
                pressEnterToContinue();
                break;
            case 5:
                reservationMenu(&resQueue, bookList, memberList, &transList, &counters);
                break;
            case 6:
                searchMenu(bookList, memberList, bstRoot, &viewStack);
                break;
            case 7:
                sortingMenu(bookList);
                break;
            case 8:
                transactionHistoryMenu(transList, memberList, bookList);
                break;
            case 9:
                fineManagementMenu(memberList, transList, bookList);
                break;
            case 10:
                reportsMenu(bookList, memberList, transList, &resQueue);
                break;
            case 11:
                libraryStatistics(bookList, memberList, transList, &resQueue);
                break;
            case 12:
                printHeader("Recently Viewed Books");
                displayViewStack(viewStack, bookList);
                pressEnterToContinue();
                break;
            case 13:
                undoLastOperation(&bookList, &memberList, &bstRoot, transList, &undoStack);
                break;
            case 14:
                initializeSampleData(&bookList, &memberList, &transList, &bstRoot, &counters);
                pressEnterToContinue();
                break;
            case 15:
                saveAllData(bookList, memberList, transList, &resQueue, &counters);
                pressEnterToContinue();
                break;
            case 16:
                aboutProject();
                break;
            case 0:
                printf("\nSaving data before exit...\n");
                saveAllData(bookList, memberList, transList, &resQueue, &counters);
                printf("Thank you for using Library Management System!\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
                pressEnterToContinue();
                break;
        }
    } while (choice != 0);

    freeBooks(bookList);
    freeMembers(memberList);
    freeTransactions(transList);
    freeQueue(&resQueue);
    freeBST(bstRoot);
    freeViewStack(&viewStack);
    freeUndoStack(&undoStack);

    return 0;
}
