/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Parser for the rooms.
*/

#include "./../../include/amazed.h"

static size_t is_room_line_correct(char buff[LINE_BUFF_SIZE])
{
    int i = 0;
    size_t space_amount = 0;
    size_t num_amount = 0;

    for (; buff[i] && buff[i] != ' '; i++) {
        if (buff[i] == '-')
            return (size_t)EXIT_FAIL;
    }
    if (i == 0)
        return (size_t)EXIT_FAIL;
    for (; buff[i]; i++) {
        if (buff[i] == ' ')
            space_amount++;
        if (buff[i] != ' ' && (buff[i] < '0' || buff[i] > '9'))
            return (size_t)EXIT_FAIL;
        if (buff[i] >= '0' && buff[i] <= '9' && buff[i - 1] == ' ')
            num_amount++;
    }
    return (space_amount == 2 && num_amount == 2) ? (size_t)EXIT_SUCC
        : (size_t)EXIT_FAIL;
}

static room_t *write_room(main_t *restrict main, char buff[LINE_BUFF_SIZE],
    char *saveptr, room_t *room)
{
    ssize_t result = 0;
    size_t name_len = 0;

    if (!room)
        return NULL;
    room->neighbors = NULL;
    for (; buff[name_len] && buff[name_len] != ' '; name_len++);
    room->name = c_alloc(sizeof(char), name_len + 1, main->alloc);
    str_ncpy(buff, room->name, name_len);
    result = a_to_i(buff + name_len, &saveptr);
    if (!saveptr) {
        c_free(room->name, main->alloc);
        return NULL;
    }
    room->x = result;
    result = a_to_i(saveptr, &saveptr);
    room->y = result;
    return room;
}

static size_t process_start_end(main_t *restrict main,
    char buff[LINE_BUFF_SIZE], char *saveptr)
{
    if (!str_cmp(buff, "##start")) {
        if (main->start->name)
            return (size_t)EXIT_FAIL;
        if (parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL ||
            is_room_line_correct(buff) == (size_t)EXIT_FAIL ||
            !write_room(main, buff, saveptr, main->start) ||
            parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
    }
    if (!str_cmp(buff, "##end")) {
        if (main->end->name)
            return (size_t)EXIT_FAIL;
        if (parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL ||
            is_room_line_correct(buff) == (size_t)EXIT_FAIL ||
            !write_room(main, buff, saveptr, main->end) ||
            parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
    }
    return (size_t)EXIT_SUCC;
}

static size_t alloc_ini(main_t *restrict main)
{
    main->rooms = c_alloc(sizeof(room_t), 1, main->alloc);
    if (!main->rooms)
        return return_error(main, EXIT_FAIL, ERR_ROOMALC, false);
    main->start = c_alloc(sizeof(room_t), 1, main->alloc);
    if (!main->start)
        return (size_t)EXIT_FAIL;
    main->end = c_alloc(sizeof(room_t), 1, main->alloc);
    if (!main->end)
        return (size_t)EXIT_FAIL;
    main->room_amount = 0;
    return (size_t)EXIT_SUCC;
}

static void realloc_add(main_t *restrict main, bool add)
{
    if (add) {
        main->room_amount += 1;
        main->rooms = c_realloc(main->alloc, main->rooms,
            &((c_realloc_t){main->room_amount,
                    main->room_amount + 1}), sizeof(room_t));
    } else {
        main->rooms = c_realloc(main->alloc, main->rooms,
            &((c_realloc_t){main->room_amount + 1, main->room_amount}),
            sizeof(room_t));
        main->room_amount -= 1;
    }
}

size_t parse_rooms(main_t *restrict main, char buff[LINE_BUFF_SIZE],
    char *saveptr, size_t result)
{
    if (alloc_ini(main) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    while (result == (size_t)EXIT_SUCC) {
        result = parse_stdin_line(LINE_BUFF_SIZE, buff);
        if (result || process_start_end(main, buff, saveptr))
            return (size_t)EXIT_FAIL;
        if (buff[0] == '#')
            continue;
        realloc_add(main, true);
        if (!write_room(main, buff, saveptr,
                &(main->rooms[main->room_amount - 1]))) {
            realloc_add(main, false);
            break;
        }
        if (is_room_line_correct(buff) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
    }
    return (main->start->name && main->end->name) ?
        (size_t)EXIT_SUCC : (size_t)EXIT_FAIL;
}
