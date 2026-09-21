#ifndef STACK_H
#define STACK_H

#include "utils.h"

/* ---- Recently Viewed Books Stack ---- */
void pushView(ViewStackNode **top, int bookId);
int popView(ViewStackNode **top);
int peekView(ViewStackNode *top);
bool isViewStackEmpty(ViewStackNode *top);
void displayViewStack(ViewStackNode *top, Book *bookList);
void freeViewStack(ViewStackNode **top);

/* ---- Undo Operations Stack ---- */
void pushUndo(UndoNode **top, UndoType type, void *data, int id);
UndoNode *popUndo(UndoNode **top);
bool isUndoStackEmpty(UndoNode *top);
void freeUndoStack(UndoNode **top);

#endif
