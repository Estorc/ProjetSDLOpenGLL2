/**
 * @file queue.h
 * @brief Queue data structure and related operations.
 *
 * This file contains the definition of a simple queue data structure and
 * functions to manipulate it. The queue is implemented as a singly linked list.
 */

/**
 * @defgroup Queue Queue Data Structure
 * @brief A simple queue data structure.
 *
 * This module provides a simple queue data structure and functions to
 * manipulate it. The queue is implemented as a singly linked list where each
 * node contains a pointer to the next node and a pointer to the data.
 * @{
 */

/**
 * @struct Queue
 * @brief A node in the queue.
 *
 * This structure represents a node in the queue. Each node contains a pointer
 * to the next node in the queue and a pointer to the data stored in the node.
 */

typedef struct Queue {
    struct Queue *next; /**< Pointer to the next node in the queue. */
    void *data; /**< Pointer to the data stored in the node. */
} Queue;


/**
 * @brief Checks if the queue is empty.
 *
 * This function checks if the given queue is empty.
 *
 * @param queue Pointer to the queue to check.
 * @return true if the queue is empty, false otherwise.
 */
bool queue_is_empty(Queue *queue);

/**
 * @brief Pushes data onto the queue.
 *
 * This function adds a new node with the given data to the end of the queue.
 *
 * @param queue Pointer to the queue to push data onto.
 * @param data Pointer to the data to be added to the queue.
 */
void queue_push(Queue *queue, void *data);

/**
 * @brief Pops data from the queue.
 *
 * This function removes the node at the front of the queue and returns the
 * data stored in that node. If the queue is empty, the behavior is undefined.
 *
 * @param queue Pointer to the queue to pop data from.
 * @return Pointer to the data stored in the node that was removed from the queue.
 */
void *queue_pop(Queue *queue);

/**
 * @brief Frees the queue.
 *
 * This function frees all the nodes in the queue. The data stored in the nodes
 * is not freed.
 *
 * @param queue Pointer to the queue to be freed.
 */
void queue_free(Queue *queue);

/**
 * @brief External queue instance.
 *
 * This is an external instance of a queue that can be used globally.
 */
extern Queue callQueue;

/** @} */ // end of Queue group