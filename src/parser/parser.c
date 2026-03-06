/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Stdin parser.
*/

#include "./../../include/amazed.h"

static void print_single_room(room_t *room, int i, char *name)
{
    if (i != -1)
        printf("-- ROOM %d\n", i);
    else
        printf("-- ROOM %s\n", name);
    printf("NAME : %s\n", room->name);
    printf("COORDINATES : %zu %zu\n", room->x, room->y);
    if (!room->neighbors || !room->ngh_nbr)
        return;
    for (size_t j = 0; j < room->ngh_nbr; j++)
        printf("NEIGHBOR %zu : ROOM %s - %p\n", j, room->neighbors[j]->name,
            room->neighbors[j]);
}

void print_rooms(main_t *restrict main)
{
    printf("----- ROOM PRINT ST\n\n");
    for (int i = 0; (size_t)i < main->room_amount; i++) {
        print_single_room(&(main->rooms[i]), i, NULL);
        printf("\n");
    }
    print_single_room(main->start, -1, "START");
    print_single_room(main->end, -1, "END");
    printf("\n");
    printf("----- ROOM PRINT END\n\n");
}

size_t parse_stdin_line(size_t buff_size, char buff[buff_size])
{
    size_t p = 0;

    for (size_t i = 0; i < buff_size; i++)
        buff[i] = 0;
    for (; p < buff_size - 1; p++) {
        if (read(STDIN_FILENO, &(buff[p]), 1) <= 0) {
            buff[p] = 0;
            return EXIT_FAIL;
        }
        if (buff[p] == '\n')
            break;
    }
    if (p == buff_size - 1 && buff[buff_size - 1] != '\n') {
        for (; buff[buff_size - 1] != '\n';)
            read(STDIN_FILENO, &(buff[buff_size - 1]), 1);
        buff[buff_size - 1] = 0;
        return (size_t)EXIT_SUCC;
    }
    buff[p] = 0;
    return (size_t)EXIT_SUCC;
}

size_t parser(main_t *restrict main)
{
    char buff[LINE_BUFF_SIZE] = {0};
    char *saveptr = NULL;
    ssize_t result = 0;
    size_t result2 = 0;

    parse_stdin_line(LINE_BUFF_SIZE, buff);
    result = a_to_i(buff, &saveptr);
    if (!saveptr || result < 0)
        return (size_t)return_error(main, EXIT_FAIL, ERR_RBTAMT, false);
    main->robot_amount = (size_t)result;
    if (parse_rooms(main, buff, saveptr, result2) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    if (parse_links(main, buff, result2) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    return (size_t)EXIT_SUCC;
}
