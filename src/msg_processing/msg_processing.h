// msg_processing.h
// Module of processing message.
#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include "../msg_queue/list.h"

struct MsgQueues
{
    List *recv_queue;
    List *send_queue;
};

// Thread of reading message from the queue.
void *ReadMessage(void *msg_queues_ptr);

// Signal the msg-processing thread to dequeue message from queue.
void SignalMsgProcessing();

// Init message processing thread.
void InitMessageProcessing(struct MsgQueues *msg_queues_ptr);

// Join message processing thread.
void ShutDownMessageProcessing();

#endif