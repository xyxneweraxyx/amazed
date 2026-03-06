/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Parser for the links.
*/

#include "./../../include/amazed.h"

static size_t is_line_correct(char buff[LINE_BUFF_SIZE])
{
    size_t hyphen_amount = 0;

    for (int i = 0; buff[i]; i++) {
        if (buff[i] == '-')
            hyphen_amount++;
        if (buff[i] == '-' && !buff[i + 1])
            return (size_t)EXIT_FAIL;
        if (buff[i] == ' ')
            return (size_t)EXIT_FAIL;
    }
    return (hyphen_amount == 1) ? (size_t)EXIT_SUCC : (size_t)EXIT_FAIL;
}

static size_t get_names(char buff[LINE_BUFF_SIZE], char name1[LINE_BUFF_SIZE],
    char name2[LINE_BUFF_SIZE])
{
    size_t chars_copy = 0;

    for (int i = 0; i < LINE_BUFF_SIZE; i++) {
        name1[i] = 0;
        name2[i] = 0;
    }
    for (; buff[chars_copy] && buff[chars_copy] != '-'; chars_copy++);
    str_ncpy(buff, name1, chars_copy);
    buff += chars_copy + 1;
    chars_copy = 0;
    for (; buff[chars_copy]; chars_copy++);
    str_ncpy(buff, name2, chars_copy);
    return (size_t)EXIT_SUCC;
}

static room_t *find_room(main_t *restrict main, char name[LINE_BUFF_SIZE])
{
    size_t i = 0;

    for (; i < main->room_amount &&
        str_cmp(main->rooms[i].name, name); i++);
    if (i < main->room_amount)
        return &(main->rooms[i]);
    if (!str_cmp(main->start->name, name))
        return main->start;
    if (!str_cmp(main->end->name, name))
        return main->end;
    return NULL;
}

static size_t add_neighbor(main_t *restrict main, char room[LINE_BUFF_SIZE],
    char neighbor[LINE_BUFF_SIZE])
{
    room_t *room_a = find_room(main, room);
    room_t *neighbor_a = find_room(main, neighbor);

    if (!room_a || !neighbor_a)
        return (size_t)EXIT_FAIL;
    for (size_t i = 0; i < room_a->ngh_nbr; i++) {
        if (room_a->neighbors[i] == neighbor_a)
            return return_error(main, EXIT_FAIL, ERR_NGH, false);
    }
    room_a->neighbors = c_realloc(main->alloc, room_a->neighbors,
        &(c_realloc_t){room_a->ngh_nbr, room_a->ngh_nbr + 1},
        sizeof(room_t *));
    room_a->ngh_nbr += 1;
    room_a->neighbors[room_a->ngh_nbr - 1] = neighbor_a;
    return (size_t)EXIT_SUCC;
}

size_t parse_links(main_t *restrict main,
    char buff[LINE_BUFF_SIZE], size_t result)
{
    char name1[LINE_BUFF_SIZE] = {0};
    char name2[LINE_BUFF_SIZE] = {0};

    result = (size_t)(EXIT_SUCC);
    while (result == (size_t)EXIT_SUCC) {
        if (buff[0] == '#') {
            result = parse_stdin_line(LINE_BUFF_SIZE, buff);
            continue;
        }
        if (is_line_correct(buff) == (size_t)EXIT_FAIL ||
            get_names(buff, name1, name2) == (size_t)EXIT_FAIL ||
            add_neighbor(main, name1, name2) == (size_t)EXIT_FAIL ||
            add_neighbor(main, name2, name1) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
        result = parse_stdin_line(LINE_BUFF_SIZE, buff);
    }
    return (size_t)EXIT_SUCC;
}
