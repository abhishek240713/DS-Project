#include "bst.h"

BSTNode *insertBST(BSTNode *root, int bookId, Book *bookPtr) {
    if (root == NULL) {
        BSTNode *newNode = (BSTNode *)malloc(sizeof(BSTNode));
        if (!newNode) {
            printf("Error: Memory allocation failed.\n");
            return NULL;
        }
        newNode->bookId = bookId;
        newNode->bookPtr = bookPtr;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }
    if (bookId < root->bookId) {
        root->left = insertBST(root->left, bookId, bookPtr);
    } else if (bookId > root->bookId) {
        root->right = insertBST(root->right, bookId, bookPtr);
    }
    return root;
}

BSTNode *searchBST(BSTNode *root, int bookId) {
    if (root == NULL || root->bookId == bookId) {
        return root;
    }
    if (bookId < root->bookId) {
        return searchBST(root->left, bookId);
    }
    return searchBST(root->right, bookId);
}

BSTNode *findMinBST(BSTNode *root) {
    if (root == NULL) return NULL;
    if (root->left == NULL) return root;
    return findMinBST(root->left);
}

BSTNode *deleteBST(BSTNode *root, int bookId) {
    if (root == NULL) return NULL;
    
    if (bookId < root->bookId) {
        root->left = deleteBST(root->left, bookId);
    } else if (bookId > root->bookId) {
        root->right = deleteBST(root->right, bookId);
    } else {
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }
        
        BSTNode *temp = findMinBST(root->right);
        root->bookId = temp->bookId;
        root->bookPtr = temp->bookPtr;
        root->right = deleteBST(root->right, temp->bookId);
    }
    return root;
}

void inorderTraversal(BSTNode *root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        if (root->bookPtr != NULL) {
            printf("  [%d] %s\n", root->bookPtr->id, root->bookPtr->title);
        }
        inorderTraversal(root->right);
    }
}

void preorderTraversal(BSTNode *root) {
    if (root != NULL) {
        if (root->bookPtr != NULL) {
            printf("  [%d] %s\n", root->bookPtr->id, root->bookPtr->title);
        }
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

void postorderTraversal(BSTNode *root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        if (root->bookPtr != NULL) {
            printf("  [%d] %s\n", root->bookPtr->id, root->bookPtr->title);
        }
    }
}

BSTNode *buildBSTFromList(Book *bookList) {
    BSTNode *root = NULL;
    Book *curr = bookList;
    while (curr != NULL) {
        root = insertBST(root, curr->id, curr);
        curr = curr->next;
    }
    return root;
}

void freeBST(BSTNode *root) {
    if (root != NULL) {
        freeBST(root->left);
        freeBST(root->right);
        free(root);
    }
}

int countBSTNodes(BSTNode *root) {
    if (root == NULL) return 0;
    return 1 + countBSTNodes(root->left) + countBSTNodes(root->right);
}

int bstHeight(BSTNode *root) {
    if (root == NULL) return 0;
    int leftHeight = bstHeight(root->left);
    int rightHeight = bstHeight(root->right);
    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
}
