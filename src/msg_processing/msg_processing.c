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
#include "../utility/utility.h"
#include "../screen/OLED_text.h"

static pthread_t msg_processing_thread;
static pthread_mutex_t msg_processing_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t msg_processing_cond = PTHREAD_COND_INITIALIZER;

void process_message(List *message_queue, char* message)
{
    if (strcmp(message, "SCREEN_SHOW_PUZZLE") == 0)
    {
        MessageEnqueueAndSignalClient(message_queue, "PUZZLE_ANSWER");
        printf("1\n");
    }
    else if (strncmp(message, "PUZZLE_ANSWER", 13) == 0)
    {
        // received a message which contains the problem struct from BBG 1
        printf("msg PUZZLE_ANSWER recved");
        int answer = 0;
        sscanf(message, "PUZZLE_ANSWER %d", &answer);

        // read input from user
        while (1)
        {
            Clock_setDisplayType(rectangle);
            int number = Keypad_readUserInput();
            printf("User input: %d\n", number);
            Clock_displayNumber(number);
            if (number == answer)
                break;
        }
        
        // if correct, disable alarm for today
        // sned message to BBG 1 to disbale their alarm
        Alarm_stopRinging();
    }
    else if (strcmp(message, "ANSWER_CORRECT") == 0)
    {
        printf("msg ANSWER_CORRECT recved\n");
        Alarm_stopRinging();
        OLED_text_clearDisplay();
    }else if(strncmp(message, "update", 6) == 0){
        // Get time
        // char* msg = Alarm_getScheduledTime();
        //Send Time to website
        // MessageEnqueueAndSignalClient(send_queue, msg);

    }else if(strncmp(message, "submit", 6) == 0){
        // Set the Alarm

    }else if(strncmp(message, "remove", 6) == 0){
        // Remove the Alarm
    }else if(strcmp(message, "trigger") == 0){
        // Send Alarm to Website
        printf("message trigger recved\n");
        my_lock_signal_signal();
        Alarm_trigger_alarm_manually();
    }
}

void *ReadMessage(void *message_queue)
{
    while (1)
    {
        pthread_mutex_lock(&msg_processing_mutex);
        pthread_cond_wait(&msg_processing_cond, &msg_processing_mutex);
        pthread_mutex_unlock(&msg_processing_mutex);

        char *message = MessageDequeue(message_queue);
        if (message == NULL)
        {
            fprintf(stderr, "message dequeue failed.");
            continue;
        }

        // int msg_size = write(STDOUT_FILENO, message, strlen(message));
        // if(msg_size == -1) {
        //     handle_error("Output function write() failed.");
        // }

        process_message(message_queue, message);

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

void InitMessageProcessing(List *message_queue)
{
    int error_num = pthread_create(&msg_processing_thread, NULL, &ReadMessage, message_queue);
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

void CancelMessageProcessing()
{
    int error_num = pthread_cancel(msg_processing_thread);
    if (error_num != 0)
    {
        handle_error_en(error_num, "pthread_cancel msg_processing_thread failed.");
    }
}