#ifndef _OUTPUT_WRITER_H_
#define _OUTPUT_WRITER_H_

#include "../msg_queue/list.h"

void* ReadMessage(void* message_queue);
void SignalMsgProcessing();
void InitMessageProcessing(List* message_queue);
void ShutDownMessageProcessing();
void CancelMessageProcessing();

#endif