#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include "../utils/handle_error.h"
#include "../msg_queue/list.h"
#include "../msg_queue/message_queue.h"


static pthread_t udp_client_thread;
static char* server_name;
static char* server_port;
static pthread_mutex_t udp_client_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t udp_client_cond = PTHREAD_COND_INITIALIZER;

void* UdpSendto(void* message_queue) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int error_num = getaddrinfo(server_name, server_port, &hints, &result);
    if (error_num != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error_num));
        exit(EXIT_FAILURE);
    }

    // getaddrinfo() returns a list of address structures.
    // Try each address until we successfully connect(2).
    // If socket(2) (or connect(2)) fails, we (close the socket
    // and) try the next address.

    int sfd;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1) {
            continue;
        }

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1) {
            break;                  /* Success */
        }

        close(sfd);
    }

    freeaddrinfo(result);           /* No longer needed */

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        pthread_mutex_lock(&udp_client_mutex);
        pthread_cond_wait(&udp_client_cond, &udp_client_mutex);
        pthread_mutex_unlock(&udp_client_mutex);

        char* message = MessageDequeue(message_queue);
        if (message == NULL) {
            fprintf(stderr, "message dequeue failed.");
            continue;
        }

        int buf_size = sendto(sfd, message, strlen(message), 0, rp->ai_addr, rp->ai_addrlen);
        if (buf_size == -1) {
            handle_error("sendto() failed");
        } else if (buf_size != strlen(message)) {
            fprintf(stderr, "sendto() partial failed.\n");
            exit(EXIT_FAILURE);
        }

        // if (strcmp(message, "!\n") == 0) {
        //     free(message);
        //     close(sfd);
        //     return NULL;
        // }

        free(message);
    }

    return NULL;
}

void SignalUdpClient() {
    pthread_mutex_lock(&udp_client_mutex);

    pthread_cond_signal(&udp_client_cond);

    pthread_mutex_unlock(&udp_client_mutex);
}

void InitUdpClient(List* message_queue, char* remote_name, char* remote_port) {
    server_name = remote_name;
    server_port = remote_port;

    int error_num = pthread_create(&udp_client_thread, NULL, &UdpSendto, message_queue);
    if (error_num != 0) {
        handle_error_en(error_num, "pthread_create udp_client_thread failed");
    }
}

void ShutDownUdpClient() {
    int error_num = pthread_join(udp_client_thread, NULL);
    if (error_num != 0) {
        handle_error_en(error_num, "pthread_join udp_client_thread failed.");
    }
}

// void CancelUdpClient() {
//     int error_num = pthread_cancel(udp_client_thread);
//     if (error_num != 0) {
//         handle_error_en(error_num, "pthread_cancel udp_client_thread failed.");
//     }
// }