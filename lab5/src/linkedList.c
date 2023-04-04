#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "linkedList.h"
#include "bool.h"

//display the list
void printList(node* head) {
   printf("Current clients:\n");
   printf("%-8s %10s %8s\n", "Process", "IP", "Port");
   node *ptr = head;
	
   //start from the beginning
   while(ptr != NULL) {
      printf("%-8d %10s %8d\n", ptr->pid, ptr->ip, ptr->port);
      ptr = ptr->next;
   }
	
}

//insert link at the first location
node* insertFirst(int pid, int port, char* ip, node* head) {
   //create a link
   node *link = (struct node*) malloc(sizeof(struct node));
	
   link->pid = pid;
   link->port = port;
   strcpy(link->ip, ip);
	
   //point it to old first node
   link->next = head;
	
   //point first to new first node
   head = link;

   return head;
}

//delete first item
node* deleteFirst(node* head) {

   //save reference to first link
   struct node *tempLink = head;
	
   //mark next to first link as first 
   head = head->next;
	
   //return the deleted link
   return tempLink;
}

//is list empty
bool isEmpty(node* head) {
   return (head == NULL);
}

int length(node* head) {
   int length = 0;
   struct node *current;
	
   for(current = head; current != NULL; current = current->next) {
      length++;
   }
	
   return length;
}

//find a link with given pid
node* find(int pid, node* head) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->pid != pid) {
	
      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //go to next link
         current = current->next;
      }
   }      
	
   //if port found, return the current Link
   return current;
}

//delete a link with given pid
node* deleteNode(int pid, node* head) {

   //start from the first link
   node* current = head;
   node* previous = NULL;
	
   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->pid != pid) {

      //if it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         //store reference to current link
         previous = current;
         //move to next link
         current = current->next;
      }
   }

   //found a match, update the link
   if(current == head) {
      //change first to point to next link
      head = head->next;
   } else {
      //bypass the current link
      previous->next = current->next;
   }    
	
   return head;
}

void sort(node* head) {

   int i, j, k, tempPid, tempData;
   node *current;
   node *next;

   char tempBuf[50];
	
   int size = length(head);
   k = size ;
	
   for ( i = 0 ; i < size - 1 ; i++, k-- ) {
      current = head;
      next = head->next;
		
      for ( j = 1 ; j < k ; j++ ) {   

         if ( current->port > next->port ) {
            tempData = current->port;
            current->port = next->port;
            next->port = tempData;

            strcpy(tempBuf, current->ip);
            strcpy(current->ip, next->ip);
            strcpy(next->ip, tempBuf);

            tempPid = current->pid;
            current->pid = next->pid;
            next->pid = tempPid;
         }
			
         current = current->next;
         next = next->next;
      }
   }   
}