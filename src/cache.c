#include "lucu/cache.h"
#include "lucu/lucu.h"
#include "lucu/vector.h"
#include <assert.h>
#include <string.h>

typedef struct KeyValue {
	void* key;
	void* value;
	void (*key_free_function)(void*);
	void (*value_free_function)(void*);
} KeyValue;

static void keyvalue_destroy(void* keyvalue, void* _data) {
	(void)_data;
	KeyValue* kv = (KeyValue*)keyvalue;
	if (kv->key_free_function != NULL) {
		kv->key_free_function(kv->key);
	}
	if (kv->value_free_function != NULL) {
		kv->value_free_function(kv->value);
	}
}

struct LucuCacheData {
	LucuVector cache;
	int cache_size;
	bool (*keys_equal_function)(const void*, const void*, void*);
	void* keys_equal_function_params;
	void* (*generate_function)(void*);
	void (*key_free_function)(void*);
	void (*value_free_function)(void*);
};

LucuCache lucu_cache_new(const int cache_size, bool (*keys_equal_function)(const void*, const void*, void*), void* keys_equal_function_params, void* (*generate_function)(void*), void (*key_free_function)(void*), void (*value_free_function)(void*)) {
	LucuCache al = malloc(sizeof(LucuCacheData));
	LucuCacheData cache = {
		.cache = lucu_vector_new_with_size(cache_size, sizeof(KeyValue), keyvalue_destroy, NULL),
		.cache_size = cache_size,
		.generate_function = generate_function,
		.keys_equal_function = keys_equal_function,
		.keys_equal_function_params = keys_equal_function_params,
		.key_free_function = key_free_function,
		.value_free_function = value_free_function,
	};
	memcpy(al, &cache, sizeof(LucuCacheData));
	return al;
}

void lucu_cache_destroy(LucuCache cache) {
	lucu_vector_destroy(&cache->cache);
	free(cache);
}

static void evict(LucuCache cache) {
	KeyValue* keyvalue = lucu_vector_dequeue(&cache->cache, NULL);
	keyvalue_destroy(keyvalue, NULL);
	free(keyvalue);
}

static void insert(LucuCache cache, KeyValue* keyvalue) {
	if (lucu_vector_length(&cache->cache) == cache->cache_size) {
		evict(cache);
	}
	lucu_vector_enqueue(&cache->cache, keyvalue);
}

static bool key_matches(const void* keyvalue, const void* key, void* params) {
	const KeyValue* kv = (const KeyValue*)keyvalue;
	void** pars = (void**)params;
	bool (*keys_equal_function)(const void*, const void*, void*) = (bool(*)(const void*, const void*, void*))((LucuGenericFunction*)pars[0])->f;
	void* p = pars[1];
	return keys_equal_function(kv->key, key, p);
}

void* lucu_cache_get(LucuCache cache, void* key) {
	LucuGenericFunction ke = { (void(*)(void))cache->keys_equal_function };
	void* params[] = {(void*)&ke, cache->keys_equal_function_params};
	int i = lucu_vector_index(&cache->cache, key, key_matches, params);
	if (i == -1) {
		void* value = cache->generate_function(key);
		KeyValue keyvalue = {
			.key = key,
			.value = value,
			.key_free_function = cache->key_free_function,
			.value_free_function = cache->value_free_function
		};
		insert(cache, &keyvalue);
		i = lucu_vector_length(&cache->cache) - 1;
	}
	KeyValue* keyvalue = lucu_vector_get(&cache->cache, i);
	return keyvalue->value;
}
