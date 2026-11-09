/// @file cache.h
#ifndef LUCU_CACHE_H
#define LUCU_CACHE_H

#include <lucu/heap.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct LucuCacheFreeData {
	size_t key_bytewidth;
	void (*key_free_function)(void*, void*);
	void* key_free_function_data;
	void (*value_free_function)(void*, void*);
	void* value_free_function_data;
} LucuCacheFreeData;

typedef struct LucuCache {
	LucuHeap c;
	const int size;
	const size_t key_bytewidth;
	const size_t value_bytewidth;
	LucuCacheFreeData* free_data;
	bool (*compare_function)(const void*, const void*, void*);
	void* compare_function_data;
	void* (*generate_function)(const void*, void*);
	void* generate_function_data;
} LucuCache;

/**
 * Creates a new `LucuCache`.
 *
 * A key is used to identify cached values. If the `LucuCache` contains
 * a matching key, it will return a pointer to the corresponding value.
 * If it doesn't, it will create the value and store it, evicting values if necessary.
 * It keeps track of how many times a value has been accessed, and the value that
 * has been accessed the fewest number of times will be evicted.
 * @param cache_size The max number of elements to store at a time.
 * @param key_bytewidth The number of bytes a key takes up.
 * @param value_bytewidth The number of bytes a value takes up.
 * @param key_free_function Function used to free memory used by a key when evicting
 * and destroying the cache. Can be NULL to do nothing.
 * @param key_free_function_data Passed as the second argument to `key_free_function`.
 * @param value_free_function Function used to free memory used by a cached value
 * when evicting and destroying the cache. Can be NULL to do nothing.
 * @param value_free_function_data Passed as the second argument to `value_free_function`.
 * @param compare_function Function used to determine if two keys are equal.
 * The first parameter is a pointer to the key value stored in the cache.
 * The second parameter is a pointer to the provided key being searched for.
 * And the third parameter is the value passed to `compare_function_data`.
 * @param compare_function_data A value passed as the third parameter to `keys_equal_function`.
 * @param generate_function Function used to create new values to be cached.
 * Takes a pointer to the key and `generate_function_data`, and returns a pointer to the created value.
 * @param generate_function_data Passed as the second argument to `generate_function`.
 * @return A newly created `LucuCache`
 */
LucuCache lucu_cache_new(int cache_size, size_t key_bytewidth, size_t value_bytewidth, void (*key_free_function)(void*, void*), void* key_free_function_data, void (*value_free_function)(void*, void*), void* value_free_function_data, bool (*compare_function)(const void*, const void*, void*), void* compare_function_data, void* (*generate_function)(const void*, void*), void* generate_function_data);

/**
 * Frees the memory used by a `LucuCache` and any memory used
 * by cached values and keys.
 *
 * @param cache The `LucuCache` to destroy.
 */
void lucu_cache_destroy(LucuCache* cache);

/**
 * Gets a value from a `LucuCache`.
 *
 * Returns a pointer to a value stored in a `LucuCache`.
 * May create a new value if one isn't already in the cache.
 *
 * @param cache The LucuCache to get from.
 * @param key The key used to get the value from the cache.
 * @param value Pointer to where the data will be stored.
 * If `NULL`, memory will be allocated with `malloc` to store the data.
 * @return Pointer to the value. The same as `value` if it was not `NULL`,
 * otherwise will point to the newly allocated memory that will have to be freed by the user.
 */
void* lucu_cache_get(LucuCache* cache, void* key, void* value);

#endif
