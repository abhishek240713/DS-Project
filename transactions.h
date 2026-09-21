#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "utils.h"

Transaction *createTransaction(int id, int memberId, int bookId, time_t issueDate, time_t dueDate);
Transaction *addTransactionToList(Transaction *head, Transaction *newTrans);
Transaction *issueBook(Transaction *head, Book *bookList, Member *memberList,
                       BSTNode *bstRoot, IdCounters *counters, ReservationQueue *resQueue);
Transaction *returnBook(Transaction *head, Book *bookList, Member *memberList,
                        ReservationQueue *resQueue, IdCounters *counters);
Transaction *findActiveTransaction(Transaction *head, int memberId, int bookId);
Transaction *findActiveTransactionByMember(Transaction *head, int memberId);
void displayAllTransactions(Transaction *head, Book *bookList, Member *memberList);
void displayActiveTransactions(Transaction *head, Book *bookList, Member *memberList);
void displayReturnedTransactions(Transaction *head, Book *bookList, Member *memberList);
void displayOverdueTransactions(Transaction *head, Book *bookList, Member *memberList);
void displayMemberTransactions(Transaction *head, int memberId, Book *bookList);
void displayBookTransactions(Transaction *head, int bookId, Member *memberList);
int countTransactions(Transaction *head);
int countActiveTransactions(Transaction *head);
int countOverdueTransactions(Transaction *head);
float calculateTotalFines(Transaction *head);
float calculatePendingFines(Member *memberList);
void transactionHistoryMenu(Transaction *head, Member *memberList, Book *bookList);
void freeTransactions(Transaction *head);

#endif
