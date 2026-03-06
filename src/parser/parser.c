/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Stdin parser.
*/

#include "./../../include/amazed.h"

size_t parse_stdin_line(size_t buff_size, char buff[buff_size])
{
    size_t p = 0;

    for (size_t i = 0; i < buff_size; i++)
        buff[i] = 0;
    for (; p < buff_size - 1; p++) {
        if (read(STDIN_FILENO, &(buff[p]), 1) <= 0) {
            buff[p] = 0;
            return EXIT_FAIL;
        }
        if (buff[p] == '\n')
            break;
    }
    if (!str_cmp("##start\n", buff) || !str_cmp("##end\n", buff))
        write(STDOUT_FILENO, buff, str_len(buff));
    buff[p] = 0;
    return (size_t)EXIT_SUCC;
}

static void alloc_rooms(main_t *main)
{
    main->print_rooms = c_alloc(sizeof(char *), 1024, main->alloc);
    main->print_connexions = c_alloc(sizeof(char *), 1024, main->alloc);
    for (int i = 0; i < 1024; i++)
        main->print_rooms[i] = c_alloc(sizeof(char), LINE_BUFF_SIZE,
            main->alloc);
    for (int i = 0; i < 1024; i++)
        main->print_connexions[i] = c_alloc(sizeof(char), LINE_BUFF_SIZE,
            main->alloc);
}

static void write_robots(char buff[LINE_BUFF_SIZE])
{
    write(STDOUT_FILENO, "#number_of_robots", str_len("#number_of_robots"));
    write(STDOUT_FILENO, "\n", 1);
    write(STDOUT_FILENO, buff, str_len(buff));
    write(STDOUT_FILENO, "\n", 1);
}

static size_t validate_robot_line(char buff[LINE_BUFF_SIZE])
{
    int i = 0;

    for (; buff[i] == ' ' || buff[i] == '\t'; i++);
    if (buff[i] < '1' || buff[i] > '9')
        return (size_t)EXIT_FAIL;
    for (i++; buff[i] && buff[i] != ' ' && buff[i] != '\n'; i++) {
        if (buff[i] < '0' || buff[i] > '9')
            return (size_t)EXIT_FAIL;
    }
    return (size_t)EXIT_SUCC;
}

size_t parser(main_t *main)
{
    char buff[LINE_BUFF_SIZE] = {0};
    char *saveptr = NULL;
    ssize_t result = 0;
    size_t result2 = 0;

    alloc_rooms(main);
    parse_stdin_line(LINE_BUFF_SIZE, buff);
    result = a_to_i(buff, &saveptr);
    if (!saveptr || result < 0 ||
        validate_robot_line(buff) == (size_t)EXIT_FAIL)
        return (size_t)return_error(main, EXIT_FAIL, ERR_RBTAMT, false);
    write_robots(buff);
    main->robot_amount = (size_t)result;
    if (parse_rooms(main, buff, saveptr, result2) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    if (parse_links(main, buff, result2) == (size_t)EXIT_FAIL)
        return (size_t)EXIT_FAIL;
    if (!main->start->neighboors || !main->end->neighboors)
        return (size_t)EXIT_FAIL;
    return (size_t)EXIT_SUCC;
}
