/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	The purpose of the functions in this file is to allow 
 *                      the client and server to communicate seamlessly while maintaining
 *                      a record of the previous chat messages displayed on screen
 *                      
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

// Socket file descriptor
int chat_fd;

// Mechanisms for thread managements
int screenRendered = 0;
sem_t promptSem;

// Variables for file managements
char* username;
char* logfileName;
FILE* logfp;

// Maximum lengths
#define MAXUSERNAMELENGTH 50
#define BUFFER_SIZE 1024

// Signal handler for closing the chat
void closeChat(int signal){
    // Final display
    system("clear");
    printf("Chat ended\n");

    // Close all pertinent files, sockets, and semaphores, and free memory
    fclose(logfp);
    remove(logfileName);
    free(username);
    free(logfileName);
    close(chat_fd);
    sem_close(&promptSem);
    exit(0);
}

// Open file to be used for storing chat history
void openLogFile(){
    // Determine log file name
    logfileName = (char*)malloc(sizeof(char) * MAXUSERNAMELENGTH + 7);
    strcpy(logfileName, username);
    strcat(logfileName, ".log");

    // Create and open log file
    logfp = fopen(logfileName, "a+");
}

// Records current string (sent or received) to the log file
void record(const char* input){
    fputs(input, logfp);
}

// Gather chat history from log file and output to the terminal
void outputChatLog(){
    char buff[BUFFER_SIZE];

    // Go to the beginning of the file
    fseek(logfp, 0, SEEK_SET);

    // Continually read and print to the terminal until the end of the file 
    while(fgets(buff, BUFFER_SIZE, (FILE*)logfp) != NULL)
        printf("%s", buff);

    // Flush the output
    fflush(stdout);

    // Return to the end of the file
    fseek(logfp, 0, SEEK_END);
}

// Print the user prompt
void printPrompt(){
    printf(">> ");
    fflush(stdout);
}

// Render the screen
void renderScreen(){
    system("clear");
    outputChatLog();
    printPrompt();
}

// Send information entered into the terminal to the recipient
void* sendTo(void* arg){
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];

    // Print prompt to begin with
    sem_wait(&promptSem);

    printPrompt();

    sem_post(&promptSem);

    while(1){      
        // Get information from user
        fgets(buffer, BUFFER_SIZE, stdin);

        // Close the terminal if specified
        if(strcmp("Close", buffer) == 0)
            break;

        // Construct message
        strcpy(message, username);
        strcat(message, ": ");
        strcat(message, buffer);

        // Record message to log file and render screen
        sem_wait(&promptSem);

        record(message);
        renderScreen();

        sem_post(&promptSem);

        // Send message to the client
        send(chat_fd, message, strlen(message), 0);
    }

    closeChat(0);

    return NULL;
}

// Receive information from the recipient and output it to the console
void* recvFrom(void* arg){
    while(1){
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);

        // Receive a string into the buffer
        int bytes_received = recv(chat_fd, buffer, BUFFER_SIZE, 0);
        
        // Break if the connection was closed
        if (bytes_received <= 0) {
            printf("Connection closed by other user.\n");
            break;
        }

        // Record the string received to the log file and re-render the screen
        sem_wait(&promptSem);

        record(buffer);
        renderScreen();

        sem_post(&promptSem);
    }

    closeChat(0);

    return NULL;
}

// Initialize chat functionality
void chat(int cfd, const char* uname){
    // Establish signal handlers
    signal(SIGTERM, closeChat);
    signal(SIGKILL, closeChat);
    signal(SIGINT, closeChat);

    // Set global variables
    chat_fd = cfd;
    username = (char*)malloc(sizeof(char) * MAXUSERNAMELENGTH);
    strcpy(username, uname);

    // Create the log file
    openLogFile();

    // Structure for threads
    pthread_t send_id, receive_id;

    // Initialize a semaphore    
    sem_init(&promptSem, 0, 1);

    // Create two threads, one for sending data, and one for receiving it
    pthread_create(&send_id, NULL, sendTo, NULL);
    pthread_create(&receive_id, NULL, recvFrom, NULL);

    // Wait for both threads to conclude before exiting
    pthread_join(send_id, NULL);
    pthread_join(receive_id, NULL);
    
    // Close threads
    pthread_exit(&send_id);
    pthread_exit(&receive_id);

    // Close semaphore
    sem_close(&promptSem);
}