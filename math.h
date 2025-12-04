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
	double *items;
	size_t length;
} Vector;

constexpr Vector NULL_VECTOR = { .length = 0, .items = nullptr };


#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define vector_new devkit_vector_new
#define vector_fromptr devkit_vector_fromptr
#define vector_copy devkit_vector_copy

#else
#define vector_new( length, ...) devkit_vector_new( nullptr, (length), __VA_ARGS__)
#define vector_fromptr( length, items) devkit_vector_fromptr( nullptr, (length), (items))
#define vector_copy( vec) devkit_vector_copy( nullptr, (vec))

#endif

bool vector_equals( const Vector *vec, const Vector *other);
void vector_sum( Vector *vec, ...);
Vector vector_multiply( Vector




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


Vector devkit_vector_copy( DEVKIT_ALLOCATOR *alloc, Vector *vec) {
	void *items = DEVKIT_MALLOC( alloc, vec->length * sizeof(double));
	memcpy( items, vec->items, vec->length*sizeof(double));
	return (Vector) {
		.items=items,
		.length=vec->length,
	};
}



bool vector_equals( const Vector *vec, const Vector *other) {
	if (vec->length != other->length) return false;
	for (size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != other->items[idx]) 
			return false;
	}
	return true;
}


void vector_sum( Vector *vec, ...) {
	va_list args;
	va_start( args);

	Vector arg;
	while ( !vector_equals((arg = va_arg( args, Vector), &arg), &NULL_VECTOR)) {
		assert( arg.length == vec->length);
		for (size_t idx = 0; idx < vec->length; idx++)
			vec->items[idx] += arg.items[idx];
	}
}


#endif
