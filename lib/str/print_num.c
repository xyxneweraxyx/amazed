/*
** EPITECH PROJECT, 2026
** print_num
** File description:
** print_num
*/

#include <unistd.h>

int print_num(int nbr)
{
    int size = 1;
    char buff[16] = {0};

    if (nbr < 0)
        return 84;
    for (int total = 1; total * 10 < nbr; total *= 10)
        size += 1;
    for (int i = 0; i < size; i++) {
        buff[size - i - 1] = (char)((nbr % 10) + '0');
        nbr /= 10;
    }
    write(STDOUT_FILENO, buff, size);
    return 0;
}
