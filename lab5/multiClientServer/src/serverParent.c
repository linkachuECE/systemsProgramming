/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	The functions in this file are used to manage the server socket and
 *                      to spawn children when a client connection arrives
 *      CONTRIBUTORS: 	None
 */

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

// Linked list to keep track of client data
node* childIDs = NULL;
int clientCount = 0;

// Server socket file descriptor
int server_fd = -1;

// Signal handler to close the server
void closeServer(int signal){
    // Kill each child in successiono
    for(node* curr = childIDs; curr != NULL; curr = curr->next)
        kill(curr->pid, SIGTERM);

    // Free the memory for the remaining nodes in the list
    deleteList(childIDs);

    printf("Server terminated\n");
    if (server_fd != -1)
        close(server_fd);

    exit(0);
}

// Handle death of a single child
void clientCloseHandle(int signal){
    // Get PID of child
    int status;
    int childPID = wait(&status);

    childIDs = deleteNode(childPID, childIDs);

    // Print rest of child processes
    printf("\n");
    if(childIDs){
        usleep(1000);
        printList(childIDs);
        printf("\n");
    }

    // Close the server if all of the child processes have exited
    if (isEmpty(childIDs)){
        printf("All clients have terminated connection\n");
        closeServer(0);
    }
}

// Establish and launch the server
void runServer(int sfd){
    // Signal handlers
    signal(SIGINT, closeServer);
    signal(SIGTERM, closeServer);
    signal(SIGCHLD, clientCloseHandle);

    // File descriptors
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
        // Fork the process to allow the child to handle the client
        if((child_id = fork()) == 0){
            signal(SIGINT, closeChild);
            signal(SIGTERM, closeChild);
            clientChild(&client_addr, client_fd);
        } else {    
            printf("Launched server child %d\n", child_id);
            // Store information of child process and client server
            childIDs = insertFirst(child_id, client_addr.sin_port, inet_ntoa(client_addr.sin_addr), childIDs);
            
            usleep(1000);
            printList(childIDs);
            printf("\n");
        }
    }
}