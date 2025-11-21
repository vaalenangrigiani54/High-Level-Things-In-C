#ifndef ARRAY_DATA_STRUCTURE_H
#define ARRAY_DATA_STRUCTURE_H

#include <dataStructures/types.h>
#include <moreTypes_advanced.h>

#ifndef arrayIterator_t
    #define arrayIterator_t struct __arrayIter__*
#endif


/*===================================================================================================*/
/*----------------------------------------- BASIC FUNCTIONS -----------------------------------------*/
/*===================================================================================================*/

T_ARRAY* new_array(type_t type, size_t initialCapacity);

// clone_array(), delete_array() and compare_array() declared in <dataStructures/types.h>

bool array_enableDeepCopyMode(T_ARRAY* array, bool enabled);

bool array_clear(T_ARRAY* array);

size_t array_size(const T_ARRAY* array);

bool array_add(T_ARRAY* array, __ptr_t element, size_t pos);

bool array_addFirst(T_ARRAY* array, __ptr_t element);

bool array_addLast(T_ARRAY* array, __ptr_t element);

__ptr_t array_get(const T_ARRAY* array, size_t pos);
#define array_get_(array, pos, type_cast) *(type_cast*) array_get(array, pos)

__ptr_t array_getFirst(const T_ARRAY* array);
#define array_getFirst_(array, type_cast) *(type_cast*) array_getFirst(array)

__ptr_t array_getLast(const T_ARRAY* array);
#define array_getLast_(array, type_cast) *(type_cast*) array_getLast(array)

__ptr_t array_getRandom(const T_ARRAY* array);
#define array_getRandom_(array, type_cast) *(type_cast*) array_getRandom(array)

__ptr_t array_remove(T_ARRAY* array, size_t pos);
#define array_remove_(array, pos, type_cast) *(type_cast*) array_remove(array, pos)

__ptr_t array_removeFirst(T_ARRAY* array);
#define array_removeFirst_(array, type_cast) *(type_cast*) array_removeFirst(array)

__ptr_t array_removeLast(T_ARRAY* array);
#define array_removeLast_(array, type_cast) *(type_cast*) array_removeLast(array)


/*===================================================================================================*/
/*----------------------------------------- EXTRA UTILITIES -----------------------------------------*/
/*===================================================================================================*/

__ptr_t array_find(const T_ARRAY* array, T_FUNC_COMPARE cmp, const __ptr_t criteria, size_t* pos);
#define array_find_(array, cmp, criteria, pos, type_cast) *(type_cast*) array_find(array, (T_FUNC_COMPARE) cmp, criteria, pos)

int64_t array_sum(const T_ARRAY* array, int64_t (*func)(__ptr_t, __ptr_t), __ptr_t arg);

T_ARRAY* array_filter(const T_ARRAY* array, bool (*filter)(__ptr_t, __ptr_t), __ptr_t arg);

T_ARRAY* array_extend(T_ARRAY* array, const T_ARRAY* newElements, bool clonearray);

T_ARRAY* array_sort(T_ARRAY* array, bool clonearray);

T_ARRAY* array_sortByKey(T_ARRAY* array, T_FUNC_COMPARE cmp, bool clonearray);

T_ARRAY* array_shuffle(T_ARRAY* array, bool clonearray);


/*====================================================================================================*/
/*---------------------------------------- ITERATOR FUNCTIONS ----------------------------------------*/
/*====================================================================================================*/

arrayIterator_t arrayIter_begin(T_ARRAY* associated);

arrayIterator_t arrayIter_end(T_ARRAY* associated);

bool arrayIter_next(arrayIterator_t iter);

bool arrayIter_back(arrayIterator_t iter);

bool arrayIter_hasNext(const arrayIterator_t iter);

__ptr_t arrayIter_get(const arrayIterator_t iter);
#define arrayIter_get_(iter, type_cast) *(type_cast*) arrayIter_get(iter)

size_t arrayIter_getPos(const arrayIterator_t iter);


#endif // ARRAY_DATA_STRUCTURE_H