#include <hash.h>
#include <string.h>


uint64_t hash_fnv64_generic(const __ptr_t n, uint64_t size) {
    if (n == NULL) return 0;
    
    const uint64_t prime = 1099511628211ULL;
    uint64_t hash = 14695981039346656037ULL;

    const uint8_t* bytes = (const uint8_t*) n;
    
    for (uint64_t i = 0; i < size; i++) {
        hash ^= bytes[i];
        hash *= prime;
    }
    
    return hash;
}


uint64_t hash_int8(const int8_t* n) {
    return hash_fnv64_generic(n, sizeof(int8_t));
}

uint64_t hash_int16(const int16_t* n) {
    return hash_fnv64_generic(n, sizeof(int16_t));
}

uint64_t hash_int32(const int32_t* n) {
    return hash_fnv64_generic(n, sizeof(int32_t));
}

uint64_t hash_int64(const int64_t* n) {
    return hash_fnv64_generic(n, sizeof(int64_t));
}

uint64_t hash_float(const float* x) {
    return hash_fnv64_generic(x, sizeof(float));
}

uint64_t hash_double64(const double* x) {
    return hash_fnv64_generic(x, sizeof(double));
}

uint64_t hash_double80(const long double* x) {
    return hash_fnv64_generic(x, sizeof(long double));
}

uint64_t hash_character(const char* c) {
    return hash_fnv64_generic((int32_t*) c, sizeof(int32_t));
}

uint64_t hash_charSequence(const char* str) {
    return hash_fnv64_generic(str, sizeof(char) * (strlen(str) + 1));
}

uint64_t hash_pointer(const __ptr_t p) {
    return hash_fnv64_generic(p, sizeof(__ptr_t));
}