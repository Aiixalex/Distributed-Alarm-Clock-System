#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include "../msg_queue/list.h"

struct MsgQueues
{
    List *recv_queue;
    List *send_queue;
};

void *ReadMessage(void *msg_queues_ptr);
void SignalMsgProcessing();
void InitMessageProcessing(struct MsgQueues *msg_queues_ptr);
void ShutDownMessageProcessing();
void CancelMessageProcessing();

#endif