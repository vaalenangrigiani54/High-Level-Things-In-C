#ifndef COMPARATORS_H
#define COMPARATORS_H

#include <moreTypes.h>


int32_t compare_int8(const int8_t* n1, const int8_t* n2);

int32_t compare_int16(const int16_t* n1, const int16_t* n2);

int32_t compare_int32(const int32_t* n1, const int32_t* n2);

int32_t compare_int64(const int64_t* n1, const int64_t* n2);

int32_t compare_float(const float* x1, const float* x2);

int32_t compare_double64(const double* x1, const double* x2);

int32_t compare_double80(const long double* x1, const long double* x2);

int32_t compare_character(const char* c1, const char* c2);

int32_t compare_charSequence(const char* str1, const char* str2);

int32_t compare_pointer(const __ptr_t p1, const __ptr_t p2);


#endif // COMPARATORS_H