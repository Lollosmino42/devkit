#ifndef __DEVKIT_ARRAY_H
#define __DEVKIT_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bits/comparator.h"
#include "bits/iterable.h"

/*
   ARRAY
*/

//#include "bits/array_struct.h"

typedef struct devkit_array Array;


#if DEVKIT_STRIP_PREFIXES

#define array_new	devkit_array_new
#define array_new_stack	devkit_array_new_stack

#define array_itemat	devkit_array_itemat
#define array_copyto	devkit_array_copyto
#define array_sliceinto	devkit_array_sliceinto
#define array_set	devkit_array_set
#define array_sort	devkit_array_sort
#define array_free	devkit_array_freeitems

#endif


/* Allocates a new Array on the heap */
extern Array* _devkit_array_new( size_t typesize, size_t length);
#define devkit_array_new( type, length) _devkit_array_new( sizeof(type), (length))

/* Allocates a new Array on the stack (with items in heap) */
extern Array _devkit_array_new_stack( size_t typesize, size_t length);
#define devkit_array_new_stack( type, length) _devkit_array_new_stack( sizeof(type), (length))


/* Gets a reference to the item at 'index' in 'array' */
extern void* devkit_array_itemat( Array *array, size_t index);

/* Sets 'array' item at 'index' to 'value' */
extern void devkit_array_set( Array *restrict array, size_t index, void *restrict value);

/* Copy 'array' items to buffer 'dest' */
extern void devkit_array_copyto( void *restrict dest, Array *restrict array);

/* Copy 'array' items from 'start' to 'end' into buffer 'dest' */
extern void devkit_array_sliceinto( void *restrict dest, Array *restrict array, const size_t start, const size_t end);

/* Concatenate 'array' and 'other', copying items into buffer 'dest' */
extern void devkit_array_concat( void *restrict dest, Array *array, Array *other);

/* Qsort adaptation for Array */
extern void devkit_array_sort( Array *array, Comparator func);

/* Deallocates item buffer of 'array' if allocated on heap using the standard library,
 * sets all array values to 0 */
extern void devkit_array_freeitems( Array *array);



/* IMPLEMENTATION */

//#define DEVKIT_ARRAY_IMPLEMENTATION
#ifdef DEVKIT_ARRAY_IMPLEMENTATION


Array* _devkit_array_new( size_t typesize, size_t length) {
	Array *this = malloc( sizeof(*this) + typesize*length);
	this->typesize = typesize;
	this->length = length;
	this->items = this + 1;
	return this;
}

Array _devkit_array_new_stack( size_t typesize, size_t length) {
	void *items = calloc( length, typesize);
#ifdef DEVKIT_DEBUG
	assert(items);
#endif
	return (Array) { 
		.typesize=typesize, 
		.length=length, 
		.items=items
	};
}


void* devkit_array_itemat( Array *array, size_t index) {
#ifdef DEVKIT_DEBUG
	assert(array);
	assert( index < array->length);
#endif
	return array->items + index*array->typesize;
}


void devkit_array_copyto( void *dest, Array *array) {
#ifdef DEVKIT_DEBUG
	assert( dest && array );
#endif
	memcpy( dest, array->items, array->length*array->typesize);
}


void devkit_array_set( Array *array, size_t index, void* value) {
#ifdef DEVKIT_DEBUG
	assert(array!=nullptr);
#endif
	memcpy( array + index, value, array->typesize);
}

void devkit_array_sort( Array *array, Comparator func) {
#ifdef DEVKIT_DEBUG
	assert( array != nullptr);
#endif
	qsort( array, array->length, array->typesize, func);
}



void devkit_array_sliceinto( void *restrict dest, Array *restrict array, size_t start, size_t end) {
	size_t delta = end - start;

#ifdef DEVKIT_DEBUG
	assert( array );
	assert( delta >= 0 && delta < array->length);
#endif
	void *slice = dest;
	void *src = array->items + start*array->typesize;
	memcpy( slice, src, array->typesize*delta);
}


/* Concatenates 'array' and 'other' and copies the buffer into 'dest'.
 * NOTE: arrays must be of same type */
void devkit_array_concat( void *restrict dest, Array *array, Array *other) {
#ifdef DEVKIT_DEBUG
	assert( array->typesize == other->typesize);
	assert( array && other );
#endif

	size_t newlen = array->length + other->length;

	void *concat = dest;
	memmove( concat, array->items, array->typesize*array->length);
	memmove( concat + array->typesize*array->length, other->items, array->typesize*other->length);
}


void devkit_array_freeitems( Array *array) {
	array->length = 0, array->typesize = 0;
	free(array->items);
}

#endif

#endif
