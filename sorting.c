#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compareBooks(Book *a, Book *b, int sortBy) {
    char lowerA[200], lowerB[200];
    switch (sortBy) {
        case SORT_BY_ID:
            return a->id - b->id;
        case SORT_BY_TITLE:
            toLowerStr(lowerA, a->title);
            toLowerStr(lowerB, b->title);
            return strcmp(lowerA, lowerB);
        case SORT_BY_AUTHOR:
            toLowerStr(lowerA, a->author);
            toLowerStr(lowerB, b->author);
            return strcmp(lowerA, lowerB);
        case SORT_BY_CATEGORY:
            toLowerStr(lowerA, a->category);
            toLowerStr(lowerB, b->category);
            return strcmp(lowerA, lowerB);
        case SORT_BY_MOST_BORROWED:
            return b->timesBorrowed - a->timesBorrowed; // Descending
        default:
            return a->id - b->id;
    }
}

Book **bookListToArray(Book *head, int *count) {
    int n = 0;
    Book *curr = head;
    while (curr) {
        n++;
        curr = curr->next;
    }
    *count = n;
    if (n == 0) return NULL;

    Book **arr = (Book **)malloc(n * sizeof(Book *));
    if (!arr) return NULL;

    curr = head;
    for (int i = 0; i < n; i++) {
        arr[i] = curr;
        curr = curr->next;
    }
    return arr;
}

/* Time Complexity: O(n^2), Space: O(1) */
void bubbleSortBooks(Book **arr, int n, int sortBy) {
    int swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (compareBooks(arr[j], arr[j + 1], sortBy) > 0) {
                Book *temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

/* Time Complexity: O(n^2), Space: O(1) */
void selectionSortBooks(Book **arr, int n, int sortBy) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (compareBooks(arr[j], arr[min_idx], sortBy) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Book *temp = arr[min_idx];
            arr[min_idx] = arr[i];
            arr[i] = temp;
        }
    }
}

/* Time Complexity: O(n^2) worst, O(n) best. Space: O(1) */
void insertionSortBooks(Book **arr, int n, int sortBy) {
    for (int i = 1; i < n; i++) {
        Book *key = arr[i];
        int j = i - 1;
        while (j >= 0 && compareBooks(arr[j], key, sortBy) > 0) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

static void merge(Book **arr, int left, int mid, int right, int sortBy) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Book **L = (Book **)malloc(n1 * sizeof(Book *));
    Book **R = (Book **)malloc(n2 * sizeof(Book *));
    if (!L || !R) {
        if (L) free(L);
        if (R) free(R);
        return;
    }

    for (i = 0; i < n1; i++) L[i] = arr[left + i];
    for (j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    i = 0; j = 0; k = left;
    while (i < n1 && j < n2) {
        if (compareBooks(L[i], R[j], sortBy) <= 0) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
    free(L);
    free(R);
}

/* Time Complexity: O(n log n), Space: O(n) */
void mergeSortBooks(Book **arr, int left, int right, int sortBy) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortBooks(arr, left, mid, sortBy);
        mergeSortBooks(arr, mid + 1, right, sortBy);
        merge(arr, left, mid, right, sortBy);
    }
}

static int partition(Book **arr, int low, int high, int sortBy) {
    Book *pivot = arr[high];
    int i = (low - 1);

    for (int j = low; j <= high - 1; j++) {
        if (compareBooks(arr[j], pivot, sortBy) < 0) {
            i++;
            Book *temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    Book *temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;
    return (i + 1);
}

/* Time Complexity: O(n log n) average, O(n^2) worst */
void quickSortBooks(Book **arr, int low, int high, int sortBy) {
    if (low < high) {
        int pi = partition(arr, low, high, sortBy);
        quickSortBooks(arr, low, pi - 1, sortBy);
        quickSortBooks(arr, pi + 1, high, sortBy);
    }
}

void displaySortedBooks(Book **arr, int n) {
    if (n == 0) {
        printf("No books to display.\n");
        return;
    }
    printf("%-5s | %-30s | %-20s | %-15s | %-15s\n", "ID", "Title", "Author", "Category", "Times Borrowed");
    printSeparator();
    for (int i = 0; i < n; i++) {
        printf("%-5d | %-30.30s | %-20.20s | %-15.15s | %-15d\n",
               arr[i]->id, arr[i]->title, arr[i]->author, arr[i]->category, arr[i]->timesBorrowed);
    }
}

void sortingMenu(Book *head) {
    int count = 0;
    Book **arr = bookListToArray(head, &count);
    if (!arr) {
        printf("No books available to sort.\n");
        pressEnterToContinue();
        return;
    }

    int algoChoice, criteriaChoice;
    clearScreen();
    printHeader("Sort Books");
    printf("1. Bubble Sort\n");
    printf("2. Selection Sort\n");
    printf("3. Insertion Sort\n");
    printf("4. Merge Sort\n");
    printf("5. Quick Sort\n");
    printf("0. Back\n");
    algoChoice = getIntInput("Select sorting algorithm: ");
    
    if (algoChoice == 0) {
        free(arr);
        return;
    }

    printf("\n1. By Book ID\n");
    printf("2. By Title\n");
    printf("3. By Author\n");
    printf("4. By Category\n");
    printf("5. By Most Borrowed\n");
    criteriaChoice = getIntInput("Select sorting criteria: ");

    if (criteriaChoice < 1 || criteriaChoice > 5) {
        printf("Invalid criteria.\n");
        free(arr);
        pressEnterToContinue();
        return;
    }

    switch (algoChoice) {
        case 1: bubbleSortBooks(arr, count, criteriaChoice); break;
        case 2: selectionSortBooks(arr, count, criteriaChoice); break;
        case 3: insertionSortBooks(arr, count, criteriaChoice); break;
        case 4: mergeSortBooks(arr, 0, count - 1, criteriaChoice); break;
        case 5: quickSortBooks(arr, 0, count - 1, criteriaChoice); break;
        default:
            printf("Invalid algorithm.\n");
            free(arr);
            pressEnterToContinue();
            return;
    }

    clearScreen();
    printHeader("Sorted Books Result");
    displaySortedBooks(arr, count);
    
    free(arr);
    pressEnterToContinue();
}
