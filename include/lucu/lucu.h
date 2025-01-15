/// @file lucu.h
#ifndef LIBLUCU_H
#define LIBLUCU_H

/**
 * Holds a generic function.
 *
 * Since function pointers are not guaranteed to be the same size
 * as data pointers, we can't directly cast a function pointer to
 * `void*`, which means we can't pass them as generic parameters.
 * To get around this, we put the function pointer into a struct,
 * and then we can have a `void*` pointing to that struct. That
 * is what this struct is for.
 */
typedef struct LucuGenericFunction {
	void (*f)(void); ///< Function pointer
} LucuGenericFunction;

#endif
