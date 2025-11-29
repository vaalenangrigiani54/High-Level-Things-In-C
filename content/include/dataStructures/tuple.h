#ifndef TUPLE_DATA_STRUCTURE_H
#define TUPLE_DATA_STRUCTURE_H

#include <dataStructures/types.h>
#include <moreTypes_advanced.h>


/*===================================================================================================*/
/*-------------------------------------------- FUNCTIONS --------------------------------------------*/
/*===================================================================================================*/

// clone_tuple(), delete_tuple(), compare_tuple() and hash_tuple() declared in <dataStructures/types.h>

T_TUPLE* new_tuple(size_t size, bool cloneElements, type_t type, ...);

bool tuple_enableDeepCopyMode(T_TUPLE* tuple, bool enabled);

size_t tuple_size(const T_TUPLE* tuple);

__ptr_t tuple_get(const T_TUPLE* tuple, size_t pos);
#define tuple_get_(tuple, pos, type_cast) *(type_cast*) tuple_get(tuple, pos)

__ptr_t tuple_getFirst(const T_TUPLE* tuple);
#define tuple_getFirst_(tuple, type_cast) *(type_cast*) tuple_getFirst(tuple)

__ptr_t tuple_getLast(const T_TUPLE* tuple);
#define tuple_getLast_(tuple, type_cast) *(type_cast*) tuple_getLast(tuple)

__ptr_t tuple_getRandom(const T_TUPLE* tuple);
#define tuple_getRandom_(tuple, type_cast) *(type_cast*) tuple_getRandom(tuple)

__ptr_t tuple_find(const T_TUPLE* tuple, T_FUNC_COMPARE cmp, const __ptr_t criteria, size_t* pos);
#define tuple_find_(tuple, cmp, criteria, pos, type_cast) *(type_cast*) tuple_find(tuple, (T_FUNC_COMPARE) cmp, criteria, pos)

int64_t tuple_sum(const T_TUPLE* tuple, int64_t (*func)(__ptr_t, __ptr_t), __ptr_t arg);


#endif // TUPLE_DATA_STRUCTURE_H