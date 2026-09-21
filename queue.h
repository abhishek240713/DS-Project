#ifndef QUEUE_H
#define QUEUE_H

#include "utils.h"

void initQueue(ReservationQueue *q);
bool enqueue(ReservationQueue *q, int memberId, int bookId);
bool hasReservation(const ReservationQueue *q, int memberId, int bookId);
int reservationPosition(const ReservationQueue *q, int memberId, int bookId);
ReservationNode *dequeue(ReservationQueue *q);
ReservationNode *dequeueForBook(ReservationQueue *q, int bookId);
bool isQueueEmpty(ReservationQueue *q);
int queueSize(ReservationQueue *q);
void displayQueue(ReservationQueue *q);
void displayQueueForBook(ReservationQueue *q, int bookId, Member *memberList);
void freeQueue(ReservationQueue *q);

#endif
