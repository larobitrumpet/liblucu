#include "../include/vector.h"
#include <criterion/criterion.h>
#include <criterion/internal/test.h>

Test(vector, push_back) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);

	const int first = 1;
	lucu_vector_push_back(v, &first);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_expect((int*)lucu_vector_get(v, 0) != &first);

	const int second = 2;
	lucu_vector_push_back(v, &second);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_expect((int*)lucu_vector_get(v, 0) != &first);
	cr_expect((int*)lucu_vector_get(v, 1) != &second);

	const int third = 3;
	lucu_vector_push_back(v, &third);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_expect((int*)lucu_vector_get(v, 0) != &first);
	cr_expect((int*)lucu_vector_get(v, 1) != &second);
	cr_expect((int*)lucu_vector_get(v, 2) != &third);

	lucu_deconstruct_vector(v);
}

Test(vector, push_front) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);

	const int first = 1;
	lucu_vector_push_front(v, &first);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_expect((int*)lucu_vector_get(v, 0) != &first);

	const int second = 2;
	lucu_vector_push_front(v, &second);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 1);
	cr_expect((int*)lucu_vector_get(v, 0) != &second);
	cr_expect((int*)lucu_vector_get(v, 1) != &first);

	const int third = 3;
	lucu_vector_push_front(v, &third);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 1);
	cr_expect((int*)lucu_vector_get(v, 0) != &third);
	cr_expect((int*)lucu_vector_get(v, 1) != &second);
	cr_expect((int*)lucu_vector_get(v, 2) != &first);

	lucu_deconstruct_vector(v);
}

Test(vector, pop_back) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);

	const int first = 1;
	lucu_vector_push_back(v, &first);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_expect((int*)lucu_vector_get(v, 0) != &first);

	int* const first_pop = lucu_vector_pop_back(v);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);
	cr_assert(*first_pop == 1);
	cr_expect(&first != first_pop);

	const int second = 2;
	lucu_vector_push_back(v, &second);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_expect((int*)lucu_vector_get(v, 0) != &second);

	const int third = 3;
	lucu_vector_push_back(v, &third);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 3);
	cr_expect((int*)lucu_vector_get(v, 0) != &second);
	cr_expect((int*)lucu_vector_get(v, 1) != &third);

	int* const third_pop = lucu_vector_pop_back(v);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*third_pop == 3);
	cr_expect(&third != third_pop);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_expect((int*)lucu_vector_get(v, 0) != &second);

	int* const second_pop = lucu_vector_pop_back(v);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);
	cr_assert(*second_pop == 2);
	cr_expect(&second != second_pop);

	free(first_pop);
	free(second_pop);
	free(third_pop);

	lucu_deconstruct_vector(v);
}

Test(vector, pop_front) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);

	const int first = 1;
	lucu_vector_push_back(v, &first);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_expect((int*)lucu_vector_get(v, 0) != &first);

	int* const first_pop = lucu_vector_pop_front(v);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);
	cr_assert(*first_pop == 1);
	cr_expect(&first != first_pop);

	const int second = 2;
	lucu_vector_push_back(v, &second);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_expect((int*)lucu_vector_get(v, 0) != &second);

	const int third = 3;
	lucu_vector_push_back(v, &third);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 3);
	cr_expect((int*)lucu_vector_get(v, 0) != &second);
	cr_expect((int*)lucu_vector_get(v, 1) != &third);

	int* const second_pop = lucu_vector_pop_front(v);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*second_pop == 2);
	cr_expect(&second != second_pop);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 3);
	cr_expect((int*)lucu_vector_get(v, 0) != &third);

	int* const third_pop = lucu_vector_pop_front(v);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);
	cr_assert(*third_pop == 3);
	cr_expect(&third != third_pop);

	free(first_pop);
	free(second_pop);
	free(third_pop);

	lucu_deconstruct_vector(v);
}

bool int_equal(void* a, void* b, void* p) {
	(void)p;
	return *(int*)a == *(int*)b;
}

Test(vector, find_index) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	cr_assert(lucu_vector_is_empty(v) == true);
	cr_assert(lucu_vector_length(v) == 0);

	int first = 1;
	lucu_vector_push_back(v, &first);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);

	int second = 2;
	lucu_vector_push_back(v, &second);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);

	int third = 3;
	lucu_vector_push_back(v, &third);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);

	int fourth = 4;
	lucu_vector_push_back(v, &fourth);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 4);

	cr_expect(lucu_vector_index(v, &first, int_equal, NULL) == 0);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 4);

	cr_expect(lucu_vector_index(v, &second, int_equal, NULL) == 1);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 4);

	cr_expect(lucu_vector_index(v, &third, int_equal, NULL) == 2);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 4);

	cr_expect(lucu_vector_index(v, &fourth, int_equal, NULL) == 3);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 4);

	int fifth = 5;
	cr_expect(lucu_vector_index(v, &fifth, int_equal, NULL) == -1);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 4);

	lucu_deconstruct_vector(v);
}

Test(vector, remove) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	for (int i = 1; i < 6; i++) {
		lucu_vector_push_back(v, &i);
	}
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 5);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 4) == 5);

	lucu_vector_remove(v, 2);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 5);

	lucu_vector_remove(v, 3);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 4);

	lucu_vector_remove(v, 0);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 4);

	lucu_deconstruct_vector(v);
}

Test(vector, insert) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);

	int first = 1;
	int second = 2;
	int third = 3;
	int fourth = 4;
	int fifth = 5;

	lucu_vector_insert(v, &first, 0);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 1);

	lucu_vector_insert(v, &second, 0);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 1);

	lucu_vector_insert(v, &third, 1);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 1);

	lucu_vector_insert(v, &fourth, 2);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 1);

	lucu_vector_insert(v, &fifth, 8);
	cr_assert(lucu_vector_is_empty(v) == false);
	cr_assert(lucu_vector_length(v) == 5);
	cr_assert(*(int*)lucu_vector_get(v, 0) == 2);
	cr_assert(*(int*)lucu_vector_get(v, 1) == 3);
	cr_assert(*(int*)lucu_vector_get(v, 2) == 4);
	cr_assert(*(int*)lucu_vector_get(v, 3) == 1);
	cr_assert(*(int*)lucu_vector_get(v, 4) == 5);

	lucu_deconstruct_vector(v);
}
