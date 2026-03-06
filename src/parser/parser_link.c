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
        if (buff[i] == '#')
            break;
        if (buff[i] == '-')
            hyphen_amount++;
        if (buff[i] == '-' && !buff[i + 1])
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
    for (; buff[chars_copy] && buff[chars_copy] != '#'
        && buff[chars_copy] != ' '; chars_copy++);
    str_ncpy(buff, name2, chars_copy);
    return (size_t)EXIT_SUCC;
}

static room_t *find_room(main_t *main, char name[LINE_BUFF_SIZE])
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

static size_t add_neighbor(main_t *main, char room[LINE_BUFF_SIZE],
    char neighbor[LINE_BUFF_SIZE])
{
    room_t *room_a = find_room(main, room);
    room_t *neighbor_a = find_room(main, neighbor);

    if (!room_a || !neighbor_a)
        return (size_t)EXIT_FAIL;
    for (size_t i = 0; i < (size_t)room_a->ngh_nbr; i++) {
        if (room_a->neighboors[i] == neighbor_a)
            return return_error(main, EXIT_FAIL, ERR_NGH, false);
    }
    room_a->neighboors = c_realloc(main->alloc, room_a->neighboors,
        &(c_realloc_t){room_a->ngh_nbr, room_a->ngh_nbr + 2},
        sizeof(room_t *));
    room_a->neighboors[room_a->ngh_nbr] = neighbor_a;
    room_a->neighboors[room_a->ngh_nbr + 1] = NULL;
    room_a->ngh_nbr += 1;
    return (size_t)EXIT_SUCC;
}

// Returns -1 = duplicate (fail), 0 = new, 1 = reverse (skip)
static int check_tunnel_keys(char key[LINE_BUFF_SIZE],
    char rkey[LINE_BUFF_SIZE], tunnels_t *tunnels)
{
    for (int i = 0; i < tunnels->count; i++) {
        if (!str_cmp(tunnels->data[i], key))
            return -1;
        if (!str_cmp(tunnels->data[i], rkey))
            return 1;
    }
    return 0;
}

static size_t validate_tunnel(char buff[LINE_BUFF_SIZE],
    char name1[LINE_BUFF_SIZE], char name2[LINE_BUFF_SIZE])
{
    if (is_line_correct(buff) == (size_t)EXIT_FAIL ||
        get_names(buff, name1, name2) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    if (!str_cmp(name1, name2))
        return (size_t)EXIT_FAIL;
    return (size_t)EXIT_SUCC;
}

static size_t add_tunnel(main_t *main, char name1[LINE_BUFF_SIZE],
    char name2[LINE_BUFF_SIZE], tunnels_t *tunnels)
{
    char key[LINE_BUFF_SIZE] = {0};
    char rkey[LINE_BUFF_SIZE] = {0};
    int check = 0;

    str_cat(key, 3, name1, "-", name2);
    str_cat(rkey, 3, name2, "-", name1);
    check = check_tunnel_keys(key, rkey, tunnels);
    if (check == -1)
        return (size_t)EXIT_FAIL;
    if (check == 0) {
        if (add_neighbor(main, name1, name2) == (size_t)EXIT_FAIL ||
            add_neighbor(main, name2, name1) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
        str_cpy(key, tunnels->data[tunnels->count]);
        tunnels->count++;
    }
    return (size_t)EXIT_SUCC;
}

static size_t process_tunnel(main_t *main, char buff[LINE_BUFF_SIZE],
    tunnels_t *tunnels)
{
    char name1[LINE_BUFF_SIZE] = {0};
    char name2[LINE_BUFF_SIZE] = {0};

    if (validate_tunnel(buff, name1, name2) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    if (add_tunnel(main, name1, name2, tunnels) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    write(STDOUT_FILENO, buff, str_len(buff));
    write(STDOUT_FILENO, "\n", 1);
    return (size_t)EXIT_SUCC;
}

size_t parse_links(main_t *main,
    char buff[LINE_BUFF_SIZE], size_t result)
{
    tunnels_t tunnels = {0};

    result = (size_t)EXIT_SUCC;
    write(STDOUT_FILENO, "#tunnels\n", 9);
    while (result == (size_t)EXIT_SUCC) {
        if (buff[0] == '#') {
            result = parse_stdin_line(LINE_BUFF_SIZE, buff);
            continue;
        }
        if (process_tunnel(main, buff, &tunnels) == (size_t)EXIT_FAIL)
            return (size_t)EXIT_FAIL;
        result = parse_stdin_line(LINE_BUFF_SIZE, buff);
    }
    return (size_t)EXIT_SUCC;
}
