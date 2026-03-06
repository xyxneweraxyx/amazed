/*
** EPITECH PROJECT, 2025
** amazed
** File description:
** Entry point.
*/

#include "./../include/amazed.h"

int return_error(main_t *restrict main, int return_code,
    const char *restrict error_msg, bool delete_main)
{
    write(STDERR_FILENO, error_msg, str_len(error_msg));
    if (delete_main && main)
        c_delete(main->alloc, true);
    return return_code;
}

int main(int argc, char **argv)
{
    c_alloc_t *alloc = NULL;
    main_t *main = NULL;

    if (argc != 1 || !argv)
        return return_error(NULL, EXIT_FAIL, ERR_ARGC, true);
    alloc = c_ini((uint16_t)50);
    if (!alloc)
        return EXIT_FAIL;
    alloc->debug = false;
    main = c_alloc(sizeof(main_t), 1, alloc);
    if (!main)
        return EXIT_FAIL;
    main->alloc = alloc;
    if (parser(main) == (size_t)EXIT_FAIL)
        return return_error(main, EXIT_FAIL, ERR_PARSER, true);
    c_delete(main->alloc, true);
    return 0;
}
