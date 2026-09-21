#include "queue.h"

void initQueue(ReservationQueue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

bool hasReservation(const ReservationQueue *q, int memberId, int bookId) {
    if (!q) return false;
    for (ReservationNode *curr = q->front; curr; curr = curr->next) {
        if (curr->memberId == memberId && curr->bookId == bookId) return true;
    }
    return false;
}

int reservationPosition(const ReservationQueue *q, int memberId, int bookId) {
    if (!q) return -1;
    int position = 0;
    for (ReservationNode *curr = q->front; curr; curr = curr->next) {
        if (curr->bookId == bookId) {
            position++;
            if (curr->memberId == memberId) return position;
        }
    }
    return -1;
}

bool enqueue(ReservationQueue *q, int memberId, int bookId) {
    ReservationNode *newNode = (ReservationNode *)malloc(sizeof(ReservationNode));
    if (!newNode) {
        printf("Error: Memory allocation failed.\n");
        return false;
    }
    newNode->memberId = memberId;
    newNode->bookId = bookId;
    newNode->reservationDate = time(NULL);
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = newNode;
        q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->count++;
    return true;
}

ReservationNode *dequeue(ReservationQueue *q) {
    if (q->front == NULL) return NULL;
    ReservationNode *temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    q->count--;
    return temp;
}

ReservationNode *dequeueForBook(ReservationQueue *q, int bookId) {
    if (q->front == NULL) return NULL;
    
    ReservationNode *curr = q->front;
    ReservationNode *prev = NULL;
    
    while (curr != NULL) {
        if (curr->bookId == bookId) {
            if (prev == NULL) {
                q->front = curr->next;
                if (q->front == NULL) q->rear = NULL;
            } else {
                prev->next = curr->next;
                if (curr->next == NULL) q->rear = prev;
            }
            q->count--;
            return curr;
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}

bool isQueueEmpty(ReservationQueue *q) {
    return q->front == NULL;
}

int queueSize(ReservationQueue *q) {
    return q->count;
}

void displayQueue(ReservationQueue *q) {
    if (q->front == NULL) {
        printf("Queue is empty.\n");
        return;
    }
    ReservationNode *curr = q->front;
    while (curr != NULL) {
        char dateStr[20];
        formatDate(curr->reservationDate, dateStr, sizeof(dateStr));
        printf("Member: %d, Book: %d, Date: %s\n", curr->memberId, curr->bookId, dateStr);
        curr = curr->next;
    }
}

void displayQueueForBook(ReservationQueue *q, int bookId, Member *memberList) {
    if (q->front == NULL) {
        printf("Queue is empty for this book.\n");
        return;
    }
    ReservationNode *curr = q->front;
    int num = 1;
    bool found = false;
    while (curr != NULL) {
        if (curr->bookId == bookId) {
            found = true;
            char *memberName = "Unknown";
            Member *m = memberList;
            while (m != NULL) {
                if (m->id == curr->memberId) {
                    memberName = m->name;
                    break;
                }
                m = m->next;
            }
            printf("%d. Member ID: %d, Name: %s\n", num++, curr->memberId, memberName);
        }
        curr = curr->next;
    }
    if (!found) {
        printf("No reservations for this book.\n");
    }
}

void freeQueue(ReservationQueue *q) {
    while (q->front != NULL) {
        ReservationNode *temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
    q->count = 0;
}
