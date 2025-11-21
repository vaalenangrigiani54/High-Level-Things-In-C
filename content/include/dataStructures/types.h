#ifndef DATA_STRUCTURES_TYPES_H
/**
 * You shouldn't include this header manually in your projects. It is not useful.
 */
#define DATA_STRUCTURES_TYPES_H

#include <stdint.h>


#ifndef T_STRING
    typedef struct __str__ String;
    #define T_STRING String
#endif

#ifndef T_LIST
    typedef struct __list__ List;
    #define T_LIST List
#endif

#ifndef T_TUPLE
    typedef struct __tuple__ Tuple;
    #define T_TUPLE Tuple
#endif

#ifndef T_SET
    typedef struct __set__ Set;
    #define T_SET Set
#endif


/*=================================================*/
/*-------------------- CLONERS --------------------*/
/*=================================================*/

T_STRING* clone_str(const T_STRING* str);

T_LIST* clone_list(const T_LIST* list);

T_TUPLE* clone_tuple(const T_TUPLE* tuple);

T_SET* clone_set(const T_SET* set);


/*==================================================*/
/*------------------- DESTROYERS -------------------*/
/*==================================================*/

void delete_str(T_STRING* str);

void delete_list(T_LIST* list);

void delete_tuple(T_TUPLE* tuple);

void delete_set(T_SET* set);


/*=================================================*/
/*------------------ COMPARATORS ------------------*/
/*=================================================*/

int32_t compare_str(const T_STRING* str1, const T_STRING* str2);

int32_t compare_list(const T_LIST* list1, const T_LIST* list2);

int32_t compare_tuple(const T_TUPLE* tuple1, const T_TUPLE* tuple2);

int32_t compare_set(const T_SET* set1, const T_SET* set2);


/*=================================================*/
/*-------------------- HASHERS --------------------*/
/*=================================================*/

int64_t hash_str(const T_STRING* str);

int64_t hash_tuple(const T_TUPLE* tuple);


#endif // DATA_STRUCTURES_TYPES_H