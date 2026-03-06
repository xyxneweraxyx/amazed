/*
** EPITECH PROJECT, 2025
** str_cmp
** File description:
** str_cmp
*/

int str_cmp(const char *str1, const char *str2)
{
    int i = 0;

    for (; str1[i] && str2[i] && str2[i] == str1[i]; i++);
    return str1[i] - str2[i];
}
