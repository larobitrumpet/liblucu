#include <lucu/cache.h>
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <stdio.h>
#include <string.h>

bool equal(const void* key_1, const void* key_2, void* p);
void* generate(const void* n, void* _);
void generate_call_test(int n[6]);

int generate_call[6];

bool equal(const void* key_1, const void* key_2, void* p) {
	(void)p;
	return *(const int*)key_1 == *(const int*)key_2;
}

void* generate(const void* n, void* _) {
	generate_call[*(const int*)n]++;

	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wcast-qual"
	switch (*(int*)n) {
		case 0:
			return (char*)"zero\0";
		case 1:
			return (char*)"one\0";
		case 2:
			return (char*)"two\0";
		case 3:
			return (char*)"three\0";
		case 4:
			return (char*)"four\0";
		case 5:
			return (char*)"five\0";
		default:
			cr_assert(false);
	}
	#pragma GCC diagnostic pop
	return NULL;
}

void generate_call_test(int n[6]) {
	for (int i = 0; i < 6; i++) {
		cr_expect(generate_call[i] == n[i]);
	}
}

Test(cache, test) {
	int n[6] = {0, 1, 2, 3, 4, 5};
	LucuCache c = lucu_cache_new(4, sizeof(int), sizeof(char) * 6, NULL, NULL, NULL, NULL, equal, NULL, generate, NULL);
	char ch[6];

	generate_call_test((int[]){0, 0, 0, 0, 0, 0});

	// {0: 1, -, -, -} what the heap looks like (value: key (hits))
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 0, 0, 0, 0, 0});

	// {0: 2, -, -, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 0, 0, 0, 0, 0});

	// {1: 1, 0: 2, -, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	// {1: 2, 0: 2, -, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	// {1: 2, 0: 3, -, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	// {1: 3, 0: 3, -, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 0, 0, 0, 0});

	// {2: 1, 0: 3, 1: 3, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[2], &ch), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	// {2: 2, 0: 3, 1: 3, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[2], &ch), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	// {2: 2, 0: 4, 1: 3, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	// {2: 2, 0: 4, 1: 4, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	// {2: 3, 0: 4, 1: 4, -}
	cr_expect(strcmp(lucu_cache_get(&c, &n[2], &ch), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 0, 0, 0});

	// {3: 1, 2: 3, 1: 4, 0: 4}
	cr_expect(strcmp(lucu_cache_get(&c, &n[3], &ch), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	// {3: 2, 2: 3, 1: 4, 0: 4}
	cr_expect(strcmp(lucu_cache_get(&c, &n[3], &ch), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	// {3: 2, 2: 3, 1: 4, 0: 5}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	// {3: 2, 2: 3, 1: 5, 0: 5}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	// {3: 2, 2: 4, 1: 5, 0: 5}
	cr_expect(strcmp(lucu_cache_get(&c, &n[2], &ch), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	// {3: 3, 2: 4, 1: 5, 0: 5}
	cr_expect(strcmp(lucu_cache_get(&c, &n[3], &ch), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 0, 0});

	// {4: 1, 2: 4, 1: 5, 0: 5}
	cr_expect(strcmp(lucu_cache_get(&c, &n[4], &ch), "four") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 1, 0});

	// {4: 2, 2: 4, 1: 5, 0: 5}
	cr_expect(strcmp(lucu_cache_get(&c, &n[4], &ch), "four") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 1, 0});

	// {4: 2, 2: 4, 1: 5, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 1, 0});

	// {4: 2, 2: 5, 1: 5, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[2], &ch), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 1, 1, 0});

	// {3: 1, 2: 5, 1: 5, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[3], &ch), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 2, 1, 0});

	// {4: 1, 2: 5, 1: 5, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[4], &ch), "four") == 0);
	generate_call_test((int[]){1, 1, 1, 2, 2, 0});

	// {4: 1, 2: 5, 1: 6, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 2, 2, 0});

	// {4: 1, 2: 5, 1: 7, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 2, 2, 0});

	// {3: 1, 2: 5, 1: 7, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[3], &ch), "three") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 2, 0});

	// {4: 1, 2: 5, 1: 7, 0: 6}
	cr_expect(strcmp(lucu_cache_get(&c, &n[4], &ch), "four") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 3, 0});

	// {4: 1, 2: 5, 1: 7, 0: 7}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 3, 0});

	// {5: 1, 2: 5, 1: 7, 0: 7}
	cr_expect(strcmp(lucu_cache_get(&c, &n[5], &ch), "five") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 3, 1});

	// {5: 2, 2: 5, 1: 7, 0: 7}
	cr_expect(strcmp(lucu_cache_get(&c, &n[5], &ch), "five") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 3, 1});

	// {4: 1, 2: 5, 1: 7, 0: 7}
	cr_expect(strcmp(lucu_cache_get(&c, &n[4], &ch), "four") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 1});

	// {4: 1, 2: 5, 1: 7, 0: 8}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 1});

	// {4: 1, 2: 5, 1: 8, 0: 8}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 1});

	// {4: 1, 2: 6, 1: 8, 0: 8}
	cr_expect(strcmp(lucu_cache_get(&c, &n[2], &ch), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 1});

	// {4: 1, 2: 7, 1: 8, 0: 8}
	cr_expect(strcmp(lucu_cache_get(&c, &n[2], &ch), "two") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 1});

	// {4: 1, 2: 7, 1: 8, 0: 9}
	cr_expect(strcmp(lucu_cache_get(&c, &n[0], &ch), "zero") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 1});

	// {4: 1, 2: 7, 1: 9, 0: 9}
	cr_expect(strcmp(lucu_cache_get(&c, &n[1], &ch), "one") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 1});

	// {5: 1, 2: 7, 1: 9, 0: 9}
	cr_expect(strcmp(lucu_cache_get(&c, &n[5], &ch), "five") == 0);
	generate_call_test((int[]){1, 1, 1, 3, 4, 2});

	lucu_cache_destroy(&c);
}
