#include <stdio.h>
#include "myLibrary.h"

int main(int argc , char **argv)
{
    if(argc==2)
    {
        //printOptions(argv[1]);
        testOpt(argv[1]);
    }
    return 0;
}