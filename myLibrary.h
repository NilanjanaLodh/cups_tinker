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

int addDestToList(dest_list_t *user_data , unsigned flags , cups_dest_t *dest);  /** call back function **/
int printDest(void *user_data , unsigned flags , cups_dest_t *dest); /**call back function **/
void listCupsQueues(); /** print all cups local, and remote queues (hardwired + tmporary) **/
void listLocalQueues(); /**print only the local cups queues **/
void addAllQueues(dest_list_t *arr); /** Adds all destinations(local + remote), (hardwired + temporary) to the destination list **/
void addLocalQueues(dest_list_t *arr);/** Adds local destinations to the destination list **/
void printDestList(dest_list_t *arr);/** Print all the destinations in the list **/
void printUserDefaultOptions(char *name); /** print User defaults for a particular printer **/
void printDestDefaultOptions(char *name); /** print destination defaults for a particular printer **/
void listSupportedOptions(char *name); /** Print list of supported(customizable) options for the particular destination **/
void printSupportedValues(char *name);/** Supposed to print all the supported (Ready) Values for each option But thos doesnt really work :( ) **/
void testPrintJob(char *filename, char *dname); /** submit a job with sample options **/
void testPrintJob2(char *filename, char *dname); /**same as above but streams the data instead **/

#endif // MYLIB_H_