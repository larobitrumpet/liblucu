/// @file cache.h
#ifndef LUCU_CACHE_H
#define LUCU_CACHE_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct LucuCacheData LucuCacheData;

/**
 * Caches values to be used later. Essentially memoization with a set limit.
 */
typedef LucuCacheData* LucuCache;

/**
 * Creates a new `LucuCache`.
 *
 * A key is used to identify cached values. If the `LucuCache` contains
 * a matching key, it will return a pointer to the corresponding value.
 * If it doesn't, it will create the value and store it, evicting old values
 * if necessary.
 * @param cache_size The max number of elements to store at a time.
 * @param keys_equal_function Function used to determine if two keys are equal.
 * The first parameter is a pointer to the key value stored in the cache.
 * The second parameter is a pointer to the provided key being searched for.
 * And the third parameter is the value passed to `keys_equal_function_params`.
 * @param keys_equal_function_params A value passed as the third parameter to `keys_equal_function`.
 * @param generate_function Function used to create new values to be cached.
 * Takes a pointer to the key and returns a pointer to the created value.
 * @param key_free_function Function used to free memory used by a key when evicting
 * and destroying the cache. Can be NULL to do nothing.
 * @param value_free_function Function used to free memory used by a cached value
 * when evicting and destroying the cache. Can be NULL to do nothing.
 * @return A newly created `LucuCache`
 */
LucuCache lucu_cache_new(const int cache_size, bool (*keys_equal_function)(void*, void*, void*), void* keys_equal_function_params, void* (*generate_function)(void*), void (*key_free_function)(void*), void (*value_free_function)(void*));

/**
 * Frees the memory used by a `LucuCache` and any memory used
 * by cached values and keys.
 *
 * @param cache The `LucuCache` to destroy.
 */
void lucu_cache_destroy(LucuCache cache);

/**
 * Gets a value from a `LucuCache`.
 *
 * Returns a pointer to a value stored in a `LucuCache`.
 * May create a new value if one isn't already in the cache.
 *
 * @param cache The LucuCache to get from.
 * @param key The key used to get the value from the cache.
 * @return A pointer to the value in the cache corresponding to `key`.
 */
void* lucu_cache_get(LucuCache cache, void* key);

#endif
