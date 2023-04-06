/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	The purpose of the functions in this file is to setup a 
 *                      server socket before launching the chat room
 *      CONTRIBUTORS: 	None.
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
#include <pthread.h>
#include <semaphore.h>
#include <getopt.h>

#include "chat.h"

// Default values
#define DEFAULTPORT         4444
#define DEFAULTIP           "0.0.0.0"
#define DEFAULTUSERNAME     "Server"

// Maximum sizes
#define BUFFER_SIZE         1024
#define MAXUSERNAMELENGTH   50

// Socket file descriptors
int server_fd, client_fd;


// Function to set up the server
int serverSetup(int port, char* ip){
    struct sockaddr_in server_addr;

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &(server_addr.sin_addr));
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

    printf("Server listening on port %d...\n", port);

    return server_fd;
}

// Displays information and handles arguments before setting up and launching the server
int main(int argc, char* argv[]) {
    if(argc > 1 && strcmp(argv[1], "--help") == 0){
        printf("chatServer: %s [-u <username>] [-p <port>] [-i <IP_address>]\n\
\tCreate a server chat room to communicate with a client\n\
\tOptions:\n\
\t\t-i <IP_address>\tSpecify an IP address to listen for (default: 0.0.0.0)\n\
\t\t-p <port>\tSpecify a port number to listen on (default: 4444)\n\
\t\t-u <username>\tSpecify a username to use (default: Server)\n", argv[0]);
        exit(0);
    }

    // Set values to the default to start
    int port = DEFAULTPORT;
    char username[MAXUSERNAMELENGTH] = DEFAULTUSERNAME;
    char ip[50] = DEFAULTIP;
    
    // Process arguments and option flags
    int opt;
    while((opt = getopt(argc, argv, ":u:U:p:P:i:I:")) != -1) { 
        switch(opt) {
            // Username
            case 'u':
            case 'U':
                strcpy(username, optarg);
                break;
            // Port number
            case 'p':
            case 'P': 
                port = atoi(optarg);
                break;
            // IP address
            case 'i':
            case 'I':
                strcpy(ip, optarg);
                break; 
            case '?': 
                printf("unknown option: %c\n", optopt);
                break; 
        }
    }

    // Set up server
    serverSetup(port, ip);

    // Structure for client connection
    struct sockaddr_in client_addr;
    int addr_len = sizeof(client_addr);

    // Accept connection from client
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Extract information from client structure
    char* clientIP = inet_ntoa(client_addr.sin_addr);
    int clientPort = client_addr.sin_port;

    // Initialize chat room and display information
    system("clear");
    printf("Username: %s\n", username);
    printf("Connected to client %s:%d\n", clientIP, clientPort);

    // Start chat
    chat(client_fd, username);

    // Close sockets
    close(client_fd);
    close(server_fd);
}
