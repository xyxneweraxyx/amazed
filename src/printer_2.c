/*
** EPITECH PROJECT, 2026
** amazed
** File description:
** printer_2.c
*/
#include "../include/amazed.h"

static room_t *room_from_name(main_t *main, char *name)
{
    int i = 0;

    while (i < (int)main->room_amount) {
        if (!str_cmp(main->rooms[i].name, name))
            return (&main->rooms[i]);
        i++;
    }
    if (!str_cmp(main->start->name, name))
        return (main->start);
    if (!str_cmp(main->end->name, name))
        return (main->end);
    return (NULL);
}

static void print_move(int robot_num, char *room_name)
{
    write(1, "P", 1);
    print_num(robot_num);
    write(1, "-", 1);
    write(1, room_name, str_len(room_name));
}

static void print_separator(bool *first)
{
    if (!*first)
        write(1, " ", 1);
}

int count_dots(char *s)
{
    int n = 0;
    int i = 0;

    while (s[i]) {
        n += (s[i] == '.');
        i++;
    }
    return n + 1;
}

static void get_next_name(char *path, char buff[LINE_BUFF_SIZE], int *st)
{
    int i = 0;
    int start = *st;

    while (i < LINE_BUFF_SIZE) {
        buff[i] = 0;
        i++;
    }
    while (path[*st] != '\0' && path[*st] != '.') {
        buff[*st - start] = path[*st];
        *st = *st + 1;
    }
    if (path[*st] == '.')
        *st = *st + 1;
}

room_t **build_rooms(main_t *main, char *path_str, int len)
{
    room_t **rooms = c_alloc(sizeof(room_t *), len, main->alloc);
    char name[LINE_BUFF_SIZE];
    int st = 0;
    int i = 0;

    while (i < len) {
        get_next_name(path_str, name, &st);
        rooms[i] = room_from_name(main, name);
        i++;
    }
    return rooms;
}

int best_path(int *queued, int *lens, int count)
{
    int best = 0;
    int i = 0;

    while (i < count) {
        if (queued[i] + lens[i] < queued[best] + lens[best])
            best = i;
        i++;
    }
    return best;
}

static void end_reached(main_t *main, room_t *nxt, int *reached)
{
    if (nxt == main->end) {
        nxt->robot_num = 0;
        *reached = *reached + 1;
    }
}

int advance(main_t *main, room_t **rooms, int len, bool *first)
{
    int reached = 0;
    int i = len - 2;
    room_t *cur;
    room_t *nxt;

    while (i >= 1) {
        cur = rooms[i];
        nxt = rooms[i + 1];
        if (cur->robot_num > 0 && (nxt->robot_num == 0 || nxt == main->end)) {
            nxt->robot_num = cur->robot_num;
            cur->robot_num = 0;
            print_separator(first);
            print_move(nxt->robot_num, nxt->name);
            *first = false;
            end_reached(main, nxt, &reached);
        }
        i--;
    }
    return reached;
}

int inject(main_t *main, room_t **rooms, int *staged, bool *first)
{
    if (*staged == 0 || rooms[1]->robot_num != 0)
        return 0;
    rooms[1]->robot_num = *staged;
    if (!*first)
        write(1, " ", 1);
    print_move(*staged, rooms[1]->name);
    *first = false;
    *staged = 0;
    if (rooms[1] != main->end)
        return 0;
    rooms[1]->robot_num = 0;
    return 1;
}
