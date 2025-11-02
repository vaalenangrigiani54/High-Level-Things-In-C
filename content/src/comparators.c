#include <comparators.h>
#include <string.h>


int32_t compare_int8(const int8_t* n1, const int8_t* n2) {
    if (n1 == NULL || n2 == NULL) return compare_pointer(n1, n2);
    return (int32_t) (*n1 - *n2);
}

int32_t compare_int16(const int16_t* n1, const int16_t* n2) {
    if (n1 == NULL || n2 == NULL) return compare_pointer(n1, n2);
    return (int32_t) (*n1 - *n2);
}

int32_t compare_int32(const int32_t* n1, const int32_t* n2) {
    if (n1 == NULL || n2 == NULL) return compare_pointer(n1, n2);
    return (int32_t) (*n1 - *n2);
}

int32_t compare_int64(const int64_t* n1, const int64_t* n2) {
    if (n1 == NULL || n2 == NULL) return compare_pointer(n1, n2);
    if (*n1 == *n2) return 0;
    return (*n1 > *n2) ? 1 : -1;
}

int32_t compare_float(const float* x1, const float* x2) {
    if (x1 == NULL || x2 == NULL) return compare_pointer(x1, x2);
    if (*x1 == *x2) return 0;
    return (*x1 > *x2) ? 1 : -1;
}

int32_t compare_double64(const double* x1, const double* x2) {
    if (x1 == NULL || x2 == NULL) return compare_pointer(x1, x2);
    if (*x1 == *x2) return 0;
    return (*x1 > *x2) ? 1 : -1;
}

int32_t compare_double80(const long double* x1, const long double* x2) {
    if (x1 == NULL || x2 == NULL) return compare_pointer(x1, x2);
    if (*x1 == *x2) return 0;
    return (*x1 > *x2) ? 1 : -1;
}

int32_t compare_character(const char* c1, const char* c2) {
    if (c1 == NULL || c2 == NULL) return compare_pointer(c1, c2);
    return (int32_t) (*c1 - *c2);
}

int32_t compare_charSequence(const char* str1, const char* str2) {
    if (str1 == NULL || str2 == NULL) return compare_pointer(str1, str2);
    return (int32_t) strcmp(str1, str2);
}

int32_t compare_pointer(const __ptr_t p1, const __ptr_t p2) {
    if (p1 == p2) return 0;
    return (p1 > p2) ? 1 : -1;
}