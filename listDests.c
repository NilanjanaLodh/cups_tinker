#include <stdio.h>
#include "myLibrary.h"
int main()
{
    /** 
    calling and testing all functions :- 
  **/

    //listCupsQueues();
    //listLocalQueues();

    dest_list_t local_dests = {0, NULL};
    dest_list_t all_dests = {0, NULL};

    printf("Adding Local destinations ..");
    fflush(stdout);
    addAllQueues(&all_dests);
    printf("done.\n");

    printf("Adding all destinations ..");
    fflush(stdout);
    addLocalQueues(&local_dests);
    printf("done.\n");

    printf("Local Queues :-\n");
    printDestList(&local_dests);

    printf("\n\n\n");

    printf("Common Queues :-\n");
    printDestList(&all_dests);

    cupsFreeDests(local_dests.num_dests, local_dests.dests);
    cupsFreeDests(all_dests.num_dests, all_dests.dests);
    
}