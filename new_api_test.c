/**
Write functions using the new CUPS API for

  o Listing available CUPS queues

  o Of a chosen printer list all options, choices, and default settings, and also important capabilities (like duplex, color, ...)

  o Print a job wit the settings the user has chosen

**/
#include <cups/cups.h>
#include <stdio.h>

#define _CUPS_NO_DEPRECATED 1 

typedef struct
{
  int num_dests;
  cups_dest_t *dests;
} dest_list_t;

int print_dest(void *user_data , unsigned flags , cups_dest_t *dest); 
void listCupsQueues();
void listLocalQueues();

int main()
{
  //listCupsQueues();
  listLocalQueues();
}





int addDestToList(dest_list_t *user_data, unsigned flags,
           cups_dest_t *dest)
{
  if (flags & CUPS_DEST_FLAGS_REMOVED)
  {
   /*
    * Remove destination from array...
    */

    user_data->num_dests =
        cupsRemoveDest(dest->name, dest->instance,
                       user_data->num_dests,
                       &(user_data->dests));
  }
  else
  {
   /*
    * Add destination to array...
    */
    user_data->num_dests =
        cupsCopyDest(dest, user_data->num_dests,
                     &(user_data->dests));
  }

  return (1);
}

/**
  Prints the destination name(printer or class)
  CallBack function for cupsEnumDests()
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
  Lists all available CUPS queues (hardwired + temporary)
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

void listLocalQueues()
{
  printf("Listing local Queues..\n");
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, CUPS_PRINTER_LOCAL, CUPS_PRINTER_REMOTE, print_dest, NULL);
}

