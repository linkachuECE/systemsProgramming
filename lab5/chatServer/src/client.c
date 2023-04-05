#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 4444
#define BUFFER_SIZE 1024

int client_fd;

void* sendTo(void* arg){
    while(1){
        char buffer[BUFFER_SIZE];
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        if(strcmp("Close", buffer) == 0)
            break;

        // Send message to the server
        send(client_fd, buffer, strlen(buffer), 0);
    }

    return NULL;
}

void* recvFrom(void* arg){
    while(1){
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Connection closed by server.\n");
            break;
        }

        printf("Server: %s\n", buffer);
    }
    
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    pthread_t send_id, receive_id;

    // Create a socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(PORT);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server %s:%d\n", argv[1], PORT);

    pthread_create(&send_id, NULL, sendTo, NULL);
    pthread_create(&receive_id, NULL, recvFrom, NULL);

    pthread_join(send_id, NULL);
    pthread_join(receive_id, NULL);

    printf("Server echo: %s", buffer);

    // Close socket
    close(client_fd);

    return 0;
}
