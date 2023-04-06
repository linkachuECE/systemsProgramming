/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	The purpose of this program is to establish a client socket to
 *                      connect to an echo server which will parrot whatever data it is sent
 *      CONTRIBUTORS: 	None.
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Defaults
#define PORT 4444

// Max sizes
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    // Establish usage
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_ip>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Socket file descriptor, socket structure, and data buffer
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

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

    while (1) {
        printf("Enter message: ");
        fgets(buffer, BUFFER_SIZE, stdin);

        // Send message to the server
        send(client_fd, buffer, strlen(buffer), 0);

        // Receive echo from the server
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Connection closed by server.\n");
            break;
        }

        printf("Server echo: %s", buffer);
    }

    // Close socket
    close(client_fd);

    return 0;
}
