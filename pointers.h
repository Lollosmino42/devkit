#ifndef __DEVKIT_POINTERS_H
#define __DEVKIT_POINTERS_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "settings.h"
#include "bits/iterable.h"


#if __DEVKIT_USE_CUSTOM_ALLOCATOR

#define devkit_asiterable __devkit_asiterable
#define devkit_linspace( alloc, start, end) __devkit_linspace( (alloc), (start), (end), false)
#define devkit_flinspace( alloc, start, end) __devkit_linspace( (alloc), (start), (end), true)
#define devkit_generator __devkit_generator
#define devkit_range( alloc, start, end) __devkit_range( (alloc), (start), (end), false)
#define devkit_lrange( alloc, start, end) __devkit_range( (alloc), (start), (end), true)

#else
#define devkit_asiterable( array, length, typesize) __devkit_asiterable( nullptr, (array), (length), typesize)	
#define devkit_linspace( start, end, steps) __devkit_linspace( nullptr, (start), (end), (steps))
#define devkit_generator( base, length, typesize, map) __devkit_generator( nullptr, (base), (length), (typesize), (map))
#define devkit_range( start, end) __devkit_range( nullptr, (start), (end), false)
#define devkit_lrange( start, end) __devkit_range( nullptr, (start), (end), true)

#endif


#define asiterable devkit_asiterable
#define linspace devkit_linspace
#define flinspace devkit_flinspace
#define generator devkit_generator
#define range devkit_range
#define lrange devkit_lrange


#define devkit_contains( array, len, var) __devkit_contains( (array), (len), sizeof(array[0]), &(var))
#define contains devkit_contains

/* Unreferences to pointer after casting */
#define devkit_unref( type) *(type*)
#define unref devkit_unref

/* Makes a reference of values */
#define devkit_refof( type, ...) (type[]) {__VA_ARGS__}
#define refof devkit_refof

/* Asserts that ptr is not null and returns it */
#define devkit_nonnull( ptr) ( assert( (ptr) != nullptr), (ptr))
#define nonnull devkit_nonnull




/* IMPLEMENTATION */

#define DEVKIT_POINTERS_IMPLEMENTATION
#ifdef DEVKIT_POINTERS_IMPLEMENTATION

/* Returns true if 'array' contains 'value' */
extern bool __devkit_contains( 
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
extern inline Iterable __devkit_asiterable( DEVKIT_ALLOCATOR *alloc, void* array, size_t length, size_t typesize) {
	return (Iterable) { 
		.alloc=alloc,
		.typesize=typesize, 
		.length=length, 
		.items=nonnull(array)
	};
}


/* Gives a set of numbers from 'start' to 'end' - 1 */
extern void* __devkit_range( DEVKIT_ALLOCATOR *alloc, size_t start, size_t end, bool islong) {
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


/* Generates a new array from 'base' with 'length' elements of 'typesize' with 'map' */
extern void* __devkit_generator( DEVKIT_ALLOCATOR *alloc, void *base, size_t length, size_t typesize, Map map) {
	void *items = DEVKIT_CALLOC( alloc, length, typesize);
	memcpy( items, nonnull(base), length*typesize);
	for (size_t idx = 0; idx < length; idx++) {
		map( items + idx*typesize);
	}
	return items;
}


/* Returns a uniform devkit_range of 'steps' values between 'start' and 'end'.
 * NOTE: steps must be larger or equal than 2*/
extern void* __devkit_linspace( DEVKIT_ALLOCATOR *alloc, double start, double end, size_t steps, bool isfloat) {
	assert( steps >= 2);

	if (isfloat) {
		float delta = (end - start) / (steps - 1);
		float *values = DEVKIT_CALLOC( alloc, end - start, sizeof(float));
		for ( steps--; steps >= 0; steps--) values[steps] = delta*steps;
		return values;
	}
	else {
		double delta = (end - start) / (steps - 1);
		double *values = DEVKIT_CALLOC( alloc, end - start, sizeof(double));
		for ( steps--; steps >= 0; steps--) values[steps] = delta*steps;
		return values;
	}
}

#endif

#endif
