#ifndef MEMBERS_H
#define MEMBERS_H

#include "utils.h"

Member *createMember(int id, const char *name, const char *phone, const char *email);
Member *addMemberToList(Member *head, Member *newMember);
Member *addMember(Member *head, IdCounters *counters, UndoNode **undoStack);
Member *deleteMember(Member *head, Transaction *transList, UndoNode **undoStack);
void updateMember(Member *head, UndoNode **undoStack);
Member *findMemberById(Member *head, int id);
void displayAllMembers(Member *head);
void displayMemberDetails(Member *member);
int countMembers(Member *head);
void memberManagementMenu(Member **head, IdCounters *counters, Transaction *transList,
                          UndoNode **undoStack);
void freeMembers(Member *head);

#endif
