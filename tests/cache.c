#include "lucu/cache.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <string.h>

bool equal(void* key_1, void* key_2, void* p) {
	(void)p;
	return *(int*)key_1 == *(int*)key_2;
}

int generate_call[6];

void* generate(void* n) {
	generate_call[*(int*)n]++;

	switch (*(int*)n) {
		case 0:
			return "zero";
		case 1:
			return "one";
		case 2:
			return "two";
		case 3:
			return "three";
		case 4:
			return "four";
		case 5:
			return "five";
		default:
			cr_assert(false);
	}
	return NULL;
}

void generate_call_test(int n[6]) {
	for (int i = 0; i < 6; i++) {
		cr_expect(generate_call[i] == n[i]);
	}
}

Test(cache, test) {
	int n[6] = {0, 1, 2, 3, 4, 5};
	LucuCache c = lucu_cache_new(4, sizeof(int), sizeof(char*), equal, NULL, generate, NULL, NULL);

	generate_call_test((int[]){0, 0, 0, 0, 0, 0});

	// {0, N, N, N}
	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){1, 0, 0, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){1, 0, 0, 0, 0, 0});

	// {0, 1, N, N}
	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	// {0, 1, 2, N}
	cr_expect(strcmp(lucu_cache_get(c, &n[2]), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[2]), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[2]), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	// {0, 1, 2, 3}
	cr_expect(strcmp(lucu_cache_get(c, &n[3]), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[3]), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[2]), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[3]), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	// {1, 2, 3, 4}
	cr_expect(strcmp(lucu_cache_get(c, &n[4]), "four") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[4]), "four") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 1, 0});

	// {2, 3, 4, 0}
	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){2, 1, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[2]), "two") == 0);
	generate_call_test((int[]){2, 1, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[3]), "three") == 0);
	generate_call_test((int[]){2, 1, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[4]), "four") == 0);
	generate_call_test((int[]){2, 1, 1, 1, 1, 0});

	// {3, 4, 0, 1}
	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[3]), "three") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[4]), "four") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 0});

	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 0});

	// {4, 0, 1, 5}
	cr_expect(strcmp(lucu_cache_get(c, &n[5]), "five") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[5]), "five") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[4]), "four") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){2, 2, 1, 1, 1, 1});

	// {0, 1, 5, 2}
	cr_expect(strcmp(lucu_cache_get(c, &n[2]), "two") == 0);
	generate_call_test((int[]){2, 2, 2, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[2]), "two") == 0);
	generate_call_test((int[]){2, 2, 2, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[0]), "zero") == 0);
	generate_call_test((int[]){2, 2, 2, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[1]), "one") == 0);
	generate_call_test((int[]){2, 2, 2, 1, 1, 1});

	cr_expect(strcmp(lucu_cache_get(c, &n[5]), "five") == 0);
	generate_call_test((int[]){2, 2, 2, 1, 1, 1});

	lucu_cache_destroy(c);
}
