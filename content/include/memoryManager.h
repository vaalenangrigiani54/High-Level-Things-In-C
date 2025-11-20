#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <moreTypes_advanced.h>


// Garbage collector is called automatically after finishing main() scope.

bool addToGarbageCollector(__ptr_t element, type_t type);

bool removeFromGarbageCollector(__ptr_t element);


/**
 * Wrapper for malloc() / calloc().
 * It adds automatically the allocated pointer to the garbage collector.
 */
bool allocate(__ptr_t* ptr, size_t bytes, bool setToZero);

/**
 * Wrapper for realloc().
 * It adds automatically the reallocated pointer to the garbage collector.
 */
bool reallocate(__ptr_t* ptr, size_t bytes);

/**
 * Wrapper for free().
 * It removes automatically the freed pointer from the garbage collector.
 */
void deallocate(__ptr_t ptr);


#endif // MEMORY_MANAGER_H