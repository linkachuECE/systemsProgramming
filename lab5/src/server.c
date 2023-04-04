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

#include "linkedList.h"

#define PORT 4444
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int server_fd = -1;
int client_fd = -1;

node* childIDs = NULL;
int clientCount = 0;

void closeServer(int signal){
    for(node* curr = childIDs; curr != NULL; curr = curr->next){
        kill(curr->pid, SIGTERM);
    }

    printf("Server terminated\n");
    if (server_fd != -1)
        close(server_fd);

    exit(0);
}

void closeChild(int signal){
    printf("Child process %d terminated\n", getpid());
    if(client_fd != -1)
        close(client_fd);
    exit(0);
}

void clientCloseHandle(int signal){
    int status;
    int childPID = wait(&status);

    childIDs = deleteNode(childPID, childIDs);

    printf("\n");
    printList(childIDs);

    if (isEmpty(childIDs)){
        printf("All clients have terminated connection\n");
        closeServer(0);
    }
}

int clientChild(struct sockaddr_in* client_addr, int client_fd){
    int serverChildID = getpid();
    char buffer[BUFFER_SIZE];
    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port));

    // Echo loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Connection closed by client.\n");
            break;
        }

        printf("Received from %s:%d: %s\n", inet_ntoa(client_addr->sin_addr), ntohs(client_addr->sin_port), buffer);
        send(client_fd, buffer, bytes_received, 0);
    }

    printf("Server child %d now exiting...\n", serverChildID);
    close(client_fd);
    exit(0);
}

int main() {
    signal(SIGINT, closeServer);
    signal(SIGTERM, closeServer);
    signal(SIGCHLD, clientCloseHandle);

    int addr_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

    int child_id;

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

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

    printf("Server listening on port %d...\n", PORT);

    addr_len = sizeof(client_addr);

    while(1){
        // Accept an incoming connection
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        if((child_id = fork()) == 0){
            signal(SIGINT, closeChild);
            signal(SIGTERM, closeChild);
            clientChild(&client_addr, client_fd);
        } else {    
            printf("Launched server child %d\n", child_id);
            childIDs = insertFirst(child_id, client_addr.sin_port, inet_ntoa(client_addr.sin_addr), childIDs);

            printList(childIDs);
        }
    }

    return 0;
}
