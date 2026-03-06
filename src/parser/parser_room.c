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
    char *ptr = NULL;

    (void)saveptr;
    if (!room)
        return NULL;
    room->neighboors = NULL;
    for (; buff[name_len] && buff[name_len] != ' '; name_len++);
    if (name_len == 0)
        return NULL;
    room->name = c_alloc(sizeof(char), name_len + 1, main->alloc);
    str_ncpy(buff, room->name, name_len);
    ptr = buff + name_len;
    result = a_to_i(ptr, &ptr);
    if (!ptr)
        return NULL;
    room->x = (size_t)result;
    result = a_to_i(ptr, &ptr);
    room->y = (size_t)result;
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

static size_t read_skip_comments(char buff[LINE_BUFF_SIZE])
{
    while (1) {
        if (parse_stdin_line(LINE_BUFF_SIZE, buff) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
        if (!(buff[0] == '#' && buff[1] != '#'))
            return (size_t)EXIT_SUCC;
    }
}

static size_t check_duplicate(main_t *main, room_t *new_room)
{
    size_t i = 0;

    for (; i < main->room_amount - 1; i++) {
        if (!str_cmp(main->rooms[i].name, new_room->name))
            return (size_t)EXIT_FAIL;
        if (main->rooms[i].x == new_room->x &&
            main->rooms[i].y == new_room->y)
            return (size_t)EXIT_FAIL;
    }
    if (main->start->name && !str_cmp(main->start->name, new_room->name))
        return (size_t)EXIT_FAIL;
    if (main->start->name && main->start->x == new_room->x &&
        main->start->y == new_room->y)
        return (size_t)EXIT_FAIL;
    if (main->end->name && !str_cmp(main->end->name, new_room->name))
        return (size_t)EXIT_FAIL;
    if (main->end->name && main->end->x == new_room->x &&
        main->end->y == new_room->y)
        return (size_t)EXIT_FAIL;
    return (size_t)EXIT_SUCC;
}

static size_t parse_tagged_room(main_t *main,
    char buff[LINE_BUFF_SIZE], char *saveptr, room_t *room, bool is_start)
{
    const char *same_tag = is_start ? "##start" : "##end";

    if (room->name)
        return (size_t)EXIT_FAIL;
    if (read_skip_comments(buff) == (size_t)EXIT_FAIL ||
        is_room_line_correct(buff) == (size_t)EXIT_FAIL ||
        !write_room(main, buff, saveptr, room) ||
        read_skip_comments(buff) == (size_t)EXIT_FAIL ||
        write_room_nums(main, is_start) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    if (!str_cmp(buff, same_tag))
        return (size_t)EXIT_FAIL;
    return (size_t)EXIT_SUCC;
}

static size_t process_start(main_t *main,
    char buff[LINE_BUFF_SIZE], char *saveptr)
{
    if (!str_cmp(buff, "##start"))
        return parse_tagged_room(main, buff, saveptr, main->start, true);
    if (!str_cmp(buff, "##end"))
        return parse_tagged_room(main, buff, saveptr, main->end, false);
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

static size_t try_add_room(main_t *main, char buff[LINE_BUFF_SIZE],
    char *saveptr, bool *done)
{
    realloc_add(main, true);
    if (!write_room(main, buff, saveptr,
            &(main->rooms[main->room_amount - 1])) ||
        is_room_line_correct(buff) == (size_t)EXIT_FAIL) {
        realloc_add(main, false);
        *done = true;
        return (size_t)EXIT_SUCC;
    }
    if (check_duplicate(main, &(main->rooms[main->room_amount - 1]))
        == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    return (size_t)EXIT_SUCC;
}

static size_t handle_buff(main_t *main, char buff[LINE_BUFF_SIZE],
    char *saveptr, bool *buff_ready, bool *done)
{
    if (buff[0] == '#' && buff[1] != '#')
        return (size_t)EXIT_SUCC;
    if (buff[0] == '#' && buff[1] == '#' &&
        str_cmp(buff, "##start") && str_cmp(buff, "##end"))
        return (size_t)EXIT_SUCC;
    if (!str_cmp(buff, "##start") || !str_cmp(buff, "##end")) {
        if (process_start(main, buff, saveptr) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
        *buff_ready = true;
        return (size_t)EXIT_SUCC;
    }
    return try_add_room(main, buff, saveptr, done);
}

size_t parse_rooms(main_t *main, char buff[LINE_BUFF_SIZE],
    char *saveptr, size_t result)
{
    bool buff_ready = false;
    bool done = false;

    (void)result;
    if (alloc_ini(main) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    write(STDOUT_FILENO, "#rooms\n", 7);
    while (!done) {
        if (!buff_ready &&
            parse_stdin_line(LINE_BUFF_SIZE, buff) != (size_t)EXIT_SUCC)
            break;
        buff_ready = false;
        if (handle_buff(main, buff, saveptr, &buff_ready, &done)
            == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
    }
    return (main->start->name && main->end->name) ?
        (size_t)EXIT_SUCC : (size_t)EXIT_FAIL;
}
