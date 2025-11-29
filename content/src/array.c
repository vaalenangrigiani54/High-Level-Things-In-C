#include <dataStructures/array.h>
#include <ioExtras.h>
#include <comparators.h>
#include <string.h>

struct __array__ {
    __ptr_t* content;
    size_t size;
    size_t capacity;
    bool* deepCopied;
    type_t type;
    bool deepCopyMode;
};


/*===================================================================================================*/
/*--------------------------------------- AUXILIARY FUNCTIONS ---------------------------------------*/
/*===================================================================================================*/


static bool aux_array_reallocate(T_ARRAY* array, size_t newCapacity) {
    if (!reallocate((__ptr_t*) &array->content, newCapacity * sizeof(__ptr_t)) ||
        !reallocate((__ptr_t*) &array->deepCopied, newCapacity * sizeof(bool))) {
            return false;
    }

    array->capacity = newCapacity;
    return true;
}


static void aux_array_swapElements(T_ARRAY* array, size_t pos1, size_t pos2) {
    if (pos1 == pos2 || pos1 > array->size || pos2 > array->size)
        return;
    
    __ptr_t temp1 = array->content[pos1];
    bool temp2 = array->deepCopied[pos1];
    array->content[pos1] = array->content[pos2];
    array->deepCopied[pos1] = array->deepCopied[pos2];
    array->content[pos1] = temp1;
    array->deepCopied[pos2] = temp2;
}


static void aux_array_merge(T_ARRAY* array, size_t beginLeft, size_t endLeft, size_t beginRight, size_t endRight, T_FUNC_COMPARE cmp) {
    size_t size = endRight - beginLeft + 1;
    size_t i_left = beginLeft, i_right = beginRight, i_aux = 0;
    __ptr_t elem_aux[size];
    bool deep_aux[size];

    while (i_left <= endLeft && i_right <= endRight) {
        __ptr_t elem_left = array->content[i_left];
        __ptr_t elem_right = array->content[i_right];
        bool deep_left = array->deepCopied[i_left];
        bool deep_right = array->deepCopied[i_right];

        if (cmp(elem_left, elem_right) <= 0) {
            elem_aux[i_aux] = elem_left;
            deep_aux[i_aux] = deep_left;
            i_left++;
        } else {
            elem_aux[i_aux] = elem_right;
            deep_aux[i_aux] = deep_right;
            i_right++;
        }

        i_aux++;
    }
    
    while (i_left <= endLeft) {
        i_left++;
        i_aux++;
    }

    while (i_right <= endRight) {
        i_right++;
        i_aux++;
    }

    memcpy(array->content + beginLeft, elem_aux, size * sizeof(__ptr_t));
    memcpy(array->deepCopied + beginLeft, deep_aux, size * sizeof(bool));
}


static void aux_array_mergeSort(T_ARRAY* array, size_t begin, size_t end, T_FUNC_COMPARE cmp) {
    if (begin == end || end >= array->size)
        return;
    else if (begin + 1 == end) {
        aux_array_swapElements(array, begin, end);
        return;
    }

    size_t mid = (begin + end) >> 1;
    size_t begin_left = begin, end_left = mid;
    size_t begin_right = mid + 1, end_right = end;

    aux_array_mergeSort(array, begin_left, end_left, cmp);
    aux_array_mergeSort(array, begin_right, end_right, cmp);
    aux_array_merge(array, begin_left, end_left, begin_right, end_right, cmp);
}



/*====================================================================================================*/
/*----------------------------------------- HEADER FUNCTIONS -----------------------------------------*/
/*====================================================================================================*/


T_ARRAY* new_array(type_t type, size_t initialCapacity) {
    if (initialCapacity < 4) initialCapacity = 4;

    T_ARRAY* array = malloc(sizeof(T_ARRAY));
    if (array == NULL) return NULL;

    array->content = calloc(initialCapacity, sizeof(__ptr_t));
    array->deepCopied = calloc(initialCapacity, sizeof(bool));
    if (array->content == NULL || array->deepCopied == NULL) {
        free(array->content);
        free(array->deepCopied);
        free(array);
        return NULL;
    }

    array->size = 0;
    array->capacity = initialCapacity;
    array->type = type;
    array->deepCopyMode = false;
    return array;
}


T_ARRAY* clone_array(const T_ARRAY* array) {
    if (array == NULL) return NULL;

    T_ARRAY* cloned = new_array(array->type, array->capacity);
    if (cloned == NULL) return NULL;

    cloned->deepCopyMode = array->deepCopyMode;
    
    if (!array_extend(cloned, array, false)) {
        delete_array(cloned);
        return NULL;
    }

    return cloned;
}


void delete_array(T_ARRAY* array) {
    if (array == NULL) return;

    array_clear(array);
    free(array->content);
    free(array->deepCopied);
    free(array);
}


int32_t compare_array(const T_ARRAY* array1, const T_ARRAY* array2) {
    if (array1 == NULL || array2 == NULL)
        return compare_pointer(array1, array2);
    
    int32_t diff = (int32_t) (array1->size - array2->size);
    if (diff != 0) return diff;

    diff = (int32_t) (array1->type - array2->type);
    if (diff != 0) return diff;

    T_FUNC_COMPARE cmp = type_getCompareFunction(array1->type);
    if (cmp == NULL) cmp = compare_pointer;

    for (size_t i = 0; i < array1->size; i++) {
        __ptr_t elem1 = array1->content[i];
        __ptr_t elem2 = array2->content[i];

        if (elem1 == NULL || elem2 == NULL)
            diff = compare_pointer(elem1, elem2);
        else
            diff = cmp(elem1, elem2);

        if (diff != 0) return diff;
    }

    return diff;
}


bool array_enableDeepCopyMode(T_ARRAY* array, bool enabled) {
    if (array == NULL) return false;

    array->deepCopyMode = enabled;
    return true;
}


bool array_markElementAsDeepCopied(T_ARRAY* array, size_t pos, bool deepCopied) {
    if (array == NULL || pos >= array->size) return false;

    array->deepCopied[pos] = deepCopied;
    return true;
}


void array_clear(T_ARRAY* array) {
    if (array == NULL) return;

    T_FUNC_DELETE delete = type_getDeleteFunction(array->type);

    while (array->size > 0) {
        __ptr_t element = array_removeLast(array);
        size_t pos_last = array->size;

        if (element != NULL && delete != NULL && array->deepCopied[pos_last]) {
            delete(element);
            array->content[pos_last] = NULL;
            array->deepCopied[pos_last] = false;
        }
    }
}


size_t array_size(const T_ARRAY* array) {
    return (array == NULL) ? 0 : array->size;
}


bool array_add(T_ARRAY* array, __ptr_t element, size_t pos) {
    if (array == NULL || pos > array->size) return false;

    if (array->size == array->capacity) {
        if (!aux_array_reallocate(array, array->capacity * 2))
            return false;
    }

    T_FUNC_CLONE clone = type_getCloneFunction(array->type);
    __ptr_t elemAux = element;
    bool cloned = false;

    if (clone != NULL && element != NULL && array->deepCopyMode) {
        elemAux = clone(element);
        if (elemAux == NULL)
            return false;
        cloned = true;
    }

    for (size_t i = array->size; i > pos; i--) {
        array->content[i] = array->content[i - 1];
        array->deepCopied[i] = array->deepCopied[i - 1];
    }

    array->content[pos] = elemAux;
    array->deepCopied[pos] = cloned;
    array->size++;
    return true;
}


bool array_addFirst(T_ARRAY* array, __ptr_t element) {
    return array_add(array, element, 0);
}


bool array_addLast(T_ARRAY* array, __ptr_t element) {
    return array_add(array, element, array->size);
}


__ptr_t array_get(const T_ARRAY* array, size_t pos) {
    return (array == NULL) ? NULL : array->content[pos];
}


__ptr_t array_getFirst(const T_ARRAY* array) {
    return (array == NULL) ? NULL : array->content[0];
}


__ptr_t array_getLast(const T_ARRAY* array) {
    return (array == NULL) ? NULL : array->content[array->size - 1];
}


__ptr_t array_getRandom(const T_ARRAY* array) {
    return (array == NULL) ? NULL : array->content[(size_t) rand() % array->size];
}


__ptr_t array_remove(T_ARRAY* array, size_t pos) {
    if (array == NULL || pos >= array->size) return NULL;

    __ptr_t removed = array->content[pos];

    for (size_t i = pos; i < array->size - 1; i++) {
        array->content[i] = array->content[i + 1];
        array->deepCopied[i] = array->deepCopied[i + 1];
    }

    array->size--;
    return removed;
}


__ptr_t array_removeFirst(T_ARRAY* array) {
    return array_remove(array, 0);
}


__ptr_t array_removeLast(T_ARRAY* array) {
    return array_remove(array, array->size - 1);
}


__ptr_t array_find(const T_ARRAY* array, T_FUNC_COMPARE cmp, const __ptr_t criteria, size_t* pos) {
    if (array == NULL) return NULL;

    if (cmp == NULL) cmp = compare_pointer;
    size_t pos_found = 0;

    while (pos_found < array->size && cmp(array->content[pos_found], criteria) != 0)
        pos_found++;

    if (pos != NULL) *pos = pos_found;
    return (pos_found >= array->size) ? NULL : array->content[pos_found];
}


int64_t array_sum(const T_ARRAY* array, int64_t (*func)(__ptr_t, __ptr_t), __ptr_t arg) {
    if (array == NULL || func == NULL) return -1;

    int64_t sum = 0;
    for (size_t i = 0; i < array->size; i++)
        sum += func(array->content[i], arg);

    return sum;
}


T_ARRAY* array_filter(const T_ARRAY* array, bool (*filter)(__ptr_t, __ptr_t), __ptr_t arg) {
    if (array == NULL || filter == NULL) return NULL;

    T_ARRAY* filtered = new_array(array->type, array->size);
    if (filtered == NULL) return NULL;

    filtered->deepCopyMode = array->deepCopyMode;

    for (size_t i = 0; i < array->size; i++) {
        __ptr_t element = array->content[i];

        if (filter(element, arg)) {
            if (!array_addLast(filtered, element)) {
                delete_array(filtered);
                return NULL;
            }
        }
    }

    return filtered;
}


T_ARRAY* array_extend(T_ARRAY* array, const T_ARRAY* newElements, bool cloneArray) {
    if (array->type != newElements->type) return NULL;
    
    array = cloneArray ? clone_array(array) : array;
    if (array == NULL) return NULL;

    if (newElements->size > array->capacity - array->size) {
        if (!aux_array_reallocate(array, array->size + newElements->size)) {
            if (cloneArray) delete_array(array);
            return NULL;
        }
    }

    bool deepCopyModeAux = array->deepCopyMode;
    array->deepCopyMode = newElements->deepCopyMode;
    funcDelete_t delete = type_getDeleteFunction(array->type);

    for (size_t i = 0; i < newElements->size; i++) {
        __ptr_t element = newElements->content[i];

        if (!array_addLast(array, element)) {
            array->deepCopyMode = deepCopyModeAux;

            if (cloneArray)
                delete_array(array);
            else {
                for (size_t j = 0; j < i; j++) {
                    element = array_removeLast(array);
                    if (delete != NULL && element != NULL && array->deepCopied[array->size])
                        delete(element);
                }
            }

            return NULL;
        }
    }

    array->deepCopyMode = deepCopyModeAux;
    return array;
}


T_ARRAY* array_reverse(T_ARRAY* array, bool cloneArray) {
    array = cloneArray ? clone_array(array) : array;
    if (array == NULL) return NULL;

    size_t limit = array->size >> 1;
    for (size_t i = 0; i < limit; i++)
        aux_array_swapElements(array, i, array->size - i - 1);

    return array;
}


T_ARRAY* array_sort(T_ARRAY* array, bool cloneArray) {
    return array_sortByKey(array, type_getCompareFunction(array->type), cloneArray);
}


T_ARRAY* array_sortByKey(T_ARRAY* array, T_FUNC_COMPARE cmp, bool cloneArray) {
    array = cloneArray ? clone_array(array) : array;
    if (array == NULL) return NULL;

    if (cmp == NULL) cmp = compare_pointer;

    aux_array_mergeSort(array, 0, array->size - 1, cmp);
    return array;
}


T_ARRAY* array_shuffle(T_ARRAY* array, bool cloneArray) {
    array = cloneArray ? clone_array(array) : array;
    if (array == NULL) return NULL;

    for (size_t i = 0; i < array->size; i++)
        aux_array_swapElements(array, i, (size_t) rand() % array->size);

    return array;
}