#ifndef __DEVKIT_POINTERS_H
#define __DEVKIT_POINTERS_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "settings.h"
#include "bits/iterable.h"


#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define asiterable devkit_asiterable
#define linspace devkit_linspace
#define generator devkit_generator
#define range devkit_range

#else
#define asiterable( array, length, typesize) devkit_asiterable( nullptr, (array), (length), typesize)	
#define linspace( start, end, steps) devkit_linspace( nullptr, (start), (end), (steps))
#define generator( base, length, typesize, map) devkit_range( nullptr, (base), (length), (typesize), (map))
#define range( start, end) devkit_generator( nullptr, (start), (end))

#endif


#define contains( array, len, var) devkit_contains( array, len, sizeof(array[0]), &(var))

/* Unreferences to pointer after casting */
#define unref_cast( type) *(type*)

/* Makes a reference of values */
#define as_ref( type, ...) (type[]) {__VA_ARGS__}

/* Asserts that ptr is not null and returns it */
#define nonnull( ptr) ( assert( (ptr) != nullptr), ptr)




/* IMPLEMENTATION */	

/* Returns true if 'array' contains 'value' */
extern bool devkit_contains( 
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
extern inline Iterable devkit_asiterable( DEVKIT_ALLOCATOR *alloc, void* array, size_t length, size_t typesize) {
	return (Iterable) { alloc, typesize, length, nonnull(array) };
}


/* Gives a set of numbers from 'start' to 'end' - 1 */
extern long* devkit_range( DEVKIT_ALLOCATOR *alloc, unsigned long start, unsigned long end) {
	long *items = DEVKIT_MALLOC( alloc, (end - start) * sizeof(long));
	for (size_t x = start, idx = 0; x < end; x++, idx++) items[idx] = x;
	return items;
}


/* Generates a new array from 'base' with 'length' elements of 'typesize' with 'map' */
extern void* devkit_generator( DEVKIT_ALLOCATOR *alloc, void *base, size_t length, size_t typesize, Map map) {
	void *items = DEVKIT_CALLOC( alloc, length, typesize);
	memcpy( items, nonnull(base), length*typesize);
	for (size_t idx = 0; idx < length; idx++) {
		map( items + idx*typesize);
	}
	return items;
}


/* Returns a uniform devkit_range of 'steps' values between 'start' and 'end' */
extern long double* devkit_linspace( DEVKIT_ALLOCATOR *alloc, const double start, const double end, const unsigned steps) {
	assert( steps >= 2);

	long double delta = (end - start) / (steps - 1);
	long double *values = DEVKIT_CALLOC( alloc, end - start, sizeof(long double));
	for ( unsigned step = 0; step < steps; step++) values[step] = delta*step;
	return values;
}


#endif
