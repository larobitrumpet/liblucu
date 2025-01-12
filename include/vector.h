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

LucuVector lucu_construct_vector(size_t bytewidth);
void lucu_deconstruct_vector(LucuVector* vector, void (*free_function)(void*, void*), void* params);
bool lucu_vector_is_empty(LucuVector* vector);
int lucu_vector_length(LucuVector* vector);
void lucu_vector_enqueue(LucuVector* vector, void* data);
int lucu_vector_dequeue(LucuVector* vector, void* data);
int lucu_vector_index(LucuVector* vector, void* data, bool (*equal)(void*, void*));
void lucu_vector_get(LucuVector* vector, int index, void* data);
void lucu_vector_iterate(LucuVector* vector, bool (*func)(void*, void*), void* params);
LucuVector lucu_vector_filter(LucuVector* vector, bool (*filter_function)(void*, void*), void* params);
LucuVector lucu_vector_map(LucuVector* vector, size_t target_bytewidth, void* (*map_function)(void*, void*), void* params);
void* lucu_vector_min_max(LucuVector* vector, bool (*compare_function)(void*, void*, void*), void* params);

#endif
