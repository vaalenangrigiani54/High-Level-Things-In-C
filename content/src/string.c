#include <dataStructures/string.h>
#include <comparators.h>
#include <hash.h>
#include <ioExtras.h>
#include <string.h>

struct __str__ {
    char* seq;
    size_t size;
};


/*====================================================================================================*/
/*----------------------------------------- HEADER FUNCTIONS -----------------------------------------*/
/*====================================================================================================*/


T_STRING* new_str(const char* charSequence) {
    if (charSequence == NULL) return NULL;

    T_STRING* str = malloc(sizeof(T_STRING));
    if (str == NULL) return NULL;

    str->size = strlen(charSequence);
    str->seq = malloc((str->size + 1) * sizeof(char));
    if (str->seq == NULL) {
        free(str);
        return NULL;
    }

    memcpy(str->seq, charSequence, str->size + 1);
    return str;
}


T_STRING* clone_str(const T_STRING* str) {
    if (str == NULL) return NULL;
    return new_str(str->seq);
}


void delete_str(T_STRING* str) {
    if (str != NULL) {
        free(str->seq);
        free(str);
    }
}


int32_t compare_str(const T_STRING* str1, const T_STRING* str2) {
    if (str1 == NULL || str2 == NULL)
        return compare_pointer(str1, str2);
    return strcmp(str1->seq, str2->seq);
}


int64_t hash_str(const T_STRING* str) {
    return (str == NULL) ? 0 : hash_charSequence(str->seq);
}


size_t str_size(const T_STRING* str) {
    return (str == NULL) ? 0 : str->size;
}


T_STRING* str_input(const char* inputText, int32_t bufferSize) {
    char buffer[bufferSize + 2];
    if (!read_string(inputText, buffer, bufferSize, stdin))
        return NULL;

    return new_str(buffer);
}


char str_charAt(const T_STRING* str, size_t pos) {
    return (str == NULL) ? END_STRING : str->seq[pos];
}


const char* str_bytes(const T_STRING* str) {
    return (str == NULL) ? NULL : str->seq;
}


T_STRING* str_concat(const T_STRING* str1, const T_STRING* str2) {
    if (str1 == NULL || str2 == NULL) return NULL;

    size_t length = str1->size + str2->size;
    T_STRING* concatenated = malloc(sizeof(T_STRING));
    if (concatenated == NULL) return NULL;

    concatenated->size = length;
    concatenated->seq = malloc((length + 1) * sizeof(char));
    if (concatenated->seq == NULL) return NULL;

    memcpy(concatenated->seq, str1->seq, str1->size + 1);
    memcpy(concatenated->seq + str1->size, str2->seq, str2->size + 1);
    return concatenated;
}


T_STRING* str_toUpper(T_STRING* str, bool cloneStr) {
    T_STRING* strAux = cloneStr ? clone_str(str) : str;
    if (strAux == NULL) return NULL;

    for (size_t i = 0; i < str->size; i++) {
        char c = str->seq[i];

        if (c >= "a" && c <= "z")
            str->seq[i] = c + ("A" - "a");
    }

    return strAux;
}


T_STRING* str_toLower(T_STRING* str, bool cloneStr) {
    T_STRING* strAux = cloneStr ? clone_str(str) : str;
    if (strAux == NULL) return NULL;

    for (size_t i = 0; i < str->size; i++) {
        char c = str->seq[i];

        if (c >= "A" && c <= "Z")
            str->seq[i] = c + ("a" - "A");
    }

    return strAux;
}


T_ARRAY* str_split(const T_STRING* str, const char* delim) {
    if (str == NULL || delim == NULL) return NULL;

    size_t i = strlen(delim), amount = 0;
    if (i == 0) return NULL;

    const char* temp = str->seq;
    while ((temp = strstr(temp, delim)) != NULL) {
        amount++;
        temp += i;
    }

    T_ARRAY* substrings = new_array(TYPE_STRING, amount);
    if (substrings == NULL) return NULL;

    i = 0;
    char buffer[str->size + 1];
    memcpy(buffer, str->seq, str->size + 1);
    temp = strtok(buffer, delim);

    while (temp != NULL) {
        array_addLast(substrings, new_str(temp));

        if (!array_markElementAsDeepCopied(substrings, i, true)) {
            delete_array(substrings);
            return NULL;
        }

        temp = strtok(NULL, delim);
        i++;
    }

    return substrings;
}