/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	The purpose of the functions in this file is to setup a 
 *                      client socket before launching the chat room
 *      CONTRIBUTORS: 	None.
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <getopt.h>

#include "chat.h"

// Default values
#define DEFAULTPORT         4444
#define DEFAULTIP           "127.0.0.1"
#define DEFAULTUSERNAME     "Client"

// Maximum sizes
#define BUFFER_SIZE         1024
#define MAXUSERNAMELENGTH   50

// Socket file descriptor
int client_fd;


// Function to set up the client
void clientSetup(char* ip, int port){
    struct sockaddr_in server_addr;

    // Create a socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ip);
    server_addr.sin_port = htons(port);

    // Connect to the server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }
    
}

// Displays information and handles arguments before setting up and launching the client
int main(int argc, char *argv[]) {
        if(argc > 1 && strcmp(argv[1], "--help") == 0){
        printf("chatClient: %s [-u <username>] [-p <port>] [-i <IP_address>]\n\
\tConnect to a server chat room to communicate with said server\n\
\tOptions:\n\
\t\t-i <IP_address>\tSpecify an IP address to connect to (default: 127.0.0.1)\n\
\t\t-p <port>\tSpecify a port number to connect to (default: 4444)\n\
\t\t-u <username>\tSpecify a username to use (default: Client)\n", argv[0]);
        exit(0);
    }

    // Set values to the default to start
    int port = DEFAULTPORT;
    char username[MAXUSERNAMELENGTH] = DEFAULTUSERNAME;
    char ip[50] = DEFAULTIP;
    
    // Process arguments and option flags
    int opt;
    while((opt = getopt(argc, argv, ":u:U:p:P:i:I:")) != -1){
        switch(opt){
            // Username
            case 'u':
            case 'U':
                strcpy(username, optarg);
                printf("Username: %s\n", username);
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

    // Set up client
    clientSetup(ip, port);

    // Initialize chat room and display information
    system("clear");
    printf("Username: %s\n", username);
    printf("Connected to server %s:%d\n", ip, port);

    // Start chat
    chat(client_fd, username);

    // Close socket
    close(client_fd);

    return 0;
}
