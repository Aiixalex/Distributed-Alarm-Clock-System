// udp_client.h
// Module of UDP Client which send messages from the message queue to other beaglebones.
#ifndef _UDP_CLIENT_H_
#define _UDP_CLIENT_H_

#include "../msg_queue/list.h"

// Thread function that send message.
void* UdpSendto(void* message_queue);

// Signal the client to read message from the message queue and then send.
void SignalUdpClient();

// Init the client thread.
void InitUdpClient(List* message_queue, char* remote_name, char* remote_port);

// Join the client thread.
void ShutDownUdpClient();

#endif