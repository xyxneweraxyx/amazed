/*
** EPITECH PROJECT, 2026
** amazed
** File description:
** printer.c
*/
#include "../include/amazed.h"

static void update_staged(turn_t *turn, int p)
{
    if (turn->staged[p] == 0 && turn->queued[p] > 0) {
        turn->staged[p] = *turn->next_robot;
        *turn->next_robot = *turn->next_robot + 1;
        turn->queued[p]--;
    }
}

int do_turn(main_t *main, turn_t *turn)
{
    int reached = 0;
    bool first = true;
    int p = 0;

    while (p < turn->path_count) {
        reached += advance(main, turn->all_rooms[p], turn->lens[p], &first);
        reached += inject(main, turn->all_rooms[p], &turn->staged[p], &first);
        update_staged(turn, p);
        p++;
    }
    if (!first)
        write(1, "\n", 1);
    return reached;
}

static int get_path_count(char **path_strs)
{
    int count = 0;

    if (!path_strs)
        return 0;
    while (path_strs[count])
        count++;
    return count;
}

static void init_staged(turn_t *turn)
{
    int p = 0;

    while (p < turn->path_count) {
        if (turn->queued[p] > 0) {
            turn->staged[p] = *turn->next_robot;
            *turn->next_robot = *turn->next_robot + 1;
            turn->queued[p]--;
        }
        p++;
    }
}

static turn_t init_turn(main_t *main, int path_count, int *next_robot)
{
    turn_t turn;

    turn.all_rooms = c_alloc(sizeof(room_t **), path_count, main->alloc);
    turn.lens = c_alloc(sizeof(int), path_count, main->alloc);
    turn.queued = c_alloc(sizeof(int), path_count, main->alloc);
    turn.staged = c_alloc(sizeof(int), path_count, main->alloc);
    turn.path_count = path_count;
    turn.next_robot = next_robot;
    return turn;
}

void assign_robots(main_t *main, int *queued, int *lens, int count)
{
    int r = 0;

    while (r < (int)main->robot_amount) {
        queued[best_path(queued, lens, count)]++;
        r++;
    }
}

static void print_moves(main_t *main, char **path_strs, int path_count)
{
    int next_robot = 1;
    int remaining = (int)main->robot_amount;
    turn_t turn = init_turn(main, path_count, &next_robot);
    int p = 0;

    while (p < path_count) {
        turn.lens[p] = count_dots(path_strs[p]);
        turn.all_rooms[p] = build_rooms(main, path_strs[p], turn.lens[p]);
        p++;
    }
    assign_robots(main, turn.queued, turn.lens, path_count);
    init_staged(&turn);
    while (remaining > 0)
        remaining -= do_turn(main, &turn);
}

size_t display(main_t *restrict main, char **paths, size_t path_count)
{
    (void)path_count;
    write(1, "#moves\n", 7);
    print_moves(main, paths, get_path_count(paths));
    return (size_t)EXIT_SUCC;
}
