/*
** EPITECH PROJECT, 2026
** BFS_QUEUE_BUILDING
** File description:
** BFS_QUEUE_BUILDING
*/
#include "../../include/amazed.h"
#include <stdlib.h>

void block_path(main_t *main, room_t *end)
{
    room_t *tmp = end->prec;

    while (tmp && tmp != main->start) {
        tmp->blocked = 1;
        tmp = tmp->prec;
    }
}

room_t *walkthrough_rooms(main_t *main)
{
    queue_t *queue = NULL;
    room_t *current = NULL;

    if (!main)
        return NULL;
    queue = create_queue(main, main->room_amount * 2 + 10, NULL);
    if (!queue)
        return NULL;
    enqueue(queue, main->start);
    while (queue->head_ind < queue->rear_ind) {
        current = dequeue(queue);
        if (!current)
            break;
        enqueue_neighbors(queue, current);
    }
    free_queue(queue);
    if (main->end->visited)
        return main->end;
    return NULL;
}

char *build_paths(room_t *end, main_t *main)
{
    room_t *nodes[1024];
    int count = 0;
    room_t *tmp = end;
    char *path = c_alloc(sizeof(char), 1024, main->alloc);

    if (!path)
        return NULL;
    while (tmp) {
        nodes[count++] = tmp;
        if (tmp == main->start)
            break;
        tmp = tmp->prec;
    }
    for (int i = count - 1; i >= 0; i--) {
        my_strcat(path, nodes[i]->name);
        if (i > 0)
            my_strcat(path, ".");
    }
    main->shortest_path = path;
    return path;
}

void reset_graph(main_t *main)
{
    int i = 0;

    while (i < (int)main->room_amount) {
        main->rooms[i].visited = 0;
        main->rooms[i].dist = -1;
        main->rooms[i].prec = NULL;
        i++;
    }
    main->start->visited = 0;
    main->start->dist = -1;
    main->start->prec = NULL;
    main->end->visited = 0;
    main->end->dist = -1;
    main->end->prec = NULL;
}

static void init_rooms(main_t *main)
{
    int k = 0;

    while (k < (int)main->room_amount) {
        main->rooms[k].blocked = 0;
        k++;
    }
    main->start->blocked = 0;
    main->end->blocked = 0;
    reset_graph(main);
}

char **build_multiple_paths(main_t *main)
{
    int max = main->room_amount * 2 + 10;
    char **paths = malloc(sizeof(char *) * (max + 1));
    int i = 0;

    if (!paths)
        return NULL;
    init_rooms(main);
    while (i < max) {
        if (!walkthrough_rooms(main))
            break;
        paths[i] = build_paths(main->end, main);
        block_path(main, main->end);
        i++;
        reset_graph(main);
    }
    paths[i] = NULL;
    return paths;
}
