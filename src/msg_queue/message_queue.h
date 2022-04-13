// message_queue.h
// Module of functions of the message queue.
#ifndef _MESSAGE_QUEUE_UTILS_H_
#define _MESSAGE_QUEUE_UTILS_H_

#include "list.h"

// Enqueue into message queue and signal UDP Client to send.
void MessageEnqueueAndSignalClient(List* list, char* message);

// Enqueue into message queue.
int MessageEnqueue(List* list, char* message);

// Dequeue into message queue.
char* MessageDequeue(List* list);

// Get the size of message queue.
int GetMessageQueueSize(List* list);

#endif