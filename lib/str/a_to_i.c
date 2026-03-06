/*
** EPITECH PROJECT, 2025
** a_to_i
** File description:
** a_to_i
*/

#include <unistd.h>
#include <stdio.h>

ssize_t a_to_i(char *str, char **saveptr)
{
    int i = 0;
    ssize_t result = 0;

    for (; str[i] && (str[i] < '0' || str[i] > '9'); i++);
    if (!str[i]) {
        *saveptr = NULL;
        return 0;
    }
    *saveptr = &(str[i]);
    for (; str[i] >= '0' && str[i] <= '9'; i++)
        result = (result * 10) + (size_t)((str[i]) - '0');
    if (*(*saveptr - 1) == '-')
        result *= -1;
    *saveptr = &(str[i]);
    return result;
}
