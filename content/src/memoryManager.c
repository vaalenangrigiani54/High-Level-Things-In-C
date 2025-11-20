#include <memoryManager.h>
#include <hash.h>

#define GC_TABLE_CAPACITY 16


typedef struct __gc__ {
    __ptr_t element;
    funcDelete_t delete;
    struct __gc__* next;
} garbageCollectorNode_t;

static garbageCollectorNode_t* garbageCollector[GC_TABLE_CAPACITY] = { 0 };


void __callGarbageCollector__() __attribute__ ((destructor));



/*===================================================================================================*/
/*--------------------------------------- AUXILIARY FUNCTIONS ---------------------------------------*/
/*===================================================================================================*/


garbageCollectorNode_t** aux_gc_getNodePtr(__ptr_t element, garbageCollectorNode_t** prev, size_t* posTable) {
    size_t pos = hash_pointer(element) % GC_TABLE_CAPACITY;
    garbageCollectorNode_t** nodePtr = garbageCollector + pos;
    if (prev != NULL) *prev = NULL;
    
    while (*nodePtr != NULL && (*nodePtr)->element != element) {
        if (prev != NULL) *prev = nodePtr;
        nodePtr = &(*nodePtr)->next;
    }

    if (posTable != NULL) *posTable = pos;
    return nodePtr;
}



/*====================================================================================================*/
/*----------------------------------------- HEADER FUNCTIONS -----------------------------------------*/
/*====================================================================================================*/


bool addToGarbageCollector(__ptr_t element, type_t type) {
    if (element == NULL) return false;
    
    funcDelete_t* delete = type_getDeleteFunction(type);
    if (delete == NULL) return false;

    garbageCollectorNode_t** nodePtr = aux_gc_getNodePtr(element, NULL, NULL);
    if (*nodePtr != NULL) return false;

    *nodePtr = malloc(sizeof(garbageCollectorNode_t));
    if (*nodePtr == NULL) return false;

    (*nodePtr)->element = element;
    (*nodePtr)->delete = delete;
    (*nodePtr)->next = NULL;
    return true;
}


bool removeFromGarbageCollector(__ptr_t element) {
    if (element == NULL) return false;

    size_t pos;
    garbageCollectorNode_t* prev;
    garbageCollectorNode_t** nodePtr = aux_gc_getNodePtr(element, &prev, &pos);
    if (*nodePtr == NULL) return false;

    if (prev == NULL)
        garbageCollector[pos] = (*nodePtr)->next;
    else
        prev->next = (*nodePtr)->next;

    free(*nodePtr);
    return true;
}


bool allocate(__ptr_t* ptr, size_t bytes, bool setToZero) {
    if (ptr == NULL || bytes == 0) return false;

    __ptr_t (*funcAlloc)(size_t) = setToZero ? calloc : malloc;
    *ptr = funcAlloc(bytes);

    if (!addToGarbageCollector(ptr, TYPE_INT64)) {
        free(*ptr);
        return false;
    }

    return true;
}


bool reallocate(__ptr_t* ptr, size_t bytes) {
    if (bytes == 0) return false;

    __ptr_t aux = realloc(*ptr, bytes);
    if (aux == NULL) return false;

    removeFromGarbageCollector(ptr);
    *ptr = aux;

    return addToGarbageCollector(*ptr, TYPE_INT64);
}


void deallocate(__ptr_t ptr) {
    if (removeFromGarbageCollector(ptr)) free(ptr);
}



/*===================================================================================================*/
/*------------------------------------ CALLING GARBAGE COLLECTOR ------------------------------------*/
/*===================================================================================================*/


void __callGarbageCollector__() {
    for (size_t i = 0; i < GC_TABLE_CAPACITY; i++) {
        garbageCollectorNode_t* node = garbageCollector[i];

        while (node != NULL) {
            garbageCollectorNode_t* aux = node;
            node = node->next;

            aux->delete(aux->element);
            free(aux);
        }
    }
}