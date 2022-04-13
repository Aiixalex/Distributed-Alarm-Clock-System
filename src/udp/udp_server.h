// udp_server.h
// Module of UDP Server which receive messages from other beaglebones as well as web server.
#ifndef _UDP_SERVER_H_
#define _UDP_SERVER_H_

#include "../msg_queue/list.h"

// Thread function that receive message to enqueue in message queue.
void* UdpRecv(void* message_queue);

// Init the UDP server thread.
void InitUdpServer(List* message_queue, char* my_port);

// Join the UDP server thread.
void ShutDownUdpServer();

#endif