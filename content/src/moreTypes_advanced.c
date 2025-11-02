#include <moreTypes_advanced.h>
#include <unistd.h>

typedef struct __registered_type__ {
    funcClone_t* _clone_;
    funcDelete_t* _delete_;
    funcCompare_t* _compare_;
    funcHash_t* _hash_;
} registeredType_t;

static registeredType_t* registeredTypes = NULL;
static unsigned registeredTypes_size = 0;
static unsigned registeredTypes_capacity = 0;


funcClone_t type_getCloneFunction(type_t type) {

}


funcDelete_t type_getDeleteFunction(type_t type) {

}


funcCompare_t type_getCompareFunction(type_t type) {

}


funcHash_t type_getHashFunction(type_t type) {

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