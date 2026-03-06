/*
** EPITECH PROJECT, 2025
** str_ncpy
** File description:
** str_ncpy
*/

void str_ncpy(const char *src, char *dest, const int n)
{
    int i = 0;

    for (; src[i] && i < n; i++)
        dest[i] = src[i];
    dest[i] = 0;
}
