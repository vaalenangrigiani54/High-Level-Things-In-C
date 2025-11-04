#include <moreTypes_advanced.h>
#include <cloners.h>
#include <comparators.h>
#include <hash.h>
#include <dataStructures/types.h>
#include <unistd.h>

typedef struct __registered_type__ {
    funcClone_t* _clone_;
    funcDelete_t* _delete_;
    funcCompare_t* _compare_;
    funcHash_t* _hash_;
} registeredType_t;

static registeredType_t* registeredTypes = NULL;
static int registeredTypes_size = 0;
static int registeredTypes_capacity = 0;


funcClone_t type_getCloneFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return clone_int8;
        case TYPE_INT16: return clone_int16;
        case TYPE_INT32: return clone_int32;
        case TYPE_INT64: return clone_int64;
        case TYPE_FLOAT: return clone_float;
        case TYPE_DOUBLE64: return clone_double64;
        case TYPE_DOUBLE80: return clone_double80;
        case TYPE_CHARACTER: return clone_character;
        case TYPE_CHAR_SEQUENCE: return clone_charSequence;
        case TYPE_STRING: return clone_str;
        case TYPE_LIST: return clone_list;
        case TYPE_TUPLE: return clone_tuple;
        case TYPE_SET: return clone_set;
        default: {
            int idx = type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._clone_ : NULL;
        };
    }
}


funcDelete_t type_getDeleteFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return free;
        case TYPE_INT16: return free;
        case TYPE_INT32: return free;
        case TYPE_INT64: return free;
        case TYPE_FLOAT: return free;
        case TYPE_DOUBLE64: return free;
        case TYPE_DOUBLE80: return free;
        case TYPE_CHARACTER: return free;
        case TYPE_CHAR_SEQUENCE: return free;
        case TYPE_STRING: return delete_str;
        case TYPE_LIST: return delete_list;
        case TYPE_TUPLE: return delete_tuple;
        case TYPE_SET: return delete_set;
        default: {
            int idx = type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._delete_ : NULL;
        };
    }
}


funcCompare_t type_getCompareFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return compare_int8;
        case TYPE_INT16: return compare_int16;
        case TYPE_INT32: return compare_int32;
        case TYPE_INT64: return compare_int64;
        case TYPE_FLOAT: return compare_float;
        case TYPE_DOUBLE64: return compare_double64;
        case TYPE_DOUBLE80: return compare_double80;
        case TYPE_CHARACTER: return compare_character;
        case TYPE_CHAR_SEQUENCE: return compare_charSequence;
        case TYPE_STRING: return compare_str;
        case TYPE_LIST: return compare_list;
        case TYPE_TUPLE: return compare_tuple;
        case TYPE_SET: return compare_set;
        default: {
            int idx = type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._compare_ : NULL;
        };
    }
}


funcHash_t type_getHashFunction(type_t type) {
    switch (type) {
        case TYPE_INT8: return hash_int8;
        case TYPE_INT16: return hash_int16;
        case TYPE_INT32: return hash_int32;
        case TYPE_INT64: return hash_int64;
        case TYPE_FLOAT: return hash_float;
        case TYPE_DOUBLE64: return hash_double64;
        case TYPE_DOUBLE80: return hash_double80;
        case TYPE_CHARACTER: return hash_character;
        case TYPE_CHAR_SEQUENCE: return hash_charSequence;
        case TYPE_TUPLE: return hash_tuple;
        default: {
            int idx = type - TYPE_ARBITRARY - 1;
            return (idx >= 0 && idx < registeredTypes_size) ? registeredTypes[idx]._hash_ : NULL;
        };
    }
}


static void __type_register__(type_t* type, funcClone_t cloneFunction, funcDelete_t deleteFunction, funcCompare_t compareFunction, funcHash_t hashFunction) {
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
        registeredType_t* aux = realloc(registeredTypes, (size_t) (registeredTypes_capacity + 8) * sizeof(registeredType_t));
        if (aux == NULL) {
            perror("TYPE_REGISTER");
            free(registeredTypes);
            exit(1);
        }

        registeredTypes = aux;
        registeredTypes_capacity += 8;
    }

    registeredTypes[registeredTypes_size]._clone_ = cloneFunction;
    registeredTypes[registeredTypes_size]._delete_ = deleteFunction;
    registeredTypes[registeredTypes_size]._compare_ = compareFunction;
    registeredTypes[registeredTypes_size]._hash_ = hashFunction;
    registeredTypes_size++;
}


EXECUTE_AFTER_MAIN(delete_registered_types) {
    free(registeredTypes);
}