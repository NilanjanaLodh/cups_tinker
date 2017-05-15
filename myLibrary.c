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

void printUserDefaultOptions(char *name)
{
  dest_list_t list = {0, NULL};
  addAllQueues(&list);
  cups_dest_t *dest;
  dest = cupsGetDest(name, NULL, list.num_dests, list.dests);
  if (dest == NULL)
  {
    printf("Invalid name/Destination not available.\n");
    return;
  }

  printf("%d options set for destination %s.\n", dest->num_options, dest->name);
  int i;
  cups_option_t *opt = dest->options;
  for (i = 0; i < dest->num_options; i++, opt++)
  {
    printf("%s : %s\n", opt->name, opt->value);
  }
}

void printDestDefaultOptions(char *name)
{
  dest_list_t list = {0, NULL};
  addAllQueues(&list);
  cups_dest_t *dest;
  dest = cupsGetDest(name, NULL, list.num_dests, list.dests);
  if (dest == NULL)
  {
    printf("Invalid name/Destination not available.\n");
    return;
  }

  http_t *http;
  if ((http = cupsConnectDest(dest, CUPS_DEST_FLAGS_NONE, 30000, NULL, NULL, 0, NULL, NULL)) == NULL)
  {
    printf("Unable to connect to destination.\n");
    return;
  }

  cups_dinfo_t *dinfo = cupsCopyDestInfo(http, dest);
  if (dinfo == NULL)
  {
    printf("Unable to connect to destination\n");
    return;
  }
  ipp_attribute_t *attrs =
      cupsFindDestSupported(http, dest, dinfo,
                            "job-creation-attributes");

  int i, num_option = ippGetCount(attrs);
  char *opt, *val;
  ipp_attribute_t *opt_attr;
  for (i = 0; i < num_option; i++)
  {
    opt = ippGetString(attrs, i, NULL);
    opt_attr = cupsFindDestDefault(http, dest, dinfo,
                                   opt);
    val = ippGetString(opt_attr, 0, NULL);
    if (val)
      printf("%s : %s\n", opt, val);
    else
      printf("%s : %d\n", opt, ippGetInteger(opt_attr, 0));
  }
}

void printSupportedValues(char *name)
{

  dest_list_t list = {0, NULL};
  addAllQueues(&list);
  cups_dest_t *dest;
  dest = cupsGetDest(name, NULL, list.num_dests, list.dests);
  if (dest == NULL)
  {
    printf("Invalid name/Destination not available.\n");
    return;
  }

  http_t *http;
  if ((http = cupsConnectDest(dest, CUPS_DEST_FLAGS_NONE, 30000, NULL, NULL, 0, NULL, NULL)) == NULL)
  {
    printf("Unable to connect to destination.\n");
    return;
  }

  cups_dinfo_t *dinfo = cupsCopyDestInfo(http, dest);
  if (dinfo == NULL)
  {
    printf("Unable to connect to destination\n");
    return;
  }
  ipp_attribute_t *attrs =
      cupsFindDestSupported(http, dest, dinfo,
                            "job-creation-attributes");

  int i, j, num_option = ippGetCount(attrs);
  char *opt, *val;
  int num_vals;
  ipp_attribute_t *opt_attr;
  for (i = 0; i < num_option; i++)
  {
    opt = ippGetString(attrs, i, NULL);
    opt_attr = cupsFindDestReady(http, dest, dinfo,
                                 opt); ///THIS DOES NOT WORK! // all values reported as 0 :(

    num_vals = ippGetCount(opt_attr);

    if (num_vals > 0)
    {

      printf("%s : ", opt);

      for (j = 0; j < num_vals; j++)
      {
        val = ippGetString(opt_attr, j, NULL);
        if (val)
          printf(" %s ", val);

        else
          printf(" %d ", ippGetInteger(opt_attr, j));
      }
      printf("\n");
    }
  }
}

void listSupportedOptions(char *name)
{
  /**just a test function **/
  dest_list_t list = {0, NULL};
  addAllQueues(&list);
  cups_dest_t *dest;
  dest = cupsGetDest(name, NULL, list.num_dests, list.dests);
  if (dest == NULL)
  {
    printf("Invalid name/Destination not available.\n");
    return;
  }

  http_t *http;
  if ((http = cupsConnectDest(dest, CUPS_DEST_FLAGS_NONE, 30000, NULL, NULL, 0, NULL, NULL)) == NULL)
  {
    printf("Unable to connect to destination.\n");
    return;
  }

  cups_dinfo_t *dinfo = cupsCopyDestInfo(http, dest);
  if (dinfo == NULL)
  {
    printf("Unable to connect to destination\n");
    return;
  }

  printf("Supported options : \n");
  ipp_attribute_t *attrs =
      cupsFindDestSupported(http, dest, dinfo,
                            "job-creation-attributes");
  int i, count = ippGetCount(attrs);
  printf("No. of supported options is %d.\n", count);

  for (i = 0; i < count; i++)
    puts(ippGetString(attrs, i, NULL));
}

void testPrintJob(char *filename, char *dname)
{
  //submitting an example printjob with some sample options
  cups_dest_t *dest = cupsGetNamedDest(CUPS_HTTP_DEFAULT, dname, NULL);
  if (dest == NULL)
  {
    printf("Failure :(\n");
    return;
  }
  http_t *http;
  if ((http = cupsConnectDest(dest, CUPS_DEST_FLAGS_NONE, 30000, NULL, NULL, 0, NULL, NULL)) == NULL)
  {
    printf("Unable to connect to destination.\n");
    return;
  }

  cups_dinfo_t *dinfo = cupsCopyDestInfo(http, dest);
  if (dinfo == NULL)
  {
    printf("Unable to connect to destination\n");
    return;
  }

  int job_id = 0;
  int num_options = 0;
  cups_option_t *options = NULL;

  num_options = cupsAddOption(CUPS_COPIES, "1",
                              num_options, &options);

  num_options = cupsAddOption(CUPS_MEDIA, CUPS_MEDIA_A4,
                              num_options, &options);

  job_id = cupsPrintFile2(http, dname , filename , filename , num_options, options);
  if(job_id)
  {
    printf("Job %d submitted succesfully.\n", job_id);
  }  
  else
  {
    printf("Error creating job.");
  }
}


void testPrintJob2(char *filename, char *dname)
{
  //submitting an example printjob with some sample options
  //Printing by streaming the data using cupsWriteRequestData()
  cups_dest_t *dest = cupsGetNamedDest(CUPS_HTTP_DEFAULT, dname, NULL);
  if (dest == NULL)
  {
    printf("Failure :(\n");
    return;
  }
  http_t *http;
  if ((http = cupsConnectDest(dest, CUPS_DEST_FLAGS_NONE, 30000, NULL, NULL, 0, NULL, NULL)) == NULL)
  {
    printf("Unable to connect to destination.\n");
    return;
  }

  cups_dinfo_t *dinfo = cupsCopyDestInfo(http, dest);
  if (dinfo == NULL)
  {
    printf("Unable to connect to destination\n");
    return;
  }

  int job_id = 0;
  int num_options = 0;
  cups_option_t *options = NULL;

  num_options = cupsAddOption(CUPS_COPIES, "2",
                              num_options, &options);

  num_options = cupsAddOption(CUPS_MEDIA, CUPS_MEDIA_5X7,
                              num_options, &options);
  if (cupsCreateDestJob(http, dest, dinfo,
                        &job_id, "printing from myLibrary", num_options,
                        options) == IPP_STATUS_OK)
    printf("Created job: %d\n", job_id);
  else
  {
    printf("Unable to create job: %s\n",
           cupsLastErrorString());
    return;
  }

  FILE *fp = fopen(filename, "rb");
  size_t bytes;
  char buffer[65536];

  if(cupsStartDestDocument(http, dest, dinfo, job_id, filename, CUPS_FORMAT_AUTO, num_options,options, 1) == HTTP_STATUS_CONTINUE)
  {
    while ((bytes = fread(buffer, 1, sizeof(buffer), fp)) > 0)
    if (cupsWriteRequestData(http, buffer,
                             bytes) != HTTP_STATUS_CONTINUE)
      break;

  if (cupsFinishDestDocument(http, dest,
                             dinfo) == IPP_STATUS_OK)
    puts("Document send succeeded.");
  else
    printf("Document send failed: %s\n",
           cupsLastErrorString());
  }

  fclose(fp);
}
