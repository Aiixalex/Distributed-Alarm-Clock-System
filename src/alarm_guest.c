#include <stdlib.h>
#include <stdio.h>
#include "puzzle/puzzle.h"
#include "keypad/keypad.h"
#include "alarm/alarm.h"
#include "utility/utility.h"
#include "msg_queue/message_queue.h"
#include "udp/udp_client.h"
#include "udp/udp_server.h"
#include "msg_processing/msg_processing.h"

int main() {
    printf("This is alarm_guest\n");

    char* my_port = "6000";
    char* remote_name = "192.168.0.212";
    char* remote_port = "6001";

    List* send_queue = List_create();
    List* recv_queue = List_create();
    
    Keypad_init();
    initialize_my_lock_signal_wait();
    int hour = 6, minute = 27;
    bool activeDayOfWeek[7] = {0, 0, 0, 0, 1, 1, 1};
    Alarm_init(hour + 12, minute, activeDayOfWeek, guest);

    InitUdpClient(send_queue, remote_name, remote_port);
    InitUdpServer(recv_queue, my_port);
    InitMessageProcessing(recv_queue);

    ShutDownUdpClient();
    ShutDownUdpServer();
    ShutDownMessageProcessing();

    List_free(send_queue, free);
    List_free(recv_queue, free);


    while (1) {
        //TODO: the UDP module will call "my_lock_signal_signal()" to wake up main
        my_lock_signal_wait();
    }

    return 0;
}
