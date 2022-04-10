#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include "../msg_queue/list.h"

void* UdpRecv(void* message_queue);
void InitUdpServer(List* message_queue, char* my_port);
void ShutDownUdpServer();
void CancelUdpServer();

#endif