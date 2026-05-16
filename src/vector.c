#include "lucu/lucu.h"
#include "lucu/vector.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

/**
 * Initial size of the vector.
 *
 * The number of elements to initially allocate memory for when creating a new
 * vector. **Must** be an integer greater than 0.
 */
#define LUCU_VECTOR_INIT_SIZE 16
/**
 * Size increase multiplier.
 *
 * When increasing the size of a vector, the size is multiplied by this
 * constant. **Must** be greater than 1.
 */
#define LUCU_VECTOR_SIZE_INCREASE 1.5

/*static inline int mod(const int a, const int b) {
	return ((a % b) + b) % b;
}*/

static void lucu_vector_increase_size(LucuVector* vector);

LucuVector lucu_vector_new(const size_t bytewidth, void (* const free_function)(void*)) {
	return lucu_vector_new_with_size(LUCU_VECTOR_INIT_SIZE, bytewidth, free_function);
}

LucuVector lucu_vector_new_with_size(const int size, const size_t bytewidth, void (* const free_function)(void*)) {
	const int s = size == 0 ? LUCU_VECTOR_INIT_SIZE : size;

	LucuVector vector = {
		.bytewidth = bytewidth,
		.size = s,
		.v = malloc(bytewidth * (size_t)s),
		.length = 0,
		.free_function = free_function,
	};

	return vector;
}

static bool lucu_vector_destroy_func(void* const data, void* params) {
	void (*free_function)(void*) = (void (*)(void*))((LucuGenericFunction*)params)->f;
	free_function(data);
	return false;
}

void lucu_vector_destroy(LucuVector* vector) {
	if (vector->free_function != NULL) {
		LucuGenericFunction ff = { (void (*)(void))vector->free_function };
		lucu_vector_mut_iterate(vector, lucu_vector_destroy_func, (void*)&ff);
	}
	free(vector->v);
}

LucuVector lucu_vector_from_array(const void* const arr, const int length, const size_t bytewidth, void (* const free_function)(void*)) {
	assert(length > 0);
	LucuVector vector = lucu_vector_new_with_size(length, bytewidth, free_function);
	memcpy(vector.v, arr, (size_t)length * bytewidth);
	vector.length = length;
	return vector;
}

void* lucu_vector_get_data(const LucuVector* vector) {
	return vector->v;
}

static bool lucu_vector_print_func(const void* const data, void* params) {
	void** pars = (void**)params;
	void (*print_function)(const void*, void*) = (void (*)(const void*, void*))((LucuGenericFunction*)pars[0])->f;
	void* par = pars[1];
	print_function(data, par);
	printf(",");
	return false;
}

void lucu_vector_print(const LucuVector* vector, void (* const print_function)(void*, void*), void* params) {
	printf("[");

	LucuGenericFunction pf = { (void (*)(void))print_function };
	void* pars[] = {(void*)&pf, params};
	lucu_vector_iterate(vector, lucu_vector_print_func, (void*)pars);

	printf("]");
}

bool lucu_vector_is_empty(const LucuVector* vector) {
	return vector->length == 0;
}

int lucu_vector_length(const LucuVector* vector) {
	return vector->length;
}

static void lucu_vector_increase_size(LucuVector* vector) {
	vector->size = (int)(vector->size * LUCU_VECTOR_SIZE_INCREASE);
	vector->v = realloc(vector->v, vector->bytewidth * (size_t)vector->size);
}

void lucu_vector_push_back(LucuVector* vector, const void* const data) {
	if (vector->length == vector->size) {
		lucu_vector_increase_size(vector);
	}
	memcpy(lucu_vector_get(vector, vector->length), data, vector->bytewidth);
	vector->length++;
}

void lucu_vector_push(LucuVector* vector, const void* const data) {
	lucu_vector_push_back(vector, data);
}

void lucu_vector_enqueue(LucuVector* vector, const void* const data) {
	lucu_vector_push_back(vector, data);
}

void lucu_vector_push_front(LucuVector* vector, const void* const data) {
	if (lucu_vector_length(vector) == vector->size) {
		lucu_vector_increase_size(vector);
	}
	// TODO: Do benchmark testing to see if it is better to copy each
	// element one at a time, or to just use memmove
	for (int i = vector->length; i > 0; i--) {
		memcpy(lucu_vector_get(vector, i), lucu_vector_get(vector, i - 1), vector->bytewidth);
	}
	memcpy(vector->v, data, vector->bytewidth);
	vector->length++;
}

void* lucu_vector_pop_front(LucuVector* vector, void* data) {
	if (vector->length == 0) {
		return NULL;
	}

	if (data == NULL) {
		data = malloc(vector->bytewidth);
	}

	memcpy(data, vector->v, vector->bytewidth);

	vector->length--;

	// TODO: Do benchmark testing to see if it is better to copy each
	// element one at a time, or to just use memmove
	for (int i = 0; i < vector->length; i++) {
		memcpy(lucu_vector_get(vector, i), lucu_vector_get(vector, i + 1), vector->bytewidth);
	}

	return data;
}

void* lucu_vector_dequeue(LucuVector* vector, void* data) {
	return lucu_vector_pop_front(vector, data);
}

void* lucu_vector_pop_back(LucuVector* vector, void* data) {
	if (vector->length == 0) {
		return NULL;
	}

	if (data == NULL) {
		data = malloc(vector->bytewidth);
	}

	vector->length--;

	memcpy(data, lucu_vector_get(vector, vector->length), vector->bytewidth);

	return data;
}

void* lucu_vector_pop(LucuVector* vector, void* data) {
	return lucu_vector_pop_back(vector, data);
}

void lucu_vector_swap(LucuVector* vector, const int index_1, const int index_2) {
	void* tmp = malloc(vector->bytewidth);
	memcpy(tmp, lucu_vector_get(vector, index_1), vector->bytewidth);
	memcpy(lucu_vector_get(vector, index_1), lucu_vector_get(vector, index_2), vector->bytewidth);
	memcpy(lucu_vector_get(vector, index_2), tmp, vector->bytewidth);
	free(tmp);
}

static bool lucu_vector_index_func(const void* const data, void* params) {
	void** pars = (void**)params;
	// And here you can see we put back the `const` qualifier that we dropped
	const void* d = pars[0];
	bool (*equal)(const void*, const void*, void*) = (bool (*)(const void*, const void*, void*))((LucuGenericFunction*)pars[1])->f;
	int* index = pars[2];
	void* par = pars[3];
	if (equal(data, d, par)) {
		return true;
	}
	*index += 1;
	return false;
}

int lucu_vector_index(const LucuVector* vector, const void* const data, bool (* const equal)(const void*, const void*, void*), void* const params) {
	int index = 0;
	LucuGenericFunction eq = { (void (*)(void))equal };
	#pragma GCC diagnostic push
	// We will not be modifying `data`, but `lucu_vector_iterate` doesn't
	// have `const` for `params` since the user may want to modify their
	// own params and we need to pack everything in a signle structure,
	// so we must drop the `const` qualifier so the user can still enjoy
	// having `const` in the function signature.
	#pragma GCC diagnostic ignored "-Wcast-qual"
	void* pars[] = {(void*)data, (void*)&eq, (void*)&index, params};
	#pragma GCC diagnostic pop
	lucu_vector_iterate(vector, lucu_vector_index_func, pars);
	if (index == lucu_vector_length(vector)) {
		return -1;
	} else {
		return index;
	}
}

void* lucu_vector_get(const LucuVector* vector, const int index) {
	return (void*)((uintptr_t)vector->v + (size_t)index * vector->bytewidth);
}

void lucu_vector_remove(LucuVector* vector, const int index) {
	assert(index < lucu_vector_length(vector) && index >= 0);
	if (vector->free_function != NULL) {
		vector->free_function(lucu_vector_get(vector, index));
	}

	vector->length--;

	// TODO: Do benchmark testing to see if it is better to copy each
	// element one at a time, or to just use memmove
	for (int i = index; i < vector->length; i++) {
		memcpy(lucu_vector_get(vector, i), lucu_vector_get(vector, i + 1), vector->bytewidth);
	}
}

void lucu_vector_insert(LucuVector* vector, const void* const data, const int index) {
	assert(index >= 0);

	if (index >= lucu_vector_length(vector)) {
		lucu_vector_push_back(vector, data);
		return;
	}

	if (vector->length == vector->size) {
		lucu_vector_increase_size(vector);
	}

	// TODO: Do benchmark testing to see if it is better to copy each
	// element one at a time, or to just use memmove
	for (int i = vector->length; i > index; i--) {
		memcpy(lucu_vector_get(vector, i), lucu_vector_get(vector, i - 1), vector->bytewidth);
	}

	memcpy(lucu_vector_get(vector, index), data, vector->bytewidth);

	vector->length++;
}

void lucu_vector_iterate(const LucuVector* vector, bool (* const func)(const void*, void*), void* const params) {
	for (int i = 0; i < vector->length; i++) {
		if (func(lucu_vector_get(vector, i), params))
			break;
	}
}

void lucu_vector_mut_iterate(LucuVector* vector, bool (* const func)(void*, void*), void* const params) {
	for (int i = 0; i < vector->length; i++) {
		if (func(lucu_vector_get(vector, i), params))
			break;
	}
}

static bool lucu_vector_filter_func(const void* const data, void* const params) {
	void** pars = (void**)params;
	LucuVector* vector = (LucuVector*)pars[0];
	bool (*filter_func)(const void*, void*) = (bool (*)(const void*, void*))((LucuGenericFunction*)pars[1])->f;
	void* par = pars[2];
	if (filter_func(data, par)) {
		lucu_vector_push_back(vector, data);
	}
	return false;
}

LucuVector lucu_vector_filter(const LucuVector* vector, bool (* const filter_func)(const void*, void*), void* const params) {
	LucuVector new_vector = lucu_vector_new(vector->bytewidth, vector->free_function);
	LucuGenericFunction ff = { (void (*)(void))filter_func };
	void* pars[] = {(void*)&new_vector, (void*)&ff, params};
	lucu_vector_iterate(vector, lucu_vector_filter_func, (void*)pars);
	return new_vector;
}

static bool lucu_vector_map_func(const void* const data, void* const params) {
	void** pars = (void**)params;
	LucuVector* vector = (LucuVector*)pars[0];
	void* (*map_func)(const void*, void*) = (void* (*)(const void*, void*))((LucuGenericFunction*)pars[1])->f;
	void* (*map_func_return_free)(void*) = (void* (*)(void*))((LucuGenericFunction*)pars[2])->f;
	void* par = pars[3];
	void* mapped = map_func(data, par);
	lucu_vector_push_back(vector, mapped);
	if (map_func_return_free != NULL) {
		map_func_return_free(mapped);
	}
	return false;
}

LucuVector lucu_vector_map(const LucuVector* vector, const size_t target_bytewidth, void (* const target_free_function)(void*), void* (* const map_func)(const void*, void*), void (* const map_func_return_free)(void*), void* const params) {
	LucuVector new_vector = lucu_vector_new(target_bytewidth, target_free_function);
	LucuGenericFunction mf = { (void (*)(void))map_func };
	LucuGenericFunction rf = { (void (*)(void))map_func_return_free };
	void* pars[] = {(void*)&new_vector, (void*)&mf, (void*)&rf, params};
	lucu_vector_iterate(vector, lucu_vector_map_func, (void*)pars);
	return new_vector;
}

static bool lucu_vector_superlative_func(const void* data, void* params) {
	void** pars = (void**)params;
	bool (*compare_func)(const void*, const void*, void*) = (bool (*)(const void*, const void*, void*))((LucuGenericFunction*)pars[0])->f;
	const void** const superlative = (const void** const)pars[1];
	void* par = pars[2];
	if (compare_func(data, *superlative, par))
		*superlative = data;
	return false;
}

void* lucu_vector_superlative(const LucuVector* vector, bool (* const compare_func)(const void*, const void*, void*), void* const params) {
	void* superlative = vector->v;
	LucuGenericFunction cf = { (void (*)(void))compare_func };
	void* pars[] = {(void*)&cf, (void*)&superlative, params};
	lucu_vector_iterate(vector, lucu_vector_superlative_func, (void*)pars);
	return superlative;
}

static void merge(LucuVector* vector, const int start, const int middle, const int end, bool (*compare_function)(const void*, const void*, void*), void* params) {
	LucuVector merged = lucu_vector_new(vector->bytewidth, vector->free_function);

	int i = start;
	int j = middle;

	while (i < middle && j < end) {
		if (compare_function(lucu_vector_get(vector, i), lucu_vector_get(vector, j), params)) {
			lucu_vector_push(&merged, lucu_vector_get(vector, i));
			i++;
		} else {
			lucu_vector_push(&merged, lucu_vector_get(vector, j));
			j++;
		}
	}

	while (i < middle) {
		lucu_vector_push(&merged, lucu_vector_get(vector, i));
		i++;
	}

	while (j < end) {
		lucu_vector_push(&merged, lucu_vector_get(vector, j));
		j++;
	}

	memcpy(lucu_vector_get(vector, start), merged.v, vector->bytewidth * (size_t)lucu_vector_length(&merged));

	lucu_vector_destroy(&merged);
}

static void merge_sort(LucuVector* vector, const int start, const int end, bool (*compare_function)(const void*, const void*, void*), void* params) {
	if (start == end) {
		return;
	} else if (end - start == 1) {
		return;
	} else if (end - start == 2) {
		if (!compare_function(lucu_vector_get(vector, start), lucu_vector_get(vector, start + 1), params)) {
			lucu_vector_swap(vector, start, start + 1);
		}
		return;
	}

	const int middle = (end - start) / 2 + start;

	merge_sort(vector, start, middle, compare_function, params);
	merge_sort(vector, middle, end, compare_function, params);

	merge(vector, start, middle, end, compare_function, params);
}

void lucu_vector_sort(LucuVector* vector, bool (*compare_function)(const void*, const void*, void*), void* params) {
	merge_sort(vector, 0, lucu_vector_length(vector), compare_function, params);
}
