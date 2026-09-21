#include "members.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Member *createMember(int id, const char *name, const char *phone, const char *email) {
    Member *newMember = (Member *)malloc(sizeof(Member));
    if (!newMember) {
        printf("Error: Memory allocation failed for new member.\n");
        return NULL;
    }
    newMember->id = id;
    strncpy(newMember->name, name, MAX_NAME - 1);
    newMember->name[MAX_NAME - 1] = '\0';
    strncpy(newMember->phone, phone, MAX_PHONE - 1);
    newMember->phone[MAX_PHONE - 1] = '\0';
    strncpy(newMember->email, email, MAX_EMAIL - 1);
    newMember->email[MAX_EMAIL - 1] = '\0';
    newMember->booksCurrentlyBorrowed = 0;
    newMember->totalBooksBorrowed = 0;
    newMember->pendingFine = 0.0f;
    newMember->next = NULL;
    return newMember;
}

Member *addMemberToList(Member *head, Member *newMember) {
    if (!head) return newMember;
    Member *curr = head;
    while (curr->next) {
        curr = curr->next;
    }
    curr->next = newMember;
    return head;
}

Member *addMember(Member *head, IdCounters *counters, UndoNode **undoStack) {
    printHeader("Add New Member");
    char name[MAX_NAME], phone[MAX_PHONE], email[MAX_EMAIL];

    getStringInput("Enter Name: ", name, MAX_NAME);
    if (strlen(name) == 0) {
        printf("Error: Name cannot be empty.\n");
        return head;
    }

    getStringInput("Enter Phone: ", phone, MAX_PHONE);
    getStringInput("Enter Email: ", email, MAX_EMAIL);
    
    if (!isValidPhone(phone) || !isValidEmail(email)) {
        char choice[10];
        getStringInput("Validation failed. Continue anyway? (y/n): ", choice, sizeof(choice));
        if (choice[0] != 'y' && choice[0] != 'Y') {
            printf("Member addition cancelled.\n");
            return head;
        }
    }

    int id = counters->nextMemberId++;
    Member *newMember = createMember(id, name, phone, email);
    if (!newMember) return head;

    head = addMemberToList(head, newMember);

    Member *copy = (Member *)malloc(sizeof(Member));
    if (copy) {
        memcpy(copy, newMember, sizeof(Member));
        copy->next = NULL;
        pushUndo(undoStack, UNDO_ADD_MEMBER, copy, newMember->id);
    }

    printf("Member added successfully! ID: %d\n", newMember->id);
    return head;
}

Member *deleteMember(Member *head, Transaction *transList, UndoNode **undoStack) {
    printHeader("Delete Member");
    int id = getIntInput("Enter Member ID to delete: ");
    
    Member *prev = NULL;
    Member *curr = head;
    while (curr && curr->id != id) {
        prev = curr;
        curr = curr->next;
    }
    if (!curr) {
        printf("Error: Member with ID %d not found.\n", id);
        return head;
    }

    Transaction *t = transList;
    while (t) {
        if (t->memberId == id && t->status == STATUS_ACTIVE) {
            printf("Error: Cannot delete member with active transactions.\n");
            return head;
        }
        t = t->next;
    }

    if (curr->pendingFine > 0) {
        printf("Warning: Member has unpaid fines (%.2f). Deleting anyway...\n", curr->pendingFine);
    }

    Member *copy = (Member *)malloc(sizeof(Member));
    if (copy) {
        memcpy(copy, curr, sizeof(Member));
        copy->next = NULL;
        pushUndo(undoStack, UNDO_DELETE_MEMBER, copy, curr->id);
    }

    if (prev) {
        prev->next = curr->next;
    } else {
        head = curr->next;
    }

    free(curr);
    printf("Member deleted successfully!\n");
    return head;
}

void updateMember(Member *head, UndoNode **undoStack) {
    printHeader("Update Member");
    int id = getIntInput("Enter Member ID to update: ");
    Member *curr = findMemberById(head, id);
    if (!curr) {
        printf("Error: Member with ID %d not found.\n", id);
        return;
    }

    displayMemberDetails(curr);

    Member *copy = (Member *)malloc(sizeof(Member));
    if (copy) {
        memcpy(copy, curr, sizeof(Member));
        copy->next = NULL;
        pushUndo(undoStack, UNDO_UPDATE_MEMBER, copy, curr->id);
    }

    int choice;
    do {
        printf("\nUpdate Menu:\n");
        printf("1. Name\n2. Phone\n3. Email\n0. Cancel\n");
        choice = getIntInput("Enter your choice: ");
        char buf[256];
        switch (choice) {
            case 1:
                getStringInput("Enter new Name: ", buf, MAX_NAME);
                if (strlen(buf) > 0) { strncpy(curr->name, buf, MAX_NAME); curr->name[MAX_NAME-1] = '\0'; }
                break;
            case 2:
                getStringInput("Enter new Phone: ", buf, MAX_PHONE);
                if (!isValidPhone(buf)) printf("Warning: Invalid phone format.\n");
                strncpy(curr->phone, buf, MAX_PHONE); curr->phone[MAX_PHONE-1] = '\0';
                break;
            case 3:
                getStringInput("Enter new Email: ", buf, MAX_EMAIL);
                if (!isValidEmail(buf)) printf("Warning: Invalid email format.\n");
                strncpy(curr->email, buf, MAX_EMAIL); curr->email[MAX_EMAIL-1] = '\0';
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
        if (choice >= 1 && choice <= 3) {
            printf("Member updated successfully!\n");
        }
    } while (choice != 0);
}

Member *findMemberById(Member *head, int id) {
    Member *curr = head;
    while (curr) {
        if (curr->id == id) return curr;
        curr = curr->next;
    }
    return NULL;
}

void displayAllMembers(Member *head) {
    if (!head) {
        printf("No members in library.\n");
        return;
    }
    printf("%-5s | %-25.25s | %-15.15s | %-25.25s | %-8s | %-5s\n", "ID", "Name", "Phone", "Email", "Borrowed", "Fine");
    printSeparator();
    int count = 0;
    Member *curr = head;
    while (curr) {
        printf("%-5d | %-25.25s | %-15.15s | %-25.25s | %-8d | %-5.2f\n",
               curr->id, curr->name, curr->phone, curr->email, curr->booksCurrentlyBorrowed, curr->pendingFine);
        count++;
        curr = curr->next;
    }
    printf("Total members: %d\n", count);
}

void displayMemberDetails(Member *member) {
    if (!member) return;
    printf("ID: %d\n", member->id);
    printf("Name: %s\n", member->name);
    printf("Phone: %s\n", member->phone);
    printf("Email: %s\n", member->email);
    printf("Books Currently Borrowed: %d\n", member->booksCurrentlyBorrowed);
    printf("Total Books Borrowed: %d\n", member->totalBooksBorrowed);
    printf("Pending Fine: %.2f\n", member->pendingFine);
}

int countMembers(Member *head) {
    int count = 0;
    Member *curr = head;
    while (curr) {
        count++;
        curr = curr->next;
    }
    return count;
}

void memberManagementMenu(Member **head, IdCounters *counters, Transaction *transList,
                          UndoNode **undoStack) {
    int choice;
    do {
        printf("1. Add Member\n");
        printf("2. Delete Member\n");
        printf("3. Update Member\n");
        printf("4. Display All Members\n");
        printf("5. View Member Details\n");
        printf("0. Back to Main Menu\n");
        choice = getIntInput("Enter choice: ");
        
        switch (choice) {
            case 1:
                *head = addMember(*head, counters, undoStack);
                break;
            case 2:
                *head = deleteMember(*head, transList, undoStack);
                break;
            case 3:
                updateMember(*head, undoStack);
                break;
            case 4:
                displayAllMembers(*head);
                break;
            case 5: {
                int id = getIntInput("Enter Member ID to view: ");
                Member *m = findMemberById(*head, id);
                if (m) {
                    displayMemberDetails(m);
                } else {
                    printf("Error: Member not found.\n");
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

void freeMembers(Member *head) {
    Member *curr = head;
    while (curr) {
        Member *temp = curr;
        curr = curr->next;
        free(temp);
    }
}
