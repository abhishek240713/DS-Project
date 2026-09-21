#ifndef SEARCH_H
#define SEARCH_H

#include "utils.h"

/* Linear Search - O(n) */
Book *linearSearchBookById(Book *head, int id);
Book *linearSearchBookByTitle(Book *head, const char *title);
Book *linearSearchBookByAuthor(Book *head, const char *author);
Book *linearSearchBookByCategory(Book *head, const char *category);
Book *linearSearchBookByISBN(Book *head, const char *isbn);
Member *linearSearchMemberById(Member *head, int id);
Member *linearSearchMemberByName(Member *head, const char *name);

/* Binary Search on sorted array - O(log n) */
int binarySearchBookArray(Book **arr, int size, int targetId);

void searchMenu(Book *head, Member *memberList, BSTNode *bstRoot, ViewStackNode **viewStack);

#endif
