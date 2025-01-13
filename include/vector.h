/// @file vector.h
#ifndef LUCU_VECTOR_H
#define LUCU_VECTOR_H

#include "lucu.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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

/**
 * A dynamically sized circular array.
 *
 * A list that stores generic values.
 * **Only** create with `lucu_construct_vector`.
 */
typedef struct LucuVector {
	/// Array where elements are stored
	void* v;
	/// The number of bytes that an element takes up
	size_t bytewidth;
	/// Number of elements that can be stored in the currently allocated space.
	/// **Not** the number of elements in the array.
	/// **Not** the number of bytes allocated.
	int size;
	/// The index of the first element of the circular array.
	int head;
	/// The index of the last element of the circular array plus 1 mod `size`.
	int tail;
} LucuVector;

/**
 * Construct a new `LucuVector`.
 *
 * Creates a new `LucuVector`.
 * **Only** use this function to create a new `LucuVector`.
 *
 * @param bytewidth The number of bytes that an element takes up.
 * @return A new `LucuVector`
 */
LucuVector lucu_construct_vector(size_t bytewidth);

/**
 * Deconstructs a `LucuVector`.
 *
 * Frees the memory allocated to a `LucuVector` and optionally it's elements.
 * @param vector The `LucuVector` to deconstruct.
 * @param free_function A function used to free the elements of the vector.
 * Can pass `NULL` to not free any of the elements. Takes a `void*`, a pointer
 * to the element, and a `void*` to `params`.
 * @param params A `void*` passed as the second argument to `free_function`
 */
void lucu_deconstruct_vector(LucuVector* vector, void (*free_function)(void*, void*), void* params);

/**
 * Tests if a `LucuVector` is empty.
 *
 * Determines if `vector` contains any elements. `true` if `vector`
 * contains no elements and `false` if it contains at least one element.
 * @param vector The `LucuVector` to test.
 * @return `bool` of if `vector` is empty.
 */
bool lucu_vector_is_empty(LucuVector* vector);

/**
 * Length of a `LucuVector`.
 *
 * Gives the number of elements stored in a `LucuVector`.
 * @param vector The `LucuVector` to test.
 * @return The number of elements in `vector`.
 */
int lucu_vector_length(LucuVector* vector);

/**
 * Enqueue an element to the beginning of a `LucuVector`.
 *
 * Copies the data pointed to by `data` to the beginning of `vector`.
 * @param vector The `LucuVector` to append data to.
 * @param data Pointer to data to copy into `vector`.
 * @pre `data` is not `NULL` and points to valid data
 * the size of the `bytewidth` used to create `vector`.
 */
void lucu_vector_enqueue(LucuVector* vector, void* data);

int lucu_vector_dequeue(LucuVector* vector, void* data);
int lucu_vector_index(LucuVector* vector, void* data, bool (*equal)(void*, void*));
void lucu_vector_get(LucuVector* vector, int index, void* data);
void lucu_vector_iterate(LucuVector* vector, bool (*func)(void*, void*), void* params);
LucuVector lucu_vector_filter(LucuVector* vector, bool (*filter_function)(void*, void*), void* params);
LucuVector lucu_vector_map(LucuVector* vector, size_t target_bytewidth, void* (*map_function)(void*, void*), void* params);
void* lucu_vector_min_max(LucuVector* vector, bool (*compare_function)(void*, void*, void*), void* params);

#endif
