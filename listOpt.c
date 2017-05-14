#include <stdio.h>
#include "myLibrary.h"
void usage();

int main(int argc , char **argv)
{
    if(argc < 2) 
    {
        usage();
        return 0;
    }
    int i;
    if(argc==2)
    {
        printUserDefaultOptions(argv[1]);
    }
    for(i=2;i<argc;i++)
    {
        if(argv[i][0]!='-')
        {
            usage();
            break;
        }
        switch(argv[2][1])
        {
            case 's':
                listSupportedOptions(argv[1]);
                break;
            
            case 'd':
                printf("Destination default values :\n");
                printDestDefaultOptions(argv[1]);
                break;
            


        }
    }
    
    return 0;
}
void usage()
{
            printf("Usage: ./listOpt printername \
                       \nFlags(Only one at a time) :\
                       \n       [-s] (for list of supported options)\
                       \n       [-d] (for destination default settings)\n");
}