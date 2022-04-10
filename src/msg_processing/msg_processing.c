#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include "../utils/handle_error.h"
#include "../msg_queue/list.h"
#include "../msg_queue/message_queue.h"

static pthread_t msg_processing_thread;
static pthread_mutex_t msg_processing_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t msg_processing_cond = PTHREAD_COND_INITIALIZER;

void process_message(List *message_queue, char* message)
{
    if (strcmp(message, "SCREEN_SHOW_PUZZLE") == 0)
    {
        MessageEnqueue(message_queue, "PUZZLE_ANSWER");
        printf("1\n");
    }
    else if (strcmp(message, "PUZZLE_ANSWER") == 0)
    {
        MessageEnqueue(message_queue, "ANSWER_CORRECT");
        printf("2\n");
    }
    else if (strcmp(message, "ANSWER_CORRECT") == 0)
    {
        printf("3\n");
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