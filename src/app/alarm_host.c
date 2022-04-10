#include <stdlib.h>
#include <stdio.h>
#include "../joystick/joystick.h"
#include "../msg_processing/msg_processing.h"
#include "../msg_queue/list.h"
#include "../msg_queue/message_queue.h"
#include "../udp/udp_client.h"
#include "../udp/udp_server.h"

int main() {
    printf("Host: Happy Saint Patrick's Day!\n");
    Joystick_init();

    char* my_port = "6000";
    char* remote_name = "beaglebone";
    char* remote_port = "6001";

    List* send_queue = List_create();
    List* recv_queue = List_create();

    InitUdpClient(send_queue, remote_name, remote_port);
    InitUdpServer(recv_queue, my_port);
    InitMessageProcessing(recv_queue);

    MessageEnqueue(send_queue, "SCREEN_SHOW_PUZZLE");

    ShutDownUdpClient();
    ShutDownUdpServer();
    ShutDownMessageProcessing();

    List_free(send_queue, free);
    List_free(recv_queue, free);

    Joystick_quit();
    return 0;
}
