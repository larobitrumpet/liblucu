#ifndef LUCU_VECTOR_H
#define LUCU_VECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define LUCU_VECTOR_INIT_SIZE 16
#define LUCU_VECTOR_SIZE_INCREASE 1.5

typedef struct LUCU_VECTOR {
	void* v;
	size_t bytewidth;
	int size;
	int head;
	int tail;
} LUCU_VECTOR;

LUCU_VECTOR construct_vector(size_t bytewidth);
void deconstruct_vector(LUCU_VECTOR* vector, void (*free_function)(void*, void*), void* params);
bool vector_is_empty(LUCU_VECTOR* vector);
int vector_length(LUCU_VECTOR* vector);
void vector_increase_size(LUCU_VECTOR* vector);
void vector_enqueue(LUCU_VECTOR* vector, void* data);
int vector_dequeue(LUCU_VECTOR* vector, void* data);
int vector_index(LUCU_VECTOR* vector, void* data, bool (*equal)(void*, void*));
int vector_local_index_to_global_index(LUCU_VECTOR* vector, int index);
void vector_get(LUCU_VECTOR* vector, int index, void* data);
void vector_iterate(LUCU_VECTOR* vector, bool (*func)(void*, void*), void* params);
LUCU_VECTOR vector_filter(LUCU_VECTOR* vector, bool (*filter_func)(void*, void*), void* params);
LUCU_VECTOR vector_map(LUCU_VECTOR* vector, size_t target_bytewidth, void* (*map_func)(void*, void*), void* params);
void* vector_min_max(LUCU_VECTOR* vector, bool (*compare_func)(void*, void*, void*), void* params);

#endif
