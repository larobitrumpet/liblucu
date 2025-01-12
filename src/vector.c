#include "vector.h"
#include <stdint.h>

static inline int mod(int a, int b) {
	return ((a % b) + b) % b;
}

LUCU_VECTOR construct_vector(size_t bytewidth) {
	LUCU_VECTOR vector;
	vector.bytewidth = bytewidth;
	vector.size = LUCU_VECTOR_INIT_SIZE;
	vector.v = malloc(vector.bytewidth * (size_t)vector.size);
	vector.head = 0;
	vector.tail = 0;
	return vector;
}

bool deconstruct_vector_func(void* data, void* params) {
	void** pars = (void**)params;
	void (*free_function)(void*, void*) = (void (*)(void*, void*))((LUCU_GENERIC_FUNCTION*)pars[0])->f;
	void* par = pars[1];
	free_function(data, par);
	return false;
}

void deconstruct_vector(LUCU_VECTOR* vector, void (*free_function)(void*, void*), void* params) {
	if (free_function != NULL) {
		LUCU_GENERIC_FUNCTION ff;
		ff.f = (void (*)(void))free_function;
		void* pars[] = {(void*)&ff, params};
		vector_iterate(vector, deconstruct_vector_func, pars);
	}
	free(vector->v);
}

bool vector_is_empty(LUCU_VECTOR* vector) {
	return vector->head == vector->tail;
}

int vector_length(LUCU_VECTOR* vector) {
	return mod(vector->tail - vector->head, vector->size);
}

void vector_increase_size(LUCU_VECTOR* vector) {
	int old_size = vector->size;
	vector->size = (int)(old_size * LUCU_VECTOR_SIZE_INCREASE);
	void* new_q = malloc(vector->bytewidth * (size_t)vector->size);
	int i = vector->head;
	int j = 0;
	while (i != vector->tail) {
		memcpy((void*)((uintptr_t)new_q + (size_t)j * vector->bytewidth), (void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), vector->bytewidth);
		i = mod(i + 1, old_size);
		j++;
	}
	free(vector->v);
	vector->v = new_q;
	vector->head = 0;
	vector->tail = j;
}

void vector_enqueue(LUCU_VECTOR* vector, void* data) {
	if (vector->head == mod(vector->tail + 1, vector->size))
		vector_increase_size(vector);
	memcpy((void*)((uintptr_t)vector->v + (uintptr_t)vector->tail * vector->bytewidth), data, vector->bytewidth);
	vector->tail = mod(vector->tail + 1, vector->size);
}

int vector_dequeue(LUCU_VECTOR* vector, void* data) {
	if (vector_is_empty(vector)) {
		return 1;
	}
	memcpy(data, (void*)((uintptr_t)vector->v + (size_t)vector->head * vector->bytewidth), vector->bytewidth);
	vector->head = mod(vector->head + 1, vector->size);
	return 0;
}

int vector_index(LUCU_VECTOR* vector, void* data, bool (*equal)(void*, void*)) {
	int index = 0;
	int i = vector->head;
	while (i != vector->tail) {
		if (equal((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), data)) {
			return index;
		}
		i = mod(i + 1, vector->size);
		index++;
	}
	return -1;
}

int vector_local_index_to_global_index(LUCU_VECTOR* vector, int index) {
	return mod(vector->head + index, vector->size);
}

void vector_get(LUCU_VECTOR* vector, int index, void* data) {
	memcpy(data, (void*)((uintptr_t)vector->v + (size_t)vector_local_index_to_global_index(vector, index) * vector->bytewidth), vector->bytewidth);
}

void vector_remove(LUCU_VECTOR* vector, int index) {
	if (index >= vector_length(vector) || index < 0)
		return;
	int i = vector_local_index_to_global_index(vector, index);
	while (i != vector->tail) {
		memcpy((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), (void*)((uintptr_t)vector->v + (size_t)mod(i + 1, vector->size) * vector->bytewidth), vector->bytewidth);
		i = mod(i + 1, vector->size);
	}
	vector->tail = mod(vector->tail - 1, vector->size);
}

void vector_iterate(LUCU_VECTOR* vector, bool (*func)(void*, void*), void* params) {
	int i = vector->head;
	while (i != vector->tail) {
		if (func((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), params))
			break;
		i = mod(i + 1, vector->size);
	}
}

bool vector_filter_func(void* data, void* params) {
	void** pars = (void**)params;
	LUCU_VECTOR* vector = (LUCU_VECTOR*)pars[0];
	bool (*filter_func)(void*, void*) = (bool (*)(void*, void*))((LUCU_GENERIC_FUNCTION*)pars[1])->f;
	void* par = pars[2];
	if (filter_func(data, par)) {
		vector_enqueue(vector, data);
	}
	return false;
}

LUCU_VECTOR vector_filter(LUCU_VECTOR* vector, bool (*filter_func)(void*, void*), void* params) {
	LUCU_VECTOR new_vector = construct_vector(vector->bytewidth);
	LUCU_GENERIC_FUNCTION ff;
	ff.f = (void (*)(void))filter_func;
	void* pars[] = {(void*)&new_vector, (void*)&ff, params};
	vector_iterate(vector, vector_filter_func, (void*)pars);
	return new_vector;
}

bool vector_map_func(void* data, void* params) {
	void** pars = (void**)params;
	LUCU_VECTOR* vector = (LUCU_VECTOR*)pars[0];
	void* (*map_func)(void*, void*) = (void* (*)(void*, void*))((LUCU_GENERIC_FUNCTION*)pars[1])->f;
	void* par = pars[2];
	vector_enqueue(vector, map_func(data, par));
	return false;
}

LUCU_VECTOR vector_map(LUCU_VECTOR* vector, size_t target_bytewidth, void* (*map_func)(void*, void*), void* params) {
	LUCU_VECTOR new_vector = construct_vector(target_bytewidth);
	LUCU_GENERIC_FUNCTION mf;
	mf.f = (void (*)(void))map_func;
	void* pars[] = {(void*)&new_vector, (void*)&mf, params};
	vector_iterate(vector, vector_map_func, (void*)pars);
	return new_vector;
}

bool vector_min_max_func(void* data, void* params) {
	void** pars = (void**)params;
	bool (*compare_func)(void*, void*, void*) = (bool (*)(void*, void*, void*))((LUCU_GENERIC_FUNCTION*)pars[0])->f;
	void** min_max = (void**)pars[1];
	void* par = pars[2];
	if (compare_func(data, *min_max, par))
		*min_max = data;
	return false;
}

void* vector_min_max(LUCU_VECTOR* vector, bool (*compare_func)(void*, void*, void*), void* params) {
	void* min_max = (void*)((uintptr_t)vector->v + (size_t)vector->head * vector->bytewidth);
	LUCU_GENERIC_FUNCTION cf;
	cf.f = (void (*)(void))compare_func;
	void* pars[] = {(void*)&cf, (void*)&min_max, params};
	vector_iterate(vector, vector_min_max_func, (void*)pars);
	return min_max;
}
