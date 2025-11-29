#ifndef __DEVKIT_ARRAY_H
#define __DEVKIT_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "comparators.h"
#include "iterable.h"


/*
 * ------------
 * ARRAY STRUCT
 * ------------
 */

typedef struct {
	const size_t typesize;
	const size_t length;
	void* items;
} Array;


#define new_array( type, length) \
	(Array) { sizeof(type), length, calloc( length, sizeof(type)) }

extern Array array_of( size_t typesize, size_t length, void *items);

extern void array_set( Array *array, size_t index, void* value);

#define array_get( type, array, index) \
	*((type)*) __devkit_array_get( array, index)

extern inline void array_sort( Array *array, Comparator func);

extern inline Iterable array_asiterable( Array *array);

extern Array array_slice( Array *array, size_t start, size_t end);

#define array_fromarray( array, length) \
	array_of( sizeof((array)[0]), (length), (array) )

#define array_free( array) (free(array.items), array.length = 0)


/* IMPLEMENTATION */


/* Makes an array with 'nitems' in it. Values must be passed by reference */
extern Array array_of( size_t typesize, size_t length, void *items) {
	assert(items != nullptr);
	Array array = { typesize, length, calloc( length, typesize) };
	// Copy first value
	memcpy( array.items, items, typesize*length);

	return array;
}

/* Sets a value of 'array' at 'index' to 'value' */
extern void array_set( Array *array, size_t index, void* value) {
	assert(array!=nullptr);
	memcpy( array + index, value, array->typesize);
}

/* Get an item of the array at 'index' of 'type' */
extern inline void* __devkit_array_get( Array *array, size_t index) {
	assert(array!=nullptr);
	return array->items + index*array->typesize;
}

/* Sorts an array using function 'func' */
extern inline void array_sort( Array *array, Comparator func) {
	assert( array != nullptr);
	qsort( array, array->length, array->typesize, func);
}


/* Returns an Iterable object REFERENCING the items of the array.
 * WARNING: the items of this object are REFERENCES to the array items!
 * If you use a map on the items, the items of the array will also be modified,
 * because they ARE the same items! This is intentional, but caution must be used
 * to avoid any unwanted side-effects! */
extern inline Iterable array_asiterable( Array *array) {
	assert( array != nullptr);
	return (Iterable) { array->typesize, array->length, array->items};
}


/* Returns a copy of the array with all elements from 'start' to 'end' */
extern Array array_slice( Array *array, size_t start, size_t end) {
	assert( array != nullptr);
	assert( end > start);

	size_t delta = end - start;
	void *slice = malloc( array->typesize*delta);
	void *src = array->items + start*array->typesize;
	memcpy( slice, src, array->typesize*delta); 
	// Return slice
	return (Array) { array->typesize, delta, };
}


#endif
