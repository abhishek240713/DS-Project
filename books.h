#ifndef BOOKS_H
#define BOOKS_H

#include "utils.h"

Book *createBook(int id, const char *title, const char *author, const char *category,
                 const char *isbn, const char *publisher, int totalCopies);
Book *addBookToList(Book *head, Book *newBook);
Book *addBook(Book *head, BSTNode **bstRoot, IdCounters *counters, UndoNode **undoStack);
Book *deleteBook(Book *head, BSTNode **bstRoot, Transaction *transList, UndoNode **undoStack);
void updateBook(Book *head, UndoNode **undoStack);
Book *findBookById(Book *head, int id);
void displayAllBooks(Book *head);
void displayBookDetails(Book *book, ViewStackNode **viewStack);
int countBooks(Book *head);
void bookManagementMenu(Book **head, BSTNode **bstRoot, IdCounters *counters,
                        Transaction *transList, UndoNode **undoStack, ViewStackNode **viewStack);
void freeBooks(Book *head);

#endif
