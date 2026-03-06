/*
** EPITECH PROJECT, 2025
** str_len
** File description:
** str_len
*/

int str_len(const char *restrict str)
{
    int i = 0;

    for (; str[i]; i++);
    return i;
}
