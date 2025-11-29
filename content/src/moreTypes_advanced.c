#include <moreTypes_advanced.h>
#include <cloners.h>
#include <comparators.h>
#include <hash.h>
#include <dataStructures/types.h>
#include <unistd.h>

typedef struct __registered_type__ {
    funcClone_t _clone_;
    funcDelete_t _delete_;
    funcCompare_t _compare_;
    funcHash_t _hash_;
} registeredType_t;

static registeredType_t* registeredTypes = NULL;
static int registeredTypes_size = 0;
static int registeredTypes_capacity = 0;


funcClone_t type_getCloneFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return (funcClone_t) clone_int8;
        case TYPE_INT16: return (funcClone_t) clone_int16;
        case TYPE_INT32: return (funcClone_t) clone_int32;
        case TYPE_INT64: return (funcClone_t) clone_int64;
        case TYPE_FLOAT: return (funcClone_t) clone_float;
        case TYPE_DOUBLE64: return (funcClone_t) clone_double64;
        case TYPE_DOUBLE80: return (funcClone_t) clone_double80;
        case TYPE_CHARACTER: return (funcClone_t) clone_character;
        case TYPE_CHAR_SEQUENCE: return (funcClone_t) clone_charSequence;
        case TYPE_STRING: return (funcClone_t) clone_str;
        case TYPE_LIST: return (funcClone_t) clone_list;
        case TYPE_ARRAY: return (funcClone_t) clone_array;
        case TYPE_TUPLE: return (funcClone_t) clone_tuple;
        case TYPE_SET: return (funcClone_t) clone_set;
        default: {
            int32_t idx = (int32_t) type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._clone_ : NULL;
        };
    }
}


funcDelete_t type_getDeleteFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return (funcDelete_t) free;
        case TYPE_INT16: return (funcDelete_t) free;
        case TYPE_INT32: return (funcDelete_t) free;
        case TYPE_INT64: return (funcDelete_t) free;
        case TYPE_FLOAT: return (funcDelete_t) free;
        case TYPE_DOUBLE64: return (funcDelete_t) free;
        case TYPE_DOUBLE80: return (funcDelete_t) free;
        case TYPE_CHARACTER: return (funcDelete_t) free;
        case TYPE_CHAR_SEQUENCE: return (funcDelete_t) free;
        case TYPE_STRING: return (funcDelete_t) delete_str;
        case TYPE_LIST: return (funcDelete_t) delete_list;
        case TYPE_ARRAY: return (funcDelete_t) delete_array;
        case TYPE_TUPLE: return (funcDelete_t) delete_tuple;
        case TYPE_SET: return (funcDelete_t) delete_set;
        default: {
            int32_t idx = (int32_t) type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._delete_ : NULL;
        };
    }
}


funcCompare_t type_getCompareFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return (funcCompare_t) compare_int8;
        case TYPE_INT16: return (funcCompare_t) compare_int16;
        case TYPE_INT32: return (funcCompare_t) compare_int32;
        case TYPE_INT64: return (funcCompare_t) compare_int64;
        case TYPE_FLOAT: return (funcCompare_t) compare_float;
        case TYPE_DOUBLE64: return (funcCompare_t) compare_double64;
        case TYPE_DOUBLE80: return (funcCompare_t) compare_double80;
        case TYPE_CHARACTER: return (funcCompare_t) compare_character;
        case TYPE_CHAR_SEQUENCE: return (funcCompare_t) compare_charSequence;
        case TYPE_STRING: return (funcCompare_t) compare_str;
        case TYPE_LIST: return (funcCompare_t) compare_list;
        case TYPE_ARRAY: return (funcCompare_t) compare_array;
        case TYPE_TUPLE: return (funcCompare_t) compare_tuple;
        case TYPE_SET: return (funcCompare_t) compare_set;
        default: {
            int32_t idx = (int32_t) type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._compare_ : NULL;
        };
    }
}


funcHash_t type_getHashFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return (funcHash_t) hash_int8;
        case TYPE_INT16: return (funcHash_t) hash_int16;
        case TYPE_INT32: return (funcHash_t) hash_int32;
        case TYPE_INT64: return (funcHash_t) hash_int64;
        case TYPE_FLOAT: return (funcHash_t) hash_float;
        case TYPE_DOUBLE64: return (funcHash_t) hash_double64;
        case TYPE_DOUBLE80: return (funcHash_t) hash_double80;
        case TYPE_CHARACTER: return (funcHash_t) hash_character;
        case TYPE_CHAR_SEQUENCE: return (funcHash_t) hash_charSequence;
        case TYPE_TUPLE: return (funcHash_t) hash_tuple;
        default: {
            int32_t idx = (int32_t) type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._hash_ : NULL;
        };
    }
}


void __type_register__(type_t* type, funcClone_t cloneFunction, funcDelete_t deleteFunction, funcCompare_t compareFunction, funcHash_t hashFunction) {
    if (type == NULL) {
        fputs("ERROR: 'type' reference must be non-NULL.\n", stderr);
        free(registeredTypes);
        exit(1);
    }

    if (cloneFunction == NULL) {
        fputs("ERROR: 'cloneFunction' must be non-NULL.\n", stderr);
        free(registeredTypes);
        exit(1);
    }

    if (deleteFunction == NULL) {
        fputs("ERROR: 'deleteFunction' must be non-NULL.\n", stderr);
        free(registeredTypes);
        exit(1);
    }
    
    if (registeredTypes_size >= registeredTypes_capacity) {
        if (!reallocate((void**) &registeredTypes, (size_t) (registeredTypes_capacity + 8) * sizeof(registeredType_t))) {
            perror("TYPE_REGISTER");
            free(registeredTypes);
            exit(1);
        }

        registeredTypes_capacity += 8;
    }

    registeredTypes[registeredTypes_size]._clone_ = cloneFunction;
    registeredTypes[registeredTypes_size]._delete_ = deleteFunction;
    registeredTypes[registeredTypes_size]._compare_ = compareFunction;
    registeredTypes[registeredTypes_size]._hash_ = hashFunction;
    registeredTypes_size++;
    *type = TYPE_ARBITRARY + registeredTypes_size;
}


EXECUTE_AFTER_MAIN(delete_registered_types) {
    free(registeredTypes);
}