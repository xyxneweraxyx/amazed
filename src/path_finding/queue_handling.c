/*
** EPITECH PROJECT, 2026
** BFS_QUEUE_BUILDING
** File description:
** BFS_QUEUE_BUILDING
*/

#include "../../include/amazed.h"
#include <stdlib.h>

queue_t *create_queue(main_t *main, int size, queue_t *queue)
{
    if (size <= 0 || !main)
        return NULL;
    queue = malloc(sizeof(queue_t));
    queue->queued = malloc(sizeof(room_t *) * (size + 1));
    queue->head_ind = 0;
    queue->rear_ind = 0;
    queue->size = size;
    main->start->dist = 0;
    main->start->visited = 1;
    return queue;
}

//
// Move the rear_ind and fill it with the room to enqueue
//
int enqueue(queue_t *queue, room_t *room)
{
    if (!queue || !room)
        return -1;
    if (queue->rear_ind >= queue->size)
        return -1;
    queue->queued[queue->rear_ind] = room;
    queue->rear_ind++;
    return 0;
}

//
// Enqueue all neighbors of room and assign room as their prec
//
int enqueue_neighbors(queue_t *queue, room_t *room)
{
    int i = 0;
    room_t *n;

    if (!room || !queue || !room->neighboors)
        return 84;
    while (room->neighboors[i]) {
        n = room->neighboors[i];
        if (!n->visited && !n->blocked && enqueue(queue, n) == 0) {
            n->visited = 1;
            n->prec = room;
            n->dist = room->dist + 1;
        }
        i++;
    }
    return 0;
}

//
// Set the room to visited and move the head to the next room
// DOESNT DELETE/FREE THE ROOM
//
room_t *dequeue(queue_t *queue)
{
    room_t *room;

    if (!queue || queue->head_ind >= queue->rear_ind)
        return NULL;
    room = queue->queued[queue->head_ind];
    queue->head_ind++;
    return room;
}

int free_queue(queue_t *queue)
{
    if (!queue || !queue->queued)
        return 84;
    free(queue->queued);
    free(queue);
    return 0;
}
