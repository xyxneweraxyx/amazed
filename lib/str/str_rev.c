/*
** EPITECH PROJECT, 2025
** str_rev
** File description:
** str_rev
*/

#include <stdio.h>

static int str_len(const char *str)
{
    int i = 0;

    for (; str[i]; i++);
    return i;
}

void str_rev(char *str)
{
    int len = str_len(str);
    char temp = 0;

    for (int i = 0; i < len / 2; i++) {
        temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}
