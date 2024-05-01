
#pragma once

#include <stdbool.h>
#include "defs.h"

struct List_t;
typedef struct List_t List;

typedef void (*ListDeallocator)(void*);
typedef int (*ListComparator)(const void*, const void*);
typedef void* (*ListItemDuplicator)(const void*);

List* listInit(ListDeallocator nullable deallocator);
List* listCopy(List* old, ListItemDuplicator itemDuplicator);
void listAddBack(List* list, const void* value);
void listAddFront(List* list, const void* value);
const void* listGet(List* list, int index); // list is actually treated as non-const 'cause mutex inside it is being modified
int listSize(const List* list); // but not here as mutex isn't modified here
const void* nullable listBinarySearch(List* list, const void* key, ListComparator comparator);
void listClear(List* list);
void listDestroy(List* list); // all values that are still remain inside a queue at a time destroy is called are deallocated via supplied deallocator if it's not null
