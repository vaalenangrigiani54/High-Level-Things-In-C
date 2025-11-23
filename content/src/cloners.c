#include <cloners.h>
#include <stdlib.h>
#include <string.h>


static __ptr_t clone_aux(const __ptr_t p, size_t size) {
    if (p == NULL) return NULL;

    __ptr_t cloned = malloc(size);
    if (cloned == NULL) return NULL;

    memcpy(cloned, p, size);
    return cloned;
}


int8_t* clone_int8(const int8_t* n) {
    return (int8_t*) clone_aux(n, sizeof(int8_t));
}

int16_t* clone_int16(const int16_t* n) {
    return (int16_t*) clone_aux(n, sizeof(int16_t));
}

int32_t* clone_int32(const int32_t* n) {
    return (int32_t*) clone_aux(n, sizeof(int32_t));
}

int64_t* clone_int64(const int64_t* n) {
    return (int64_t*) clone_aux(n, sizeof(int64_t));
}

float* clone_float(const float* x) {
    return (float*) clone_aux(x, sizeof(float));
}

double* clone_double64(const double* x) {
    return (double*) clone_aux(x, sizeof(double));
}

long double* clone_double80(const long double* x) {
    return (long double*) clone_aux(x, sizeof(long double));
}

char* clone_character(const char* c) {
    return (char*) clone_aux(c, sizeof(char));
}

char* clone_charSequence(const char* str) {
    return (char*) clone_aux(str, sizeof(char) * (strlen(str) + 1));
}