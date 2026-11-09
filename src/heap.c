#include <lucu/lucu.h>
#include <lucu/vector.h>
#include <lucu/heap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void lucu_heap_free_key_value(void* const key_value, void* const data) {
	LucuHeapFreeData* d = (LucuHeapFreeData*)data;

	if (d->key_free_function != NULL) {
		d->key_free_function(key_value, d->key_free_function_data);
	}

	if (d->value_free_function != NULL) {
		d->value_free_function((void*)((uintptr_t)key_value + d->key_bytewidth), d->value_free_function_data);
	}
}

static LucuHeapFreeData* lucu_heap_create_free_data(const size_t key_bytewidth, void (* const key_free_function)(void*, void*), void* const key_free_function_data, void (* const value_free_function)(void*, void*), void* const value_free_function_data) {
	LucuHeapFreeData* const free_data = malloc(sizeof(LucuHeapFreeData));

	const LucuHeapFreeData fd = {
		.key_bytewidth = key_bytewidth,
		.key_free_function = key_free_function,
		.key_free_function_data = key_free_function_data,
		.value_free_function = value_free_function,
		.value_free_function_data = value_free_function_data,
	};

	memcpy(free_data, &fd, sizeof(LucuHeapFreeData));

	return free_data;
}

LucuHeap lucu_heap_new(const size_t key_bytewidth, const size_t value_bytewidth, void (* const key_free_function)(void*, void*), void* const key_free_function_data, void (* const value_free_function)(void*, void*), void* const value_free_function_data, bool (* const compare_function)(void*, void*, void*), void* const compare_function_data) {
	assert(compare_function != NULL);

	LucuHeapFreeData* const free_data = lucu_heap_create_free_data(
		key_bytewidth, key_free_function, key_free_function_data,
		value_free_function, value_free_function_data
	);

	const LucuHeap heap = {
		.h = lucu_vector_new(
			key_bytewidth + value_bytewidth,
			lucu_heap_free_key_value, free_data
		),
		.key_bytewidth = key_bytewidth,
		.value_bytewidth = value_bytewidth,
		.compare_function = compare_function,
		.compare_function_data = compare_function_data,
		.free_data = free_data,
	};

	return heap;
}

LucuHeap lucu_heap_new_with_size(const int length, const size_t key_bytewidth, const size_t value_bytewidth, void (* const key_free_function)(void*, void*), void* const key_free_function_data, void (* const value_free_function)(void*, void*), void* const value_free_function_data, bool (* const compare_function)(void*, void*, void*), void* const compare_function_data) {
	assert(compare_function != NULL);

	LucuHeapFreeData* const free_data = lucu_heap_create_free_data(
		key_bytewidth, key_free_function, key_free_function_data,
		value_free_function, value_free_function_data
	);

	const LucuHeap heap = {
		.h = lucu_vector_new_with_size(
			length, key_bytewidth + value_bytewidth,
			lucu_heap_free_key_value, free_data
		),
		.key_bytewidth = key_bytewidth,
		.value_bytewidth = value_bytewidth,
		.compare_function = compare_function,
		.compare_function_data = compare_function_data,
		.free_data = free_data,
	};

	return heap;
}

void lucu_heap_destroy(LucuHeap* heap) {
	lucu_vector_destroy(&heap->h);
	free(heap->free_data);
}

static inline int lucu_heap_left_index(const int index) {
	return 2 * index + 1;
}

static inline int lucu_heap_right_index(const int index) {
	return 2 * index + 2;
}

static inline int lucu_heap_parent_index(const int index) {
	return (index - 1) / 2;
}

void* lucu_heap_get_key(const LucuHeap* const heap, const int index) {
	return lucu_vector_get(&heap->h, index);
}

void* lucu_heap_get_value(const LucuHeap* const heap, const int index) {
	return (void*)((uintptr_t)lucu_vector_get(&heap->h, index) + heap->key_bytewidth);
}

int lucu_heap_length(const LucuHeap* const heap) {
	return lucu_vector_length(&heap->h);
}

static void lucu_heap_bubble_up(LucuHeap* const heap, const int index) {
	if (index == 0) {
		return;
	}

	const int parent = lucu_heap_parent_index(index);
	if (!heap->compare_function(
		lucu_heap_get_key(heap, parent),
		lucu_heap_get_key(heap, index),
		heap->compare_function_data
	)) {
		lucu_vector_swap(&heap->h, parent, index);

		lucu_heap_bubble_up(heap, parent);
	}
}

static void lucu_heap_bubble_down(LucuHeap* const heap, const int index) {
	const int left = lucu_heap_left_index(index);
	const int right = lucu_heap_right_index(index);

	if (left >= lucu_heap_length(heap)) {
		return;
	}

	if (!heap->compare_function(
		lucu_heap_get_key(heap, index),
		lucu_heap_get_key(heap, left),
		heap->compare_function_data
	)) {
		if (!heap->compare_function(
			lucu_heap_get_key(heap, index),
			lucu_heap_get_key(heap, right),
			heap->compare_function_data
		)) {
			if (heap->compare_function(
				lucu_heap_get_key(heap, left),
				lucu_heap_get_key(heap, right),
				heap->compare_function_data
			)) {
				lucu_vector_swap(&heap->h, index, left);

				lucu_heap_bubble_down(heap, left);
			} else {
				lucu_vector_swap(&heap->h, index, right);

				lucu_heap_bubble_down(heap, right);
			}
		} else {
			lucu_vector_swap(&heap->h, index, left);

			lucu_heap_bubble_down(heap, left);
		}
	}

	if (right >= lucu_heap_length(heap)) {
		return;
	}

	if (!heap->compare_function(
		lucu_heap_get_key(heap, index),
		lucu_heap_get_key(heap, right),
		heap->compare_function_data
	)) {
		lucu_vector_swap(&heap->h, index, right);

		lucu_heap_bubble_down(heap, right);
	}
}

static void* lucu_heap_pair_key_value(const size_t key_bytewidth, const size_t value_bytewidth, const void* const key, const void* const value) {
	void* const key_value = malloc(key_bytewidth + value_bytewidth);

	memcpy(key_value, key, key_bytewidth);
	memcpy((void*)((uintptr_t)key_value + key_bytewidth), value, value_bytewidth);

	return key_value;
}

void lucu_heap_push(LucuHeap* const heap, const void* const key, const void* const value) {
	if (heap->value_bytewidth == 0) {
		lucu_vector_push(&heap->h, key);
		lucu_heap_bubble_up(heap, lucu_heap_length(heap) - 1);
		return;
	}

	void* const key_value = lucu_heap_pair_key_value(heap->key_bytewidth, heap->value_bytewidth, key, value);
	lucu_vector_push(&heap->h, key_value);
	free(key_value);
	lucu_heap_bubble_up(heap, lucu_heap_length(heap) - 1);
}

void lucu_heap_pop(LucuHeap* const heap, void** const key, void** const value) {
	if (lucu_heap_length(heap) == 0) {
		return;
	}

	if (key != NULL && *key == NULL) {
		*key = malloc(heap->key_bytewidth);
	}

	if (value != NULL && *value == NULL && heap->value_bytewidth != 0) {
		*value = malloc(heap->value_bytewidth);
	}

	if (key != NULL) {
		memcpy(*key, lucu_vector_get(&heap->h, 0), heap->key_bytewidth);
	}

	if (value != NULL && heap->value_bytewidth != 0) {
		memcpy(*value, (void*)((uintptr_t)lucu_vector_get(&heap->h, 0) + heap->key_bytewidth), heap->value_bytewidth);
	}

	lucu_vector_swap_remove(&heap->h, 0);

	lucu_heap_bubble_down(heap, 0);
}

void lucu_heap_replace(LucuHeap* const heap, const void* const key, const void* const value) {
	if (lucu_heap_length(heap) == 0) {
		lucu_heap_push(heap, key, value);
	}

	heap->h.free_function(lucu_vector_get(&heap->h, 0), heap->h.free_function_data);

	if (heap->value_bytewidth == 0) {
		memcpy(lucu_vector_get(&heap->h, 0), key, heap->key_bytewidth);
		lucu_heap_bubble_down(heap, 0);
		return;
	}

	void* const key_value = lucu_heap_pair_key_value(heap->key_bytewidth, heap->value_bytewidth, key, value);

	memcpy(lucu_vector_get(&heap->h, 0), key_value, heap->key_bytewidth + heap->value_bytewidth);

	lucu_heap_bubble_down(heap, 0);
	free(key_value);
}

void lucu_heap_update(LucuHeap* const heap, const int index) {
	lucu_heap_bubble_up(heap, index);
	lucu_heap_bubble_down(heap, index);
}

static bool lucu_heap_index_func(const void* const data1, const void* const data2, void* const params) {
	void** pars = (void**)params;

	bool (* const equal)(const void*, const void*, const void*, const void*, void*) = (bool (*)(const void*, const void*, const void*, const void*, void*))((LucuGenericFunction*)pars[0])->f;
	const size_t key_bytewidth = *(size_t*)pars[1];
	void* par = pars[2];

	const void* key1 = data1;
	const void* value1 = (void*)((uintptr_t)data1 + key_bytewidth);
	const void* key2 = data2;
	const void* value2 = (void*)((uintptr_t)data2 + key_bytewidth);

	return equal(key1, value1, key2, value2, par);
}

int lucu_heap_index(const LucuHeap* const heap, const void* const key, const void* const value, bool (* const equal)(const void*, const void*, const void*, const void*, void*), void* const params) {
	void* key_value;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wcast-qual"
	if (heap->value_bytewidth != 0 && value != NULL) {
		key_value = malloc(sizeof(heap->key_bytewidth + heap->value_bytewidth));
		memcpy(key_value, key, heap->key_bytewidth);
		memcpy((void*)((uintptr_t)key_value + heap->key_bytewidth), value, heap->value_bytewidth);
	} else {
		key_value = (void*)key;
	}

	LucuGenericFunction eq = { (void (*)(void))equal };

	void* pars[] = {(void*)&eq, (void*)&heap->key_bytewidth, params};
	#pragma GCC diagnostic pop

	const int index = lucu_vector_index(&heap->h, key_value, lucu_heap_index_func, pars);

	if (heap->value_bytewidth != 0 && value != NULL) {
		free(key_value);
	}

	return index;
}
