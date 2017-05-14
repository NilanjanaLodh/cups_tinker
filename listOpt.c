#include <stdio.h>
#include "myLibrary.h"

int main(int argc , char **argv)
{
    if(argc==2)
    {
        printOptions(argv[1]);
    }
    else
    {
        printf("Usage: ./listOpt printername \
                       \nFlags :\
                       \n       [-d] (for default settings)\n");
    }
    return 0;
}