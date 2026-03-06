/*
** EPITECH PROJECT, 2025
** str_cat
** File description:
** str_cat
*/

#include <stdarg.h>

void str_cat(char *restrict dest, const int n, ...)
{
    va_list args;
    int pos = 0;
    const char *current = ((void *)0);
    int i = 0;

    va_start(args, n);
    for (int arg = 0; arg < n; arg++) {
        current = va_arg(args, const char *);
        for (i = 0; current[i]; i++) {
            dest[pos] = current[i];
            pos += 1;
        }
    }
    dest[pos] = 0;
    va_end(args);
}
