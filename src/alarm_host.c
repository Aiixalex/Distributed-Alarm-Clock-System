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

static diffculity puzzleDiffculity = easy;

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
    InitMessageProcessing(recv_queue);

    // default settings
    int hour = 6, minute = 27;
    bool activeDayOfWeek[7] = {0, 0, 0, 0, 1, 1, 1};
    Alarm_init(hour + 12, minute, activeDayOfWeek, guest);

    while (1) {
        Clock_setDisplayType(host);
        my_lock_signal_wait();

        // received a message which contains the problem struct from BBG 1
        Clock_setDisplayType(rectangle);

        // generate puzzle and send message to BBG1
        puzzle currentPuzzle = Puzzle_generate(puzzleDiffculity);
        Screen_set_problem(currentPuzzle);

        //send problem to BBG2
        char myData[MAX_STRING_LENGTH] = "";
        snprintf(myData, MAX_STRING_LENGTH - 1, "PUZZLE_ANSWER %d", currentPuzzle.answer);
        MessageEnqueue(send_queue, myData);        
    }

    ShutDownUdpClient();
    ShutDownUdpServer();
    ShutDownMessageProcessing();

    List_free(send_queue, free);
    List_free(recv_queue, free);

    return 0;
}
