#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "utils.h"
#include "queue.h"

void saveBooks(Book *head);
Book *loadBooks(void);
void saveMembers(Member *head);
Member *loadMembers(void);
void saveTransactions(Transaction *head);
Transaction *loadTransactions(void);
void saveReservations(ReservationQueue *q);
void loadReservations(ReservationQueue *q);
void saveCounters(IdCounters *counters);
void loadCounters(IdCounters *counters);
void saveAllData(Book *head, Member *memberList, Transaction *transList,
                 ReservationQueue *q, IdCounters *counters);

#endif
