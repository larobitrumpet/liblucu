#include "lucu/option.h"
#include <criterion/criterion.h>
#include <criterion/internal/assert.h>

Test(option, create_none) {
	LucuOption o = lucu_option_new_none();
	cr_assert(!lucu_option_is_some(o));
	lucu_option_destroy(o);
}

Test(option, create_some) {
	int n = 3;
	LucuOption o = lucu_option_new_some(&n, sizeof(int));
	cr_assert(lucu_option_is_some(o));
	cr_expect(*(int*)lucu_option_get(o) == n);
	cr_expect((int*)lucu_option_get(o) != &n);
	int* n_p = lucu_option_take(o);
	cr_assert(!lucu_option_is_some(o));
	cr_expect(*n_p == n);
	cr_expect(n_p != &n);
	free(n_p);
	lucu_option_destroy(o);
}

Test(option, set) {
	int n = 3;
	LucuOption o = lucu_option_new_none();
	lucu_option_set(o, &n, sizeof(int));
	cr_assert(lucu_option_is_some(o));
	cr_expect(*(int*)lucu_option_get(o) == n);
	int* n_p = lucu_option_take(o);
	cr_assert(!lucu_option_is_some(o));
	cr_expect(*n_p == n);
	free(n_p);
	lucu_option_destroy(o);
}
