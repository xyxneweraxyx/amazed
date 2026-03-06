/*
** EPITECH PROJECT, 2025
** parsing
** File description:
** Parsing lib.
*/

#ifndef PARSING
    #define PARSING

typedef struct str_arr {
    int skip; // Amount of words to potentially skip before writing to buffer
    int b_wrd; // Amount of words in buffer.
    int b_len; // Length of a single word in buffer.
    const char *exclude; // Characters to ignore.
} str_arr_t;

void str_to_array(const char *str, const char *sep, str_arr_t *par,
    char buf[par->b_wrd][par->b_len]);

#endif
