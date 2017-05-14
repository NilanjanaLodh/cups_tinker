/**
Write functions using the new CUPS API for

  o Listing available CUPS queues

  o Of a chosen printer list all options, choices, and default settings, and also important capabilities (like duplex, color, ...)

  o Print a job wit the settings the user has chosen

**/

#ifndef MYLIB_H_  
#define MYLIB_H_

#include <cups/cups.h>
#include <stdio.h>

#define _CUPS_NO_DEPRECATED 1 

typedef struct
{
  int num_dests;
  cups_dest_t *dests;
} dest_list_t;

int addDestToList(dest_list_t *user_data , unsigned flags , cups_dest_t *dest); 
int printDest(void *user_data , unsigned flags , cups_dest_t *dest); 
void listCupsQueues();
void listLocalQueues();
void addAllQueues(dest_list_t *arr);
void addLocalQueues(dest_list_t *arr);
void printDestList(dest_list_t *arr);
void printOptions(char *name);

#endif // MYLIB_H_