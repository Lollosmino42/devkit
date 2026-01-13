#ifndef _DEVKIT_POINTERS_H
#define _DEVKIT_POINTERS_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "settings.h"
#include "bits/iterable.h"

#if DEVKIT_STRIP_PREFIXES

#define asiterable devkit_asiterable
#define linspace devkit_linspace
#define flinspace devkit_flinspace
#define generator devkit_generator
#define range devkit_range
#define lrange devkit_lrange
#define contains devkit_contains
#define unref devkit_unref
#define refof devkit_refof
#define nonnull devkit_nonnull

#endif

#if DEVKIT_USE_CUSTOM_ALLOCATOR

#define devkit_asiterable _devkit_asiterable
#define devkit_linspace( alloc, start, end) _devkit_linspace( (alloc), (start), (end), false)
#define devkit_flinspace( alloc, start, end) _devkit_linspace( (alloc), (start), (end), true)
#define devkit_generator _devkit_generator
#define devkit_range( alloc, start, end) _devkit_range( (alloc), (start), (end), false)
#define devkit_lrange( alloc, start, end) _devkit_range( (alloc), (start), (end), true)

#else

#define devkit_asiterable( array, length, typesize) _devkit_asiterable( nullptr, (array), (length), typesize)	
#define devkit_linspace( start, end, steps) _devkit_linspace( nullptr, (start), (end), (steps), false)
#define devkit_flinspace( start, end, steps) _devkit_linspace( nullptr, (start), (end), (steps), true)
#define devkit_generator( base, length, typesize, map) _devkit_generator( nullptr, (base), (length), (typesize), (map))
#define devkit_range( start, end) _devkit_range( nullptr, (start), (end), false)
#define devkit_lrange( start, end) _devkit_range( nullptr, (start), (end), true)

#endif

/* Checks if an array contains a certain value */
#define devkit_contains( array, len, var) _devkit_contains( (array), (len), sizeof(array[0]), &(var))
/* Unreferences to pointer after casting */
#define devkit_unref( type) *(type*)
/* Makes a reference of values */
#define devkit_refof( type, ...) (type[]) {__VA_ARGS__}
/* Asserts that ptr is not null and returns it */
#define devkit_nonnull( ptr) ( assert( (ptr) != nullptr), (ptr))




/* IMPLEMENTATION */

#define DEVKIT_POINTERS_IMPLEMENTATION
#ifdef DEVKIT_POINTERS_IMPLEMENTATION

/* Returns true if 'array' contains 'value' */
extern bool _devkit_contains( 
		const void *const array, 
		const size_t len, 
		const size_t typesize, 
		const void *value) 
{
	for ( size_t idx = 0; idx < len; idx++) {
		if ( memcmp( array + idx*typesize, value, typesize) == 0)
			return true;
	}
	return false;
}


/* Creates an iterable object associated with the 'array' of 'length' items of 'typesize' */
extern inline Iterable _devkit_asiterable( DEVKIT_ALLOCATOR *alloc, void* array, size_t length, size_t typesize) {
	return (Iterable) { 
		.alloc=alloc,
		.typesize=typesize, 
		.length=length, 
		.items=devkit_nonnull(array)
	};
}


/* Gives a set of numbers from 'start' to 'end' - 1 */
extern void* _devkit_range( DEVKIT_ALLOCATOR *alloc, size_t start, size_t end, bool islong) {
	if (islong) {
		long *items = DEVKIT_CALLOC( alloc, end - start, sizeof(long));
		while (start < end)
			items[start] = start, start++;
		return items;
	}
	else {
		int *items = DEVKIT_CALLOC( alloc, end - start, sizeof(int));
		while (start < end)
			items[start] = start, start++;
		return items;
	}
}


/* Generates a new array from 'base' with 'length' elements of 'typesize' with 'map',
 * copies it into 'dest' */
extern void _devkit_generator( void *dest, void *base, size_t length, size_t typesize, Map map) {
	memcpy( dest, nonnull(base), length*typesize);
	for (size_t idx = 0; idx < length; idx++) {
		map( dest + idx*typesize);
	}
}


/* Returns a uniform devkit_range of 'steps' values between 'start' and 'end'.
 * NOTE: steps must be larger or equal than 2*/
extern void* _devkit_linspace( DEVKIT_ALLOCATOR *alloc, double start, double end, size_t steps, bool isfloat) {
	assert( steps >= 2);

	if (isfloat) {
		float delta = (end - start) / (steps - 1);
		float *values = DEVKIT_CALLOC( alloc, end - start, sizeof(float));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
	else {
		double delta = (end - start) / (steps - 1);
		double *values = DEVKIT_CALLOC( alloc, end - start, sizeof(double));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
}

#endif

#endif
