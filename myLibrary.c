#include "myLibrary.h"

/**
    CallBack function 
    adds the discovered printer to an array of destinations
    dest_list_t is a user-defined data type (see header file for definition)
**/
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
int printDest(void *user_data, unsigned flags, cups_dest_t *dest)
{
  if (dest->instance)
    printf("%s/%s\n", dest->name, dest->instance);
  else
    puts(dest->name);

  return (1);
}

/**
  Lists(Prints) all available CUPS queues (all local, remote , (hardwired as well as temporary) )
**/
void listCupsQueues()
{
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, printDest, NULL);
  /**
  Just for my later reference :
  function prototype:
  int
  cupsEnumDests(unsigned flags, int msec, int *cancel,
                cups_ptype_t type, cups_ptype_t mask,
                cups_dest_cb_t cb, void *user_data)
  **/
}

/**
  Lists(Prints) local CUPS queues 
**/
void listLocalQueues()
{
  printf("Listing local Queues..\n");
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, CUPS_PRINTER_LOCAL, CUPS_PRINTER_REMOTE, printDest, NULL);
}

/**
  Adds all local destinations to the destination list
**/
void addLocalQueues(dest_list_t *arr)
{
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, CUPS_PRINTER_LOCAL, CUPS_PRINTER_REMOTE, addDestToList, arr);
}

/**
  Adds all destinations(local + remote), (hardwired + temporary) to the destination list
**/
void addAllQueues(dest_list_t *arr)
{
  cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, NULL, 0, 0, addDestToList, arr);
}

/**
  Print the name of all the destinations in the list
**/
void printDestList(dest_list_t *arr)
{
  int i;
  printf("%d destinations in list.\n", arr->num_dests);
  cups_dest_t *dest = arr->dests;
  for (i = 0; i < arr->num_dests; i++, dest++)
  {
    if (dest->instance)
      printf("%s/%s\n", dest->name, dest->instance);
    else
      puts(dest->name);
  }
}

void printOptions(char *name)
{
  dest_list_t list = {0, NULL};
  addAllQueues(&list);
  cups_dest_t *dest = cupsGetDest(name, NULL, list.num_dests, &list.dests);
  if(dest==NULL)
  {
    printf("Invalid name/Destination not available.\n");
    return;
  }
  cups_dinfo_t *dest_info = cupsCopyDestInfo(CUPS_HTTP_DEFAULT, dest);
}
