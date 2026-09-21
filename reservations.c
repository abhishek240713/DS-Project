#include "reservations.h"
#include "books.h"
#include "members.h"
#include "transactions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void makeReservation(ReservationQueue *q, Book *bookList, Member *memberList) {
    printHeader("Make Reservation");
    int bookId = getIntInput("Enter Book ID: ");
    Book *book = findBookById(bookList, bookId);
    if (!book) {
        printf("Error: Book not found.\n");
        return;
    }
    if (book->availableCopies > 0) {
        printf("Book is available, no need to reserve. Please issue it directly.\n");
        return;
    }
    
    int memberId = getIntInput("Enter Member ID: ");
    Member *member = findMemberById(memberList, memberId);
    if (!member) {
        printf("Error: Member not found.\n");
        return;
    }
    
    if (hasReservation(q, memberId, bookId)) {
        printf("Error: Already reserved.\n");
        return;
    }

    if (!enqueue(q, memberId, bookId)) {
        printf("Error: Could not create reservation.\n");
        return;
    }
    printf("Success! Reservation added. You are number %d for this book.\n",
           reservationPosition(q, memberId, bookId));
}

void cancelReservation(ReservationQueue *q, Book *bookList, Member *memberList) {
    (void)bookList;
    (void)memberList;
    printHeader("Cancel Reservation");
    int bookId = getIntInput("Enter Book ID: ");
    int memberId = getIntInput("Enter Member ID: ");
    
    ReservationNode *curr = q->front;
    ReservationNode *prev = NULL;
    while (curr) {
        if (curr->bookId == bookId && curr->memberId == memberId) {
            if (prev) {
                prev->next = curr->next;
            } else {
                q->front = curr->next;
            }
            if (curr == q->rear) {
                q->rear = prev;
            }
            free(curr);
            q->count--;
            printf("Reservation cancelled successfully.\n");
            return;
        }
        prev = curr;
        curr = curr->next;
    }
    printf("Error: Reservation not found.\n");
}

void displayAllReservations(ReservationQueue *q, Book *bookList, Member *memberList) {
    printHeader("All Reservations");
    printf("%-8s %-20s %-8s %-20s %-15s\n", "MemberID", "MemberName", "BookID", "BookTitle", "Res.Date");
    printSeparator();
    ReservationNode *curr = q->front;
    while (curr) {
        Member *m = findMemberById(memberList, curr->memberId);
        Book *b = findBookById(bookList, curr->bookId);
        char date[30];
        formatDate(curr->reservationDate, date, 30);
        printf("%-8d %-20.20s %-8d %-20.20s %-15s\n",
               curr->memberId, m ? m->name : "Unknown",
               curr->bookId, b ? b->title : "Unknown",
               date);
        curr = curr->next;
    }
    printf("\nTotal Reservations: %d\n", q->count);
}

void displayBookReservations(ReservationQueue *q, Book *bookList, Member *memberList) {
    (void)bookList;
    printHeader("Book Reservations");
    int bookId = getIntInput("Enter Book ID: ");
    displayQueueForBook(q, bookId, memberList);
}

int countAllReservations(ReservationQueue *q) {
    return q->count;
}

void reservationMenu(ReservationQueue *q, Book *bookList, Member *memberList,
                     Transaction **transList, IdCounters *counters) {
    (void)transList;
    (void)counters;
    int choice;
    do {
        clearScreen();
        printHeader("Reservations Menu");
        printf("1. Make Reservation\n");
        printf("2. Cancel Reservation\n");
        printf("3. View All Reservations\n");
        printf("4. View Book Reservations\n");
        printf("0. Back\n");
        choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1: makeReservation(q, bookList, memberList); break;
            case 2: cancelReservation(q, bookList, memberList); break;
            case 3: displayAllReservations(q, bookList, memberList); break;
            case 4: displayBookReservations(q, bookList, memberList); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
        if (choice != 0) pressEnterToContinue();
    } while (choice != 0);
}
