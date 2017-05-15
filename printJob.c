#include <stdio.h>
#include "myLibrary.h"

void usage();
int main(int argc , char **argv)
{

    testPrintJob(argv[1], argv[2]);
}

void usage()
{
    printf("./printJob <filename> <destination name>");
}