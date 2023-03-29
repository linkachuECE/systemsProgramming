#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 4444
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd, addr_len;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];

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

    // Accept an incoming connection
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

    // Echo loop
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

        if (bytes_received <= 0) {
            printf("Connection closed by client.\n");
            break;
        }

        printf("Received: %s", buffer);
        send(client_fd, buffer, bytes_received, 0);
    }

    // Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
