/*
** EPITECH PROJECT, 2025
** c_alloc
** File description:
** Lib that wraps around malloc for extra safety.
*/

#ifndef C_ALLOC_H
    #define C_ALLOC_H

    // Includes
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <stdint.h>

    // Definitions
    #define CALLOC_SUCC 0
    #define CALLOC_FAIL -1
    #define CALLOC_TRIES 10
    #define CALLOC_RAISE 50

// Structs

typedef struct c_alloc_arr {
    uint16_t array_len;
    void **addresses;
    bool debug;
} c_alloc_t;

typedef struct c_realloc_size {
    size_t old_size;
    size_t new_size;
} c_realloc_t;

// Functions

/*
Initialize the main c_alloc array.
(Must be called before using any other function of the lib!)
Returns NULL if malloc fails.
*/
c_alloc_t *c_ini(uint16_t size);

/*
Frees a c_alloc_t struct.
*/
void c_delete(c_alloc_t *arr, bool del_addresses);

/*
Allocate a chunk of memory.
On success, returns the address of the chunk. On failure, returns NULL.
*/
void *c_alloc(size_t n, size_t count, c_alloc_t *arr);

/*
Attempts to free (if possible) a given pointer.
*/
void c_free(void *__p, c_alloc_t *arr);

/*
Reallocates an array "arr" to "size" and initializes extra space to be empty.
Only works with allocations done using c_alloc.
Returns the new array on success, or NULL on fail.
*/
void *c_realloc(c_alloc_t *c_arr, void *restrict __arr,
    const c_realloc_t *restrict sizes, const int blk_size);

/*
When allocated using c_alloc, sserts if a pointer exists, or not.
Returns the pointer's position if the pointer was found, CALLOC_FAIL otherwise.
*/
size_t c_assert_pointer(const void *restrict __p, c_alloc_t *arr);

/*
Prints the array of addresses.
*/
void c_print(c_alloc_t *arr, bool skip_null);

#endif
