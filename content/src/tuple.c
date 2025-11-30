#include <dataStructures/tuple.h>
#include <dataStructures/array.h>
#include <dataStructures/list.h>
#include <comparators.h>
#include <hash.h>
#include <stdarg.h>

struct __tuple__ {
    __ptr_t* content;
    size_t size;
    type_t type;
    
    bool deepCopiedElements;
    bool deepCopyMode;
};


/*===================================================================================================*/
/*--------------------------------------- AUXILIARY FUNCTIONS ---------------------------------------*/
/*===================================================================================================*/


T_TUPLE* tupleAux_alloc(size_t size, type_t type, bool deepCopiedElements) {
    if (size <= 1) return NULL;

    T_TUPLE* tuple = malloc(sizeof(T_TUPLE));
    if (tuple == NULL) return NULL;

    tuple->content = malloc(size * sizeof(__ptr_t));
    if (tuple->content == NULL) {
        free(tuple);
        return NULL;
    }

    tuple->size = size;
    tuple->type = type;
    tuple->deepCopiedElements = deepCopiedElements;
    tuple->deepCopyMode = false;
    return tuple;
}


bool tupleAux_setElementAtPos(T_TUPLE* tuple, __ptr_t element, size_t pos, T_FUNC_CLONE clone) {
    if (clone != NULL && element != NULL && tuple->deepCopiedElements) {
        element = clone(element);
        if (element == NULL)
            return false;
    }

    tuple->content[pos] = element;
    return true;
}



/*====================================================================================================*/
/*----------------------------------------- HEADER FUNCTIONS -----------------------------------------*/
/*====================================================================================================*/


T_TUPLE* new_tuple(size_t size, bool cloneElements, type_t type, ...) {
    T_TUPLE* tuple = tupleAux_alloc(size, type, cloneElements);
    if (tuple == NULL) return NULL;

    T_FUNC_CLONE clone = type_getCloneFunction(type);
    va_list elements;
    va_start(elements, type);

    for (size_t i = 0; i < size; i++) {
        if (!tupleAux_setElementAtPos(tuple, va_arg(elements, __ptr_t), i, clone)) {
            delete_tuple(tuple);
            return NULL;
        }
    }

    va_end(elements);
    return tuple;
}


T_TUPLE* new_tuple_fromArray(const T_ARRAY* array) {
    if (array == NULL) return NULL;

    T_TUPLE* tuple = tupleAux_alloc(array_size(array), array_type(array), array_deepCopyMode(array));
    if (tuple == NULL) return NULL;

    tuple->deepCopyMode = tuple->deepCopiedElements;
    T_FUNC_CLONE clone = type_getCloneFunction(tuple->type);

    for (size_t i = 0; i < tuple->size; i++) {
        if (!tupleAux_setElementAtPos(tuple, array_get(array, i), i, clone)) {
            delete_tuple(tuple);
            return NULL;
        }
    }

    return tuple;
}


T_TUPLE* new_tuple_fromLinkedList(const T_LIST* list) {
    if (list == NULL) return NULL;

    T_TUPLE* tuple = tupleAux_alloc(list_size(list), list_type(list), list_deepCopyMode(list));
    if (tuple == NULL) return NULL;

    tuple->deepCopyMode = tuple->deepCopiedElements;
    T_FUNC_CLONE clone = type_getCloneFunction(tuple->type);
    listIterator_t i = listIter_begin(list);

    while (listIter_hasNext(i)) {
        __ptr_t element = listIter_get(i);
        size_t pos = listIter_getPos(i);

        if (!tupleAux_setElementAtPos(tuple, element, pos, clone)) {
            delete_tuple(tuple);
            return NULL;
        }

        listIter_next(i);
    }

    return tuple;
}


T_TUPLE* clone_tuple(const T_TUPLE* tuple) {
    if (tuple == NULL) return NULL;

    T_TUPLE* cloned = tupleAux_alloc(tuple->size, tuple->type, tuple->deepCopiedElements);
    if (cloned == NULL) return NULL;

    cloned->deepCopiedElements = tuple->deepCopiedElements;
    T_FUNC_CLONE clone = type_getCloneFunction(tuple->type);

    if (clone != NULL && tuple->deepCopyMode) {
        for (size_t i = 0; i < tuple->size; i++) {
            if (!tupleAux_setElementAtPos(cloned, tuple->content[i], i, clone)) {
                delete_tuple(tuple);
                return NULL;
            }
        }
    }

    return cloned;
}


void delete_tuple(T_TUPLE* tuple) {
    if (tuple == NULL) return;

    T_FUNC_DELETE delete = type_getDeleteFunction(tuple->type);

    for (size_t i = 0; i < tuple->size; i++) {
        __ptr_t element = tuple->content[i];

        if (element != NULL && delete != NULL && tuple->deepCopiedElements)
            delete(element);
    }

    free(tuple);
}


int32_t compare_tuple(const T_TUPLE* tuple1, const T_TUPLE* tuple2) {
    if (tuple1 == NULL || tuple2 == NULL)
        return compare_pointer(tuple1, tuple2);

    int32_t diff = (int32_t) (tuple1->size - tuple2->size);
    if (diff != 0) return diff;

    diff = (int32_t) (tuple1->type - tuple2->type);
    if (diff != 0) return diff;

    T_FUNC_COMPARE cmp = type_getCompareFunction(tuple1->type);
    if (cmp == NULL) cmp = compare_pointer;

    for (size_t i = 0; i < tuple1->size; i++) {
        __ptr_t elem1 = tuple1->content[i];
        __ptr_t elem2 = tuple2->content[i];

        if (elem1 == NULL || elem2 == NULL)
            diff = compare_pointer(elem1, elem2);
        else
            diff = cmp(elem1, elem2);

        if (diff != 0) return diff;
    }

    return 0;
}


uint64_t hash_tuple(const T_TUPLE* tuple) {
    if (tuple == NULL) return 0;

    T_FUNC_HASH hash = type_getHashFunction(tuple->type);
    if (hash == NULL) hash = hash_pointer;

    uint64_t n = 1469598103934665603ULL;
    uint64_t partial, null_hash = 0x9e3779b97f4a7c15ULL;
    void* element;

    for (size_t i = 0; i < tuple->size; i++) {
        element = tuple->content[i];
        partial = (element == NULL) ? null_hash : hash(element);
        n ^= partial + null_hash + (n << 6) + (n >> 2);
    }

    return n;
}


bool tuple_enableDeepCopyMode(T_TUPLE* tuple, bool enabled) {
    if (tuple == NULL) return false;

    tuple->deepCopyMode = enabled;
    return true;
}


bool tuple_deepCopyMode(const T_TUPLE* tuple) {
    return (tuple == NULL) ? NULL : tuple->deepCopyMode;
}


size_t tuple_size(const T_TUPLE* tuple) {
    return (tuple == NULL) ? 0 : tuple->size;
}


type_t tuple_type(const T_TUPLE* tuple) {
    return (tuple == NULL) ? 0 : tuple->type;
}


__ptr_t tuple_get(const T_TUPLE* tuple, size_t pos) {
    return array_get((T_ARRAY*) tuple, pos);
}


__ptr_t tuple_getFirst(const T_TUPLE* tuple) {
    return array_getFirst((T_ARRAY*) tuple);
}


__ptr_t tuple_getLast(const T_TUPLE* tuple) {
    return array_getLast((T_ARRAY*) tuple);
}


__ptr_t tuple_getRandom(const T_TUPLE* tuple) {
    return array_getRandom((T_ARRAY*) tuple);
}


__ptr_t tuple_find(const T_TUPLE* tuple, T_FUNC_COMPARE cmp, const __ptr_t criteria, size_t* pos) {
    return array_find((T_ARRAY*) tuple, cmp, criteria, pos);
}


int64_t tuple_sum(const T_TUPLE* tuple, int64_t (*func)(__ptr_t, __ptr_t), __ptr_t arg) {
    return array_sum((T_ARRAY*) tuple, func, arg);
}