#include <lucu/heap.h>
#include <criterion/criterion.h>
#include <criterion/new/assert.h>
#include <stdio.h>

void print_heap(const LucuHeap* heap) {
	printf("[");
	for (int i = 0; i < lucu_heap_length(heap); i++) {
		printf("%d, ", *(int*)lucu_heap_get_key(heap, i));
	}
	printf("]\n");
}

void expect_is_heap(const LucuHeap* heap) {
	for (int i = 1; i < lucu_heap_length(heap); i ++) {
		cr_expect(heap->compare_function(
			lucu_heap_get_key(heap, (i - 1) / 2),
			lucu_heap_get_key(heap, i),
			heap->compare_function_data
		), "Heap property violated, parent at %d (%d) is not more than child at %d (%d)", (i-1)/2, *(int*)lucu_heap_get_key(heap, (i-1/2)), i, *(int*)lucu_heap_get_key(heap, i));
	}
}

bool int_cmp(void* a, void* b, void* _) {
	return *(int*)a >= *(int*)b;
}

Test(heap, push_just_key_ascending) {
	int nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

	LucuHeap heap = lucu_heap_new(sizeof(int), 0, NULL, NULL, NULL, NULL, int_cmp, NULL);
	cr_assert(eq(int, lucu_heap_length(&heap), 0));

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], NULL);
		cr_assert(eq(int, lucu_heap_length(&heap), i + 1));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}

Test(heap, push_just_key_decending) {
	int nums[] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};

	LucuHeap heap = lucu_heap_new(sizeof(int), 0, NULL, NULL, NULL, NULL, int_cmp, NULL);
	cr_assert(eq(int, lucu_heap_length(&heap), 0));

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], NULL);
		cr_assert(eq(int, lucu_heap_length(&heap), i + 1));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}

Test(heap, push_just_key_random) {
	int nums[] = {1, 15, 14, 7, 13, 2, 0, 4, 3, 11, 10, 9, 12, 6, 5, 8};

	LucuHeap heap = lucu_heap_new(sizeof(int), 0, NULL, NULL, NULL, NULL, int_cmp, NULL);
	cr_assert(eq(int, lucu_heap_length(&heap), 0));

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], NULL);
		cr_assert(eq(int, lucu_heap_length(&heap), i + 1));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}

Test(heap, push_key_value_ascending) {
	int nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	char letters[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p'};

	LucuHeap heap = lucu_heap_new(sizeof(int), sizeof(char), NULL, NULL, NULL, NULL, int_cmp, NULL);
	cr_assert(eq(int, lucu_heap_length(&heap), 0));

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], &letters[i]);
		cr_assert(eq(int, lucu_heap_length(&heap), i + 1));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}

Test(heap, push_key_value_decending) {
	int nums[] = {15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
	char letters[] = {'o', 'p', 'n', 'm', 'l', 'k', 'j', 'i', 'h', 'g', 'f', 'e', 'd', 'c', 'b', 'a'};

	LucuHeap heap = lucu_heap_new(sizeof(int), sizeof(char), NULL, NULL, NULL, NULL, int_cmp, NULL);
	cr_assert(eq(int, lucu_heap_length(&heap), 0));

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], &letters[i]);
		cr_assert(eq(int, lucu_heap_length(&heap), i + 1));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}

Test(heap, push_key_value_random) {
	int nums[] = {1, 15, 14, 7, 13, 2, 0, 4, 3, 11, 10, 9, 12, 6, 5, 8};
	char letters[] = {'b', 'p', 'o', 'h', 'n', 'c', 'a', 'e', 'd', 'l', 'k', 'j', 'm', 'g', 'f', 'i'};

	LucuHeap heap = lucu_heap_new(sizeof(int), sizeof(char), NULL, NULL, NULL, NULL, int_cmp, NULL);
	cr_assert(eq(int, lucu_heap_length(&heap), 0));

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], &letters[i]);
		cr_assert(eq(int, lucu_heap_length(&heap), i + 1));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}

Test(heap, pop_just_key) {
	int nums[] = {1, 15, 14, 7, 13, 2, 0, 4, 3, 11, 10, 9, 12, 6, 5, 8};

	LucuHeap heap = lucu_heap_new_with_size(16, sizeof(int), 0, NULL, NULL, NULL, NULL, int_cmp, NULL);
	cr_assert(eq(int, lucu_heap_length(&heap), 0));

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], NULL);
	}
	cr_assert(eq(int, lucu_heap_length(&heap), 16));

	int x;
	int* x_p = &x;

	for (int i = 15; i >= 0; i--) {
		lucu_heap_pop(&heap, (void**)&x_p, NULL);
		cr_assert(eq(int, lucu_heap_length(&heap), i));
		cr_expect(eq(int, x, i));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}

Test(heap, pop_key_value) {
	int nums[] = {1, 15, 14, 7, 13, 2, 0, 4, 3, 11, 10, 9, 12, 6, 5, 8};
	char letters[] = {'b', 'p', 'o', 'h', 'n', 'c', 'a', 'e', 'd', 'l', 'k', 'j', 'm', 'g', 'f', 'i'};

	LucuHeap heap = lucu_heap_new_with_size(16, sizeof(int), sizeof(char), NULL, NULL, NULL, NULL, int_cmp, NULL);

	for (int i = 0; i < 16; i++) {
		lucu_heap_push(&heap, &nums[i], &letters[i]);
	}

	int x;
	int* x_p = &x;
	char c;
	char* c_p = &c;

	for (int i = 15; i >= 0; i--) {
		lucu_heap_pop(&heap, (void**)&x_p, (void**)&c_p);
		cr_assert(eq(int, lucu_heap_length(&heap), i));
		cr_expect(eq(int, x, i));
		cr_expect(eq(int, c - 'a', x));
		expect_is_heap(&heap);
	}

	lucu_heap_destroy(&heap);
}
