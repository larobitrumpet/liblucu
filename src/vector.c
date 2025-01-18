#include "lucu/lucu.h"
#include "lucu/vector.h"
#include <stdint.h>
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

static inline int mod(const int a, const int b) {
	return ((a % b) + b) % b;
}

struct LucuVectorData {
	/// Array where elements are stored
	void* v;
	/// The number of bytes that an element takes up
	size_t bytewidth;
	/// Number of elements that can be stored in the currently allocated space.
	/// **Not** the number of elements in the array.
	/// **Not** the number of bytes allocated.
	int size;
	/// The index of the first element of the circular array.
	int head;
	/// The index of the last element of the circular array plus 1 mod `size`.
	int tail;
	/// Function used to free elements of the `LucuVector`.
	/// See `lucu_vector_new` for more information
	void (*free_function)(void*);
};

LucuVector lucu_vector_new(const size_t bytewidth, void (* const free_function)(void*)) {
	return lucu_vector_new_with_size(LUCU_VECTOR_INIT_SIZE, bytewidth, free_function);
}

LucuVector lucu_vector_new_with_size(const int length, const size_t bytewidth, void (* const free_function)(void*)) {
	int len = length == 0 ? LUCU_VECTOR_INIT_SIZE : length;

	LucuVector vector = malloc(sizeof(LucuVectorData));
	vector->bytewidth = bytewidth;
	vector->size = len;
	vector->v = malloc(bytewidth * (size_t)len);
	vector->head = 0;
	vector->tail = 0;
	vector->free_function = free_function;
	return vector;
}

static bool lucu_vector_destroy_func(void* const data, void* params) {
	void (*free_function)(void*) = (void (*)(void*))((LucuGenericFunction*)params)->f;
	free_function(data);
	return false;
}

void lucu_vector_destroy(LucuVector vector) {
	if (vector->free_function != NULL) {
		LucuGenericFunction ff = { (void (*)(void))vector->free_function };
		lucu_vector_iterate(vector, lucu_vector_destroy_func, (void*)&ff);
	}
	free(vector->v);
}

LucuVector lucu_vector_from_array(const void* const arr, const int length, const size_t bytewidth, void (* const free_function)(void*)) {
	assert(length > 0);
	LucuVector vector = lucu_vector_new_with_size(length, bytewidth, free_function);
	memcpy(vector->v, arr, (size_t)length * bytewidth);
	return vector;
}

static bool lucu_vector_print_func(void* const data, void* params) {
	void** pars = (void**)params;
	void (*print_function)(void*, void*) = (void (*)(void*, void*))((LucuGenericFunction*)pars[0])->f;
	void* par = pars[1];
	print_function(data, par);
	printf(",");
	return false;
}

void lucu_vector_print(LucuVector vector, void (*print_function)(void*, void*), void* params) {
	printf("[");

	LucuGenericFunction pf = { (void (*)(void))print_function };
	void* pars[] = {(void*)&pf, params};
	lucu_vector_iterate(vector, lucu_vector_print_func, (void*)pars);

	printf("]");
}

bool lucu_vector_is_empty(LucuVector vector) {
	return vector->head == vector->tail;
}

int lucu_vector_length(LucuVector vector) {
	return mod(vector->tail - vector->head, vector->size);
}

static void lucu_vector_increase_size(LucuVector vector) {
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

void lucu_vector_push_back(LucuVector vector, const void* const data) {
	if (vector->head == mod(vector->tail + 1, vector->size)) {
		lucu_vector_increase_size(vector);
	}
	memcpy((void*)((uintptr_t)vector->v + (size_t)vector->tail * vector->bytewidth), data, vector->bytewidth);
	vector->tail = mod(vector->tail + 1, vector->size);
}

void lucu_vector_push(LucuVector vector, const void* const data) {
	lucu_vector_push_back(vector, data);
}

void lucu_vector_enqueue(LucuVector vector, const void* const data) {
	lucu_vector_push_back(vector, data);
}

void lucu_vector_push_front(LucuVector vector, const void* const data) {
	if (mod(vector->head - 1, vector->size) == vector->tail) {
		lucu_vector_increase_size(vector);
	}
	vector->head = mod(vector->head - 1, vector->size);
	memcpy((void*)((uintptr_t)vector->v + (size_t)vector->head * vector->bytewidth), data, vector->bytewidth);
}

void* lucu_vector_pop_front(LucuVector vector) {
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

void* lucu_vector_dequeue(LucuVector vector) {
	return lucu_vector_pop_front(vector);
}

void* lucu_vector_pop_back(LucuVector vector) {
	if (lucu_vector_is_empty(vector)) {
		return NULL;
	}
	void* data = malloc(vector->bytewidth);
	vector->tail = mod(vector->tail - 1, vector->size);
	memcpy(data, (void*)((uintptr_t)vector->v + (size_t)vector->tail * vector->bytewidth), vector->bytewidth);
	if (vector->free_function != NULL) {
		vector->free_function((void*)((uintptr_t)vector->v + (size_t)vector->tail * vector->bytewidth));
	}
	return data;
}

void* lucu_vector_pop(LucuVector vector) {
	return lucu_vector_pop_back(vector);
}

void lucu_vector_swap(LucuVector vector, const int index_1, const int index_2) {
	void* tmp = malloc(vector->bytewidth);
	memcpy(tmp, lucu_vector_get(vector, index_1), vector->bytewidth);
	memcpy(lucu_vector_get(vector, index_1), lucu_vector_get(vector, index_2), vector->bytewidth);
	memcpy(lucu_vector_get(vector, index_2), tmp, vector->bytewidth);
	free(tmp);
}

static bool lucu_vector_index_func(void* const data, void* params) {
	void** pars = (void**)params;
	void* d = pars[0];
	bool (*equal)(void*, void*, void*) = (bool (*)(void*, void*, void*))((LucuGenericFunction*)pars[1])->f;
	int* index = pars[2];
	void* par = pars[3];
	if (equal(data, d, par)) {
		return true;
	}
	*index += 1;
	return false;
}

int lucu_vector_index(LucuVector vector, void* const data, bool (* const equal)(void*, void*, void*), void* const params) {
	int index = 0;
	LucuGenericFunction eq = { (void (*)(void))equal };
	void* pars[] = {data, (void*)&eq, (void*)&index, params};
	lucu_vector_iterate(vector, lucu_vector_index_func, pars);
	if (index == lucu_vector_length(vector)) {
		return -1;
	} else {
		return index;
	}
}

static int lucu_vector_local_index_to_global_index(LucuVector vector, const int index) {
	return mod(vector->head + index, vector->size);
}

void* lucu_vector_get(LucuVector vector, const int index) {
	return (void*)((uintptr_t)vector->v + (size_t)lucu_vector_local_index_to_global_index(vector, index) * vector->bytewidth);
}

void lucu_vector_remove(LucuVector vector, const int index) {
	assert(index < lucu_vector_length(vector) && index >= 0);
	int i = lucu_vector_local_index_to_global_index(vector, index);
	if (vector->free_function != NULL) {
		vector->free_function((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth));
	}
	while (i != vector->tail) {
		memcpy((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), (void*)((uintptr_t)vector->v + (size_t)mod(i + 1, vector->size) * vector->bytewidth), vector->bytewidth);
		i = mod(i + 1, vector->size);
	}
	vector->tail = mod(vector->tail - 1, vector->size);
}

void lucu_vector_insert(LucuVector vector, const void* const data, const int index) {
	assert(index >= 0);
	if (index >= lucu_vector_length(vector)) {
		lucu_vector_push_back(vector, data);
		return;
	}
	if (vector->head == mod(vector->tail + 1, vector->size)) {
		lucu_vector_increase_size(vector);
	}
	int in = lucu_vector_local_index_to_global_index(vector, index);
	for (int i = vector->tail; i != in; i = mod(i - 1, vector->size)) {
		memcpy((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), (void*)((uintptr_t)vector->v + (size_t)mod(i - 1, vector->size) * vector->bytewidth), vector->bytewidth);
	}
	vector->tail = mod(vector->tail + 1, vector->size);
	memcpy((void*)((uintptr_t)vector->v + (size_t)in * vector->bytewidth), data, vector->bytewidth);
}

void lucu_vector_iterate(LucuVector vector, bool (* const func)(void*, void*), void* const params) {
	int i = vector->head;
	while (i != vector->tail) {
		if (func((void*)((uintptr_t)vector->v + (size_t)i * vector->bytewidth), params))
			break;
		i = mod(i + 1, vector->size);
	}
}

static bool lucu_vector_filter_func(void* const data, void* const params) {
	void** pars = (void**)params;
	LucuVector vector = (LucuVector)pars[0];
	bool (*filter_func)(void*, void*) = (bool (*)(void*, void*))((LucuGenericFunction*)pars[1])->f;
	void* par = pars[2];
	if (filter_func(data, par)) {
		lucu_vector_push_back(vector, data);
	}
	return false;
}

LucuVector lucu_vector_filter(LucuVector vector, bool (* const filter_func)(void*, void*), void* const params) {
	LucuVector new_vector = lucu_vector_new(vector->bytewidth, vector->free_function);
	LucuGenericFunction ff = { (void (*)(void))filter_func };
	void* pars[] = {(void*)new_vector, (void*)&ff, params};
	lucu_vector_iterate(vector, lucu_vector_filter_func, (void*)pars);
	return new_vector;
}

static bool lucu_vector_map_func(void* const data, void* const params) {
	void** pars = (void**)params;
	LucuVector vector = (LucuVector)pars[0];
	void* (*map_func)(void*, void*) = (void* (*)(void*, void*))((LucuGenericFunction*)pars[1])->f;
	void* par = pars[2];
	lucu_vector_push_back(vector, map_func(data, par));
	return false;
}

LucuVector lucu_vector_map(LucuVector vector, const size_t target_bytewidth, void (* const target_free_function)(void*), void* (* const map_func)(void*, void*), void* const params) {
	LucuVector new_vector = lucu_vector_new(target_bytewidth, target_free_function);
	LucuGenericFunction mf = { (void (*)(void))map_func };
	void* pars[] = {(void*)new_vector, (void*)&mf, params};
	lucu_vector_iterate(vector, lucu_vector_map_func, (void*)pars);
	return new_vector;
}

static bool lucu_vector_min_max_func(void* const data, void* const params) {
	void** pars = (void**)params;
	bool (*compare_func)(void*, void*, void*) = (bool (*)(void*, void*, void*))((LucuGenericFunction*)pars[0])->f;
	void** min_max = (void**)pars[1];
	void* par = pars[2];
	if (compare_func(data, *min_max, par))
		*min_max = data;
	return false;
}

void* lucu_vector_min_max(LucuVector vector, bool (* const compare_func)(void*, void*, void*), void* const params) {
	void* min_max = (void*)((uintptr_t)vector->v + (size_t)vector->head * vector->bytewidth);
	LucuGenericFunction cf = { (void (*)(void))compare_func };
	void* pars[] = {(void*)&cf, (void*)&min_max, params};
	lucu_vector_iterate(vector, lucu_vector_min_max_func, (void*)pars);
	return min_max;
}

void merge(LucuVector vector, const int start, const int middle, const int end, bool (*compare_function)(void*, void*, void*), void* params) {
	LucuVector merged = lucu_vector_new(vector->bytewidth, vector->free_function);

	int i = start;
	int j = middle;

	while (i < middle && j < end) {
		if (compare_function(lucu_vector_get(vector, i), lucu_vector_get(vector, j), params)) {
			lucu_vector_push(merged, lucu_vector_get(vector, i));
			i++;
		} else {
			lucu_vector_push(merged, lucu_vector_get(vector, j));
			j++;
		}
	}

	while (i < middle) {
		lucu_vector_push(merged, lucu_vector_get(vector, i));
		i++;
	}

	while (j < end) {
		lucu_vector_push(merged, lucu_vector_get(vector, j));
		j++;
	}

	const int local_head = lucu_vector_local_index_to_global_index(vector, start);
	const int local_end = lucu_vector_local_index_to_global_index(vector, end);

	if (local_end >= local_head) {
		memcpy((void*)((uintptr_t)vector->v + (size_t)local_head * vector->bytewidth), merged->v, vector->bytewidth * (size_t)lucu_vector_length(merged));
	} else {
		memcpy((void*)((uintptr_t)vector->v + (size_t)local_head * vector->bytewidth), merged->v, vector->bytewidth * (size_t)(vector->size - local_head));
		memcpy(vector->v, (void*)((uintptr_t)merged->v + (size_t)(vector->size - local_head)), vector->bytewidth * (size_t)(lucu_vector_length(merged) - (vector->size - local_head)));
	}

	lucu_vector_destroy(merged);
}

void merge_sort(LucuVector vector, const int start, const int end, bool (*compare_function)(void*, void*, void*), void* params) {
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

void lucu_vector_sort(LucuVector vector, bool (*compare_function)(void*, void*, void*), void* params) {
	merge_sort(vector, 0, lucu_vector_length(vector), compare_function, params);
}
