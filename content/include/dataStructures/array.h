#ifndef ARRAY_DATA_STRUCTURE_H
#define ARRAY_DATA_STRUCTURE_H

#include <dataStructures/types.h>
#include <moreTypes_advanced.h>


/*===================================================================================================*/
/*----------------------------------------- BASIC FUNCTIONS -----------------------------------------*/
/*===================================================================================================*/

T_ARRAY* new_array(type_t type, size_t initialCapacity);

// clone_array(), delete_array() and compare_array() declared in <dataStructures/types.h>

bool array_enableDeepCopyMode(T_ARRAY* array, bool enabled);

bool array_markElementAsDeepCopied(T_ARRAY* array, size_t pos, bool deepCopied);

void array_clear(T_ARRAY* array);

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

T_ARRAY* array_extend(T_ARRAY* array, const T_ARRAY* newElements, bool cloneArray);

T_ARRAY* array_reverse(T_ARRAY* array, bool cloneArray);

T_ARRAY* array_sort(T_ARRAY* array, bool cloneArray);

T_ARRAY* array_sortByKey(T_ARRAY* array, T_FUNC_COMPARE cmp, bool cloneArray);

T_ARRAY* array_shuffle(T_ARRAY* array, bool cloneArray);


#endif // ARRAY_DATA_STRUCTURE_H