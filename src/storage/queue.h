typedef struct Queue {
    struct Queue *next;
    void *data;
} Queue;

bool queue_is_empty(Queue *queue);
void queue_push(Queue *queue, void *data);
void *queue_pop(Queue *queue);
void queue_free(Queue *queue);

extern Queue callQueue;