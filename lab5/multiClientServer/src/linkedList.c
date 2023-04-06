/*
 *      AUTHOR: 		   Ethan Braun
 *      DATE CREATED: 	04/05/23
 *      DESCRIPTION: 	This is a linked list data structure used to keep track of client connections
 *      CONTRIBUTORS: 	Copied from https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm and tweaked
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#include "linkedList.h"
#include "bool.h"

// Display the list
void printList(node* head) {
   printf("Current clients:\n");
   printf("%-8s %10s %8s\n", "Process", "IP", "Port");
   node *ptr = head;
	
   // Start from the beginning
   while(ptr != NULL) {
      printf("%-8d %10s %8d\n", ptr->pid, ptr->ip, ptr->port);
      ptr = ptr->next;
   }
	
}

// Insert link at the first location
node* insertFirst(int pid, int port, char* ip, node* head) {
   // Create a link
   node *link = (struct node*) malloc(sizeof(struct node));
	
   link->pid = pid;
   link->port = port;
   strcpy(link->ip, ip);
	
   // Point it to old first node
   link->next = head;
	
   // Point first to new first node
   head = link;

   return head;
}

// Delete first item
node* deleteFirst(node* head) {

   // Save reference to first link
   struct node *tempLink = head;
	
   // Mark next to first link as first 
   head = head->next;
	
   // Return the deleted link
   return tempLink;
}

// Is list empty
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

// Find a link with given pid
node* find(int pid, node* head) {

   // Start from the first link
   struct node* current = head;

   // If list is empty
   if(head == NULL) {
      return NULL;
   }

   // Navigate through list
   while(current->pid != pid) {
	
      // If it is last node
      if(current->next == NULL) {
         return NULL;
      } else {
         // Go to next link
         current = current->next;
      }
   }      
	
   // If port found, return the current Link
   return current;
}

// Delete a link with given pid
node* deleteNode(int pid, node* head) {

   // Start from the first link
   node* current = head;
   node* previous = NULL;
	
   // If list is empty
   if(head == NULL) {
      return NULL;
   }

   // Navigate through list
   while(current->pid != pid){
      // If it is last node
      if(current->next == NULL){
         return NULL;
      } else {
         // Store reference to current link
         previous = current;
         // Move to next link
         current = current->next;
      }
   }

   // Found a match, update the link
   if(current == head){
      // Change first to point to next link
      head = head->next;
   } else {
      // Bypass the current link
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
	
   for(i = 0; i < size - 1; i++, k--){
      current = head;
      next = head->next;
		
      for (j = 1; j < k; j++){   

         if (current->port > next->port){
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

void deleteList(node* head){
   node *curr, *next;
   for(curr = head, next = curr->next; curr->next; curr = next, next = curr->next)
      free(curr);

   free(curr);
}