/// @file vector.h
#ifndef LUCU_VECTOR_H
#define LUCU_VECTOR_H

#include "lucu.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/**
 * Initial size of the vector.
 *
 * The number of elements to initially allocate memory for when creating a new
 * vector. **Must** be an integer greater than 0.
 */
#define LUCU_VECTOR_INIT_SIZE 16
/**
 * Size increase multiplier.
 *
 * When increasing the size of a vector, the size is multiplied by this
 * constant. **Must** be greater than 1.
 */
#define LUCU_VECTOR_SIZE_INCREASE 1.5

typedef struct LucuVectorInstance LucuVectorInstance;
/**
 * A dynamically sized circular array.
 *
 * A list that stores generic values.
 */
typedef LucuVectorInstance* LucuVector;

/**
 * Construct a new `LucuVector`.
 *
 * Creates a new `LucuVector`.
 * Create new `LucuVector`'s using **only** this function.
 *
 * @param bytewidth The number of bytes that an element takes up.
 * @param free_function Function used to free elements.
 * Can be `NULL` to forgo freeing elements.
 * Takes a `void*` pointing to the element to be freed.
 * @return A new `LucuVector`
 */
LucuVector lucu_construct_vector(const size_t bytewidth, void (* const free_function)(void*));

/**
 * Deconstructs a `LucuVector`.
 *
 * Frees the memory allocated to a `LucuVector` and optionally it's elements.
 * @param vector The `LucuVector` to deconstruct.
 */
void lucu_deconstruct_vector(LucuVector vector);

/**
 * Print the contents of a `LucuVector` to `stdout`.
 * @param vector `LucuVector` to print.
 * @param print_function Function used to print elements.
 * Takes a pointer to the element and `params`.
 * @param params Passed to `print_function`.
 */
void lucu_vector_print(LucuVector vector, void (*print_function)(void*, void*), void* params);

/**
 * Tests if a `LucuVector` is empty.
 *
 * Determines if `vector` contains any elements. `true` if `vector`
 * contains no elements and `false` if it contains at least one element.
 * @param vector The `LucuVector` to test.
 * @return `bool` of if `vector` is empty.
 */
bool lucu_vector_is_empty(LucuVector vector);

/**
 * Length of a `LucuVector`.
 *
 * Gives the number of elements stored in a `LucuVector`.
 * @param vector The `LucuVector` to test.
 * @return The number of elements in `vector`.
 */
int lucu_vector_length(LucuVector vector);

/**
 * Push an element to the back of a `LucuVector`.
 *
 * @param vector The `LucuVector` to append data to.
 * @param data Pointer to data to copy into `vector`.
 * @pre `data` is not `NULL` and points to valid data
 * the size of the `bytewidth` used to create `vector`.
 */
void lucu_vector_push_back(LucuVector vector, const void* const data);

/**
 * Push an element to the front of a `LucuVector`.
 *
 * @param vector The `LucuVector` to prepend data to.
 * @param data Pointer to data to copy into `vector`.
 * @pre `data` is not `NULL` and points to valid data
 * the size of the `bytewidth` used to create `vector`.
 */
void lucu_vector_push_front(LucuVector vector, const void* const data);

/**
 * Pop an element from the front of a `LucuVector`.
 *
 * @param vector The `LucuVector` to pop from.
 * @return Pointer to the element that was at the front of `vector`.
 * This data will have to be freed by the user.
 * Is `NULL` if `vector` is empty.
 */
void* lucu_vector_pop_front(LucuVector vector);

/**
 * Pop an element from the back of a `LucuVector`.
 *
 * @param vector The `LucuVector` to pop from.
 * @return Pointer to the element that was at the back of `vector`.
 * This data will have to be freed by the user.
 * Is `NULL` if `vector` is empty.
 */
void* lucu_vector_pop_back(LucuVector vector);

/**
 * Alias for `lucu_vector_push_back`.
 * Useful when using a `LucuVector` as a stack.
 */
void lucu_vector_push(LucuVector vector, const void* const data);

/**
 * Alias for `lucu_vector_pop_back`.
 * Useful when using a `LucuVector` as a stack.
 */
void* lucu_vector_pop(LucuVector vector);

/**
 * Alias for `lucu_vector_push_back`.
 * Useful when using a `LucuVector` as a queue.
 */
void lucu_vector_enqueue(LucuVector vector, const void* const data);

/**
 * Alias for `lucu_vector_pop_front`.
 * Useful when using a `LucuVector` as a queue.
 */
void* lucu_vector_dequeue(LucuVector vector);

/**
 * Swap the elements of `vector` at `index_1` and `index_2`.
 * @param vector `LucuVector` to modify.
 * @param index_1 One of the indexes to swap.
 * @param index_2 The other index to swap.
 */
void lucu_vector_swap(LucuVector vector, const int index_1, const int index_2);

/**
 * Gets the index of an element in a `LucuVector`.
 *
 * Compares each element of `vector` until it finds an element
 * equal to `data` according to the `equal` function and returns
 * the index of that element.
 * @param vector The `LucuVector` to search.
 * @param data Pointer to data to compare to.
 * @param equal Function used to determine if an element is equal to `data`,
 * taking a pointer to each and is passed `params`.
 * @param params Passed as the last argument to `equal`.
 * @return The index of the element found. Is -1 if the element cannot be found.
 */
int lucu_vector_index(LucuVector vector, void* const data, bool (* const equal)(void*, void*, void*), void* const params);

/**
 * Gets a pointer to the element at `index` of a `LucuVector`.
 *
 * @param vector 'LucuVector' to get the element from.
 * @param index Index of element.
 * @return Pointer to the element at `index`.
 * @pre `index` **must** be a valid index within the bounds of `vector`. Otherwise will give a pointer to junk data.
 */
void* lucu_vector_get(LucuVector vector, const int index);

/**
 * Remove an element from a `LucuVector`.
 *
 * @param vector `LucuVector` to remove element from.
 * @param index Index of the element to remove.
 * @pre `index` **must** be a valid index within the bounds of `vector`.
 */
void lucu_vector_remove(LucuVector vector, const int index);

/**
 * Insert an element at an index into a `LucuVector`.
 *
 * @param vector 'LucuVector' to insert into.
 * @param data Element to insert and copy into `vector`.
 * @param index Index to insert into.
*/
void lucu_vector_insert(LucuVector vector, const void* const data, const int index);

/**
 * Iterate over elements of a `LucuVector`.
 *
 * @param vector 'LucuVector' to iterate over.
 * @param func Function to apply to each element. Accepts a pointer
 * to the element and `params`. Returns `true` if iteration should stop
 * and `false` if iteration should continue.
 * @param params Passed to `func`.
 */
void lucu_vector_iterate(LucuVector vector, bool (*func)(void*, void*), void* params);

/**
 * Filters through a `LucuVector`.
 *
 * Iterates through elements in `LucuVector` that pass `filter_function`
 * and copies them into a new `LucuVector`. The original `vector` is unchanged.
 * @param vector `LucuVector` to filter through.
 * @param filter_function Function used to determine if an element should be
 * included. Accepts an element and `params`. Returns `true` if the element
 * should be included and `false` if it shouldn't.
 * @param param Passed to `filter_function`
 * @return A new LucuVector with copied elements
 */
LucuVector lucu_vector_filter(LucuVector vector, bool (* const filter_function)(void*, void*), void* const params);

/**
 * Maps elements of a `LucuVector`.
 *
 * Maps elements from `vector` into a new `LucuVector`.
 * @param vector 'LucuVector' to map from.
 * @param target_bytewidth The number of bytes that the mapped to data takes up.
 * @param target_free_function Function used to free mapped to data.
 * @param map_function Function used to map elements.
 * Takes an element from `vector` and `params`.
 * @param params Passed to `map_function`.
 */
LucuVector lucu_vector_map(LucuVector vector, size_t target_bytewidth, void (* const target_free_function)(void*), void* (* const map_function)(void*, void*), void* const params);

/**
 * Finds the min or max of a `LucuVector`.
 *
 * @param vector `LucuVector` to search through
 * @param compare_function Function used to determine if an element
 * is greater than or less than another element. Accepts the current
 * min or max, an element from `vector`, and `params`. Returns `true`
 * if the element from `vector` should replace the current min or max,
 * returns `false` otherwise.
 * @param params Passed to compare_function.
 */
void* lucu_vector_min_max(LucuVector vector, bool (* const compare_function)(void*, void*, void*), void* const params);

/**
 * Sorts a `LucuVector`.
 *
 * Modifies `vector` so that it is sorted.
 * Takes *O(n log(n))* time and *O(n)* extra space.
 * @param vector `LucuVector` to sort.
 * @param compare_function Function used to compare two elements.
 * Takes a pointer to each element and `params`. Returns `true` if
 * the first element should be sorted before the second element and
 * `false` if the second element should be sorted before the first element.
 */
void lucu_vector_sort(LucuVector vector, bool (*compare_function)(void*, void*, void*), void* params);

#endif
