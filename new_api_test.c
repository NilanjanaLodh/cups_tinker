/**
Write functions using the new CUPS API for

  o Listing available CUPS queues

  o Of a chosen printer list all options, choices, and default settings, and also important capabilities (like duplex, color, ...)

  o Print a job wit the settings the user has chosen

**/
#include <cups/cups.h>
#include <stdio.h>

#define _CUPS_NO_DEPRECATED 1 

int print_dest(void *user_data , unsigned flags , cups_dest_t *dest); 
void listCupsQueues();

int main()
{
  listCupsQueues();
}


/**
  CallBack function for cupsEnumDests()
  Prints the destination name(printer or class)
**/
int print_dest(void *user_data, unsigned flags, cups_dest_t *dest)
{
  if (dest->instance)
    printf("%s/%s\n", dest->name, dest->instance);
  else
    puts(dest->name);

  return (1);
}


/**
  Lists all available CUPS queues (hardwired + local)
**/
void listCupsQueues()
{
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, print_dest, NULL);
  /**
  Just for my later reference :
  function prototype:
  int
  cupsEnumDests(unsigned flags, int msec, int *cancel,
                cups_ptype_t type, cups_ptype_t mask,
                cups_dest_cb_t cb, void *user_data)
  **/
}

