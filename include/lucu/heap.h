/// @file heap.h
#ifndef LUCU_HEAP_H
#define LUCU_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <lucu/vector.h>

typedef struct LucuHeapFreeData {
	const size_t key_bytewidth;
	void (* const key_free_function)(void*, void*);
	void* const key_free_function_data;
	void (* const value_free_function)(void*, void*);
	void* const value_free_function_data;
} LucuHeapFreeData;

/**
* A binary max heap that can optionally hold a value with it's key.
*
* The key is what is compared to organize the heap,
* and each key can also be associated with a value.
*/
typedef struct LucuHeap {
	/// `LucuVector` where the data of the heap is stored.
	LucuVector h;
	/// Number of bytes a key takes up.
	const size_t key_bytewidth;
	/// Number of bytes a value takes up (can be 0).
	const size_t value_bytewidth;
	/// Function that is used to compare keys to organize the heap.
	/// Takes two keys and is passed `compare_function_data`.
	/// Returns `true` if the first key is greater than or equal to the second key,
	/// returns `false` otherwise. Reverse this in order to get a min heap.
	bool (* const compare_function)(void*, void*, void*);
	/// Is passed as the third argument to `compare_function`.
	void* const compare_function_data;
	/// All the information that is required to free all the data that `LucuHeap` has allocated.
	LucuHeapFreeData* const free_data;
} LucuHeap;

/**
* Creates a new `LucuHeap`.
*
* @param key_bytewidth The number of bytes a key takes up.
* @param value_bytewidth The number of bytes a value takes up.
* Can be 0 if you only need the key.
* @param key_free_function Function used to free any data that a key has allocated.
* Takes the key and `key_free_function_data`. Can be `NULL`.
* @param key_free_function_data Passed as the second argument to `key_free_function`.
* @param value_free_function Function used to free any data that a value has allocated.
* Takes the data and `value_free_function_data`. Can be `NULL`.
* @param compare_function Function used to compare keys to organize the heap.
* Takes two keys and is passed `compare_function_data`.
* Returns `true` if the first key is greater than or equal to the second key,
* returns `false` otherwise. Reverse this in order to get a min heap.
* @param compare_function_data Passed as the third argument to `compare_function`.
* @return A new `LucuHeap`.
*/
LucuHeap lucu_heap_new(size_t key_bytewidth, size_t value_bytewidth, void (*key_free_function)(void*, void*), void* key_free_function_data, void (*value_free_function)(void*, void*), void* value_free_function_data, bool (*compare_function)(void*, void*, void*), void* compare_function_data);

/**
* Creates a new `LucuHeap` with a specified size.
*
* Creates a `LucuHeap` so that is can hold `length` key-value pairs without
* needing to reallocate.
*
 * @param length Minimum number of elements to allocate memory for.
* @param key_bytewidth The number of bytes a key takes up.
* @param value_bytewidth The number of bytes a value takes up.
* Can be 0 if you only need the key.
* @param key_free_function Function used to free any data that a key has allocated.
* Takes the key and `key_free_function_data`. Can be `NULL`.
* @param key_free_function_data Passed as the second argument to `key_free_function`.
* @param value_free_function Function used to free any data that a value has allocated.
* Takes the data and `value_free_function_data`. Can be `NULL`.
* @param compare_function Function used to compare keys to organize the heap.
* Takes two keys and is passed `compare_function_data`.
* Returns `true` if the first key is greater than or equal to the second key,
* returns `false` otherwise. Reverse this in order to get a min heap.
* @param compare_function_data Passed as the third argument to `compare_function`.
* @return A new `LucuHeap`
*/
LucuHeap lucu_heap_new_with_size(int length, size_t key_bytewidth, size_t value_bytewidth, void (*key_free_function)(void*, void*), void* key_free_function_data, void (*value_free_function)(void*, void*), void* value_free_function_data, bool (*compare_function)(void*, void*, void*), void* compare_function_data);

/**
 * Deconstructs a `LucuHeap`.
 *
 * Frees the memory allocated to a `LucuHeap` and optionally it's elements.
 * @param heap The `LucuHeap` to deconstruct.
 */
void lucu_heap_destroy(LucuHeap* heap);

/**
 * Returns a pointer to the key at `index`.
 *
 * @param heap The heap to get the key from.
 * @param index Index for the key you want to get.
 * @return Pointer to the key at `index`.
 */
void* lucu_heap_get_key(const LucuHeap* heap, int index);

/**
* Returns a pointer to the value at `index`.
*
* @param heap The heap to get the value from.
* @param index Index for the value you want.
* @param Pointer to the value at `index`.
*/
void* lucu_heap_get_value(const LucuHeap* heap, int index);

/**
* Returns the length of a `LucuHeap` (i.e. the number of key-value pairs that are stored)
*
* @param heap Heap to get the length of.
* @return The length of the heap.
*/
int lucu_heap_length(const LucuHeap* heap);

/**
* Pushes a key-value onto the heap.
*
* @param Heap to push to.
* @param key Key to push
* @param value Value to store with the key (can be NULL if you aren't storing values (if `value_bytewidth` is 0)).
*/
void lucu_heap_push(LucuHeap* heap, const void* key, const void* value);

/**
* Pops a key-value from the top of the heap.
*
* @param heap Heap to pop from
* @param[out] key Pointer to store a pointer to the key.
* If `NULL`, will discard the key.
* If points to `NULL`, will allocate space to copy the key to
* and stores the pointer to this into `key`, which will need to be freed.
* Otherwise will store the key to where `key` points.
* @param[out] value Pointer to store a pointer to the value.
* If `NULL`, will discard the value.
* If points to `NULL`, will allocate space to copy the value to
* and stores the pointer to this into `value`, which will need to be freed.
* Otherwise will store the value to where `value` points.
*/
void lucu_heap_pop(LucuHeap* heap, void** key, void** value);

/**
* Replaces the top of the heap with a new key-value.
*
* A more efficent way of popping and then pushing.
* @param heap Heap ro replace key-value
* @param key Key to push
* @param value Value to store with the key (can be NULL if you aren't storing values (if `value_bytewidth` is 0)).
*/
void lucu_heap_replace(LucuHeap* heap, const void* key, const void* value);

/**
* Updates the element at index.
*
* If you change the key at `index`, immediately call this so the heap can update.
* @param heap Heap to update.
* @param index Index to update.
*/
void lucu_heap_update(LucuHeap* heap, int index);

/**
* Gets the index of an element.
*
* Compares each element of the heap until it finds one that matches the key and/or value specified and returns the index.
* @param heap Heap to search
* @param key Key to compare to (can be `NULL` if you only care about the value).
* @param value Value to compare to (can be `NULL` if you only care about the key).
* @param equal Function used to compare if two key-values are the same.
* Takes the provided key, the provided value, the key to compare to, the value to compare to, and `params`.
* Returns `true` if the elements match.
* @param params Passed as the fifth element to `equal`.
* @return The index of the element found. If no element could be found, returns -1.
*/
int lucu_heap_index(const LucuHeap* heap, const void* key, const void* value, bool (*equal)(const void*, const void*, const void*, const void*, void*), void* params);

#endif
