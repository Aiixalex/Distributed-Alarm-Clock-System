#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_

#include "../msg_queue/list.h"

void* UdpSendto(void* message_queue);
void SignalUdpClient();
void InitUdpClient(List* message_queue, char* remote_name, char* remote_port);
void ShutDownUdpClient();
void CancelUdpClient();

#endif