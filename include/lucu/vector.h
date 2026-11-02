/// @file vector.h
#ifndef LUCU_VECTOR_H
#define LUCU_VECTOR_H

#include <stdlib.h>
#include <stdbool.h>

/**
 * A dynamic array that stored generic values.
 */
typedef struct LucuVector {
	/// Array where elements are stored
	void* v;
	/// The number of bytes that an element takes up
	const size_t bytewidth;
	/// Number of elements that can be stored in the currently allocated space.
	/// **Not** the number of elements in the array.
	/// **Not** the number of bytes allocated.
	int size;
	/// Number of elements currently stored in the array.
	int length;
	/// Function used to free elements of the `LucuVector`.
	/// See `lucu_vector_new` for more information
	void (*free_function)(void*);
	/// The minimum size that the vector should take up
	const int min_size;
} LucuVector;

/**
 * Create a new `LucuVector`.
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
LucuVector lucu_vector_new(size_t bytewidth, void (*free_function)(void*));

/**
 * Create a `LucuVector` with a specified allocated size
 *
 * Creates a `LucuVector` so that it can hold `length` elements
 * without needing to reallocate.
 * @param length Minimum number of elements to allocate memory for.
 * @param bytewidth The number of bytes that an element takes up.
 * @param free_function Function used to free elements (see `lucu_vector_new`).
 */
LucuVector lucu_vector_new_with_size(int length, size_t bytewidth, void (*free_function)(void*));

/**
 * Deconstructs a `LucuVector`.
 *
 * Frees the memory allocated to a `LucuVector` and optionally it's elements.
 * @param vector The `LucuVector` to deconstruct.
 */
void lucu_vector_destroy(LucuVector* vector);

/**
 * Creates a new `LucuVector` from an existing array.
 *
 * Copies the elements of `arr` into a new `LucuVector`.
 * @param arr Array to create `LucuVector` from.
 * @param length The number of elements in `arr`. **Must** be >= 1.
 * @param bytewidth The number of bytes that an element takes up.
 * @param free_function Function used to free elements (see `lucu_vector_new`)
 */
LucuVector lucu_vector_from_array(const void* arr, int length, size_t bytewidth, void (*free_function)(void*));

/**
 * Returns a pointer to the data inside the vector.
 *
 * Sometimes you need to use a function, perhaps from another library,
 * that needs the data in a vector. This function exposes that data.
 * **DO NOT USE UNLESS YOU KNOW WHAT YOU ARE DOING!!!**
 * Directly modifying the data in a `LucuVector` can lead to unexpected results.
 * You have been warned!
 * @param vector `LucuVector` to get data from.
 * @return Pointer to the internal data contained within the vector.
 */
void* lucu_vector_get_data(const LucuVector* vector);

/**
 * Print the contents of a `LucuVector` to `stdout`.
 * @param vector `LucuVector` to print.
 * @param print_function Function used to print elements.
 * Takes a pointer to the element and `params`.
 * @param params Passed to `print_function`.
 */
void lucu_vector_print(const LucuVector* vector, void (*print_function)(void*, void*), void* params);

/**
 * Tests if a `LucuVector` is empty.
 *
 * Determines if `vector` contains any elements. `true` if `vector`
 * contains no elements and `false` if it contains at least one element.
 * @param vector The `LucuVector` to test.
 * @return `bool` of if `vector` is empty.
 */
bool lucu_vector_is_empty(const LucuVector* vector);

/**
 * Length of a `LucuVector`.
 *
 * Gives the number of elements stored in a `LucuVector`.
 * @param vector The `LucuVector` to test.
 * @return The number of elements in `vector`.
 */
int lucu_vector_length(const LucuVector* vector);

/**
 * Push an element to the back of a `LucuVector`.
 *
 * @param vector The `LucuVector` to append data to.
 * @param data Pointer to data to copy into `vector`.
 * @pre `data` is not `NULL` and points to valid data
 * the size of the `bytewidth` used to create `vector`.
 */
void lucu_vector_push_back(LucuVector* vector, const void* data);

/**
 * Push an element to the front of a `LucuVector`.
 *
 * @param vector The `LucuVector` to prepend data to.
 * @param data Pointer to data to copy into `vector`.
 * @pre `data` is not `NULL` and points to valid data
 * the size of the `bytewidth` used to create `vector`.
 */
void lucu_vector_push_front(LucuVector* vector, const void* data);

/**
 * Pop an element from the front of a `LucuVector`.
 *
 * @param vector The `LucuVector` to pop from.
 * @param data Pointer to where the data will be stored.
 * If `NULL`, memory will be allocated with `malloc` to store the data.
 * @return Pointer to the popped data. `NULL` if `vector` is empty,
 * the same as `data` if it was not `NULL`, otherwise will point to
 * the newly allocated memory that will have to be freed by the user.
 */
void* lucu_vector_pop_front(LucuVector* vector, void* data);

/**
 * Pop an element from the back of a `LucuVector`.
 *
 * @param vector The `LucuVector` to pop from.
 * @param data Pointer to where the data will be stored.
 * If `NULL`, memory will be allocated with `malloc` to store the data.
 * @return Pointer to the popped data. `NULL` if `vector` is empty,
 * the same as `data` if it was not `NULL`, otherwise will point to
 * the newly allocated memory that will have to be freed by the user.
 */
void* lucu_vector_pop_back(LucuVector* vector, void* data);

/**
 * Alias for `lucu_vector_push_back`.
 * Useful when using a `LucuVector` as a stack.
 */
void lucu_vector_push(LucuVector* vector, const void* data);

/**
 * Alias for `lucu_vector_pop_back`.
 * Useful when using a `LucuVector` as a stack.
 */
void* lucu_vector_pop(LucuVector* vector, void* data);

/**
 * Alias for `lucu_vector_push_back`.
 * Useful when using a `LucuVector` as a queue.
 */
void lucu_vector_enqueue(LucuVector* vector, const void* data);

/**
 * Alias for `lucu_vector_pop_front`.
 * Useful when using a `LucuVector` as a queue.
 */
void* lucu_vector_dequeue(LucuVector* vector, void* data);

/**
 * Swap the elements of `vector` at `index_1` and `index_2`.
 * @param vector `LucuVector` to modify.
 * @param index_1 One of the indexes to swap.
 * @param index_2 The other index to swap.
 */
void lucu_vector_swap(LucuVector* vector, int index_1, int index_2);

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
int lucu_vector_index(const LucuVector* vector, const void* data, bool (*equal)(const void*, const void*, void*), void* params);

/**
 * Gets a pointer to the element at `index` of a `LucuVector`.
 *
 * @param vector 'LucuVector' to get the element from.
 * @param index Index of element.
 * @return Pointer to the element at `index`.
 * @pre `index` **must** be a valid index within the bounds of `vector`. Otherwise will give a pointer to junk data.
 */
void* lucu_vector_get(const LucuVector* vector, int index);

/**
 * Remove an element from a `LucuVector`.
 *
 * Removes an element from a `LucuVector` and shifts over elements to take it place,
 * thus preserving the order with a performance of O(n). If you don't need the order
 * preserved, prefer useing `lucu_vector_swap_remove` instead.
 * @param vector `LucuVector` to remove element from.
 * @param index Index of the element to remove.
 * @pre `index` **must** be a valid index within the bounds of `vector`.
 */
void lucu_vector_remove(LucuVector* vector, int index);

/**
 * Remove an element from a `LucuVector`.
 *
 * Removes an element from a `LucuVector` and replacing it with the last element in the vector.
 * This doesn't preserve the order of elements, but has a performance of O(1).
 * If you need to preserve the order, use `lucu_vector_remove` instead.
 * preserved, use `lucu_vector_swap_remove`.
 * @param vector `LucuVector` to remove element from.
 * @param index Index of the element to remove.
 * @pre `index` **must** be a valid index within the bounds of `vector`.
 */
void lucu_vector_swap_remove(LucuVector* vector, int index);

/**
 * Insert an element at an index into a `LucuVector`.
 *
 * @param vector 'LucuVector' to insert into.
 * @param data Element to insert and copy into `vector`.
 * @param index Index to insert into. If the index is greater than the bounds
 * of `vector`, the element will be inserted at the end of the vector.
*/
void lucu_vector_insert(LucuVector* vector, const void* data, int index);

/**
 * Shortens a `LucuVector` to `length`.
 *
 * Keeps the first `length` elements and removes the rest.
 * Has no effect if `length` is greater than or equal to the length of the `LucuVector`.
 * Does not effect the size of the `LucuVector`.
 * @param vector `LucuVector` to truncate.
 * @param length The new length to shorten to.
 */
void lucu_vector_truncate(LucuVector* vector, int length);

/**
 * Reserves capacity for at least `additional` more elements to be added to a `LucuVector`.
 *
 * May reserve more space than requested to speculatively avoid frequent reallocations.
 * Does nothing if there is already enough space.
 * @param vector `LucuVector` to reserve space for.
 * @param additional How many additional elements to the current length to reserve space for.
 */
void lucu_vector_reserve(LucuVector* vector, int additional);

/**
 * Reserves capacity for `additional` more elements to be added to a `LucuVector`.
 *
 * Unlike `lucu_vector_reserve`, this will not reallocate more than is requested.
 * Does nothing if there is already enough space.
 * Prefer to use `lucu_vector_reserve` if additional insertions will be performed.
 * @param vector `LucuVector` to reserve space for.
 * @param additional How many additional elements to the current length to reserve space for.
 */
void lucu_vector_reserve_exact(LucuVector* vector, int additional);

/**
 * Shrinks the size of a `LucuVector` to be as small as possible.
 *
 * Does not effect the length of the `LucuVector`, only how much memory it is allocated.
 * May have some additional capacity for small `LucuVector`s just like `lucu_vector_new_with_size`.
 * @param vector `LucuVector` to shrink
 */
void lucu_vector_shrink_to_fit(LucuVector* vector);

/**
 * Shrinks the size of a `LucuVector` to a minimum size.
 *
 * Does not effect the length of the `LucuVector`, only how much memory it is allocated.
 * The size will be at least the length of the `LucuVector`.
 * Does nothing if the size is already less than `min_size`.
 */
void lucu_vector_shrink_to(LucuVector* vector, int min_size);

/**
 * Iterate over elements of a `LucuVector`.
 *
 * @param vector 'LucuVector' to iterate over.
 * @param func Function to apply to each element. Accepts a pointer
 * to the element and `params`. Returns `true` if iteration should stop
 * and `false` if iteration should continue.
 * @param params Passed to `func`.
 */
void lucu_vector_iterate(const LucuVector* vector, bool (*func)(const void*, void*), void* params);

/**
 * Mutable version of `lucu_vector_iterate`
 *
 * @param vector 'LucuVector' to iterate over.
 * @param func Function to apply to each element. Accepts a pointer
 * to the element and `params`. Returns `true` if iteration should stop
 * and `false` if iteration should continue.
 * @param params Passed to `func`.
 */
void lucu_vector_mut_iterate(LucuVector* vector, bool (*func)(void*, void*), void* params);

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
LucuVector lucu_vector_filter(const LucuVector* vector, bool (*filter_function)(const void*, void*), void* params);

/**
 * Maps elements of a `LucuVector`.
 *
 * Maps elements from `vector` into a new `LucuVector`.
 * @param vector 'LucuVector' to map from.
 * @param target_bytewidth The number of bytes that the mapped to data takes up.
 * @param target_free_function Function used to free mapped to data.
 * @param map_function Function used to map elements.
 * Takes an element from `vector` and `params`.
 * @param map_func_return_free Function used to free the data returned by `map_function`
 * after it has been copied into `vector`. Can be `NULL` to not free any data.
 * @param params Passed to `map_function`.
 */
LucuVector lucu_vector_map(const LucuVector* vector, size_t target_bytewidth, void (*target_free_function)(void*), void* (*map_function)(const void*, void*), void (*map_func_return_free)(void*), void* params);

/**
 * Finds a superlative of a `LucuVector` based on a comparison function.
 *
 * Useful for finding the min or max in a `LucuVector`, or whatever
 * superlative you need.
 * @param vector `LucuVector` to search through
 * @param compare_function Function used to determine if an element
 * is greater than or less than another element. Accepts the current
 * superlative, an element from `vector`, and `params`. Returns `true`
 * if the element from `vector` should replace the current superlative,
 * returns `false` otherwise.
 * @param params Passed to `compare_function`.
 */
void* lucu_vector_superlative(const LucuVector* vector, bool (*compare_function)(const void*, const void*, void*), void* params);

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
 * @param params Passed to `compare_function`.
 */
void lucu_vector_sort(LucuVector* vector, bool (*compare_function)(const void*, const void*, void*), void* params);

#endif
