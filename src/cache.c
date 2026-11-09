#include <lucu/cache.h>
#include <lucu/heap.h>
#include <lucu/lucu.h>
#include <lucu/vector.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

static void lucu_cache_free_value(void* value, void* data) {
	LucuHeapFreeData* d = (LucuHeapFreeData*)data;

	if (d->key_free_function != NULL) {
		d->key_free_function(value, d->key_free_function_data);
	}

	if (d->value_free_function != NULL) {
		d->value_free_function((void*)((uintptr_t)value + d->key_bytewidth), d->value_free_function_data);
	}
}

static LucuCacheFreeData* lucu_cache_create_free_data(const size_t key_bytewidth, void (*key_free_function)(void*, void*), void* key_free_function_data, void (*value_free_function)(void*, void*), void* value_free_function_data) {
	LucuCacheFreeData* free_data = malloc(sizeof(LucuCacheFreeData));

	*free_data = (LucuCacheFreeData){
		.key_bytewidth = key_bytewidth,
		.key_free_function = key_free_function,
		.key_free_function_data = key_free_function_data,
		.value_free_function = value_free_function,
		.value_free_function_data = value_free_function_data,
	};

	return free_data;
}

bool lucu_cache_compare_function(void* a, void* b, void* _) {
	return *(int*) a <= *(int*)b;
}

LucuCache lucu_cache_new(const int cache_size, const size_t key_bytewidth, const size_t value_bytewidth, void (*key_free_function)(void*, void*), void* key_free_function_data, void (*value_free_function)(void*, void*), void* value_free_function_data, bool (*compare_function)(const void*, const void*, void*), void* compare_function_data, void* (*generate_function)(const void*, void*), void* generate_function_data) {

	LucuCacheFreeData* free_data = lucu_cache_create_free_data(
		key_bytewidth, key_free_function, key_free_function_data,
		value_free_function, value_free_function_data
	);
	
	LucuCache cache = {
		.c = lucu_heap_new_with_size(cache_size, sizeof(int), key_bytewidth + value_bytewidth, NULL, NULL, lucu_cache_free_value, free_data, lucu_cache_compare_function, NULL),
		.size = cache_size,
		.key_bytewidth = key_bytewidth,
		.value_bytewidth = value_bytewidth,
		.free_data = free_data,
		.compare_function = compare_function,
		.compare_function_data = compare_function_data,
		.generate_function = generate_function,
		.generate_function_data = generate_function_data,
	};

	return cache;
}

void lucu_cache_destroy(LucuCache* cache) {
	lucu_heap_destroy(&cache->c);
	free(cache->free_data);
}

void* lucu_cache_get(LucuCache* cache, void* key, void* value) {
	int index;

	// Because our data is in a binary min heap stored in an array based on the number of accesses,
	// keys that are accessed more often are more likely to be at the end of the array
	// (more likely to be further down the heap, towards the leaves).
	// So, searching the vector from back to front will give us our best bet on getting
	// the most accessed keys faster.
	//
	// It would be faster to store things in a hash table, but creating a good hash table
	// is complicated and finding a good hash function is very difficult, and would make
	// all of this a lot more complicated, and for what I'm doing this is good enough.
	//
	// Another approach would be to store data as a max heap instead. That would probably make
	// searching for more accessed keys faster, and we could evit the last element in the heap
	// if we care more about evicting a less accessed element than evicting the absolute least
	// frequently accessed element. This approach might be better,
	// I'll have to do some benchmark tests.
	for (index = lucu_vector_length(&cache->c.h) - 1; index >= 0; index--) {
		if (cache->compare_function(key, lucu_heap_get_value(&cache->c, index), cache->compare_function_data)) {
			break;
		}
	}

	if (index == -1) {
		void* generated_value = cache->generate_function(key, cache->generate_function_data);
		void* key_value = malloc(sizeof(cache->key_bytewidth + cache->value_bytewidth));
		memcpy(key_value, key, cache->key_bytewidth);
		memcpy((void*)((uintptr_t)key_value + cache->key_bytewidth), generated_value, cache->value_bytewidth);

		int zero = 0;

		if (lucu_heap_length(&cache->c) < cache->size) {
			// There is more space in the cache, just push
			lucu_heap_push(&cache->c, &zero, key_value);
			// Because we can guarentee that the thing we just
			// pushed is the only one with a key of zero, we know
			// that it bubbled up all the way to the top.
			// This does mean that adding values when the cache
			// still has space will always take Θ(log(n)) where n is
			// the size of the heap, which isn't ideal.
			index = 0;
		} else {
			// Evict least used and replace
			lucu_heap_replace(&cache->c, &zero, key_value);
			// Because we can guarentee that the thing we just
			// pushed is the only one with a key of zero and we
			// put it right at the top of the heap, it is the
			// first thing on the heap.
			// This means that adding values when the cache is full
			// will always be Θ(1), which is nice.
			index = 0;
		}

		free(key_value);
	}

	if (value == NULL) {
		value = malloc(sizeof(cache->value_bytewidth));
	}

	// We copy the value while we know where it is and then give the user that.
	// Otherwise we would need to find the value again after
	// we update the number of accesses and bubble down.
	// Which maybe isn't that bad since we know the key increments by one,
	// so either it stays where it is, or switches with one of its children
	// and doesn't go any further. I suppose which approach is faster depends on
	// the size of the value (how long `memcpy` takes), and how expensive
	// it is to compare keys. I might consider updating the key and finding
	// it again in the future, but this will do for now.
	memcpy(value, (void*)((uintptr_t)lucu_heap_get_value(&cache->c, index) + cache->key_bytewidth), cache->value_bytewidth);

	(*(int*)lucu_heap_get_key(&cache->c, index))++;
	lucu_heap_update(&cache->c, index);

	return value;
}
