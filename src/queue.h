
#pragma once

#include "defs.h"

struct Queue_t;
typedef struct Queue_t Queue;

typedef void (*QueueDeallocator)(void*);
typedef unsigned long (*QueueCurrentTimeMillisGetter)(void);

Queue* queueInit(QueueDeallocator nullable deallocator);
void queuePush(Queue* queue, const void* value);
void* queuePop(Queue* queue); // returns stored value that must be deallocated by a caller as reference to the value gets deleted, the queue must not be empty
void* nullable queuePeek(Queue* queue); // returns the top value if the queue is not empty and null if it is but doesn't remove the top value
void queueDropTop(Queue* queue); // drops the top value and deallocates it
int queueSize(const Queue* queue); // Queue is const here 'cause it's mutex isn't modified
void queueClear(Queue* queue);
void queueDestroy(Queue* queue); // all values that are still remain inside a queue at a time destroy is called are deallocated via supplied deallocator if it's not null
