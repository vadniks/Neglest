
#include <SDL2/SDL_stdinc.h>
#include <assert.h>
#include <stdbool.h>
#include "queue.h"

static const int VOID_PTR_SIZE = sizeof(void*);

struct Queue_t {
    void** values;
    int size;
    QueueDeallocator nullable deallocator;
    bool destroyed;
};

Queue* queueInit(QueueDeallocator nullable deallocator) {
    Queue* queue = SDL_malloc(sizeof *queue);
    queue->values = nullptr;
    queue->size = 0;
    queue->deallocator = deallocator;
    queue->destroyed = false;
    return queue;
}

void queuePush(Queue* queue, const void* value) {
    assert(!queue->destroyed && queue->size >= 0 && queue->size < (int) 0xfffffffe);

    queue->values = SDL_realloc(queue->values, ++(queue->size) * VOID_PTR_SIZE);
    queue->values[queue->size - 1] = (void*) value;
}

void* queuePop(Queue* queue) {
    assert(!queue->destroyed && queue->size > 0);
    assert(queue->values != nullptr);

    void* value = queue->values[0];

    const int newSize = queue->size - 1;
    if (!newSize) {
        SDL_free(queue->values);
        queue->values = NULL;
        queue->size = 0;
        return value;
    }

    void** temp = SDL_malloc(newSize * VOID_PTR_SIZE);
    SDL_memcpy(temp, &(queue->values[1]), newSize * VOID_PTR_SIZE);

    SDL_free(queue->values);
    queue->values = temp;
    queue->size = newSize;

    return value;
}

void* nullable queuePeek(Queue* queue) {
    assert(!queue->destroyed);
    void* result = NULL;

    if (queue->size)
        result = queue->values[0];

    return result;
}

void queueDropTop(Queue* queue) { (*(queue->deallocator))(queuePop(queue)); }

int queueSize(const Queue* queue) {
    assert(!queue->destroyed);
    return queue->size;
}

static void destroyValuesIfNotEmpty(Queue* queue) {
    if (!queue->deallocator) return;
    assert(queue->size && queue->values || !(queue->size) && !(queue->values));
    for (int i = 0; i < queue->size; (*(queue->deallocator))(queue->values[i++]));
}

void queueClear(Queue* queue) {
    assert(!queue->destroyed);

    destroyValuesIfNotEmpty(queue);
    queue->size = 0;

    SDL_free(queue->values);
    queue->values = nullptr;
}

void queueDestroy(Queue* queue) {
    assert(!queue->destroyed);
    queue->destroyed = true;

    destroyValuesIfNotEmpty(queue);
    SDL_free(queue->values);
    SDL_free(queue);
}
