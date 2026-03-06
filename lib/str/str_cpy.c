/*
** EPITECH PROJECT, 2025
** str_cpy
** File description:
** str_cpy
*/

void str_cpy(const char *restrict src, char *restrict dest)
{
    int i = 0;

    for (; src[i]; i++)
        dest[i] = src[i];
    dest[i] = 0;
}
