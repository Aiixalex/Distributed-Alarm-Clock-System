#ifndef _MESSAGE_QUEUE_UTILS_H_
#define _MESSAGE_QUEUE_UTILS_H_

#include "list.h"

int MessageEnqueue(List* list, char* message);
char* MessageDequeue(List* list);
int GetMessageQueueSize(List* list);

#endif