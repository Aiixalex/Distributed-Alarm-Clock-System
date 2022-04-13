#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "../utils/handle_error.h"
#include "../msg_queue/list.h"
#include "../msg_queue/message_queue.h"
#include "../keypad/keypad.h"
#include "../clock/clock.h"
#include "../screen/screen_manager.h"
#include "../alarm/alarm.h"
#include "../utils/utils.h"
#include "../screen/OLED_text.h"
#include "msg_processing.h"

static pthread_t msg_processing_thread;
static pthread_mutex_t msg_processing_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t msg_processing_cond = PTHREAD_COND_INITIALIZER;

void process_message(char* message, struct MsgQueues *msg_queues_ptr)
{
    if (strncmp(message, "PUZZLE_ANSWER", 13) == 0)
    {
        // received a message which contains the problem struct from BBG 1
        printf("msg PUZZLE_ANSWER recved");
        int answer = 0;
        sscanf(message, "PUZZLE_ANSWER %d", &answer);

        // read input from user
        Clock_setDisplayType(rectangle);
        Alarm_trigger_alarm_manually();
        while (1)
        {
            int number = Keypad_readUserInput();
            printf("User input: %d\n", number);
            Clock_displayNumber(number);
            if (number == answer)
                break;
        }
        Clock_setDisplayType(guest);
        // if correct, disable alarm for today
        // sned message to BBG 1 to disbale their alarm
        Alarm_stop_trigger_manually();
        Alarm_stopRinging();

        char *myData = malloc(MAX_STRING_LENGTH);
        snprintf(myData, MAX_STRING_LENGTH - 1, "ANSWER_CORRECT");
        MessageEnqueueAndSignalClient(msg_queues_ptr->send_queue, myData);
    }
    else if (strcmp(message, "ANSWER_CORRECT") == 0)
    {
        printf("msg ANSWER_CORRECT recved\n");
        Alarm_stop_trigger_manually();
        Alarm_stopRinging();
        Clock_setDisplayType(host);
        Screen_clear();
    }else if(strncmp(message, "submit", 6) == 0){
        // Set the Alarm
        int hour = 0;
        int minute = 0;
        int day = 0;
        int diff = 0;
        int temp = 0;
        sscanf(message, "submit %d:%d:%d:%d:%d", &temp, &hour, &minute, &day, &diff);
        printf("Alarm set: %d:%d:%d:%d\n", hour, minute, day, diff);
        Alarm_changeTime(hour, minute);
        Alarm_changeOneDayOfTheWeek(day);
        Puzzle_updateDiffculity(diff);
    }else if(strncmp(message, "remove", 6) == 0){
        // Remove the Alarm
        printf("Alarm removed\n");
        Alarm_clear();
    }else if(strcmp(message, "trigger") == 0){
        // Send Alarm to Website
        printf("message trigger recved\n");

        // received a message which contains the problem struct from BBG 1
        Clock_setDisplayType(rectangle);

        // generate puzzle and send message to BBG1
        puzzle currentPuzzle = Puzzle_generate();
        Screen_set_problem(currentPuzzle);

        //send problem to BBG2
        char *myData = malloc(MAX_STRING_LENGTH);
        snprintf(myData, MAX_STRING_LENGTH - 1, "PUZZLE_ANSWER %d", currentPuzzle.answer);
        MessageEnqueueAndSignalClient(msg_queues_ptr->send_queue, myData);  
        Alarm_trigger_alarm_manually();
    }
}

void *ReadMessage(void* msg_queues_ptr)
{
    while (1)
    {
        pthread_mutex_lock(&msg_processing_mutex);
        pthread_cond_wait(&msg_processing_cond, &msg_processing_mutex);
        pthread_mutex_unlock(&msg_processing_mutex);

        char *message = MessageDequeue(((struct MsgQueues *)msg_queues_ptr)->recv_queue);
        if (message == NULL)
        {
            fprintf(stderr, "message dequeue failed.");
            continue;
        }

        process_message(message, msg_queues_ptr);

        free(message);
    }
    return NULL;
}

void SignalMsgProcessing()
{
    pthread_mutex_lock(&msg_processing_mutex);

    pthread_cond_signal(&msg_processing_cond);

    pthread_mutex_unlock(&msg_processing_mutex);
}

void InitMessageProcessing(struct MsgQueues *msg_queues_ptr)
{
    int error_num = pthread_create(&msg_processing_thread, NULL, &ReadMessage, msg_queues_ptr);
    if (error_num != 0)
    {
        handle_error_en(error_num, "pthread_create msg_processing_thread failed.");
    }
}

void ShutDownMessageProcessing()
{
    int error_num = pthread_join(msg_processing_thread, NULL);
    if (error_num != 0)
    {
        handle_error_en(error_num, "pthread_join msg_processing_thread failed.");
    }
}
