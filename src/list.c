
#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include "list.h"

static const int VOID_PTR_SIZE = sizeof(void*);
static const int MAX_SIZE = (int) 0xfffffffe;

struct List_t {
    void** values;
    int size;
    ListDeallocator nullable deallocator;
    bool destroyed;
};

List* listInit(ListDeallocator nullable deallocator) {
    List* list = SDL_malloc(sizeof *list);
    list->values = nullptr;
    list->size = 0;
    list->deallocator = deallocator;
    list->destroyed = false;
    return list;
}

List* listCopy(List* old, ListItemDuplicator itemDuplicator) {
    assert(!old->destroyed);
    List* new = listInit(old->deallocator);

    const unsigned size = old->size;
    for (unsigned i = 0; i < size; i++)
        listAddBack(new, (*itemDuplicator)(old->values[i]));

    return new;
}

void listAddBack(List* list, const void* value) {
    assert(!list->destroyed && list->size >= 0 && list->size < MAX_SIZE);

    list->values = SDL_realloc(list->values, ++(list->size) * VOID_PTR_SIZE);
    list->values[list->size - 1] = (void*) value;
}

void listAddFront(List* list, const void* value) {
    assert(!list->destroyed && list->size >= 0 && list->size < MAX_SIZE);

    void** temp = SDL_malloc(++(list->size) * VOID_PTR_SIZE);
    temp[0] = (void*) value;
    for (int i = 1; i < list->size; temp[i] = (list->values)[i - 1], i++);

    SDL_free(list->values);
    list->values = temp;
}

const void* listGet(List* list, int index) {
    assert(!list->destroyed && list->size > 0 && list->size < MAX_SIZE && index < MAX_SIZE);

    assert(list->values != nullptr);
    const void* value = list->values[index];

    return value;
}

int listSize(const List* list) {
    assert(!list->destroyed);
    return list->size;
}

const void* nullable listBinarySearch(List* list, const void* key, ListComparator comparator) {
    assert(!list->destroyed && list->size > 0);
    assert(list->values != nullptr);

    const long index =
        (void**) SDL_bsearch(key, list->values, list->size, VOID_PTR_SIZE, comparator) - list->values;

    const void* value = index >= list->size ? NULL : list->values[index];
    return value;
}

static void destroyValuesIfNotEmpty(List* list) {
    if (!list->deallocator) return;
    assert(list->size > 0 && list->values != nullptr || list->size == 0 && list->values == nullptr);
    for (int i = 0; i < list->size; (*(list->deallocator))(list->values[i++]));
}

void listClear(List* list) {
    assert(!list->destroyed);

    destroyValuesIfNotEmpty(list);
    list->size = 0;

    SDL_free(list->values);
    list->values = nullptr;
}

void listDestroy(List* list) {
    assert(!list->destroyed);
    list->destroyed = true;

    destroyValuesIfNotEmpty(list);
    SDL_free(list->values);
    SDL_free(list);
}
