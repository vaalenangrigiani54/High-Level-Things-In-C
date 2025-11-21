#ifndef ERROR_MANAGER_H
#define ERROR_MANAGER_H

#include <errno.h>

/**
 * Wrapper of `perror()`.
 * It exits the program with status code 1 after printing the error message.
 */
void perrorAndExit(const char* s);


#endif // ERROR_MANAGER_H