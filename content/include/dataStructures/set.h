#ifndef SET_DATA_STRUCTURE_H
#define SET_DATA_STRUCTURE_H

#include <dataStructures/types.h>
#include <moreTypes_advanced.h>

#ifndef setIterator_t
    #define setIterator_t struct __setIter__*
#endif


/*===================================================================================================*/
/*-------------------------------------------- FUNCTIONS --------------------------------------------*/
/*===================================================================================================*/

// clone_set(), delete_set(), and compare_set() declared in <dataStructures/types.h>

T_SET* new_set(type_t type);

bool set_enableDeepCopyMode(T_SET* set, bool enabled);

size_t set_size(const T_SET* set);

bool set_add(T_SET* set, __ptr_t element);

__ptr_t set_getRandom(const T_SET* set);
#define set_getRandom_(set, type_cast) *(type_cast*) set_getRandom(set)

__ptr_t set_remove(T_SET* set, __ptr_t element);
#define set_remove_(set, element, type_cast) *(type_cast*) set_remove(set, element)

bool set_contains(const T_SET* set, const __ptr_t element);

bool set_isSubSet(const T_SET* set, const T_SET* subset);

T_SET* set_union(T_SET* set, const T_SET* anotherSet, bool inPlace);

T_SET* set_intersect(T_SET* set, const T_SET* anotherSet, bool inPlace);

T_SET* set_difference(T_SET* set, const T_SET* anotherSet, bool inPlace);

__ptr_t set_find(const T_SET* set, T_FUNC_COMPARE cmp, const __ptr_t criteria);
#define set_find_(set, cmp, criteria, type_cast) *(type_cast*) set_find(set, (T_FUNC_COMPARE) cmp, criteria)

int64_t set_sum(const T_SET* set, int64_t (*func)(__ptr_t, __ptr_t), __ptr_t arg);


/*====================================================================================================*/
/*---------------------------------------- ITERATOR FUNCTIONS ----------------------------------------*/
/*====================================================================================================*/

setIterator_t setIter_begin(T_SET* associated);

bool setIter_next(setIterator_t iter);

bool setIter_hasNext(const setIterator_t iter);

__ptr_t setIter_get(const setIterator_t iter);
#define setIter_get_(iter, type_cast) *(type_cast*) setIter_get(iter)


#endif // SET_DATA_STRUCTURE_H