#include <lucu/lucu.h>
#include <lucu/vector.h>
#include <lucu/heap.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

static void lucu_heap_free_key_value(void* key_value, void* data) {
	void** d = (void**)data;

	size_t key_bytewidth = *(size_t*)d[0];
	void (*key_free_function)(void*, void*) = (void (*)(void*, void*))((LucuGenericFunction*)d[3])->f;
	void* key_free_function_data = d[2];
	void (*value_free_function)(void*, void*) = (void (*)(void*, void*))((LucuGenericFunction*)d[3])->f;
	void* value_free_function_data = d[4];

	if (key_free_function != NULL) {
		key_free_function(key_value, key_free_function_data);
	}

	if (value_free_function != NULL) {
		value_free_function((void*)((uintptr_t)key_value + key_bytewidth), value_free_function_data);
	}
}

static void** lucu_heap_create_key_value_free_data(const size_t key_bytewidth, void (*key_free_function)(void*), void* key_free_function_data, void (*value_free_function)(void*), void* value_free_function_data) {
	size_t* key_bytewidth_p = malloc(sizeof(size_t));
	*key_bytewidth_p = key_bytewidth;

	LucuGenericFunction* key_free_function_p = malloc(sizeof(LucuGenericFunction));
	*key_free_function_p = (LucuGenericFunction){ .f = (void (*)(void))key_free_function };

	LucuGenericFunction* value_free_function_p = malloc(sizeof(LucuGenericFunction));
	*value_free_function_p = (LucuGenericFunction){ .f = (void (*)(void))value_free_function };

	void** key_value_free_data = malloc(sizeof(void*) * 5);
	key_value_free_data[0] = key_bytewidth_p;
	key_value_free_data[1] = key_free_function_p;
	key_value_free_data[2] = key_free_function_data;
	key_value_free_data[3] = value_free_function_p;
	key_value_free_data[4] = value_free_function_data;

	return key_value_free_data;
}

LucuHeap lucu_heap_new(const size_t key_bytewidth, const size_t value_bytewidth, void (*key_free_function)(void*), void* key_free_function_data, void (*value_free_function)(void*), void* value_free_function_data, bool (*compare_function)(void*, void*, void*), void* compare_function_data) {
	assert(compare_function != NULL);

	void** key_value_free_data = lucu_heap_create_key_value_free_data(
		key_bytewidth, key_free_function, key_free_function_data,
		value_free_function, value_free_function_data
	);

	LucuHeap heap = {
		.h = lucu_vector_new(
			key_bytewidth + value_bytewidth,
			lucu_heap_free_key_value, key_value_free_data
		),
		.key_bytewidth = key_bytewidth,
		.value_bytewidth = value_bytewidth,
		.compare_function = compare_function,
		.compare_function_data = compare_function_data,
		.key_value_free_data = key_value_free_data,
	};

	return heap;
}

LucuHeap lucu_heap_new_with_size(const int length, const size_t key_bytewidth, const size_t value_bytewidth, void (*key_free_function)(void*), void* key_free_function_data, void (*value_free_function)(void*), void* value_free_function_data, bool (*compare_function)(void*, void*, void*), void* compare_function_data) {
	assert(compare_function != NULL);

	void** key_value_free_data = lucu_heap_create_key_value_free_data(
		key_bytewidth, key_free_function, key_free_function_data,
		value_free_function, value_free_function_data
	);

	LucuHeap heap = {
		.h = lucu_vector_new_with_size(
			length, key_bytewidth + value_bytewidth,
			lucu_heap_free_key_value, key_value_free_data
		),
		.key_bytewidth = key_bytewidth,
		.value_bytewidth = value_bytewidth,
		.compare_function = compare_function,
		.compare_function_data = compare_function_data,
		.key_value_free_data = key_value_free_data,
	};

	return heap;
}

void lucu_heap_destroy(LucuHeap* heap) {
	lucu_vector_destroy(&heap->h);
	free(heap->key_value_free_data[0]);
	free(heap->key_value_free_data[1]);
	free(heap->key_value_free_data[3]);
	free(heap->key_value_free_data);
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

void* lucu_heap_get_key(const LucuHeap* heap, const int index) {
	return lucu_vector_get(&heap->h, index);
}

void* lucu_heap_get_value(const LucuHeap* heap, const int index) {
	return (void*)((uintptr_t)lucu_vector_get(&heap->h, index) + heap->key_bytewidth);
}

int lucu_heap_length(const LucuHeap* heap) {
	return lucu_vector_length(&heap->h);
}

static void lucu_heap_bubble_up(LucuHeap* heap, int index) {
	if (index == 0) {
		return;
	}

	int parent = lucu_heap_parent_index(index);
	if (!heap->compare_function(
		lucu_heap_get_key(heap, parent),
		lucu_heap_get_key(heap, index),
		heap->compare_function_data
	)) {
		lucu_vector_swap(&heap->h, parent, index);

		lucu_heap_bubble_up(heap, parent);
	}
}

static void lucu_heap_bubble_down(LucuHeap* heap, int index) {
	int left = lucu_heap_left_index(index);
	int right = lucu_heap_right_index(index);

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
		}

		lucu_vector_swap(&heap->h, index, left);

		lucu_heap_bubble_down(heap, left);
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

static void* lucu_heap_pair_key_value(size_t key_bytewidth, size_t value_bytewidth, void* key, void* value) {
	void* key_value = malloc(key_bytewidth + value_bytewidth);

	memcpy(key_value, key, key_bytewidth);
	memcpy((void*)((uintptr_t)key_value + key_bytewidth), value, value_bytewidth);

	return key_value;
}

void lucu_heap_push(LucuHeap* heap, void* key, void* value) {
	if (heap->value_bytewidth == 0) {
		lucu_vector_push(&heap->h, key);
		lucu_heap_bubble_up(heap, lucu_heap_length(heap) - 1);
		return;
	}

	void* key_value = lucu_heap_pair_key_value(heap->key_bytewidth, heap->value_bytewidth, key, value);
	lucu_vector_push(&heap->h, key_value);
	free(key_value);
	lucu_heap_bubble_up(heap, lucu_heap_length(heap) - 1);
}

void lucu_heap_pop(LucuHeap* heap, void** key, void** value) {
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

void lucu_heap_replace(LucuHeap* heap, void* key, void* value) {
	if (lucu_heap_length(heap) == 0) {
		lucu_heap_push(heap, key, value);
	}

	heap->h.free_function(lucu_vector_get(&heap->h, 0), heap->h.free_function_data);

	if (heap->value_bytewidth == 0) {
		memcpy(lucu_vector_get(&heap->h, 0), key, heap->key_bytewidth);
		lucu_heap_bubble_down(heap, 0);
		return;
	}

	void* key_value = lucu_heap_pair_key_value(heap->key_bytewidth, heap->value_bytewidth, key, value);

	memcpy(lucu_vector_get(&heap->h, 0), key_value, heap->key_bytewidth + heap->value_bytewidth);

	lucu_heap_bubble_down(heap, 0);
	free(key_value);
}

void lucu_heap_update(LucuHeap* heap, int index) {
	lucu_heap_bubble_up(heap, index);
	lucu_heap_bubble_down(heap, index);
}
