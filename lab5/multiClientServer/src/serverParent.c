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
#include "serverParent.h"
#include "serverChild.h"

node* childIDs = NULL;
int clientCount = 0;

int server_fd = -1;

void closeServer(int signal){
    for(node* curr = childIDs; curr != NULL; curr = curr->next){
        kill(curr->pid, SIGTERM);
    }

    printf("Server terminated\n");
    if (server_fd != -1)
        close(server_fd);

    exit(0);
}

void clientCloseHandle(int signal){
    int status;
    int childPID = wait(&status);

    childIDs = deleteNode(childPID, childIDs);

    printf("\n");
    if(childIDs){
        usleep(1000);
        printList(childIDs);
        printf("\n");
    }

    if (isEmpty(childIDs)){
        printf("All clients have terminated connection\n");
        closeServer(0);
    }
}

void runServer(int sfd){
    signal(SIGINT, closeServer);
    signal(SIGTERM, closeServer);
    signal(SIGCHLD, clientCloseHandle);

    server_fd = sfd;
    int client_fd, child_id;

    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

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
            
            usleep(1000);
            printList(childIDs);
            printf("\n");
        }
    }
}