#ifndef BST_H
#define BST_H

#include "utils.h"

/*
 * Binary Search Tree for efficient Book ID lookup
 * Average Time Complexity: O(log n)
 * Worst Case: O(n) - degenerate tree
 */
BSTNode *insertBST(BSTNode *root, int bookId, Book *bookPtr);
BSTNode *searchBST(BSTNode *root, int bookId);
BSTNode *deleteBST(BSTNode *root, int bookId);
BSTNode *findMinBST(BSTNode *root);
void inorderTraversal(BSTNode *root);
void preorderTraversal(BSTNode *root);
void postorderTraversal(BSTNode *root);
BSTNode *buildBSTFromList(Book *bookList);
void freeBST(BSTNode *root);
int countBSTNodes(BSTNode *root);
int bstHeight(BSTNode *root);

#endif
