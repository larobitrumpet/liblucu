#include "lucu/option.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct LucuOptionData {
	void* data;
	size_t data_size;
};

LucuOption lucu_option_new_none() {
	LucuOption option = malloc(sizeof(LucuOptionData));
	option->data = NULL;
	option->data_size = 0;
	return option;
}

LucuOption lucu_option_new_some(void* data, size_t data_size) {
	LucuOption option = malloc(sizeof(LucuOptionData));
	option->data = malloc(data_size);
	memcpy(option->data, data, data_size);
	option->data_size = data_size;
	return option;
}

bool lucu_option_is_some(const LucuOption option) {
	return option->data != NULL;
}

void* lucu_option_get(const LucuOption option) {
	assert(lucu_option_is_some(option));
	return option->data;
}

void* lucu_option_take(LucuOption option) {
	assert(lucu_option_is_some(option));
	void* data = option->data;
	option->data = NULL;
	return data;
}

void lucu_option_set(LucuOption option, void* data, size_t data_size) {
	assert(!lucu_option_is_some(option));
	option->data = malloc(data_size);
	memcpy(option->data, data, data_size);
	option->data_size = data_size;
}
