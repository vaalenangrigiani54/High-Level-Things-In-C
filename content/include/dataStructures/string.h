#ifndef STRING_DATA_STRUCTURE_H
#define STRING_DATA_STRUCTURE_H

#include <dataStructures/types.h>
#include <moreTypes.h>


T_STRING* new_str(const char* charSequence);

// clone_str() and delete_str() declared in <dataStructures/types.h>

uint64_t str_size(const T_STRING* str);

char str_charAt(const T_STRING* str, uint64_t pos);

const char* str_bytes(const T_STRING* str);

T_STRING* str_concat(const T_STRING* str);

T_STRING* str_toUpper(T_STRING* str, bool cloneStr);

T_STRING* str_toLower(T_STRING* str, bool cloneStr);

T_STRING** str_split(const T_STRING* str, const char* delim, uint64_t* amount);

#define ITERATE_STRING(str, c, idx) \
    for (uint64_t idx = 0, char c = str_charAt(str, idx); c != END_STRING; idx++, c = str_charAt(str, idx))


#endif // STRING_DATA_STRUCTURE_H