#include "lucu/vector.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>

Test(vector, from_array) {
	const int arr[] = {0, 1, 2, 3, 4, 5};

	LucuVector v = lucu_vector_from_array(arr, 6, sizeof(int), NULL);

	for (int i = 0; i < 6; i++) {
		cr_expect(*(int*)lucu_vector_get(v, i) == i);
	}

	lucu_vector_destroy(v);
}

Test(vector, push_back) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);
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

	lucu_vector_destroy(v);
}

Test(vector, push_front) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);
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

	lucu_vector_destroy(v);
}

Test(vector, pop_back) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);
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

	lucu_vector_destroy(v);
}

Test(vector, pop_front) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);
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

	lucu_vector_destroy(v);
}

Test(vector, print) {

}

bool int_equal(void* a, void* b, void* p) {
	(void)p;
	return *(int*)a == *(int*)b;
}

Test(vector, find_index) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);
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

	lucu_vector_destroy(v);
}

Test(vector, remove) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);
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

	lucu_vector_destroy(v);
}

Test(vector, insert) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);

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

	lucu_vector_destroy(v);
}

bool even(void* n, void* p) {
	(void)p;
	return *(int*)n % 2 == 0;
}

Test(vector, filter) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);
	for (int i = 0; i < 24; i++) {
		lucu_vector_push_back(v, &i);
	}
	LucuVector f = lucu_vector_filter(v, even, NULL);
	cr_assert(lucu_vector_length(f) == 12);
	cr_expect(*(int*)lucu_vector_get(f, 0) == 0);
	cr_expect(*(int*)lucu_vector_get(f, 1) == 2);
	cr_expect(*(int*)lucu_vector_get(f, 2) == 4);
	cr_expect(*(int*)lucu_vector_get(f, 3) == 6);
	cr_expect(*(int*)lucu_vector_get(f, 4) == 8);
	cr_expect(*(int*)lucu_vector_get(f, 5) == 10);
	cr_expect(*(int*)lucu_vector_get(f, 6) == 12);
	cr_expect(*(int*)lucu_vector_get(f, 7) == 14);
	cr_expect(*(int*)lucu_vector_get(f, 8) == 16);
	cr_expect(*(int*)lucu_vector_get(f, 9) == 18);
	cr_expect(*(int*)lucu_vector_get(f, 10) == 20);
	cr_expect(*(int*)lucu_vector_get(f, 11) == 22);

	lucu_vector_destroy(v);
	lucu_vector_destroy(f);
}

void* map(void* n, void* p) {
	(void)p;
	char* c = malloc(sizeof(char));
	*c = (char)(*(int*)n + (int)'A');
	return c;
}

Test(vector, map) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);

	for (int i = 0; i < 26; i++) {
		lucu_vector_push_back(v, &i);
	}

	LucuVector m = lucu_vector_map(v, sizeof(char), NULL, map, free, NULL);

	cr_assert(lucu_vector_length(m) == 26);
	cr_assert(*(char*)lucu_vector_get(m, 0) == 'A');
	cr_assert(*(char*)lucu_vector_get(m, 1) == 'B');
	cr_assert(*(char*)lucu_vector_get(m, 2) == 'C');
	cr_assert(*(char*)lucu_vector_get(m, 3) == 'D');
	cr_assert(*(char*)lucu_vector_get(m, 4) == 'E');
	cr_assert(*(char*)lucu_vector_get(m, 5) == 'F');
	cr_assert(*(char*)lucu_vector_get(m, 6) == 'G');
	cr_assert(*(char*)lucu_vector_get(m, 7) == 'H');
	cr_assert(*(char*)lucu_vector_get(m, 8) == 'I');
	cr_assert(*(char*)lucu_vector_get(m, 9) == 'J');
	cr_assert(*(char*)lucu_vector_get(m, 10) == 'K');
	cr_assert(*(char*)lucu_vector_get(m, 11) == 'L');
	cr_assert(*(char*)lucu_vector_get(m, 12) == 'M');
	cr_assert(*(char*)lucu_vector_get(m, 13) == 'N');
	cr_assert(*(char*)lucu_vector_get(m, 14) == 'O');
	cr_assert(*(char*)lucu_vector_get(m, 15) == 'P');
	cr_assert(*(char*)lucu_vector_get(m, 16) == 'Q');
	cr_assert(*(char*)lucu_vector_get(m, 17) == 'R');
	cr_assert(*(char*)lucu_vector_get(m, 18) == 'S');
	cr_assert(*(char*)lucu_vector_get(m, 19) == 'T');
	cr_assert(*(char*)lucu_vector_get(m, 20) == 'U');
	cr_assert(*(char*)lucu_vector_get(m, 21) == 'V');
	cr_assert(*(char*)lucu_vector_get(m, 22) == 'W');
	cr_assert(*(char*)lucu_vector_get(m, 23) == 'X');
	cr_assert(*(char*)lucu_vector_get(m, 24) == 'Y');
	cr_assert(*(char*)lucu_vector_get(m, 25) == 'Z');

	for (int i = 0; i < 26; i++) {
		cr_assert(*(int*)lucu_vector_get(v, i) == i);
	}

	lucu_vector_destroy(v);
	lucu_vector_destroy(m);
}

bool min(void* a, void* b, void* p) {
	(void)p;
	return *(int*)a < *(int*)b;
}

bool max(void* a, void* b, void* p) {
	(void)p;
	return *(int*)a > *(int*)b;
}

Test(vector, min_max) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 8;
	int e = -2;
	int f = 6;
	int g = -6;
	int h = 5;

	lucu_vector_push_back(v, &a);
	lucu_vector_push_back(v, &b);
	lucu_vector_push_back(v, &c);
	lucu_vector_push_back(v, &d);
	lucu_vector_push_back(v, &e);
	lucu_vector_push_back(v, &f);
	lucu_vector_push_back(v, &g);
	lucu_vector_push_back(v, &h);

	cr_expect(*(int*)lucu_vector_min_max(v, min, NULL) == -6);
	cr_expect(*(int*)lucu_vector_min_max(v, max, NULL) == 8);

	lucu_vector_destroy(v);
}

Test(vector, sort) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);

	int a = 1;
	int b = 2;
	int c = 3;
	int d = 4;
	int e = 5;
	int f = 6;
	int g = 7;
	int h = 8;

	lucu_vector_push(v, &a);
	lucu_vector_push(v, &f);
	lucu_vector_push(v, &b);
	lucu_vector_push(v, &h);
	lucu_vector_push(v, &e);
	lucu_vector_push(v, &d);
	lucu_vector_push(v, &g);
	lucu_vector_push(v, &c);

	lucu_vector_sort(v, min, NULL);

	cr_expect(*(int*)lucu_vector_get(v, 0) == 1);
	cr_expect(*(int*)lucu_vector_get(v, 1) == 2);
	cr_expect(*(int*)lucu_vector_get(v, 2) == 3);
	cr_expect(*(int*)lucu_vector_get(v, 3) == 4);
	cr_expect(*(int*)lucu_vector_get(v, 4) == 5);
	cr_expect(*(int*)lucu_vector_get(v, 5) == 6);
	cr_expect(*(int*)lucu_vector_get(v, 6) == 7);
	cr_expect(*(int*)lucu_vector_get(v, 7) == 8);

	lucu_vector_destroy(v);
}

Test(vector, to_array) {
	LucuVector v = lucu_vector_new(sizeof(int), NULL);

	for (int i = 0; i < 6; i++) {
		lucu_vector_push(v, &i);
	}

	size_t size;
	int* arr = lucu_vector_to_array(v, &size);

	cr_assert(size == sizeof(int) * 6);

	const int ref_arr[] = {0, 1, 2, 3, 4, 5};

	for (int i = 0; i < 6; i++) {
		cr_expect(arr[i] == ref_arr[i]);
	}

	free(arr);
	lucu_vector_destroy(v);
}

Test(vector, broken_array) {
	LucuVector v = lucu_vector_new_with_size(6, sizeof(int), NULL);

	for (int i = 0; i < 4; i++) {
		lucu_vector_push(v, &i);
	}

	lucu_vector_remove(v, 0);
	lucu_vector_remove(v, 0);
	lucu_vector_remove(v, 0);

	for (int i = 4; i < 8; i++) {
		lucu_vector_push(v, &i);
	}

	size_t size;
	int* arr = lucu_vector_to_array(v, &size);

	cr_assert(size == sizeof(int) * 5);

	const int ref_arr[] = {3, 4, 5, 6, 7};

	for (int i = 0; i < 5; i++) {
		cr_expect(arr[i] == ref_arr[i]);
	}

	free(arr);
	lucu_vector_destroy(v);
}
