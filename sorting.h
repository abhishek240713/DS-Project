#ifndef SORTING_H
#define SORTING_H

#include "utils.h"

/* Sort criteria */
#define SORT_BY_ID 1
#define SORT_BY_TITLE 2
#define SORT_BY_AUTHOR 3
#define SORT_BY_CATEGORY 4
#define SORT_BY_MOST_BORROWED 5

/* Sorting Algorithms */
void bubbleSortBooks(Book **arr, int n, int sortBy);
void selectionSortBooks(Book **arr, int n, int sortBy);
void insertionSortBooks(Book **arr, int n, int sortBy);
void mergeSortBooks(Book **arr, int left, int right, int sortBy);
void quickSortBooks(Book **arr, int low, int high, int sortBy);

/* Helpers */
Book **bookListToArray(Book *head, int *count);
int compareBooks(Book *a, Book *b, int sortBy);
void displaySortedBooks(Book **arr, int n);
void sortingMenu(Book *head);

#endif
