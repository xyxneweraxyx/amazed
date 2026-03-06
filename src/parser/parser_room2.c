/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Parser for the rooms.
*/

#include "./../../include/amazed.h"

void realloc_add(main_t *main, bool add)
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

size_t try_add_room(main_t *main, char buff[LINE_BUFF_SIZE],
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

size_t handle_buff(main_t *main, char buff[LINE_BUFF_SIZE],
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
