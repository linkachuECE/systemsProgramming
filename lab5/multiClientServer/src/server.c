/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	This is used to set up the server before splitting it into a parent
 *                      which handles the server socket, and children which handles individual client connections
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

// Defaults and maxes
#define PORT 4444
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// Initial server setup
int serverSetup(int port){
    struct sockaddr_in server_addr;

    int server_fd;

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

int main() {
    int server_fd = serverSetup(PORT);

    runServer(server_fd);
}
