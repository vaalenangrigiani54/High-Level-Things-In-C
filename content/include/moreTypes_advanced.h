#ifndef MORE_TYPES_ADVANCED_H
#define MORE_TYPES_ADVANCED_H

#include <ioExtras.h>

// Enumeration for types.
typedef enum __types_enum__ {
    TYPE_INT8, // int8_t | uint8_t
    TYPE_INT16, // int16_t | uint16_t
    TYPE_INT32, // int32_t | uint32_t
    TYPE_INT64, // int64_t | uint64_t
    TYPE_FLOAT, // float
    TYPE_DOUBLE64, // double
    TYPE_DOUBLE80, // long double
    TYPE_CHARACTER, // char
    TYPE_STRING, // String
    TYPE_CHAR_SEQUENCE, // char[]
    TYPE_LIST, // List*
    TYPE_TUPLE, // Tuple*
    TYPE_SET, // Set*
    TYPE_ARBITRARY // Any unregistered type
} type_t;


funcClone_t type_getCloneFunction(type_t type);

funcDelete_t type_getDeleteFunction(type_t type);

funcCompare_t type_getCompareFunction(type_t type);

funcHash_t type_getHashFunction(type_t type);


// You may use TYPE_REGISTER instead. It does the same thing but before main scope.
static void __type_register__(type_t* type, funcClone_t cloneFunction, funcDelete_t deleteFunction, funcCompare_t compareFunction, funcHash_t hashFunction) __attribute__((__nonnull(1 2 3)));

#define REGISTER_NEW_TYPE(type_name, reference_to_type_t, cloneFunction, deleteFunction, compareFunction, hashFunction) \
    EXECUTE_BEFORE_MAIN(type_name) { \
        __type_register__(reference_to_type_t, \
        (funcClone_t) cloneFunction, \
        (funcDelete_t) deleteFunction, \
        (funcCompare_t) compareFunction, \
        (funcHash_t) hashFunction); \
    }


#endif // MORE_TYPES_ADVANCED_H