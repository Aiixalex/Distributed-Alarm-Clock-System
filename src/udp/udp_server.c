#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#include "udp_client.h"
#include "../msg_processing/msg_processing.h"
#include "../utils/handle_error.h"
#include "../msg_queue/list.h"
#include "../msg_queue/message_queue.h"

#define MAXBUFSIZE 4096

static pthread_t udp_server_thread;
static char* server_port;

void* UdpRecv(void* message_queue) {
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    struct sockaddr_in peer_addr;
    socklen_t peer_addr_len;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;

    int error_num = getaddrinfo(NULL, server_port, &hints, &result);
    if (error_num != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error_num));
        exit(EXIT_FAILURE);
    }

    printf("%d\n", result == NULL);

    // getaddrinfo() returns a list of address structures.
    // Try each address until we successfully bind(2).
    // If socket(2) (or bind(2)) fails, we (close the socket
    // and) try the next address.

    int sfd;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */

        close(sfd);
    }

    freeaddrinfo(result);           /* No longer needed */

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    while (1) {
        char buf[MAXBUFSIZE];
        memset(&buf, 0, MAXBUFSIZE);

        peer_addr_len = sizeof(struct sockaddr_storage);
        int buf_size = recvfrom(sfd, buf, MAXBUFSIZE, 0, (struct sockaddr *)&peer_addr, &peer_addr_len);

        if (buf_size == -1) {
            handle_error("recvfrom() failed");
        }

        char* message = malloc((buf_size + 1) * sizeof(char));
        strncpy(message, buf, buf_size); // a null byte is written by strncpy() to message[buf_size]

        if (MessageEnqueue(message_queue, message) == LIST_FAIL) {
            fprintf(stderr, "message enqueue failed: %s", message);
        }

        // if (strcmp(message, "!\n") == 0) {
        //     SignalMsgProcessing();
        //     CancelInputReader();
        //     CancelUdpClient();
        //     close(sfd);
        //     return NULL;
        // }

        SignalMsgProcessing();
    }

    return NULL;
}

void InitUdpServer(List* message_queue, char* my_port) {
    server_port = my_port;

    int error_num = pthread_create(&udp_server_thread, NULL, &UdpRecv, message_queue);
    if (error_num != 0) {
        handle_error_en(error_num, "pthread_create udp_server_thread failed");
    }
}

void ShutDownUdpServer() {
    int error_num = pthread_join(udp_server_thread, NULL);
    if (error_num != 0) {
        handle_error_en(error_num, "pthread_join udp_server_thread failed.");
    }
}

void CancelUdpServer() {
    int error_num = pthread_cancel(udp_server_thread);
    if (error_num != 0) {
        handle_error_en(error_num, "pthread_cancel udp_server_thread failed.");
    }
}
