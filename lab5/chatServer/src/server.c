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
#include <pthread.h>

#define PORT 4444
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int server_fd, client_fd;

int serverSetup(int port){
    struct sockaddr_in server_addr;

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // Bind the socket to the server address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 1) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n\n", port);

    return server_fd;
}

void* sendTo(void* arg){
    while(1){
        char buffer[BUFFER_SIZE];
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if(strcmp("Close", buffer) == 0)
            break;

        // Send message to the server
        send(server_fd, buffer, strlen(buffer), 0);
    }

    return NULL;
}

void* recvFrom(void* arg){
    while(1){
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        int bytes_received = recv(server_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Connection closed by server.\n");
            break;
        }

        printf("Server: %s\n", buffer);
    }

    return NULL;
}

int main() {
    serverSetup(PORT);

    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    pthread_t send_id, receive_id;

    pthread_create(&send_id, NULL, sendTo, NULL);
    pthread_create(&receive_id, NULL, recvFrom, NULL);

    pthread_join(send_id, NULL);
    pthread_join(receive_id, NULL);

}
