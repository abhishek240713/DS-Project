#include "transactions.h"
#include "books.h"
#include "members.h"
#include "queue.h"
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

Transaction *createTransaction(int id, int memberId, int bookId, time_t issueDate, time_t dueDate) {
    Transaction *t = (Transaction *)malloc(sizeof(Transaction));
    if (!t) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    t->id = id;
    t->memberId = memberId;
    t->bookId = bookId;
    t->issueDate = issueDate;
    t->dueDate = dueDate;
    t->returnDate = 0;
    t->fine = 0.0f;
    t->status = STATUS_ACTIVE;
    t->next = NULL;
    return t;
}

Transaction *addTransactionToList(Transaction *head, Transaction *newTrans) {
    if (!head) return newTrans;
    Transaction *curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    curr->next = newTrans;
    return head;
}

Transaction *issueBook(Transaction *head, Book *bookList, Member *memberList,
                       BSTNode *bstRoot, IdCounters *counters, ReservationQueue *resQueue) {
    printHeader("Issue Book");
    int memberId = getIntInput("Enter Member ID: ");
    Member *member = findMemberById(memberList, memberId);
    if (!member) {
        printf("Error: Member not found.\n");
        return head;
    }
    if (member->pendingFine > 0) {
        printf("Member has pending fine of Rs.%.2f. Clear fine before issuing.\n", member->pendingFine);
        return head;
    }
    if (member->booksCurrentlyBorrowed >= MAX_BORROW_LIMIT) {
        printf("Error: Borrow limit reached.\n");
        return head;
    }
    
    int bookId = getIntInput("Enter Book ID: ");
    Book *book = NULL;
    BSTNode *node = searchBST(bstRoot, bookId);
    if (node && node->bookPtr) {
        book = node->bookPtr;
    } else {
        book = findBookById(bookList, bookId);
    }
    if (!book) {
        printf("Error: Book not found.\n");
        return head;
    }
    
    if (book->availableCopies <= 0) {
        printf("No copies available.\n");
        char ch[10];
        getStringInput("Would you like to reserve? (y/n): ", ch, 10);
        if (ch[0] == 'y' || ch[0] == 'Y') {
            if (hasReservation(resQueue, memberId, bookId)) {
                printf("You already have a reservation for this book.\n");
            } else if (enqueue(resQueue, memberId, bookId)) {
                printf("Reservation added. You are number %d for this book.\n",
                       reservationPosition(resQueue, memberId, bookId));
            }
        }
        return head;
    }
    
    time_t now = time(NULL);
    time_t due = addDays(now, BORROW_PERIOD_DAYS);
    Transaction *t = createTransaction(counters->nextTransactionId++, memberId, bookId, now, due);
    if (!t) return head;
    
    head = addTransactionToList(head, t);
    book->availableCopies--;
    book->issuedCopies++;
    book->timesBorrowed++;
    member->booksCurrentlyBorrowed++;
    member->totalBooksBorrowed++;
    
    char dateStr[30], dueStr[30];
    formatDate(now, dateStr, 30);
    formatDate(due, dueStr, 30);
    printf("Success! Transaction ID: %d\n", t->id);
    printf("Book: %s\n", book->title);
    printf("Member: %s\n", member->name);
    printf("Issue Date: %s\n", dateStr);
    printf("Due Date: %s\n", dueStr);
    
    return head;
}

Transaction *returnBook(Transaction *head, Book *bookList, Member *memberList,
                        ReservationQueue *resQueue, IdCounters *counters) {
    printHeader("Return Book");
    int memberId = getIntInput("Enter Member ID: ");
    int bookId = getIntInput("Enter Book ID: ");
    
    Member *member = findMemberById(memberList, memberId);
    if (!member) {
        printf("Error: Member not found.\n");
        return head;
    }
    Book *book = findBookById(bookList, bookId);
    if (!book) {
        printf("Error: Book not found.\n");
        return head;
    }
    
    Transaction *trans = findActiveTransaction(head, memberId, bookId);
    if (!trans) {
        printf("Error: No active issue record found.\n");
        return head;
    }
    
    trans->returnDate = time(NULL);
    trans->status = STATUS_RETURNED;
    int overdue = daysBetween(trans->dueDate, trans->returnDate);
    if (overdue > 0) {
        trans->fine = overdue * FINE_PER_DAY;
        member->pendingFine += trans->fine;
    } else {
        trans->fine = 0.0f;
    }
    
    book->availableCopies++;
    book->issuedCopies--;
    member->booksCurrentlyBorrowed--;
    
    char retStr[30];
    formatDate(trans->returnDate, retStr, 30);
    printf("Book '%s' returned on %s.\n", book->title, retStr);
    printf("Overdue Days: %d\n", overdue > 0 ? overdue : 0);
    printf("Fine for this transaction: Rs.%.2f\n", trans->fine);
    
    ReservationNode *res = dequeueForBook(resQueue, bookId);
    while (res) {
        Member *resMember = findMemberById(memberList, res->memberId);
        if (resMember && resMember->booksCurrentlyBorrowed < MAX_BORROW_LIMIT &&
            resMember->pendingFine <= 0 &&
            !findActiveTransaction(head, resMember->id, bookId)) {
            time_t now = time(NULL);
            time_t due = addDays(now, BORROW_PERIOD_DAYS);
            Transaction *newT = createTransaction(counters->nextTransactionId, resMember->id, bookId, now, due);
            if (newT) {
                counters->nextTransactionId++;
                head = addTransactionToList(head, newT);
                book->availableCopies--;
                book->issuedCopies++;
                book->timesBorrowed++;
                resMember->booksCurrentlyBorrowed++;
                resMember->totalBooksBorrowed++;
                printf("Reservation processed: book auto-issued to %s.\n", resMember->name);
                free(res);
                break;
            }
            printf("Could not create the reservation transaction due to memory allocation failure.\n");
            free(res);
            break;
        }

        printf("Skipped an ineligible reservation (Member ID %d).\n", res->memberId);
        free(res);
        res = dequeueForBook(resQueue, bookId);
    }
    
    return head;
}

Transaction *findActiveTransaction(Transaction *head, int memberId, int bookId) {
    Transaction *curr = head;
    while (curr) {
        if (curr->memberId == memberId && curr->bookId == bookId && curr->status == STATUS_ACTIVE) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

Transaction *findActiveTransactionByMember(Transaction *head, int memberId) {
    Transaction *curr = head;
    while (curr) {
        if (curr->memberId == memberId && curr->status == STATUS_ACTIVE) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

void displayAllTransactions(Transaction *head, Book *bookList, Member *memberList) {
    (void)bookList;
    (void)memberList;
    printHeader("All Transactions");
    printf("%-5s %-8s %-8s %-15s %-15s %-15s %-8s %-10s\n", "TxnID", "MemberID", "BookID", "IssueDate", "DueDate", "ReturnDate", "Fine", "Status");
    printSeparator();
    int count = 0;
    Transaction *curr = head;
    while (curr) {
        char issue[30], due[30], ret[30];
        formatDate(curr->issueDate, issue, 30);
        formatDate(curr->dueDate, due, 30);
        if (curr->returnDate > 0) formatDate(curr->returnDate, ret, 30);
        else strcpy(ret, "-");
        printf("%-5d %-8d %-8d %-15s %-15s %-15s %-8.2f %-10s\n",
               curr->id, curr->memberId, curr->bookId, issue, due, ret, curr->fine,
               curr->status == STATUS_ACTIVE ? "Active" : "Returned");
        count++;
        curr = curr->next;
    }
    printf("\nTotal Transactions: %d\n", count);
}

void displayActiveTransactions(Transaction *head, Book *bookList, Member *memberList) {
    (void)bookList;
    (void)memberList;
    printHeader("Active Transactions");
    printf("%-5s %-8s %-8s %-15s %-15s %-8s\n", "TxnID", "MemberID", "BookID", "IssueDate", "DueDate", "Fine");
    printSeparator();
    int count = 0;
    Transaction *curr = head;
    while (curr) {
        if (curr->status == STATUS_ACTIVE) {
            char issue[30], due[30];
            formatDate(curr->issueDate, issue, 30);
            formatDate(curr->dueDate, due, 30);
            printf("%-5d %-8d %-8d %-15s %-15s %-8.2f\n",
                   curr->id, curr->memberId, curr->bookId, issue, due, curr->fine);
            count++;
        }
        curr = curr->next;
    }
    printf("\nTotal Active Transactions: %d\n", count);
}

void displayReturnedTransactions(Transaction *head, Book *bookList, Member *memberList) {
    (void)bookList;
    (void)memberList;
    printHeader("Returned Transactions");
    printf("%-5s %-8s %-8s %-15s %-15s %-15s %-8s\n", "TxnID", "MemberID", "BookID", "IssueDate", "DueDate", "ReturnDate", "Fine");
    printSeparator();
    int count = 0;
    Transaction *curr = head;
    while (curr) {
        if (curr->status == STATUS_RETURNED) {
            char issue[30], due[30], ret[30];
            formatDate(curr->issueDate, issue, 30);
            formatDate(curr->dueDate, due, 30);
            formatDate(curr->returnDate, ret, 30);
            printf("%-5d %-8d %-8d %-15s %-15s %-15s %-8.2f\n",
                   curr->id, curr->memberId, curr->bookId, issue, due, ret, curr->fine);
            count++;
        }
        curr = curr->next;
    }
    printf("\nTotal Returned Transactions: %d\n", count);
}

void displayOverdueTransactions(Transaction *head, Book *bookList, Member *memberList) {
    (void)bookList;
    (void)memberList;
    printHeader("Overdue Transactions");
    printf("%-5s %-8s %-8s %-15s %-15s %-10s %-10s\n", "TxnID", "MemberID", "BookID", "DueDate", "OverdueDays", "Proj.Fine", "Status");
    printSeparator();
    int count = 0;
    time_t now = time(NULL);
    Transaction *curr = head;
    while (curr) {
        if (curr->status == STATUS_ACTIVE && now > curr->dueDate) {
            int overdue = daysBetween(curr->dueDate, now);
            if (overdue > 0) {
                float projFine = overdue * FINE_PER_DAY;
                char due[30];
                formatDate(curr->dueDate, due, 30);
                printf("%-5d %-8d %-8d %-15s %-10d %-10.2f %-10s\n",
                       curr->id, curr->memberId, curr->bookId, due, overdue, projFine, "Overdue");
                count++;
            }
        }
        curr = curr->next;
    }
    printf("\nTotal Overdue Transactions: %d\n", count);
}

void displayMemberTransactions(Transaction *head, int memberId, Book *bookList) {
    (void)bookList;
    printHeader("Member Transactions");
    printf("%-5s %-8s %-15s %-15s %-15s %-8s %-10s\n", "TxnID", "BookID", "IssueDate", "DueDate", "ReturnDate", "Fine", "Status");
    printSeparator();
    Transaction *curr = head;
    while (curr) {
        if (curr->memberId == memberId) {
            char issue[30], due[30], ret[30];
            formatDate(curr->issueDate, issue, 30);
            formatDate(curr->dueDate, due, 30);
            if (curr->returnDate > 0) formatDate(curr->returnDate, ret, 30);
            else strcpy(ret, "-");
            printf("%-5d %-8d %-15s %-15s %-15s %-8.2f %-10s\n",
                   curr->id, curr->bookId, issue, due, ret, curr->fine,
                   curr->status == STATUS_ACTIVE ? "Active" : "Returned");
        }
        curr = curr->next;
    }
}

void displayBookTransactions(Transaction *head, int bookId, Member *memberList) {
    (void)memberList;
    printHeader("Book Transactions");
    printf("%-5s %-8s %-15s %-15s %-15s %-8s %-10s\n", "TxnID", "MemberID", "IssueDate", "DueDate", "ReturnDate", "Fine", "Status");
    printSeparator();
    Transaction *curr = head;
    while (curr) {
        if (curr->bookId == bookId) {
            char issue[30], due[30], ret[30];
            formatDate(curr->issueDate, issue, 30);
            formatDate(curr->dueDate, due, 30);
            if (curr->returnDate > 0) formatDate(curr->returnDate, ret, 30);
            else strcpy(ret, "-");
            printf("%-5d %-8d %-15s %-15s %-15s %-8.2f %-10s\n",
                   curr->id, curr->memberId, issue, due, ret, curr->fine,
                   curr->status == STATUS_ACTIVE ? "Active" : "Returned");
        }
        curr = curr->next;
    }
}

int countTransactions(Transaction *head) {
    int count = 0;
    while (head) { count++; head = head->next; }
    return count;
}

int countActiveTransactions(Transaction *head) {
    int count = 0;
    while (head) {
        if (head->status == STATUS_ACTIVE) count++;
        head = head->next;
    }
    return count;
}

int countOverdueTransactions(Transaction *head) {
    int count = 0;
    time_t now = time(NULL);
    while (head) {
        if (head->status == STATUS_ACTIVE && now > head->dueDate && daysBetween(head->dueDate, now) > 0) {
            count++;
        }
        head = head->next;
    }
    return count;
}

float calculateTotalFines(Transaction *head) {
    float total = 0.0f;
    while (head) {
        total += head->fine;
        head = head->next;
    }
    return total;
}

float calculatePendingFines(Member *memberList) {
    float total = 0.0f;
    while (memberList) {
        total += memberList->pendingFine;
        memberList = memberList->next;
    }
    return total;
}

void transactionHistoryMenu(Transaction *head, Member *memberList, Book *bookList) {
    int choice;
    do {
        clearScreen();
        printHeader("Transaction History");
        printf("1. All Transactions\n");
        printf("2. Active Transactions\n");
        printf("3. Returned Transactions\n");
        printf("4. Overdue Transactions\n");
        printf("5. Member Transaction History\n");
        printf("6. Book Transaction History\n");
        printf("0. Back\n");
        choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1: displayAllTransactions(head, bookList, memberList); break;
            case 2: displayActiveTransactions(head, bookList, memberList); break;
            case 3: displayReturnedTransactions(head, bookList, memberList); break;
            case 4: displayOverdueTransactions(head, bookList, memberList); break;
            case 5: {
                int mid = getIntInput("Enter Member ID: ");
                displayMemberTransactions(head, mid, bookList);
                break;
            }
            case 6: {
                int bid = getIntInput("Enter Book ID: ");
                displayBookTransactions(head, bid, memberList);
                break;
            }
            case 0: break;
            default: printf("Invalid choice.\n");
        }
        if (choice != 0) pressEnterToContinue();
    } while (choice != 0);
}

void freeTransactions(Transaction *head) {
    while (head) {
        Transaction *temp = head;
        head = head->next;
        free(temp);
    }
}
