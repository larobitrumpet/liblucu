#include "../include/vector.h"
#include <criterion/criterion.h>

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
