#ifndef SERVERCHILD_H
#define SERVERCHILD_H

void closeChild(int signal);

int clientChild(struct sockaddr_in* client_addr, int cfd);

#endif