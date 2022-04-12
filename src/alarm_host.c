// main code for the host alarm
// will initialize everything and start all threads
#include <stdlib.h>
#include <stdio.h>
#include "puzzle/puzzle.h"
#include "clock/clock.h"
#include "alarm/alarm.h"
#include "utility/utility.h"
#include "screen/screen_manager.h"
#include "msg_queue/message_queue.h"
#include "msg_processing/msg_processing.h"
#include "udp/udp_client.h"
#include "udp/udp_server.h"

int main() {
    printf("This is alarm_host\n");
    initialize_my_lock_signal_wait();
    Screen_init();

    char* my_port = "6001";
    char* remote_name = "192.168.7.2";
    char* remote_port = "6000";

    List* send_queue = List_create();
    List* recv_queue = List_create();

    InitUdpClient(send_queue, remote_name, remote_port);
    InitUdpServer(recv_queue, my_port);

    struct MsgQueues msg_queues = {recv_queue, send_queue};
    InitMessageProcessing(&msg_queues);

    // default settings
    int hour = 6, minute = 27;
    bool activeDayOfWeek[7] = {0, 0, 0, 0, 0, 0, 0};
    Alarm_init(hour + 12, minute, activeDayOfWeek, 0);

    while (1) {
        my_lock_signal_wait();

        printf("Signal received, genreating puzzle and sending to BBG2\n");

        // received a message which contains the problem struct from BBG 1
        Clock_setDisplayType(rectangle);

        // generate puzzle and send message to BBG1
        puzzle currentPuzzle = Puzzle_generate();
        Screen_set_problem(currentPuzzle);

        //send problem to BBG2
        char *myData = malloc(MAX_STRING_LENGTH);
        snprintf(myData, MAX_STRING_LENGTH - 1, "PUZZLE_ANSWER %d", currentPuzzle.answer);
        MessageEnqueueAndSignalClient(send_queue, myData);        
    }

    ShutDownUdpClient();
    ShutDownUdpServer();
    ShutDownMessageProcessing();

    List_free(send_queue, free);
    List_free(recv_queue, free);

    return 0;
}
