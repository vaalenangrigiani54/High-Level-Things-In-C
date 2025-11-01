#ifndef MORE_TYPES_H
#define MORE_TYPES_H

// To include more existing types
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>


/*====================================================================================================*/
/*------------------------------------------ FUNCTION TYPES ------------------------------------------*/
/*====================================================================================================*/

#ifndef T_FUNC_CLONE
    // Function type for allocators.
    typedef __ptr_t (*funcClone_t)(const __ptr_t);
    #define T_FUNC_CLONE funcClone_t
#endif

#ifndef T_FUNC_DELETE
    // Function type for destroyers.
    typedef void (*funcDelete_t)(__ptr_t);
    #define T_FUNC_DELETE funcDelete_t
#endif

#ifndef T_FUNC_COMPARE
    // Function type for comparators.
    typedef int64_t (*funcCompare_t)(const __ptr_t, const __ptr_t);
    #define T_FUNC_COMPARE funcCompare_t
#endif

#ifndef T_FUNC_HASH
    // Function type for hashers.
    typedef int64_t (*funcHash_t)(const __ptr_t);
    #define T_FUNC_HASH funcHash_t
#endif


/*===================================================================================================*/
/*------------------------------------------- EXTRA TYPES -------------------------------------------*/
/*===================================================================================================*/

#ifndef END_STRING
    #define END_STRING '\0'
#endif

#ifndef NEWLINE
    #define NEWLINE '\n'
#endif

#ifndef TAB
    #define TAB '\t'
#endif

#ifndef UP_ARROW
    #define UP_ARROW 256
#endif

#ifndef DOWN_ARROW
    #define DOWN_ARROW 257
#endif

#ifndef LEFT_ARROW
    #define LEFT_ARROW 258
#endif

#ifndef RIGHT_ARROW
    #define RIGHT_ARROW 259
#endif


#endif // MORE_TYPES_H