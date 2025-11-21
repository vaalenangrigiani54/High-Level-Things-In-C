#ifndef LIST_DATA_STRUCTURE_H
#define LIST_DATA_STRUCTURE_H

#include <dataStructures/types.h>
#include <moreTypes_advanced.h>

#ifndef listIterator_t
    #define listIterator_t struct __listIter__*
#endif


/*===================================================================================================*/
/*----------------------------------------- BASIC FUNCTIONS -----------------------------------------*/
/*===================================================================================================*/

T_LIST* new_list(type_t type);

// clone_list(), delete_list() and compare_list() declared in <dataStructures/types.h>

bool list_enableDeepCopyMode(T_LIST* list, bool enabled);

bool list_clear(T_LIST* list);

size_t list_size(const T_LIST* list);

bool list_add(T_LIST* list, __ptr_t element, size_t pos);

bool list_addFirst(T_LIST* list, __ptr_t element);

bool list_addLast(T_LIST* list, __ptr_t element);

__ptr_t list_get(const T_LIST* list, size_t pos);
#define list_get_(list, pos, type_cast) *(type_cast*) list_get(list, pos)

__ptr_t list_getFirst(const T_LIST* list);
#define list_getFirst_(list, type_cast) *(type_cast*) list_getFirst(list)

__ptr_t list_getLast(const T_LIST* list);
#define list_getLast_(list, type_cast) *(type_cast*) list_getLast(list)

__ptr_t list_getRandom(const T_LIST* list);
#define list_getRandom_(list, type_cast) *(type_cast*) list_getRandom(list)

__ptr_t list_remove(T_LIST* list, size_t pos);
#define list_remove_(list, pos, type_cast) *(type_cast*) list_remove(list, pos)

__ptr_t list_removeFirst(T_LIST* list);
#define list_removeFirst_(list, type_cast) *(type_cast*) list_removeFirst(list)

__ptr_t list_removeLast(T_LIST* list);
#define list_removeLast_(list, type_cast) *(type_cast*) list_removeLast(list)


/*===================================================================================================*/
/*----------------------------------------- EXTRA UTILITIES -----------------------------------------*/
/*===================================================================================================*/

__ptr_t list_find(const T_LIST* list, T_FUNC_COMPARE cmp, const __ptr_t criteria, size_t* pos);
#define list_find_(list, cmp, criteria, pos, type_cast) *(type_cast*) list_find(list, (T_FUNC_COMPARE) cmp, criteria, pos)

int64_t list_sum(const T_LIST* list, int64_t (*func)(__ptr_t, __ptr_t), __ptr_t arg);

T_LIST* list_filter(const T_LIST* list, bool (*filter)(__ptr_t, __ptr_t), __ptr_t arg);

T_LIST* list_extend(T_LIST* list, const T_LIST* newElements, bool cloneList);

T_LIST* list_sort(T_LIST* list, bool cloneList);

T_LIST* list_sortByKey(T_LIST* list, T_FUNC_COMPARE cmp, bool cloneList);

T_LIST* list_shuffle(T_LIST* list, bool cloneList);


/*====================================================================================================*/
/*---------------------------------------- ITERATOR FUNCTIONS ----------------------------------------*/
/*====================================================================================================*/

listIterator_t listIter_begin(T_LIST* associated);

listIterator_t listIter_end(T_LIST* associated);

bool listIter_next(listIterator_t iter);

bool listIter_back(listIterator_t iter);

bool listIter_hasNext(const listIterator_t iter);

__ptr_t listIter_get(const listIterator_t iter);
#define listIter_get_(iter, type_cast) *(type_cast*) listIter_get(iter)

size_t listIter_getPos(const listIterator_t iter);


#endif // LIST_DATA_STRUCTURE_H