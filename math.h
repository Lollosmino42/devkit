#ifndef __DEVKIT_MATH_H
#define __DEVKIT_MATH_H

#include <stddef.h>
#include <stdarg.h>

#include "settings.h"

/*
	-------------
	VECTOR STRUCT
	-------------
*/

typedef struct {

	size_t length;
	double *items;
} Vector;

constexpr Vector NULL_VECTOR = { .length = 0, .items = nullptr };


#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define vector_new devkit_vector_new
#define vector_fromptr devkit_vector_fromptr

#else
#define vector_new devkit_vector_new
#define vector_fromptr devkit_vector_fromptr

#endif

void vector_sum( Vector *vec, ...);



/*
	IMPLEMENTATION
*/

Vector devkit_vector_new( DEVKIT_ALLOCATOR *alloc, size_t length, ...) {
	Vector vec = {
		.length = length,
		.items = DEVKIT_MALLOC( alloc, length * sizeof(double))
	};
	va_list args;
	va_start( args);
	for (size_t idx = 0; idx < length; idx++)
		vec.items[idx] = va_arg( args, double);

	return va_end(args), vec;
}

Vector devkit_vector_fromptr( DEVKIT_ALLOCATOR *alloc, size_t length, double *items) {
	double *copy = DEVKIT_MALLOC( alloc, length * sizeof(double));
	memcpy( copy, items, length * sizeof(double));
	return (Vector) {
		.length = length,
		.items = copy
	};
}


#endif
