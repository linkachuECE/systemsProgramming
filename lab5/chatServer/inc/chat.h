/*
 *      AUTHOR: 		Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	This is the header file for the chat room
 *                      
 *      CONTRIBUTORS: 	None.
 */

#ifndef CHAT_H
#define CHAT_H

void closeChat(int signal);

void openLogFile();
void record(const char* input);
void outputChatLog();
void printPrompt();
void renderScreen();

void* sendTo(void* arg);
void* recvFrom(void* arg);

void chat(int cfd, const char* username);

#endif