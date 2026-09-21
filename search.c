#include "search.h"
#include "books.h"
#include "bst.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

Book *linearSearchBookById(Book *head, int id) {
    Book *curr = head;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

Book *linearSearchBookByTitle(Book *head, const char *title) {
    Book *curr = head;
    char lowerTitle[MAX_TITLE];
    char lowerSearch[MAX_TITLE];
    toLowerStr(lowerSearch, title);

    while (curr) {
        toLowerStr(lowerTitle, curr->title);
        if (strstr(lowerTitle, lowerSearch) != NULL) return curr;
        curr = curr->next;
    }
    return NULL;
}

Book *linearSearchBookByAuthor(Book *head, const char *author) {
    Book *curr = head;
    char lowerAuthor[MAX_AUTHOR];
    char lowerSearch[MAX_AUTHOR];
    toLowerStr(lowerSearch, author);

    while (curr) {
        toLowerStr(lowerAuthor, curr->author);
        if (strstr(lowerAuthor, lowerSearch) != NULL) return curr;
        curr = curr->next;
    }
    return NULL;
}

Book *linearSearchBookByCategory(Book *head, const char *category) {
    Book *curr = head;
    char lowerCat[MAX_CATEGORY];
    char lowerSearch[MAX_CATEGORY];
    toLowerStr(lowerSearch, category);

    while (curr) {
        toLowerStr(lowerCat, curr->category);
        if (strstr(lowerCat, lowerSearch) != NULL) return curr;
        curr = curr->next;
    }
    return NULL;
}

Book *linearSearchBookByISBN(Book *head, const char *isbn) {
    Book *curr = head;
    char lowerIsbn[MAX_ISBN];
    char lowerSearch[MAX_ISBN];
    toLowerStr(lowerSearch, isbn);

    while (curr) {
        toLowerStr(lowerIsbn, curr->isbn);
        if (strcmp(lowerIsbn, lowerSearch) == 0) return curr;
        curr = curr->next;
    }
    return NULL;
}

Member *linearSearchMemberById(Member *head, int id) {
    Member *curr = head;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

Member *linearSearchMemberByName(Member *head, const char *name) {
    Member *curr = head;
    char lowerName[MAX_NAME];
    char lowerSearch[MAX_NAME];
    toLowerStr(lowerSearch, name);

    while (curr) {
        toLowerStr(lowerName, curr->name);
        if (strstr(lowerName, lowerSearch) != NULL) return curr;
        curr = curr->next;
    }
    return NULL;
}

/* O(log n) time complexity */
int binarySearchBookArray(Book **arr, int size, int targetId) {
    int low = 0;
    int high = size - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (arr[mid]->id == targetId) return mid;
        if (arr[mid]->id < targetId) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

void searchMenu(Book *head, Member *memberList, BSTNode *bstRoot, ViewStackNode **viewStack) {
    int choice;
    char term[100];
    do {
        clearScreen();
        printHeader("Search");
        printf("1. Search Book by ID (BST)\n");
        printf("2. Search Book by Title\n");
        printf("3. Search Book by Author\n");
        printf("4. Search Book by Category\n");
        printf("5. Search Book by ISBN\n");
        printf("6. Search Member by ID\n");
        printf("7. Search Member by Name\n");
        printf("0. Back\n");
        choice = getIntInput("Select an option: ");

        switch (choice) {
            case 1: {
                int id = getIntInput("Enter Book ID: ");
                BSTNode *node = searchBST(bstRoot, id);
                if (node) {
                    displayBookDetails(node->bookPtr, viewStack);
                } else {
                    Book *b = linearSearchBookById(head, id);
                    if (b) {
                        displayBookDetails(b, viewStack);
                    } else {
                        printf("Book not found.\n");
                    }
                }
                pressEnterToContinue();
                break;
            }
            case 2: {
                getStringInput("Enter title to search: ", term, 100);
                Book *b = linearSearchBookByTitle(head, term);
                if (b) {
                    displayBookDetails(b, viewStack);
                } else {
                    printf("Book not found.\n");
                }
                pressEnterToContinue();
                break;
            }
            case 3: {
                getStringInput("Enter author to search: ", term, 100);
                Book *b = linearSearchBookByAuthor(head, term);
                if (b) {
                    displayBookDetails(b, viewStack);
                } else {
                    printf("Book not found.\n");
                }
                pressEnterToContinue();
                break;
            }
            case 4: {
                getStringInput("Enter category to search: ", term, 100);
                Book *b = linearSearchBookByCategory(head, term);
                if (b) {
                    displayBookDetails(b, viewStack);
                } else {
                    printf("Book not found.\n");
                }
                pressEnterToContinue();
                break;
            }
            case 5: {
                getStringInput("Enter ISBN to search: ", term, 100);
                Book *b = linearSearchBookByISBN(head, term);
                if (b) {
                    displayBookDetails(b, viewStack);
                } else {
                    printf("Book not found.\n");
                }
                pressEnterToContinue();
                break;
            }
            case 6: {
                int id = getIntInput("Enter Member ID: ");
                Member *m = linearSearchMemberById(memberList, id);
                if (m) {
                    printf("Found Member: %s (ID: %d)\n", m->name, m->id);
                } else {
                    printf("Member not found.\n");
                }
                pressEnterToContinue();
                break;
            }
            case 7: {
                getStringInput("Enter member name to search: ", term, 100);
                Member *m = linearSearchMemberByName(memberList, term);
                if (m) {
                    printf("Found Member: %s (ID: %d)\n", m->name, m->id);
                } else {
                    printf("Member not found.\n");
                }
                pressEnterToContinue();
                break;
            }
            case 0:
                break;
            default:
                printf("Invalid option.\n");
                pressEnterToContinue();
        }
    } while (choice != 0);
}
