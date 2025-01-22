/// @file option.h
#ifndef LUCU_OPTION_H
#define LUCU_OPTION_H

#include <stdlib.h>

typedef struct LucuOptionData LucuOptionData;
/**
 * A structure that *may* contain data.
 *
 * Contains a maybe value. Could be `None`, or `Some`.
 */
typedef LucuOptionData* LucuOption;

/**
 * Creates a `None` varient of `LucuOption`.
 * @return A new `None`
 */
LucuOption lucu_option_new_none();

/**
 * Creates a `Some` varient of `LucuOption`.
 *
 * Stores some data which is copied from `data`.
 * @param data Pointer to the data to be copied and held in the `LucuOption`.
 * @param data_size The size in bytes of the data that `data` points to.
 * @return A `Some` `LucuOption` with the provided data.
 */
LucuOption lucu_option_new_some(void* data, size_t data_size);

/**
 * If the `LucuOption` is a `Some`.
 *
 * @param option The `LucuOption` to test.
 * @return `true` if `Some` and `false` if `None`.
 */
bool lucu_option_is_some(const LucuOption option);

/**
 * Get a pointer to the data held in a `LucuOption`.
 * @pre `option` **must** be a `Some` (check with `lucu_option_is_some` first).
 * @param option The `LucuOption` to get data from.
 * @return A pointer to the data in `option`.
 */
void* lucu_option_get(const LucuOption option);

/**
 * Takes the data held in a `LucuOption`.
 * @pre `option` **must** be a `Some` (check with `lucu_option_is_some` first).
 * @post `option` will become a `None`.
 * @param option The `LucuOption` to take data from.
 * @return A pointer to the data that was held by `option`.
 */
void* lucu_option_take(LucuOption option);

/**
 * Sets the data held in a `LucuOption`.
 * @pre `option` **must** be a `None` (check with `lucu_option_is_some` first).
 * @post `option` will become a `Some` with the provided data.
 * @param option The `LucuOption` to put data in.
 * @param data A pointer to the data to copy into `option`.
 * @param `data_size` the size in bytes of the data that `data` points to.
 */
void lucu_option_set(LucuOption option, void* data, size_t data_size);

#endif
