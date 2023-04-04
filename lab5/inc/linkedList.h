#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "bool.h"
#include <arpa/inet.h>

typedef struct node {
   int port;
   int pid;
   char ip[50];
   struct node *next;
} node;

//display the list
void printList(node* head);
//insert link at the first location
node* insertFirst(int pid, int port, char* ip, node* head);

//delete first item
struct node* deleteFirst(node* head);

//is list empty
bool isEmpty(node* head);

int length(node* head);

//find a link with given PID
struct node* find(int PID, node* head);

//delete a link with given key
struct node* deleteNode(int PID, node* head);

void sort(node* head);

#endif