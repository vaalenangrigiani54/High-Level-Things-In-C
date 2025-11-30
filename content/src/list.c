#include <dataStructures/list.h>
#include <dataStructures/array.h>
#include <dataStructures/tuple.h>
#include <comparators.h>


typedef struct __listNode__ {
    __ptr_t element;
    struct __listNode__* prev;
    struct __listNode__* next;
    bool deepCopied;
} listNode_t;

struct __listIter__ {
    listNode_t* node;
    size_t pos;
    size_t* list_size;
};

struct __list__ {
    listNode_t* first;
    listNode_t* last;
    size_t size;
    struct __listIter__ iter;
    type_t type;
    bool deepCopyMode;
};


/*===================================================================================================*/
/*--------------------------------------- AUXILIARY FUNCTIONS ---------------------------------------*/
/*===================================================================================================*/


static listNode_t* listAux_createNode(T_LIST* list, __ptr_t element) {
    listNode_t* node = calloc(1, sizeof(listNode_t));
    if (node == NULL) return NULL;

    T_FUNC_CLONE clone = type_getCloneFunction(list->type);
    if (list->deepCopyMode && element != NULL && clone != NULL) {
        node->element = clone(element);
        if (node->element == NULL) {
            free(node);
            return NULL;
        }
    } else
        node->element = element;

    return node;
}


static void listAux_addNodeBetween(T_LIST* list, listNode_t* nodeToAdd, listNode_t* nodeLeft, listNode_t* nodeRight) {
    nodeToAdd->prev = nodeLeft;
    nodeToAdd->next = nodeRight;
    
    if (nodeLeft == NULL)
        list->first = nodeToAdd;
    else
        nodeLeft->next = nodeToAdd;

    if (nodeRight == NULL)
        list->last = nodeToAdd;
    else
        nodeRight->prev = nodeToAdd;
}


static listNode_t* listAux_getNodeAtPos(const T_LIST* list, size_t pos) {
    bool fromLeft = pos < list->size >> 1;
    listNode_t* node;
    size_t i;

    if (fromLeft) {
        node = list->first;
        i = 0;
    } else {
        node = list->last;
        i = list->size - 1;
    }

    while (i != pos) {
        if (fromLeft) {
            node = node->next;
            i++;
        } else {
            node = node->prev;
            i--;
        }
    }

    return node;
}


static void listAux_removeNode(T_LIST* list, listNode_t* node) {
    if (node == NULL) return NULL;

    listNode_t* prevNode = node->prev;
    listNode_t* nextNode = node->next;

    if (prevNode == NULL)
        list->first = nextNode;
    else
        prevNode->next = nextNode;

    if (nextNode == NULL)
        list->last = prevNode;
    else
        nextNode->prev = prevNode;
}


static T_LIST* listAux_extendRightOrLeft(T_LIST* list, const T_LIST* newElements, bool (*add)(T_LIST*, __ptr_t), bool listWasCloned) {
    bool deepCopyModeAux = list->deepCopyMode;
    list->deepCopyMode = newElements->deepCopyMode;
    listNode_t* node = newElements->first;

    while (node != NULL) {
        if (!add(list, node->element)) {
            if (listWasCloned)
                delete_list(list);
            
            list->deepCopyMode = deepCopyModeAux;
            return NULL;
        }

        node = node->next;
    }
    
    list->deepCopyMode = deepCopyModeAux;
    return list;
}


static void listAux_swapElements(listNode_t* node1, listNode_t* node2) {
    __ptr_t elemAux = node1->element;
    bool deepCopiedAux = node1->deepCopied;
    node1->element = node2->element;
    node1->deepCopied = node2->deepCopied;
    node2->element = elemAux;
    node2->deepCopied = deepCopiedAux;
}


static void listAux_mergeSort_combine(T_LIST* list, T_FUNC_COMPARE cmp, listNode_t* first, listNode_t* half, listNode_t* last) {
    listNode_t* left = first;
    listNode_t* right = half->next;

    while (left != right && right != last->next) {
        bool leftGoesFirst = (cmp == NULL) ? (rand() % 2) : (cmp(left->element, right->element) <= 0);

        if (leftGoesFirst)
            left = left->next;
        else {
            listNode_t* nodeToSwap = right;
            right = right->next;

            listAux_removeNode(list, nodeToSwap);
            listAux_addNodeBetween(list, nodeToSwap, left->prev, left);
        }
    }
}


static void listAux_mergeSort(T_LIST* list, T_FUNC_COMPARE cmp, listNode_t* first, listNode_t* last) {
    if (first == last) return;
    if (first->next == last) {
        if (cmp(first, last) > 0)
            listAux_swapElements(first, last);
        return;
    }

    listNode_t* left = first;
    listNode_t* right = last;
    listNode_t* half;
    
    while (left != right && left->next != right) {
        left = left->next;
        right = right->prev;
        half = left;
    }
    
    listAux_mergeSort(list, cmp, first, half);
    listAux_mergeSort(list, cmp, half->next, last);
    listAux_mergeSort_combine(list, cmp, first, half, last);
}



/*====================================================================================================*/
/*----------------------------------------- HEADER FUNCTIONS -----------------------------------------*/
/*====================================================================================================*/


T_LIST* new_list(type_t type) {
    T_LIST* list = calloc(1, sizeof(T_LIST));
    if (list == NULL) return NULL;

    list->iter.list_size = &list->size;
    list->type = type;
    return list;
}


T_LIST* new_list_fromArray(const T_ARRAY* array) {
    if (array == NULL) return NULL;

    T_LIST* list = new_list(array_type(array));
    if (list == NULL) return NULL;

    list->deepCopyMode = array_deepCopyMode(array);

    for (size_t i = 0; i < list->size; i++) {
        if (!list_addLast(list, array_get(array, i))) {
            delete_list(list);
            return NULL;
        }
    }

    return list;
}


T_LIST* new_list_fromTuple(const T_TUPLE* tuple) {
    if (tuple == NULL) return NULL;

    T_LIST* list = new_list(tuple_type(tuple));
    if (list == NULL) return NULL;

    list->deepCopyMode = tuple_deepCopyMode(tuple);

    for (size_t i = 0; i < list->size; i++) {
        if (!list_addLast(list, tuple_get(tuple, i))) {
            delete_list(list);
            return NULL;
        }
    }

    return list;
}


T_LIST* clone_list(const T_LIST* list) {
    if (list == NULL) return NULL;

    T_LIST* cloned = new_list(list->type);
    if (cloned == NULL) return NULL;

    cloned->deepCopyMode = list->deepCopyMode;

    if (!list_extendRight(cloned, list, false)) {
        delete_list(cloned);
        return NULL;
    }

    return cloned;
}


void delete_list(T_LIST* list) {
    list_clear(list);
    free(list);
}


int32_t compare_list(const T_LIST* list1, const T_LIST* list2) {
    if (list1 == NULL || list2 == NULL)
        return compare_pointer(list1, list2);
    
    int32_t diff = (int32_t) (list1->size - list2->size);
    if (diff != 0) return diff;

    diff = (int32_t) (list1->type - list2->type);
    if (diff != 0) return diff;

    T_FUNC_COMPARE cmp = type_getCompareFunction(list1->type);
    if (cmp == NULL) cmp = compare_pointer;

    listNode_t* node1 = list1->first;
    listNode_t* node2 = list2->first;

    while (node1 != NULL && node2 != NULL) {
        __ptr_t elem1 = node1->element;
        __ptr_t elem2 = node2->element;

        if (elem1 == NULL || elem2 == NULL)
            diff = compare_pointer(elem1, elem2);
        else
            diff = cmp(elem1, elem2);

        if (diff != 0) return diff;

        node1 = node1->next;
        node2 = node2->next;
    }

    return 0;
}


bool list_enableDeepCopyMode(T_LIST* list, bool enabled) {
    if (list == NULL) return false;

    list->deepCopyMode = enabled;
    return true;
}


bool list_deepCopyMode(const T_LIST* list) {
    return (list == NULL) ? false : list->deepCopyMode;
}


bool list_markElementAsDeepCopied(T_LIST* list, size_t pos, bool deepCopied) {
    if (list == NULL || pos >= list->size) return false;

    listNode_t* node = listAux_getNodeAtPos(list, pos);
    if (node == NULL) return false;

    node->deepCopied = deepCopied;
    return true;
}


void list_clear(T_LIST* list) {
    if (list == NULL) return;

    T_FUNC_DELETE delete = type_getDeleteFunction(list->type);
    listNode_t* node = list->first;

    while (node != NULL) {
        list->first = list->first->next;

        if (node->element != NULL && delete != NULL && node->deepCopied)
            delete(node->element);

        free(node);
        node = list->first;
    }

    list->last = NULL;
    list->size = 0;
}


size_t list_size(const T_LIST* list) {
    return (list == NULL) ? 0 : list->size;
}


type_t list_type(const T_LIST* list) {
    return (list == NULL) ? TYPE_ARBITRARY : list->type;
}


bool list_add(T_LIST* list, __ptr_t element, size_t pos) {
    if (list == NULL || pos > list->size) return false;
    if (pos == 0) return list_addFirst(list, element);
    if (pos == list->size) return list_addLast(list, element);

    listNode_t* node = listAux_createNode(list, element);
    if (node == NULL) return false;

    listNode_t* nextNode = listAux_getNodeAtPos(list, pos);
    listNode_t* prevNode = (nextNode == NULL) ? list->last : nextNode->prev;
    
    listAux_addNodeBetween(list, node, prevNode, nextNode);
    list->size++;
    return true;
}


bool list_addFirst(T_LIST* list, __ptr_t element) {
    return list_add(list, element, 0);
}


bool list_addLast(T_LIST* list, __ptr_t element) {
    return list_add(list, element, list->size);
}


__ptr_t list_get(const T_LIST* list, size_t pos) {
    if (list == NULL || pos >= list->size) return NULL;

    listNode_t* node = listAux_getNodeAtPos(list, pos);
    return (node == NULL) ? NULL : node->element;
}


__ptr_t list_getFirst(const T_LIST* list) {
    return list_get(list, 0);
}


__ptr_t list_getLast(const T_LIST* list) {
    return list_get(list, list->size - 1);
}


__ptr_t list_getRandom(const T_LIST* list) {
    return list_get(list, (size_t) rand() % list->size);
}


__ptr_t list_remove(T_LIST* list, size_t pos) {
    if (list == NULL || pos >= list->size) return NULL;

    listNode_t* node = listAux_getNodeAtPos(list, pos);
    if (node == NULL) return NULL;

    listAux_removeNode(list, node);
    __ptr_t removed = node->element;

    free(node);
    list->size--;
    return removed;
}


__ptr_t list_removeFirst(T_LIST* list) {
    return list_remove(list, 0);
}


__ptr_t list_removeLast(T_LIST* list) {
    return list_remove(list, list->size - 1);
}


__ptr_t list_find(const T_LIST* list, T_FUNC_COMPARE cmp, const __ptr_t criteria, size_t* pos) {
    if (list == NULL) return NULL;
    if (cmp == NULL) cmp = compare_pointer;

    size_t _pos = 0;
    listNode_t* node = list->first;
    __ptr_t found = NULL;

    while (node != NULL && cmp(node->element, criteria) != 0) {
        _pos++;
        node = node->next;
    }

    if (node != NULL) found = node->element;
    if (pos != NULL) *pos = _pos;

    return found;
}


int64_t list_sum(const T_LIST* list, int64_t (*func)(__ptr_t, __ptr_t), __ptr_t arg) {
    if (list == NULL || func == NULL) return 0;

    int64_t sum = 0;
    listNode_t* node = list->first;

    while (node != NULL) {
        sum += func(node->element, arg);
        node = node->next;
    }

    return sum;
}


T_LIST* list_filter(const T_LIST* list, bool (*filter)(__ptr_t, __ptr_t), __ptr_t arg) {
    if (list == NULL || filter == NULL) return NULL;
    
    T_LIST* filtered = new_list(list->type);
    if (filtered == NULL) return NULL;

    filtered->deepCopyMode = list->deepCopyMode;
    listNode_t* node = list->first;

    while (node != NULL) {
        if (filter(node->element, arg)) {
            if (!list_addLast(filtered, node->element)) {
                delete_list(filtered);
                return NULL;
            }
        }

        node = node->next;
    }

    return filtered;
}


T_LIST* list_extendLeft(T_LIST* list, const T_LIST* newElements, bool cloneList) {
    list = (cloneList) ? clone_list(list) : list;
    return (list == NULL) ? NULL : listAux_extendRightOrLeft(list, newElements, list_addFirst, cloneList);
}


T_LIST* list_extendRight(T_LIST* list, const T_LIST* newElements, bool cloneList) {
    list = (cloneList) ? clone_list(list) : list;
    return (list == NULL) ? NULL : listAux_extendRightOrLeft(list, newElements, list_addLast, cloneList);
}


T_LIST* list_reverse(T_LIST* list, bool cloneList) {
    list = (cloneList) ? clone_list(list) : list;
    if (list == NULL) return NULL;

    size_t i = 0, limit = list->size >> 1;
    listNode_t* left = list->first;
    listNode_t* right = list->last;

    while (i < limit) {
        listAux_swapElements(left, right);

        i++;
        left = left->next;
        right = right->prev;
    }

    return list;
}


T_LIST* list_sort(T_LIST* list, bool cloneList) {
    return list_sortByKey(list, type_getCompareFunction(list->type), cloneList);
}


T_LIST* list_sortByKey(T_LIST* list, T_FUNC_COMPARE cmp, bool cloneList) {
    list = (cloneList) ? clone_list(list) : list;
    if (list == NULL) return NULL;

    if (cmp == NULL) cmp = compare_pointer;
    
    if (list->size >= 2)
        listAux_mergeSort(list, cmp, list->first, list->last);
    
    return list;
}


T_LIST* list_shuffle(T_LIST* list, bool cloneList) {
    list = (cloneList) ? clone_list(list) : list;
    if (list == NULL) return NULL;

    if (list->size >= 2)
        listAux_mergeSort(list, NULL, list->first, list->last);

    return list;
}


listIterator_t listIter_begin(T_LIST* associated) {
    if (associated == NULL) return NULL;

    associated->iter.node = associated->first;
    associated->iter.pos = 0;

    return &associated->iter;
}


listIterator_t listIter_end(T_LIST* associated) {
    if (associated == NULL) return NULL;

    associated->iter.node = associated->last;
    associated->iter.pos = associated->size - 1;
    
    return &associated->iter;
}


bool listIter_next(listIterator_t iter) {
    if (!listIter_hasNext(iter)) return false;

    iter->node = iter->node->next;
    iter->pos += 1;

    return true;
}


bool listIter_back(listIterator_t iter) {
    if (!listIter_hasNext(iter)) return false;

    iter->node = iter->node->prev;
    iter->pos -= 1;
    
    return true;
}


bool listIter_hasNext(const listIterator_t iter) {
    return (iter != NULL && iter->node != NULL && iter->pos >= iter->list_size);
}


__ptr_t listIter_get(const listIterator_t iter) {
    return (iter == NULL || iter->node == NULL) ? NULL : iter->node->element;
}


size_t listIter_getPos(const listIterator_t iter) {
    return (iter == NULL) ? 0 : iter->pos;
}