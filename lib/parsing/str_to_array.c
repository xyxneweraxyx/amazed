/*
** EPITECH PROJECT, 2025
** str_to_array
** File description:
** Convert a string to an array based on a number of seps and a max amount.
*/

#include "./parsing.h"

static int c_s(const char c, const char *str)
{
    int i = 0;

    if (!str)
        return -1;
    for (i = 0; str[i] != c && str[i]; i++);
    return (str[i] == 0) ? -1 : i;
}

static int skip_wrds(const char *str, const char *sep, str_arr_t *par)
{
    int skip = 0;
    int cur = 0;

    for (cur = 0; skip < par->skip && str[cur]; cur++) {
        if (c_s(str[cur], sep) == -1 && c_s(str[cur + 1], sep) != -1)
            skip++;
    }
    if (skip != par->skip)
        return -2;
    for (; c_s(str[cur], sep) != -1; cur++);
    return cur;
}

static void write_single_char(int *word, int *write, str_arr_t *par,
    char buf[par->b_wrd][par->b_len])
{
    buf[*word][*write] = 0;
    (*word)++;
    (*write) = 0;
}

void str_to_array(const char *str, const char *sep, str_arr_t *par,
    char buf[par->b_wrd][par->b_len])
{
    int word = 0;
    int cur = skip_wrds(str, sep, par) + 1;
    int write = 1;

    if (cur == -1)
        return;
    buf[0][0] = str[cur - 1];
    for (; word < par->b_wrd - 1 && str[cur]; cur++) {
        if (c_s(str[cur], par->exclude) != -1)
            continue;
        if (c_s(str[cur - 1], sep) == -1 && c_s(str[cur], sep) != -1)
            write_single_char(&word, &write, par, buf);
        if (c_s(str[cur], sep) == -1 && write == par->b_len - 1)
            continue;
        if (c_s(str[cur], sep) == -1) {
            buf[word][write] = str[cur];
            write++;
        }
    }
    buf[word + 1][0] = 0;
}
