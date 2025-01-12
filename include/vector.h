#ifndef LUCU_VECTOR_H
#define LUCU_VECTOR_H

#include "lucu.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define LUCU_VECTOR_INIT_SIZE 16
#define LUCU_VECTOR_SIZE_INCREASE 1.5

typedef struct LucuVector {
	void* v;
	size_t bytewidth;
	int size;
	int head;
	int tail;
} LucuVector;

LucuVector construct_vector(size_t bytewidth);
void deconstruct_vector(LucuVector* vector, void (*free_function)(void*, void*), void* params);
bool vector_is_empty(LucuVector* vector);
int vector_length(LucuVector* vector);
void vector_increase_size(LucuVector* vector);
void vector_enqueue(LucuVector* vector, void* data);
int vector_dequeue(LucuVector* vector, void* data);
int vector_index(LucuVector* vector, void* data, bool (*equal)(void*, void*));
int vector_local_index_to_global_index(LucuVector* vector, int index);
void vector_get(LucuVector* vector, int index, void* data);
void vector_iterate(LucuVector* vector, bool (*func)(void*, void*), void* params);
LucuVector vector_filter(LucuVector* vector, bool (*filter_func)(void*, void*), void* params);
LucuVector vector_map(LucuVector* vector, size_t target_bytewidth, void* (*map_func)(void*, void*), void* params);
void* vector_min_max(LucuVector* vector, bool (*compare_func)(void*, void*, void*), void* params);

#endif
