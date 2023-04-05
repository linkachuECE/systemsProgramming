#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int client_fd = 0;

void closeChild(int signal){
    printf("Child process %d terminated\n\n", getpid());
    if(client_fd != -1)
        close(client_fd);
    exit(0);
}

int clientChild(struct sockaddr_in* client_addr, int cfd){
    client_fd = cfd;
    int serverChildID = getpid();
    char buffer[BUFFER_SIZE];

    printf("Connection accepted from %s:%d\n\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

    // Echo loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Connection closed from %s:%d\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));
            break;
        }

        printf("Received from %s:%d: %s\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port), buffer);
        send(client_fd, buffer, bytes_received, 0);
    }

    printf("Server child %d now exiting...\n", serverChildID);
    close(client_fd);
    exit(0);
}