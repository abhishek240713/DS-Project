#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include "utils.h"
#include "queue.h"

void makeReservation(ReservationQueue *q, Book *bookList, Member *memberList);
void cancelReservation(ReservationQueue *q, Book *bookList, Member *memberList);
void displayAllReservations(ReservationQueue *q, Book *bookList, Member *memberList);
void displayBookReservations(ReservationQueue *q, Book *bookList, Member *memberList);
int countAllReservations(ReservationQueue *q);
void reservationMenu(ReservationQueue *q, Book *bookList, Member *memberList,
                     Transaction **transList, IdCounters *counters);

#endif
