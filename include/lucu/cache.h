#ifndef LUCU_CACHE_H
#define LUCU_CACHE_H

#include <stdlib.h>

typedef struct LucuCacheData LucuCacheData;
typedef LucuCacheData* LucuCache;

LucuCache lucu_cache_new(const int cache_size, const size_t key_size, const size_t value_size, bool (*keys_equal_function)(void*, void*, void*), void* keys_equal_function_params, void* (*generate_function)(void*), void (*key_free_function)(void*), void (*value_free_function)(void*));

void lucu_cache_destroy(LucuCache cache);

void* lucu_cache_get(LucuCache cache, void* key);

#endif
