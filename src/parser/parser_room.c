/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Parser for the rooms.
*/

#include "./../../include/amazed.h"
#include <stddef.h>

static size_t return_room_line(size_t num_amount, char buff[LINE_BUFF_SIZE])
{
    if (num_amount < 2)
        return (size_t)EXIT_FAIL;
    write(STDOUT_FILENO, buff, str_len(buff));
    write(STDOUT_FILENO, "\n", 1);
    return (size_t)EXIT_SUCC;
}

static size_t is_room_line_correct(char buff[LINE_BUFF_SIZE])
{
    int i = 0;
    size_t num_amount = 0;

    for (; buff[i] && buff[i] != ' '; i++) {
        if (buff[i] == '-')
            return (size_t)EXIT_FAIL;
    }
    if (i == 0)
        return (size_t)EXIT_FAIL;
    for (; buff[i]; i++) {
        if (buff[i] == '#')
            break;
        if (buff[i] != ' ' && (buff[i] < '0' || buff[i] > '9'))
            return (size_t)EXIT_FAIL;
        if (buff[i] >= '0' && buff[i] <= '9' && buff[i - 1] == ' ')
            num_amount++;
    }
    return return_room_line(num_amount, buff);
}

static room_t *write_room(main_t *main, char buff[LINE_BUFF_SIZE],
    char *saveptr, room_t *room)
{
    ssize_t result = 0;
    size_t name_len = 0;

    if (!room)
        return NULL;
    room->neighboors = NULL;
    for (; buff[name_len] && buff[name_len] != ' '; name_len++);
    room->name = c_alloc(sizeof(char), name_len + 1, main->alloc);
    str_ncpy(buff, room->name, name_len);
    if (!saveptr) {
        c_free(room->name, main->alloc);
        return NULL;
    }
    room->x = result;
    result = a_to_i(saveptr, &saveptr);
    room->y = result;
    return room;
}

static size_t write_room_nums(main_t *main, bool start)
{
    if (start)
        main->start_num = main->print_rooms_written - 1;
    else
        main->end_num = main->print_rooms_written - 1;
    return (size_t)EXIT_SUCC;
}

static size_t process_end(main_t *main,
    char buff[LINE_BUFF_SIZE], char *saveptr)
{
    if (!str_cmp(buff, "##end")) {
        if (main->end->name)
            return (size_t)EXIT_FAIL;
        if (parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL ||
            is_room_line_correct(buff) == (size_t)EXIT_FAIL ||
            !write_room(main, buff, saveptr, main->end) ||
            parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL ||
            write_room_nums(main, false) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
    }
    return (size_t)EXIT_SUCC;
}

static size_t process_start(main_t *main,
    char buff[LINE_BUFF_SIZE], char *saveptr)
{
    if (!str_cmp(buff, "##start")) {
        if (main->start->name)
            return (size_t)EXIT_FAIL;
        if (parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL ||
            is_room_line_correct(buff) == (size_t)EXIT_FAIL ||
            !write_room(main, buff, saveptr, main->start) ||
            parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL ||
            write_room_nums(main, true) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
    }
    if (process_end(main, buff, saveptr) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    return (size_t)EXIT_SUCC;
}

static size_t alloc_ini(main_t *main)
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

static void realloc_add(main_t *main, bool add)
{
    if (add) {
        main->room_amount += 1;
        main->rooms = c_realloc(main->alloc, main->rooms,
            &((c_realloc_t){main->room_amount,
                    main->room_amount + 1}), sizeof(room_t));
    } else {
        main->rooms = c_realloc(main->alloc, main->rooms,
            &((c_realloc_t){main->room_amount + 3, main->room_amount}),
            sizeof(room_t));
        main->room_amount -= 1;
    }
}

size_t parse_rooms(main_t *main, char buff[LINE_BUFF_SIZE],
    char *saveptr, size_t result)
{
    if (alloc_ini(main) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    write(STDOUT_FILENO, "#rooms\n", 7);
    while (result == (size_t)EXIT_SUCC) {
        result = parse_stdin_line(LINE_BUFF_SIZE, buff);
        if (result || process_start(main, buff, saveptr))
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
            break;
    }
    return (main->start->name && main->end->name) ?
        (size_t)EXIT_SUCC : (size_t)EXIT_FAIL;
}
