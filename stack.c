#include "stack.h"

void pushView(ViewStackNode **top, int bookId) {
    ViewStackNode *curr = *top;
    ViewStackNode *prev = NULL;
    int count = 0;
    
    while (curr != NULL) {
        if (curr->bookId == bookId) {
            if (prev == NULL) {
                *top = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    
    curr = *top;
    count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    
    if (count >= MAX_VIEW_STACK) {
        curr = *top;
        prev = NULL;
        while (curr != NULL && curr->next != NULL) {
            prev = curr;
            curr = curr->next;
        }
        if (prev != NULL) {
            prev->next = NULL;
            free(curr);
        } else if (curr != NULL) {
            *top = NULL;
            free(curr);
        }
    }
    
    ViewStackNode *newNode = (ViewStackNode *)malloc(sizeof(ViewStackNode));
    if (!newNode) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    newNode->bookId = bookId;
    newNode->next = *top;
    *top = newNode;
}

int popView(ViewStackNode **top) {
    if (*top == NULL) return -1;
    ViewStackNode *temp = *top;
    int id = temp->bookId;
    *top = temp->next;
    free(temp);
    return id;
}

int peekView(ViewStackNode *top) {
    if (top == NULL) return -1;
    return top->bookId;
}

bool isViewStackEmpty(ViewStackNode *top) {
    return top == NULL;
}

void displayViewStack(ViewStackNode *top, Book *bookList) {
    printHeader("Recently Viewed Books");
    if (top == NULL) {
        printf("No recently viewed books.\n");
        return;
    }
    int num = 1;
    ViewStackNode *curr = top;
    while (curr != NULL) {
        Book *b = bookList;
        char *title = "Unknown";
        while (b != NULL) {
            if (b->id == curr->bookId) {
                title = b->title;
                break;
            }
            b = b->next;
        }
        printf("%d. [ID: %d] %s\n", num++, curr->bookId, title);
        curr = curr->next;
    }
}

void freeViewStack(ViewStackNode **top) {
    while (*top != NULL) {
        ViewStackNode *temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

void pushUndo(UndoNode **top, UndoType type, void *data, int id) {
    UndoNode *newNode = (UndoNode *)malloc(sizeof(UndoNode));
    if (!newNode) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    newNode->type = type;
    newNode->data = data;
    newNode->id = id;
    newNode->next = *top;
    *top = newNode;
}

UndoNode *popUndo(UndoNode **top) {
    if (*top == NULL) return NULL;
    UndoNode *temp = *top;
    *top = temp->next;
    return temp;
}

bool isUndoStackEmpty(UndoNode *top) {
    return top == NULL;
}

void freeUndoStack(UndoNode **top) {
    while (*top != NULL) {
        UndoNode *temp = *top;
        *top = (*top)->next;
        if (temp->data) {
            free(temp->data);
        }
        free(temp);
    }
}
