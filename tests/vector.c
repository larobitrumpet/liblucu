#include "../include/vector.h"
#include <criterion/criterion.h>

Test(vector, construct) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	cr_expect(v.v != NULL);
	cr_expect(v.bytewidth == sizeof(int));
	cr_expect(v.size == LUCU_VECTOR_INIT_SIZE);
	cr_expect(v.head == 0);
	cr_expect(v.tail == 0);
	cr_expect(v.free_function == NULL);
	cr_expect(lucu_vector_is_empty(&v) == true);
	cr_expect(lucu_vector_length(&v) == 0);
	lucu_deconstruct_vector(&v);
}

Test(vector, push_back) {
	LucuVector v = lucu_construct_vector(sizeof(int), NULL);
	cr_assert(lucu_vector_is_empty(&v) == true);
	cr_assert(lucu_vector_length(&v) == 0);

	int first = 1;
	lucu_vector_push_back(&v, &first);
	cr_assert(lucu_vector_is_empty(&v) == false);
	cr_assert(lucu_vector_length(&v) == 1);
	cr_assert(*(int*)lucu_vector_get(&v, 0) == 1);
	cr_expect((int*)lucu_vector_get(&v, 0) != &first);

	int second = 2;
	lucu_vector_push_back(&v, &second);
	cr_assert(lucu_vector_is_empty(&v) == false);
	cr_assert(lucu_vector_length(&v) == 2);
	cr_assert(*(int*)lucu_vector_get(&v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(&v, 1) == 2);
	cr_expect((int*)lucu_vector_get(&v, 0) != &first);
	cr_expect((int*)lucu_vector_get(&v, 1) != &second);

	int third = 3;
	lucu_vector_push_back(&v, &third);
	cr_assert(lucu_vector_is_empty(&v) == false);
	cr_assert(lucu_vector_length(&v) == 3);
	cr_assert(*(int*)lucu_vector_get(&v, 0) == 1);
	cr_assert(*(int*)lucu_vector_get(&v, 1) == 2);
	cr_assert(*(int*)lucu_vector_get(&v, 2) == 3);
	cr_expect((int*)lucu_vector_get(&v, 0) != &first);
	cr_expect((int*)lucu_vector_get(&v, 1) != &second);
	cr_expect((int*)lucu_vector_get(&v, 2) != &third);

	lucu_deconstruct_vector(&v);
}
