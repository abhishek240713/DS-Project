#include "reports.h"
#include "books.h"
#include "members.h"
#include "transactions.h"
#include <stdio.h>
#include <stdlib.h>

void bookReport(Book *head) {
    int choice;
    do {
        clearScreen();
        printHeader("Book Report");
        printf("1. All Books\n");
        printf("2. Available Books\n");
        printf("3. Issued Books\n");
        printf("4. Most Borrowed Books\n");
        printf("0. Back\n");
        choice = getIntInput("Select an option: ");

        switch (choice) {
            case 1: {
                // Call displayAllBooks from books module, assuming it exists or doing it here
                Book *curr = head;
                while (curr) {
                    printf("ID: %d | Title: %s | Total: %d | Available: %d\n", curr->id, curr->title, curr->totalCopies, curr->availableCopies);
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 2: {
                Book *curr = head;
                while (curr) {
                    if (curr->availableCopies > 0)
                        printf("ID: %d | Title: %s | Available: %d\n", curr->id, curr->title, curr->availableCopies);
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 3: {
                Book *curr = head;
                while (curr) {
                    if (curr->issuedCopies > 0)
                        printf("ID: %d | Title: %s | Issued: %d\n", curr->id, curr->title, curr->issuedCopies);
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 4: {
                int count = countBooks(head);
                if (count > 0) {
                    Book **arr = (Book **)malloc(count * sizeof(Book *));
                    Book *curr = head;
                    for (int i = 0; i < count; i++) {
                        arr[i] = curr;
                        curr = curr->next;
                    }
                    // Simple selection sort descending by timesBorrowed
                    for (int i = 0; i < count - 1; i++) {
                        int max_idx = i;
                        for (int j = i + 1; j < count; j++) {
                            if (arr[j]->timesBorrowed > arr[max_idx]->timesBorrowed) max_idx = j;
                        }
                        Book *temp = arr[max_idx];
                        arr[max_idx] = arr[i];
                        arr[i] = temp;
                    }
                    int displayCount = count < 10 ? count : 10;
                    for (int i = 0; i < displayCount; i++) {
                        printf("%d. %s (Borrowed %d times)\n", i + 1, arr[i]->title, arr[i]->timesBorrowed);
                    }
                    free(arr);
                }
                pressEnterToContinue();
                break;
            }
            case 0: break;
            default: printf("Invalid option.\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}

void memberReport(Member *head, Transaction *transList) {
    (void)transList;
    int choice;
    do {
        clearScreen();
        printHeader("Member Report");
        printf("1. All Members\n");
        printf("2. Active Borrowers\n");
        printf("3. Members with Fines\n");
        printf("0. Back\n");
        choice = getIntInput("Select an option: ");

        switch (choice) {
            case 1: {
                Member *curr = head;
                while (curr) {
                    printf("ID: %d | Name: %s | Borrowed: %d\n", curr->id, curr->name, curr->booksCurrentlyBorrowed);
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 2: {
                Member *curr = head;
                while (curr) {
                    if (curr->booksCurrentlyBorrowed > 0)
                        printf("ID: %d | Name: %s | Borrowed: %d\n", curr->id, curr->name, curr->booksCurrentlyBorrowed);
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 3: {
                Member *curr = head;
                while (curr) {
                    if (curr->pendingFine > 0)
                        printf("ID: %d | Name: %s | Fine: %.2f\n", curr->id, curr->name, curr->pendingFine);
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 0: break;
            default: printf("Invalid option.\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}

void transactionReport(Transaction *head, Book *bookList, Member *memberList) {
    int choice;
    do {
        clearScreen();
        printHeader("Transaction Report");
        printf("1. All Transactions\n");
        printf("2. Active Transactions\n");
        printf("3. Returned Transactions\n");
        printf("4. Overdue Transactions\n");
        printf("0. Back\n");
        choice = getIntInput("Select an option: ");

        switch (choice) {
            case 1: {
                Transaction *curr = head;
                while (curr) {
                    printf("ID: %d | Member: %d | Book: %d | Status: %d\n", curr->id, curr->memberId, curr->bookId, curr->status);
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 2:
                displayActiveTransactions(head, bookList, memberList);
                pressEnterToContinue();
                break;
            case 3: {
                Transaction *curr = head;
                while (curr) {
                    if (curr->status == STATUS_RETURNED) {
                        printf("ID: %d | Member: %d | Book: %d | Fine: %.2f\n", curr->id, curr->memberId, curr->bookId, curr->fine);
                    }
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 4:
                displayOverdueTransactions(head, bookList, memberList);
                pressEnterToContinue();
                break;
            case 0: break;
            default: printf("Invalid option.\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}

void reservationReport(ReservationQueue *q, Book *bookList, Member *memberList) {
    clearScreen();
    printHeader("Reservation Report");
    if (!q || !q->front) {
        printf("No active reservations.\n");
    } else {
        ReservationNode *curr = q->front;
        while (curr) {
            Book *b = findBookById(bookList, curr->bookId);
            Member *m = findMemberById(memberList, curr->memberId);
            char dateStr[20];
            formatDate(curr->reservationDate, dateStr, sizeof(dateStr));
            printf("Book: %s (ID:%d) | Member: %s (ID:%d) | Date: %s\n",
                   b ? b->title : "Unknown", curr->bookId,
                   m ? m->name : "Unknown", curr->memberId,
                   dateStr);
            curr = curr->next;
        }
    }
    pressEnterToContinue();
}

void reportsMenu(Book *head, Member *memberList, Transaction *transList, ReservationQueue *q) {
    int choice;
    do {
        clearScreen();
        printHeader("Reports");
        printf("1. Book Report\n");
        printf("2. Member Report\n");
        printf("3. Transaction Report\n");
        printf("4. Reservation Report\n");
        printf("0. Back\n");
        choice = getIntInput("Select an option: ");

        switch (choice) {
            case 1: bookReport(head); break;
            case 2: memberReport(memberList, transList); break;
            case 3: transactionReport(transList, head, memberList); break;
            case 4: reservationReport(q, head, memberList); break;
            case 0: break;
            default: printf("Invalid option.\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}

void libraryStatistics(Book *head, Member *memberList, Transaction *transList, ReservationQueue *q) {
    clearScreen();
    printHeader("Library Statistics");

    int totalCopies = 0, availableCopies = 0, issuedCopies = 0;
    int maxBorrowed = -1;
    char mostBorrowedTitle[MAX_TITLE] = "None";

    Book *curr = head;
    while (curr) {
        totalCopies += curr->totalCopies;
        availableCopies += curr->availableCopies;
        issuedCopies += curr->issuedCopies;
        if (curr->timesBorrowed > maxBorrowed) {
            maxBorrowed = curr->timesBorrowed;
            strcpy(mostBorrowedTitle, curr->title);
        }
        curr = curr->next;
    }

    printf("Total Books: %d\n", countBooks(head));
    printf("Total Copies: %d\n", totalCopies);
    printf("Available Copies: %d\n", availableCopies);
    printf("Issued Copies: %d\n", issuedCopies);
    printf("Total Members: %d\n", countMembers(memberList));
    printf("Active Transactions: %d\n", countActiveTransactions(transList));
    printf("Overdue Books: %d\n", countOverdueTransactions(transList));
    printf("Pending Fines: Rs.%.2f\n", calculatePendingFines(memberList));
    printf("Total Fines Recorded: Rs.%.2f\n", calculateTotalFines(transList));
    printf("Reservations: %d\n", queueSize(q));
    printf("Most Borrowed Book: %s (%d times)\n", mostBorrowedTitle, maxBorrowed);
    printSeparator();
    pressEnterToContinue();
}

void fineManagementMenu(Member *memberList, Transaction *transList, Book *bookList) {
    int choice;
    do {
        clearScreen();
        printHeader("Fine Management");
        printf("1. View All Pending Fines\n");
        printf("2. View Total Fines Recorded\n");
        printf("3. Pay/Clear Member Fine\n");
        printf("4. View Overdue Books\n");
        printf("0. Back\n");
        choice = getIntInput("Select an option: ");

        switch (choice) {
            case 1: {
                Member *curr = memberList;
                printf("%-5s | %-20s | %-10s\n", "ID", "Name", "Fine");
                printSeparator();
                while (curr) {
                    if (curr->pendingFine > 0) {
                        printf("%-5d | %-20.20s | %.2f\n", curr->id, curr->name, curr->pendingFine);
                    }
                    curr = curr->next;
                }
                pressEnterToContinue();
                break;
            }
            case 2:
                printf("Total Fines Recorded: Rs.%.2f\n", calculateTotalFines(transList));
                pressEnterToContinue();
                break;
            case 3: {
                int id = getIntInput("Enter Member ID: ");
                Member *m = findMemberById(memberList, id);
                if (m) {
                    printf("Current Fine for %s: Rs.%.2f\n", m->name, m->pendingFine);
                    if (m->pendingFine > 0) {
                        float amount;
                        printf("Enter payment amount: ");
                        if (scanf("%f", &amount) == 1) {
                            clearInputBuffer();
                            if (amount <= 0.0f) {
                                printf("Payment must be greater than zero.\n");
                            } else {
                                if (amount > m->pendingFine) amount = m->pendingFine;
                                m->pendingFine -= amount;
                                if (m->pendingFine < 0) m->pendingFine = 0;
                                printf("Payment successful. Remaining fine: Rs.%.2f\n", m->pendingFine);
                            }
                        } else {
                            clearInputBuffer();
                            printf("Invalid amount.\n");
                        }
                    } else {
                        printf("No pending fine.\n");
                    }
                } else {
                    printf("Member not found.\n");
                }
                pressEnterToContinue();
                break;
            }
            case 4:
                displayOverdueTransactions(transList, bookList, memberList);
                pressEnterToContinue();
                break;
            case 0: break;
            default: printf("Invalid option.\n"); pressEnterToContinue();
        }
    } while (choice != 0);
}
