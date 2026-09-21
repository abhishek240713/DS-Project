#include "books.h"
#include "stack.h"
#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

Book *createBook(int id, const char *title, const char *author, const char *category,
                 const char *isbn, const char *publisher, int totalCopies) {
    Book *newBook = (Book *)malloc(sizeof(Book));
    if (!newBook) {
        printf("Error: Memory allocation failed for new book.\n");
        return NULL;
    }
    newBook->id = id;
    strncpy(newBook->title, title, MAX_TITLE - 1);
    newBook->title[MAX_TITLE - 1] = '\0';
    strncpy(newBook->author, author, MAX_AUTHOR - 1);
    newBook->author[MAX_AUTHOR - 1] = '\0';
    strncpy(newBook->category, category, MAX_CATEGORY - 1);
    newBook->category[MAX_CATEGORY - 1] = '\0';
    strncpy(newBook->isbn, isbn, MAX_ISBN - 1);
    newBook->isbn[MAX_ISBN - 1] = '\0';
    strncpy(newBook->publisher, publisher, MAX_PUBLISHER - 1);
    newBook->publisher[MAX_PUBLISHER - 1] = '\0';
    newBook->totalCopies = totalCopies;
    newBook->availableCopies = totalCopies;
    newBook->issuedCopies = 0;
    newBook->timesBorrowed = 0;
    newBook->next = NULL;
    return newBook;
}

Book *addBookToList(Book *head, Book *newBook) {
    if (!head) return newBook;
    Book *curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    curr->next = newBook;
    return head;
}

static int compareIsbnIgnoreCase(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return 1;
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) != tolower((unsigned char)*s2);
}

Book *addBook(Book *head, BSTNode **bstRoot, IdCounters *counters, UndoNode **undoStack) {
    printHeader("Add New Book");
    char title[MAX_TITLE], author[MAX_AUTHOR], category[MAX_CATEGORY], isbn[MAX_ISBN], publisher[MAX_PUBLISHER];
    int totalCopies = 0;

    getStringInput("Enter Title: ", title, MAX_TITLE);
    if (strlen(title) == 0) {
        printf("Error: Title cannot be empty.\n");
        return head;
    }

    getStringInput("Enter Author: ", author, MAX_AUTHOR);
    getStringInput("Enter Category: ", category, MAX_CATEGORY);
    getStringInput("Enter ISBN: ", isbn, MAX_ISBN);
    if (strlen(isbn) == 0) {
        printf("Error: ISBN cannot be empty.\n");
        return head;
    }
    
    Book *curr = head;
    while (curr) {
        if (compareIsbnIgnoreCase(curr->isbn, isbn) == 0) {
            printf("Error: A book with this ISBN already exists.\n");
            return head;
        }
        curr = curr->next;
    }

    getStringInput("Enter Publisher: ", publisher, MAX_PUBLISHER);
    totalCopies = getIntInput("Enter Total Copies: ");
    if (totalCopies <= 0) {
        printf("Error: Total copies must be greater than 0.\n");
        return head;
    }

    int id = counters->nextBookId++;
    Book *newBook = createBook(id, title, author, category, isbn, publisher, totalCopies);
    if (!newBook) return head;

    head = addBookToList(head, newBook);
    *bstRoot = insertBST(*bstRoot, newBook->id, newBook);

    Book *copy = (Book *)malloc(sizeof(Book));
    if (copy) {
        memcpy(copy, newBook, sizeof(Book));
        copy->next = NULL;
        pushUndo(undoStack, UNDO_ADD_BOOK, copy, newBook->id);
    }

    printf("Book added successfully! ID: %d\n", newBook->id);
    return head;
}

Book *deleteBook(Book *head, BSTNode **bstRoot, Transaction *transList, UndoNode **undoStack) {
    printHeader("Delete Book");
    int id = getIntInput("Enter Book ID to delete: ");
    
    Book *prev = NULL;
    Book *curr = head;
    while (curr && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        printf("Error: Book with ID %d not found.\n", id);
        return head;
    }

    Transaction *t = transList;
    while (t) {
        if (t->bookId == id && t->status == STATUS_ACTIVE) {
            printf("Cannot delete: book has active transactions\n");
            return head;
        }
        t = t->next;
    }

    if (curr->issuedCopies > 0) {
        printf("Error: Cannot delete book because there are issued copies.\n");
        return head;
    }

    Book *copy = (Book *)malloc(sizeof(Book));
    if (copy) {
        memcpy(copy, curr, sizeof(Book));
        copy->next = NULL;
        pushUndo(undoStack, UNDO_DELETE_BOOK, copy, curr->id);
    }

    if (prev) {
        prev->next = curr->next;
    } else {
        head = curr->next;
    }

    *bstRoot = deleteBST(*bstRoot, id);
    free(curr);
    printf("Book deleted successfully!\n");
    return head;
}

void updateBook(Book *head, UndoNode **undoStack) {
    printHeader("Update Book");
    int id = getIntInput("Enter Book ID to update: ");
    Book *curr = findBookById(head, id);
    if (!curr) {
        printf("Error: Book with ID %d not found.\n", id);
        return;
    }

    displayBookDetails(curr, NULL);

    Book *copy = (Book *)malloc(sizeof(Book));
    if (copy) {
        memcpy(copy, curr, sizeof(Book));
        copy->next = NULL;
        pushUndo(undoStack, UNDO_UPDATE_BOOK, copy, curr->id);
    }

    int choice;
    do {
        printf("\nUpdate Menu:\n");
        printf("1. Title\n2. Author\n3. Category\n4. ISBN\n5. Publisher\n6. Total Copies\n0. Cancel\n");
        choice = getIntInput("Enter your choice: ");
        char buf[256];
        switch (choice) {
            case 1:
                getStringInput("Enter new Title: ", buf, MAX_TITLE);
                if (strlen(buf) > 0) { strncpy(curr->title, buf, MAX_TITLE); curr->title[MAX_TITLE-1] = '\0'; }
                break;
            case 2:
                getStringInput("Enter new Author: ", buf, MAX_AUTHOR);
                strncpy(curr->author, buf, MAX_AUTHOR); curr->author[MAX_AUTHOR-1] = '\0';
                break;
            case 3:
                getStringInput("Enter new Category: ", buf, MAX_CATEGORY);
                strncpy(curr->category, buf, MAX_CATEGORY); curr->category[MAX_CATEGORY-1] = '\0';
                break;
            case 4:
                getStringInput("Enter new ISBN: ", buf, MAX_ISBN);
                strncpy(curr->isbn, buf, MAX_ISBN); curr->isbn[MAX_ISBN-1] = '\0';
                break;
            case 5:
                getStringInput("Enter new Publisher: ", buf, MAX_PUBLISHER);
                strncpy(curr->publisher, buf, MAX_PUBLISHER); curr->publisher[MAX_PUBLISHER-1] = '\0';
                break;
            case 6: {
                int newTotal = getIntInput("Enter new Total Copies: ");
                if (newTotal <= 0) {
                    printf("Error: Total copies must be greater than 0.\n");
                } else if (newTotal < curr->issuedCopies) {
                    printf("Error: Total copies cannot be less than issued copies (%d).\n", curr->issuedCopies);
                } else {
                    curr->totalCopies = newTotal;
                    curr->availableCopies = newTotal - curr->issuedCopies;
                }
                break;
            }
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
        if (choice >= 1 && choice <= 6) {
            printf("Book updated successfully!\n");
        }
    } while (choice != 0);
}

Book *findBookById(Book *head, int id) {
    Book *curr = head;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

void displayAllBooks(Book *head) {
    if (!head) {
        printf("No books in library\n");
        return;
    }
    printf("%-5s | %-30.30s | %-20.20s | %-15.15s | %-5s | %-5s\n", "ID", "Title", "Author", "Category", "Avail", "Iss");
    printSeparator();
    int count = 0;
    Book *curr = head;
    while (curr) {
        printf("%-5d | %-30.30s | %-20.20s | %-15.15s | %-5d | %-5d\n",
               curr->id, curr->title, curr->author, curr->category, curr->availableCopies, curr->issuedCopies);
        count++;
        curr = curr->next;
    }
    printf("Total books: %d\n", count);
}

void displayBookDetails(Book *book, ViewStackNode **viewStack) {
    if (!book) return;
    printf("ID: %d\n", book->id);
    printf("Title: %s\n", book->title);
    printf("Author: %s\n", book->author);
    printf("Category: %s\n", book->category);
    printf("ISBN: %s\n", book->isbn);
    printf("Publisher: %s\n", book->publisher);
    printf("Total Copies: %d\n", book->totalCopies);
    printf("Available Copies: %d\n", book->availableCopies);
    printf("Issued Copies: %d\n", book->issuedCopies);
    printf("Times Borrowed: %d\n", book->timesBorrowed);
    
    if (viewStack) {
        pushView(viewStack, book->id);
    }
}

int countBooks(Book *head) {
    int count = 0;
    Book *curr = head;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

void bookManagementMenu(Book **head, BSTNode **bstRoot, IdCounters *counters,
                        Transaction *transList, UndoNode **undoStack, ViewStackNode **viewStack) {
    int choice;
    do {
        printf("1. Add Book\n");
        printf("2. Delete Book\n");
        printf("3. Update Book\n");
        printf("4. Display All Books\n");
        printf("5. View Book Details\n");
        printf("0. Back to Main Menu\n");
        choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1:
                *head = addBook(*head, bstRoot, counters, undoStack);
                break;
            case 2:
                *head = deleteBook(*head, bstRoot, transList, undoStack);
                break;
            case 3:
                updateBook(*head, undoStack);
                break;
            case 4:
                displayAllBooks(*head);
                break;
            case 5: {
                int id = getIntInput("Enter Book ID to view: ");
                Book *b = findBookById(*head, id);
                if (b) {
                    displayBookDetails(b, viewStack);
                } else {
                    printf("Error: Book not found.\n");
                }
                break;
            }
            case 0:
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void freeBooks(Book *head) {
    Book *curr = head;
    while (curr) {
        Book *temp = curr;
        curr = curr->next;
        free(temp);
    }
}
