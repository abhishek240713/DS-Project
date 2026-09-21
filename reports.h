#ifndef REPORTS_H
#define REPORTS_H

#include "utils.h"
#include "queue.h"

void bookReport(Book *head);
void memberReport(Member *head, Transaction *transList);
void transactionReport(Transaction *head, Book *bookList, Member *memberList);
void reservationReport(ReservationQueue *q, Book *bookList, Member *memberList);
void reportsMenu(Book *head, Member *memberList, Transaction *transList, ReservationQueue *q);
void libraryStatistics(Book *head, Member *memberList, Transaction *transList, ReservationQueue *q);
void fineManagementMenu(Member *memberList, Transaction *transList, Book *bookList);

#endif
