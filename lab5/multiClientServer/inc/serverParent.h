#ifndef SERVERPARENT_H
#define SERVERPARENT_H

void closeServer(int signal);
void closeChild(int signal);

void clientCloseHandle(int signal);
void runServer(int sfd);

#endif