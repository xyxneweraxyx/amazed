/*
** EPITECH PROJECT, 2025
** str_cmp
** File description:
** str_cmp
*/

int str_ncmp(const char *restrict str1, const char *restrict str2, const int n)
{
    int i = 0;

    for (; str1[i] && str2[i] && str2[i] == str1[i] && i < n - 1; i++);
    return str1[i] - str2[i];
}
