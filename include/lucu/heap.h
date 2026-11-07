#ifndef LUCU_HEAP_H
#define LUCU_HEAP_H

#include <stdlib.h>
#include <stdbool.h>
#include <lucu/vector.h>

typedef struct LucuHeap {
	LucuVector h;
	const size_t key_bytewidth;
	const size_t value_bytewidth;
	bool (*compare_function)(void*, void*, void*);
	void* compare_function_data;
	void** key_value_free_data;
} LucuHeap;

LucuHeap lucu_heap_new(size_t key_bytewidth, size_t value_bytewidth, void (*key_freefunction)(void*, void*), void* key_free_function_data, void (*value_freefunction)(void*, void*), void* value_free_function_data, bool (*compare_function)(void*, void*, void*), void* compare_function_data);

LucuHeap lucu_heap_new_with_size(int length, size_t key_bytewidth, size_t value_bytewidth, void (*key_free_function)(void*, void*), void* key_free_function_data, void (*value_free_function)(void*, void*), void* value_free_function_data, bool (*compare_function)(void*, void*, void*), void* compare_function_data);

void lucu_heap_destroy(LucuHeap* heap);

void* lucu_heap_get_key(const LucuHeap* heap, int index);

void* lucu_heap_get_value(const LucuHeap* heap, int index);

int lucu_heap_length(const LucuHeap* heap);

void lucu_heap_push(LucuHeap* heap, void* key, void* value);

void lucu_heap_pop(LucuHeap* heap, void** key, void** value);

void lucu_heap_replace(LucuHeap* heap, void* key, void* value);

void lucu_heap_update(LucuHeap* heap, int index);

int lucu_heap_index(const LucuHeap* heap, const void* key, const void* value, bool (*equal)(const void*, const void*, const void*, const void*, void*), void* params);

#endif
