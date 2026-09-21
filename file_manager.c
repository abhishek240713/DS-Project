#include "file_manager.h"
#include "books.h"
#include "members.h"
#include "transactions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void saveBooks(Book *head) {
    FILE *fp = fopen(BOOKS_FILE, "w");
    if (!fp) {
        printf("Error: Could not open %s for writing.\n", BOOKS_FILE);
        return;
    }
    Book *curr = head;
    while (curr) {
        fprintf(fp, "%d|%s|%s|%s|%s|%s|%d|%d|%d|%d\n",
                curr->id, curr->title, curr->author, curr->category,
                curr->isbn, curr->publisher, curr->totalCopies,
                curr->availableCopies, curr->issuedCopies, curr->timesBorrowed);
        curr = curr->next;
    }
    fclose(fp);
    printf("Books saved successfully.\n");
}

Book *loadBooks(void) {
    FILE *fp = fopen(BOOKS_FILE, "r");
    if (!fp) return NULL;

    Book *head = NULL;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int id, totalCopies, availableCopies, issuedCopies, timesBorrowed;
        char title[MAX_TITLE], author[MAX_AUTHOR], category[MAX_CATEGORY];
        char isbn[MAX_ISBN], publisher[MAX_PUBLISHER];
        int fields = sscanf(line,
            "%d|%99[^|]|%99[^|]|%49[^|]|%19[^|]|%99[^|]|%d|%d|%d|%d",
            &id, title, author, category, isbn, publisher,
            &totalCopies, &availableCopies, &issuedCopies, &timesBorrowed);
        if (fields != 10 || totalCopies < 0 || availableCopies < 0 || issuedCopies < 0 ||
            availableCopies + issuedCopies != totalCopies) {
            printf("Warning: Skipping invalid book record.\n");
            continue;
        }

        Book *b = createBook(id, title, author, category, isbn, publisher, totalCopies);
        if (!b) break;
        b->availableCopies = availableCopies;
        b->issuedCopies = issuedCopies;
        b->timesBorrowed = timesBorrowed < 0 ? 0 : timesBorrowed;
        head = addBookToList(head, b);
    }
    fclose(fp);
    return head;
}

void saveMembers(Member *head) {
    FILE *fp = fopen(MEMBERS_FILE, "w");
    if (!fp) {
        printf("Error: Could not open %s for writing.\n", MEMBERS_FILE);
        return;
    }
    Member *curr = head;
    while (curr) {
        fprintf(fp, "%d|%s|%s|%s|%d|%d|%.2f\n",
                curr->id, curr->name, curr->phone, curr->email,
                curr->booksCurrentlyBorrowed, curr->totalBooksBorrowed, curr->pendingFine);
        curr = curr->next;
    }
    fclose(fp);
    printf("Members saved successfully.\n");
}

Member *loadMembers(void) {
    FILE *fp = fopen(MEMBERS_FILE, "r");
    if (!fp) return NULL;

    Member *head = NULL;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int id, currentlyBorrowed, totalBorrowed;
        float fine;
        char name[MAX_NAME], phone[MAX_PHONE], email[MAX_EMAIL];
        int fields = sscanf(line, "%d|%99[^|]|%19[^|]|%99[^|]|%d|%d|%f",
                            &id, name, phone, email, &currentlyBorrowed, &totalBorrowed, &fine);
        if (fields != 7 || currentlyBorrowed < 0 || totalBorrowed < 0 || fine < 0.0f) {
            printf("Warning: Skipping invalid member record.\n");
            continue;
        }

        Member *m = createMember(id, name, phone, email);
        if (!m) break;
        m->booksCurrentlyBorrowed = currentlyBorrowed;
        m->totalBooksBorrowed = totalBorrowed;
        m->pendingFine = fine;
        head = addMemberToList(head, m);
    }
    fclose(fp);
    return head;
}

void saveTransactions(Transaction *head) {
    FILE *fp = fopen(TRANSACTIONS_FILE, "w");
    if (!fp) {
        printf("Error: Could not open %s for writing.\n", TRANSACTIONS_FILE);
        return;
    }
    Transaction *curr = head;
    while (curr) {
        fprintf(fp, "%d|%d|%d|%ld|%ld|%ld|%.2f|%d\n",
                curr->id, curr->memberId, curr->bookId,
                (long)curr->issueDate, (long)curr->dueDate, (long)curr->returnDate,
                curr->fine, curr->status);
        curr = curr->next;
    }
    fclose(fp);
    printf("Transactions saved successfully.\n");
}

Transaction *loadTransactions(void) {
    FILE *fp = fopen(TRANSACTIONS_FILE, "r");
    if (!fp) return NULL;

    Transaction *head = NULL;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int id, memberId, bookId, status;
        long issueRaw, dueRaw, returnRaw;
        float fine;
        int fields = sscanf(line, "%d|%d|%d|%ld|%ld|%ld|%f|%d",
                            &id, &memberId, &bookId, &issueRaw, &dueRaw, &returnRaw, &fine, &status);
        if (fields != 8 || id <= 0 || memberId <= 0 || bookId <= 0 || fine < 0.0f ||
            (status != STATUS_ACTIVE && status != STATUS_RETURNED)) {
            printf("Warning: Skipping invalid transaction record.\n");
            continue;
        }

        Transaction *t = createTransaction(id, memberId, bookId,
                                           (time_t)issueRaw, (time_t)dueRaw);
        if (!t) break;
        t->returnDate = (time_t)returnRaw;
        t->fine = fine;
        t->status = (TransStatus)status;
        head = addTransactionToList(head, t);
    }
    fclose(fp);
    return head;
}

void saveReservations(ReservationQueue *q) {
    FILE *fp = fopen(RESERVATIONS_FILE, "w");
    if (!fp) {
        printf("Error: Could not open %s for writing.\n", RESERVATIONS_FILE);
        return;
    }
    ReservationNode *curr = q->front;
    while (curr) {
        fprintf(fp, "%d|%d|%ld\n", curr->memberId, curr->bookId, (long)curr->reservationDate);
        curr = curr->next;
    }
    fclose(fp);
    printf("Reservations saved successfully.\n");
}

void loadReservations(ReservationQueue *q) {
    initQueue(q);
    FILE *fp = fopen(RESERVATIONS_FILE, "r");
    if (!fp) return;

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        int memberId, bookId;
        long resRaw;
        int fields = sscanf(line, "%d|%d|%ld", &memberId, &bookId, &resRaw);
        if (fields != 3 || memberId <= 0 || bookId <= 0) {
            printf("Warning: Skipping invalid reservation record.\n");
            continue;
        }
        if (enqueue(q, memberId, bookId) && q->rear) {
            q->rear->reservationDate = (time_t)resRaw;
        }
    }
    fclose(fp);
}

void saveCounters(IdCounters *counters) {
    FILE *fp = fopen(COUNTERS_FILE, "w");
    if (!fp) return;
    fprintf(fp, "%d|%d|%d\n", counters->nextBookId, counters->nextMemberId, counters->nextTransactionId);
    fclose(fp);
    printf("Counters saved successfully.\n");
}

void loadCounters(IdCounters *counters) {
    FILE *fp = fopen(COUNTERS_FILE, "r");
    if (!fp) {
        counters->nextBookId = 1001;
        counters->nextMemberId = 101;
        counters->nextTransactionId = 1;
        return;
    }
    char line[1024];
    if (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, "|");
        if (token) counters->nextBookId = atoi(token);
        token = strtok(NULL, "|");
        if (token) counters->nextMemberId = atoi(token);
        token = strtok(NULL, "|");
        if (token) counters->nextTransactionId = atoi(token);
    }
    fclose(fp);
}

void saveAllData(Book *head, Member *memberList, Transaction *transList,
                 ReservationQueue *q, IdCounters *counters) {
    saveBooks(head);
    saveMembers(memberList);
    saveTransactions(transList);
    saveReservations(q);
    saveCounters(counters);
    printf("All data saved successfully.\n");
}
