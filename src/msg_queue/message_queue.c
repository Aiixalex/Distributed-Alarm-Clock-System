#include <pthread.h>

#include "list.h"

static pthread_mutex_t message_queue_mutex = PTHREAD_MUTEX_INITIALIZER;

int MessageEnqueue(List* list, char* message) {
    pthread_mutex_lock(&message_queue_mutex);

    int prepend_result = List_prepend(list, message);

    pthread_mutex_unlock(&message_queue_mutex);

    return prepend_result;
}

char* MessageDequeue(List* list) {
    pthread_mutex_lock(&message_queue_mutex);

    char* dequeued_message = List_trim(list);

    pthread_mutex_unlock(&message_queue_mutex);

    return dequeued_message;
}

int GetMessageQueueSize(List* list) {
    pthread_mutex_lock(&message_queue_mutex);

    int queue_size = List_count(list);

    pthread_mutex_unlock(&message_queue_mutex);

    return queue_size;
}