/*
** EPITECH PROJECT, 2025
** c_alloc
** File description:
** Lib that wraps around malloc for extra safety.
*/

#include "./c_alloc.h"

static void increase_arr_size(c_alloc_t *arr)
{
    uint16_t curr_size = arr->array_len;
    void **new = NULL;

    if (!arr)
        return;
    new = malloc(sizeof(void *) * (size_t)(curr_size + CALLOC_RAISE));
    if (!new)
        return;
    for (uint16_t i = 0; i < arr->array_len; i++)
        new[i] = arr->addresses[i];
    for (uint16_t i = 0; i < 50; i++)
        new[i + arr->array_len] = 0;
    arr->array_len += (uint16_t)CALLOC_RAISE;
    free(arr->addresses);
    arr->addresses = new;
}

/*
Helper function to allocate memory with retries.
*/
static void *alloc(size_t n, size_t count)
{
    void *allocated = NULL;

    for (int i = 0; i < CALLOC_TRIES; i++) {
        allocated = malloc(n * count);
        if (allocated)
            break;
    }
    return allocated;
}

/*
Helper function to replace a pointer in a c_alloc_t address list.
Avoids (null) addresses in the middle of the list.
*/
static void replace_pointer(c_alloc_t *arr, void *__p)
{
    size_t i = 0;
    size_t free = 0;

    for (; arr->addresses[free]; free++);
    if (arr->addresses[free - 1] == __p || free >= arr->array_len)
        return;
    for (; i < arr->array_len && arr->addresses[i] != __p; i++);
    arr->addresses[free] = __p;
    arr->addresses[i] = NULL;
}

/*
Allocate a chunk of memory.
On success, returns the address of the chunk. On failure, returns NULL.
*/
void *c_alloc(size_t n, size_t count, c_alloc_t *arr)
{
    void *allocated = alloc(n, count);
    uint16_t address = 0;

    if (!allocated)
        return NULL;
    for (size_t i = 0; i < n * count; i++)
        ((char *)allocated)[i] = 0;
    for (; address < arr->array_len && arr->addresses[address]; address++);
    if (address == arr->array_len)
        increase_arr_size(arr);
    arr->addresses[address] = allocated;
    return allocated;
}

/*
Attempts to free (if possible) a given pointer.
*/
void c_free(void *__p, c_alloc_t *arr)
{
    uint16_t pos = 0;

    for (; pos < arr->array_len && arr->addresses[pos] != __p; pos++);
    if (pos == arr->array_len) {
        return;
    }
    arr->addresses[pos] = NULL;
    free(__p);
}

/*
When allocated using c_alloc, sserts if a pointer exists, or not.
Returns the pointer's position if the pointer was found, CALLOC_FAIL otherwise.
*/
size_t c_assert_pointer(const void *__p, c_alloc_t *arr)
{
    uint16_t pos = 0;

    for (; pos < arr->array_len && arr->addresses[pos] != __p; pos++);
    return (pos == arr->array_len) ? CALLOC_FAIL : (size_t)pos;
}

/*
Reallocates an array "arr" to "size" and initializes extra space to be empty.
Only works with allocations done using c_alloc.
Returns the new array on success, or NULL on fail.
*/
void *c_realloc(c_alloc_t *c_arr, void *__arr,
    const c_realloc_t *sizes, const int blk_size)
{
    size_t pos = c_assert_pointer(__arr, c_arr);
    void *new_arr = NULL;

    if (pos == (size_t)CALLOC_FAIL) {
        return NULL;
    }
    new_arr = c_alloc(sizes->new_size, blk_size, c_arr);
    if (!new_arr)
        return NULL;
    for (size_t w = 0; w < sizes->new_size * blk_size &&
        w < sizes->old_size * blk_size; w++)
        ((char *)new_arr)[w] = ((char *)__arr)[w];
    c_free(__arr, c_arr);
    replace_pointer(c_arr, new_arr);
    return new_arr;
}

/*
Prints the array of addresses.
*/
void c_print(c_alloc_t *arr, bool skip_null)
{
    for (uint16_t i = 0; i < arr->array_len; i++) {
        if (skip_null && !arr->addresses[i])
            continue;
    }
}
