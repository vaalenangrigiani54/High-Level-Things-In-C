#ifndef CLONERS_H
#define CLONERS_H

#include <moreTypes.h>


int8_t* clone_int8(const int8_t* n);

int16_t* clone_int16(const int16_t* n);

int32_t* clone_int32(const int32_t* n);

int64_t* clone_int64(const int64_t* n);

float* clone_float(const float* x);

double* clone_double64(const double* x);

long double* clone_double80(const long double* x);

char* clone_character(const char* c);

char* clone_charSequence(const char* str);


#endif // CLONERS_H