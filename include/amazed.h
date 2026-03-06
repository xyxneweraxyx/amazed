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
    #include "./../lib/c_alloc/c_alloc.h"
    #include "./../lib/parsing/parsing.h"

    // RETURN CODES
    #define EXIT_SUCC 0
    #define EXIT_FAIL 84
    #define LINE_BUFF_SIZE 16

    // ERROR MSGS
    #define ERR_ARGC "Wrong arguments! Usage : ./amazed < [filepath]\n"
    #define ERR_PARSER "The parser failed during runtime.\n"
    #define ERR_RBTAMT "Amount of robots not given, or negative number found.\n"
    #define ERR_ROOMALC "Allocation of the rooms array failed.\n"
    #define ERR_NGH "A room has two connexions to the same neighbor.\n"

    // HARDCODED VALUES

/// TYPEDEFS

// FORMATTAGE D'UN LINK : "A/B/C" --> CONNEXION ROOM-A, ROOM-B, ROOM-C
typedef struct room {
    char *name;
    struct room **neighbors;
    size_t ngh_nbr;
    size_t x;
    size_t y;
} room_t;

// FORMATTAGE D'UN PATH : "A/B/C/D"
typedef struct path {
    size_t id;
    char *path;
} path_t;

typedef struct main {
    c_alloc_t *alloc;
    room_t *rooms;
    path_t *paths;
    room_t *start;
    room_t *end;
    size_t robot_amount;
    size_t room_amount;
} main_t;

/// FUNCTIONS

// MAIN FUNCTIONS
int return_error(main_t *restrict main, int return_code,
    const char *restrict error_msg, bool delete_main);
size_t parser(main_t *main);
size_t parse_stdin_line(size_t buff_size, char buff[buff_size]);
size_t parse_rooms(main_t *restrict main, char buff[LINE_BUFF_SIZE],
    char *saveptr, size_t result);
size_t parse_links(main_t *restrict main,
    char buff[LINE_BUFF_SIZE], size_t result);
void print_rooms(main_t *restrict main);

// LIB FUNCTIONS
int str_len(const char *restrict len);
void str_cpy(const char *restrict src, char *restrict dest);
int str_cmp(const char *restrict str1, const char *restrict str2);
int str_ncmp(const char *restrict str1,
    const char *restrict str2, const int n);
void str_cat(char *restrict dest, int n, ...);
void str_ncpy(const char *restrict src, char *restrict dest, const int n);
ssize_t a_to_i(char *restrict str, char **saveptr);

#endif
