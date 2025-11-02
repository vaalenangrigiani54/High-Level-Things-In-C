#ifndef HASH_H
#define HASH_H

#include <moreTypes.h>


uint64_t hash_int8(const int8_t* n);

uint64_t hash_int16(const int16_t* n);

uint64_t hash_int32(const int32_t* n);

uint64_t hash_int64(const int64_t* n);

uint64_t hash_float(const float* x);

uint64_t hash_double64(const double* x);

uint64_t hash_double80(const long double* x);

uint64_t hash_character(const char* c);

uint64_t hash_charSequence(const char* str);

uint64_t hash_pointer(const __ptr_t p);


#endif // HASH_H