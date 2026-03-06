/*
** EPITECH PROJECT, 2025
** c_alloc
** File description:
** Lib that wraps around malloc for extra safety.
*/

#include "./c_alloc.h"

/*
Initialize the main c_alloc array.
(Must be called before using any other function of the lib!)
Returns NULL if malloc fails.
*/
c_alloc_t *c_ini(uint16_t size)
{
    c_alloc_t *arr = malloc(sizeof(c_alloc_t));

    if (!arr)
        return NULL;
    arr->array_len = size;
    arr->debug = false;
    arr->addresses = malloc(sizeof(void *) * (size_t)size);
    if (!arr->addresses) {
        free(arr);
        return NULL;
    }
    for (uint16_t i = 0; i < size; i++)
        arr->addresses[i] = 0;
    return arr;
}

/*
Frees a c_alloc_t struct.
*/
void c_delete(c_alloc_t *arr, bool del_addresses)
{
    if (!arr)
        return;
    if (!arr->addresses) {
        if (arr->debug)
            printf("DELETE: %p - Success (No addresses found)\n", arr);
        free(arr);
        return;
    }
    for (uint16_t i = 0; del_addresses && i < arr->array_len; i++) {
        if (!arr->addresses[i])
            continue;
        if (arr->debug)
            printf("DELETE - %p inside of address array\n", arr->addresses[i]);
        if (arr->addresses[i])
            free(arr->addresses[i]);
    }
    if (arr->debug)
        printf("DELETE - %p - Success\n", arr);
    free(arr->addresses);
    free(arr);
}
