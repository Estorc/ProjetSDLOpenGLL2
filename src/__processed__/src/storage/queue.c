#line 1 "src/storage/queue.c"
#include "../raptiquax.h"
#include "queue.h"

void queue_push(Queue *queue, void *data) {
    Queue *new_element = malloc(sizeof(Queue));
    new_element->data = data;
    new_element->next = NULL;
    if (queue->next == NULL) {
        queue->next = new_element;
    } else {
        Queue *cursor = queue->next;
        while (cursor->next != NULL) {
            cursor = cursor->next;
        }
        cursor->next = new_element;
    }
}

bool queue_is_empty(Queue *queue) {
    return queue->next == NULL;
}

void *queue_pop(Queue *queue) {
    void * data = queue->next->data;
    Queue *old = queue->next;
    queue->next = queue->next->next;
    free(old);
    return data;
}

void queue_free(Queue *queue) {
    Queue *cursor = queue->next;
    while (cursor->next != NULL) {
        Queue *next = cursor->next;
        free(cursor);
        cursor = next;
    }
}