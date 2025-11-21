#include <errorManager.h>
#include <stdio.h>
#include <unistd.h>


void perrorAndExit(const char* s) {
    printf("\n");
    perror(s);
    exit(1);
}