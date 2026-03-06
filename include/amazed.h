/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Header file.
*/

#ifndef AMAZED
    #define AMAZED

    #include <stdlib.h>
    #include <unistd.h>
    #include <stdio.h>
    #include <fcntl.h>
    #include "../lib/c_alloc/c_alloc.h"
    #include "../lib/parsing/parsing.h"

    // RETURN CODES
    #define EXIT_SUCC 0
    #define EXIT_FAIL 84
    #define LINE_BUFF_SIZE 1024

    // ERROR MSGS
    #define ERR_ARGC "Wrong arguments! Usage : ./amazed < [filepath]\n"
    #define ERR_PARSER "The parser failed during runtime.\n"
    #define ERR_RBTAMT "Amount of robots not given, or negative number found.\n"
    #define ERR_ROOMALC "Allocation of the rooms array failed.\n"
    #define ERR_NGH "A room has two connexions to the same neighbor.\n"

    // HARDCODED VALUES

/// TYPEDEFS

typedef struct robot_assignment {
    size_t path_index;
    size_t robots_on_path;
} robot_assignment_t;


// FORMATTAGE D'UN LINK : "A/B/C" --> CONNEXION ROOM-A, ROOM-B, ROOM-C
typedef struct room {
    char *name;
    struct room **neighboors;
    struct room *prec;
    int visited;
    int used;
    int dist;
    size_t x;
    size_t y;
    int ngh_nbr;
    int robot_num;
    int blocked;
} room_t;
// QUEUE DE NODES A CHECK POUR LA RECHERCHE
typedef struct queue {
    room_t **queued;
    int head_ind;
    int rear_ind;
    int size;
} queue_t;

// FORMATTAGE D'UN PATH : "A/B/C/D"
typedef struct path {
    char *paths;
    room_t **rooms;   // array of rooms in path order
    size_t length;    // number of rooms
} path_t;

// file d'attente de robot, stage est celui a l'entrée, ramaining
// c ceux qui doivent encore passé dedans
typedef struct {
    int staged;
    size_t remaining;
} slot_t;

typedef struct turn {
    room_t ***all_rooms;
    int *lens;
    int *queued;
    int *staged;
    int path_count;
    int *next_robot;
} turn_t;

typedef struct main {
    c_alloc_t *alloc;
    room_t *rooms;
    path_t *paths;
    room_t *start;
    room_t *end;
    size_t robot_amount;
    size_t room_amount;
    size_t print_rooms_written;
    size_t print_cnx_written;
    char *shortest_path;
    char **print_rooms;
    char **print_connexions;
    size_t start_num;
    size_t end_num;
} main_t;

typedef struct {
    char data[256][LINE_BUFF_SIZE];
    int count;
} tunnels_t;

/// FUNCTIONS
queue_t *create_queue(main_t *main, int size, queue_t *queue);
void remove_link(room_t *a, room_t *b);
int enqueue(queue_t *queue, room_t *room);
int enqueue_neighbors(queue_t *queue, room_t *room);
room_t *dequeue(queue_t *queue);
int free_queue(queue_t *queue);
size_t find_all_paths(main_t *main);
size_t find_path(main_t *main);
room_t *bfs_find_path(main_t *main);
room_t *walkthrough_rooms(main_t *main);
char *build_paths(room_t *end, main_t *main);
size_t write_print_data(main_t *main, char buff[LINE_BUFF_SIZE]);

// MAIN FUNCTIONS
int return_error(main_t *main, int return_code,
    const char *error_msg, bool delete_main);
size_t parser(main_t *main);
size_t parse_stdin_line(size_t buff_size, char buff[buff_size]);
size_t parse_rooms(main_t *main, char buff[LINE_BUFF_SIZE],
    char *saveptr, size_t result);
size_t parse_links(main_t *main,
    char buff[LINE_BUFF_SIZE], size_t result);
size_t parser(main_t *main);

// PRINTER
int advance(main_t *main, room_t **rooms, int len, bool *first);
int inject(main_t *main, room_t **rooms, int *staged, bool *first);
room_t **build_rooms(main_t *main, char *path_str, int len);
int count_dots(char *s);
int best_path(int *queued, int *lens, int count);

// LIB FUNCTIONS
int str_len(const char *len);
void str_cpy(const char *src, char *dest);
int str_cmp(const char *str1, const char *str2);
int str_ncmp(const char *str1,
    const char *str2, const int n);
void str_cat(char *dest, int n, ...);
void str_ncpy(const char *src, char *dest, const int n);
ssize_t a_to_i(char *str, char **saveptr);
char *my_strcat(char *dest, const char *src);
int print_num(int nbr);
size_t display(main_t *main, char **paths, size_t path_count);
void str_rev(char *str);
char **build_multiple_paths(main_t *main);
// void print_all_moves(char **move_rooms, size_t *move_robots,
//     size_t move_count);
// path_t *build_path(main_t *main, char *path_str);
// size_t *assign_robots(main_t *main, path_t **paths, size_t path_count);
// void reset_used(main_t *main);
// void print_move(int robot_num, char *room_name);
// int inject_staged(main_t *main, path_t *path, slot_t *slot,
//     bool *first_move);

#endif
