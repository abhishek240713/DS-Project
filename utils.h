#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

/* ---- Constants ---- */
#define MAX_TITLE 100
#define MAX_AUTHOR 100
#define MAX_CATEGORY 50
#define MAX_ISBN 20
#define MAX_PUBLISHER 100
#define MAX_NAME 100
#define MAX_PHONE 20
#define MAX_EMAIL 100
#define MAX_BORROW_LIMIT 5
#define BORROW_PERIOD_DAYS 14
#define FINE_PER_DAY 5.0f
#define MAX_VIEW_STACK 20

#define BOOKS_FILE "books.dat"
#define MEMBERS_FILE "members.dat"
#define TRANSACTIONS_FILE "transactions.dat"
#define RESERVATIONS_FILE "reservations.dat"
#define COUNTERS_FILE "counters.dat"

/* ---- Book Structure (Linked List Node) ---- */
typedef struct Book {
    int id;
    char title[MAX_TITLE];
    char author[MAX_AUTHOR];
    char category[MAX_CATEGORY];
    char isbn[MAX_ISBN];
    char publisher[MAX_PUBLISHER];
    int totalCopies;
    int availableCopies;
    int issuedCopies;
    int timesBorrowed;
    struct Book *next;
} Book;

/* ---- Member Structure (Linked List Node) ---- */
typedef struct Member {
    int id;
    char name[MAX_NAME];
    char phone[MAX_PHONE];
    char email[MAX_EMAIL];
    int booksCurrentlyBorrowed;
    int totalBooksBorrowed;
    float pendingFine;
    struct Member *next;
} Member;

/* ---- Transaction Status ---- */
typedef enum { STATUS_ACTIVE = 0, STATUS_RETURNED = 1 } TransStatus;

/* ---- Transaction Structure (Linked List Node) ---- */
typedef struct Transaction {
    int id;
    int memberId;
    int bookId;
    time_t issueDate;
    time_t dueDate;
    time_t returnDate;
    float fine;
    TransStatus status;
    struct Transaction *next;
} Transaction;

/* ---- Reservation Queue Node ---- */
typedef struct ReservationNode {
    int memberId;
    int bookId;
    time_t reservationDate;
    struct ReservationNode *next;
} ReservationNode;

/* ---- Reservation Queue ---- */
typedef struct ReservationQueue {
    ReservationNode *front;
    ReservationNode *rear;
    int count;
} ReservationQueue;

/* ---- BST Node for Book ID Search ---- */
typedef struct BSTNode {
    int bookId;
    Book *bookPtr;
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;

/* ---- View Stack Node (Recently Viewed Books) ---- */
typedef struct ViewStackNode {
    int bookId;
    struct ViewStackNode *next;
} ViewStackNode;

/* ---- Undo Operation Types ---- */
typedef enum {
    UNDO_ADD_BOOK,
    UNDO_DELETE_BOOK,
    UNDO_UPDATE_BOOK,
    UNDO_ADD_MEMBER,
    UNDO_DELETE_MEMBER,
    UNDO_UPDATE_MEMBER
} UndoType;

/* ---- Undo Stack Node ---- */
typedef struct UndoNode {
    UndoType type;
    void *data;   /* malloc'd copy of Book or Member struct */
    int id;        /* ID of the affected record */
    struct UndoNode *next;
} UndoNode;

/* ---- Global ID Counters ---- */
typedef struct {
    int nextBookId;
    int nextMemberId;
    int nextTransactionId;
} IdCounters;

/* ---- Utility Function Declarations ---- */
void clearInputBuffer(void);
void pressEnterToContinue(void);
void clearScreen(void);
int getIntInput(const char *prompt);
float getFloatInput(const char *prompt);
void getStringInput(const char *prompt, char *buffer, int maxLen);
bool isValidEmail(const char *email);
bool isValidPhone(const char *phone);
void formatDate(time_t t, char *buffer, int bufLen);
time_t addDays(time_t t, int days);
int daysBetween(time_t t1, time_t t2);
void printHeader(const char *title);
void printSeparator(void);
void toLowerStr(char *dest, const char *src);

#endif
