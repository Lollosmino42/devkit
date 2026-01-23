#ifndef _DEVKIT_POINTERS_H
#define _DEVKIT_POINTERS_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bits/iterable.h"

#if DEVKIT_STRIP_PREFIXES

#define asiterable devkit_asiterable
#define linspace devkit_linspace
#define flinspace devkit_flinspace
#define range devkit_range
#define lrange devkit_lrange
#define contains devkit_contains
#define unref devkit_unref

#endif


#define devkit_asiterable( array, length, typesize) _devkit_asiterable( (array), (length), typesize)	
#define devkit_linspace( start, end, steps) _devkit_linspace( (start), (end), (steps), false)
#define devkit_flinspace( start, end, steps) _devkit_linspace( (start), (end), (steps), true)
#define devkit_range( start, end) _devkit_range( (start), (end), false)
#define devkit_lrange( start, end) _devkit_range( (start), (end), true)

/* Checks if an array contains a certain value */
#define devkit_contains( array, len, var) _devkit_contains( (array), (len), sizeof(*(array)), &(var))
/* Unreferences to pointer after casting */
#define devkit_unref( type) *(type*)



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
extern inline DevkitIterable _devkit_asiterable( void* array, size_t length, size_t typesize) {
#ifdef DEVKIT_DEBUG
	assert( array);
#endif
	return (DevkitIterable) { 
		.typesize=typesize, 
		.length=length, 
		.items=array
	};
}

#include <math.h>

/* Gives a set of numbers from 'start' to 'end' - 1 */
extern void* _devkit_range( long start, long end, bool islong) {
#ifdef DEVKIT_DEBUG
	assert(start != end)
#endif
	int sign = (start < end) ? 1 : -1;
	long delta = labs(end - start);
	if (islong) {
		long *dest = calloc( delta, sizeof(long));
		for (long l = 0; l != delta; l++, start += sign) {
			dest[l] = start;
		}
		return dest;
	}
	else {
		int *dest = calloc( delta, sizeof(int));
		for (long l = 0; l != delta; l++, start += sign) {
			dest[l] = (int) start;
		}
		return dest;
	}
}


/* Returns a uniform devkit_range of 'steps' values between 'start' and 'end'.
 * NOTE: steps must be larger or equal than 2 */
extern void* _devkit_linspace( double start, double end, size_t steps, bool isfloat) {
#ifdef DEVKIT_DEBUG
	assert( steps >= 2);
#endif

	if (isfloat) {
		float delta = (end - start) / (steps - 1);
		float *values = calloc( end - start, sizeof(float));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
	else {
		double delta = (end - start) / (steps - 1);
		double *values = calloc( end - start, sizeof(double));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
}

#endif

#endif
