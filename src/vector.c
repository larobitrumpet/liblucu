#include "../include/vector.h"

static inline int mod(int a, int b) {
	return ((a % b) + b) % b;
}

LucuVector lucu_construct_vector(size_t bytewidth, void (*free_function)(void*)) {
	LucuVector vector;
	vector.bytewidth = bytewidth;
	vector.size = LUCU_VECTOR_INIT_SIZE;
	vector.v = malloc(vector.bytewidth * (size_t)vector.size);
	vector.head = 0;
	vector.tail = 0;
	vector.free_function = free_function;
	return vector;
}

static bool lucu_deconstruct_vector_func(void* data, void* params) {
	void (*free_function)(void*) = (void (*)(void*))((LucuGenericFunction*)params)->f;
	free_function(data);
	return false;
}

void lucu_deconstruct_vector(LucuVector* vector) {
	if (vector->free_function != NULL) {
		LucuGenericFunction ff;
		ff.f = (void (*)(void))vector->free_function;
		lucu_vector_iterate(vector, lucu_deconstruct_vector_func, (void*)&ff);
	}
	free(vector->v);
}

bool lucu_vector_is_empty(LucuVector* vector) {
	return vector->head == vector->tail;
}

int lucu_vector_length(LucuVector* vector) {
	return mod(vector->tail - vector->head, vector->size);
}

static void lucu_vector_increase_size(LucuVector* vector) {
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

void lucu_vector_push_front(LucuVector* vector, void* data) {
	if (vector->head == mod(vector->tail + 1, vector->size)) {
		lucu_vector_increase_size(vector);
	}
	memcpy((void*)((uintptr_t)vector->v + (uintptr_t)vector->tail * vector->bytewidth), data, vector->bytewidth);
	vector->tail = mod(vector->tail + 1, vector->size);
}

void* lucu_vector_pop_front(LucuVector* vector) {
	if (lucu_vector_is_empty(vector)) {
		return NULL;
	}
	void* data = malloc(vector->bytewidth);
	memcpy(data, (void*)((uintptr_t)vector->v + (size_t)vector->head * vector->bytewidth), vector->bytewidth);
	if (vector->free_function != NULL) {
		vector->free_function((void*)((uintptr_t)vector->v + (size_t)vector->head * vector->bytewidth));
	}
	vector->head = mod(vector->head + 1, vector->size);
	return data;
}

int lucu_vector_index(LucuVector* vector, void* data, bool (*equal)(void*, void*)) {
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

static int lucu_vector_local_index_to_global_index(LucuVector* vector, int index) {
	return mod(vector->head + index, vector->size);
}

void lucu_vector_get(LucuVector* vector, int index, void* data) {
	memcpy(data, (void*)((uintptr_t)vector->v + (size_t)lucu_vector_local_index_to_global_index(vector, index) * vector->bytewidth), vector->bytewidth);
}

void lucu_vector_remove(LucuVector* vector, int index) {
	if (index >= lucu_vector_length(vector) || index < 0)
		return;
	int i = lucu_vector_local_index_to_global_index(vector, index);
	while (i != vector->tail) {
		memcpy((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), (void*)((uintptr_t)vector->v + (size_t)mod(i + 1, vector->size) * vector->bytewidth), vector->bytewidth);
		i = mod(i + 1, vector->size);
	}
	vector->tail = mod(vector->tail - 1, vector->size);
}

void lucu_vector_iterate(LucuVector* vector, bool (*func)(void*, void*), void* params) {
	int i = vector->head;
	while (i != vector->tail) {
		if (func((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), params))
			break;
		i = mod(i + 1, vector->size);
	}
}

static bool lucu_vector_filter_func(void* data, void* params) {
	void** pars = (void**)params;
	LucuVector* vector = (LucuVector*)pars[0];
	bool (*filter_func)(void*, void*) = (bool (*)(void*, void*))((LucuGenericFunction*)pars[1])->f;
	void* par = pars[2];
	if (filter_func(data, par)) {
		lucu_vector_enqueue(vector, data);
	}
	return false;
}

LucuVector lucu_vector_filter(LucuVector* vector, bool (*filter_func)(void*, void*), void* params) {
	LucuVector new_vector = lucu_construct_vector(vector->bytewidth);
	LucuGenericFunction ff;
	ff.f = (void (*)(void))filter_func;
	void* pars[] = {(void*)&new_vector, (void*)&ff, params};
	lucu_vector_iterate(vector, lucu_vector_filter_func, (void*)pars);
	return new_vector;
}

static bool lucu_vector_map_func(void* data, void* params) {
	void** pars = (void**)params;
	LucuVector* vector = (LucuVector*)pars[0];
	void* (*map_func)(void*, void*) = (void* (*)(void*, void*))((LucuGenericFunction*)pars[1])->f;
	void* par = pars[2];
	lucu_vector_enqueue(vector, map_func(data, par));
	return false;
}

LucuVector lucu_vector_map(LucuVector* vector, size_t target_bytewidth, void* (*map_func)(void*, void*), void* params) {
	LucuVector new_vector = lucu_construct_vector(target_bytewidth);
	LucuGenericFunction mf;
	mf.f = (void (*)(void))map_func;
	void* pars[] = {(void*)&new_vector, (void*)&mf, params};
	lucu_vector_iterate(vector, lucu_vector_map_func, (void*)pars);
	return new_vector;
}

static bool lucu_vector_min_max_func(void* data, void* params) {
	void** pars = (void**)params;
	bool (*compare_func)(void*, void*, void*) = (bool (*)(void*, void*, void*))((LucuGenericFunction*)pars[0])->f;
	void** min_max = (void**)pars[1];
	void* par = pars[2];
	if (compare_func(data, *min_max, par))
		*min_max = data;
	return false;
}

void* lucu_vector_min_max(LucuVector* vector, bool (*compare_func)(void*, void*, void*), void* params) {
	void* min_max = (void*)((uintptr_t)vector->v + (size_t)vector->head * vector->bytewidth);
	LucuGenericFunction cf;
	cf.f = (void (*)(void))compare_func;
	void* pars[] = {(void*)&cf, (void*)&min_max, params};
	lucu_vector_iterate(vector, lucu_vector_min_max_func, (void*)pars);
	return min_max;
}
