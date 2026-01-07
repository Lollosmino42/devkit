#ifndef __DEVKIT_ARRAY_H
#define __DEVKIT_ARRAY_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "settings.h"
#include "bits/comparator.h"
#include "bits/iterable.h"

/*
   ARRAY
*/

//#include "bits/array_struct.h"

typedef struct devkit_array Array;

#if __DEVKIT_USE_CUSTOM_ALLOCATOR

/* Custom allocator version */
// Is there a way to fix this macro hell? Is it so bad it has to be fixed?

#define array_new( alloc, type, length) devkit_array_new( (alloc), (length), sizeof(type))
#define array_of( alloc, type, length, ...) devkit_array_from( (alloc), (length), sizeof(type), (type[]) {__VA_ARGS__})
#define array_fromptr( alloc, length, ptr) devkit_array_from( (alloc), (length), sizeof((ptr)[0]), (ptr) )
#define array_fromlist( alloc, list) devkit_array_from( (alloc), (list).length, (list).typesize, (list).items)
#define array_fromstr( alloc, str) devkit_array_from( (alloc), strlen(str), 1, (str))

#define array_getref devkit_array_get
#define array_get( alloc, type, array, index) (*(type*) array_getref( (alloc), (array), (index)) )
#define array_getitems devkit_array_getitems

#define array_slice devkit_array_slice
#define array_concat devkit_array_concat

#define array_free devkit_array_free

#else

/* Standard allocator version */
#define array_new( type, length) devkit_array_new( nullptr, (length), sizeof(type))
#define array_of( type, length, ...) devkit_array_from( nullptr, (length), sizeof(type), (type[]) {__VA_ARGS__})
#define array_fromptr( ptr, length) devkit_array_from( nullptr, (length), sizeof((ptr)[0]), (ptr) )
#define array_fromlist( list) devkit_array_from( nullptr, (list).length, (list).typesize, (list).items)
#define array_fromstr( str) devkit_array_from( nullptr, strlen(str), 1, (str))

#define array_getref( array, index) devkit_array_get( nullptr, (array), (index)) )
#define array_get( type, array, index) (*(type*) array_getref( (array), (index) ) )
#define array_getitems( array) devkit_array_getitems( nullptr, (array))

#define array_slice( array, start, end) devkit_array_slice( nullptr, (array), (start), (end))
#define array_concat( array, other) devkit_array_concat( nullptr, (array), (other))

#define array_free( array) devkit_array_free( nullptr, (array))

#endif

/* Raw declarations */
extern Array devkit_array_new( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize);
extern Array devkit_array_from( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize, void *items);
extern void* devkit_array_get( DEVKIT_ALLOCATOR *alloc, Array *array, size_t index);
extern void* devkit_array_getitems( DEVKIT_ALLOCATOR *alloc, Array *array);
extern Array devkit_array_slice( DEVKIT_ALLOCATOR *alloc, Array *array, size_t start, size_t end);
extern Array devkit_array_concat( DEVKIT_ALLOCATOR *alloc, Array *array, Array *other);
extern void devkit_array_free( DEVKIT_ALLOCATOR *alloc, Array *array);

/* Other functions that survived macro hell */
extern void array_set( Array *array, size_t index, void* value);
extern inline void array_sort( Array *array, Comparator func);



/* IMPLEMENTATION */

#define DEVKIT_ARRAY_IMPLEMENTATION
#ifdef DEVKIT_ARRAY_IMPLEMENTATION

Array devkit_array_new( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize) {
	return (Array) { 
		.typesize=typesize, 
		.length=length, 
		.items=DEVKIT_CALLOC( alloc, length, typesize)
	};
}


/* Makes an array with 'nitems' in it. Values must be passed by reference */
Array devkit_array_from( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize, void *items) {
	assert(items != nullptr);
	Array array = { 
		.typesize=typesize, 
		.length=length, 
		.items=DEVKIT_CALLOC( alloc, length, typesize) 
	};
	// Copy values into array
	memcpy( array.items, items, typesize*length);

	return array;
}


void* devkit_array_get( DEVKIT_ALLOCATOR *alloc, Array *array, size_t index) {
	void *item = DEVKIT_MALLOC( alloc, array->typesize);
	index *= array->typesize,
	memcpy( item, array->items + index, array->typesize);
	return item;
}


/* Returns a copy of the items */
void* devkit_array_getitems( DEVKIT_ALLOCATOR *alloc, Array *array) {
	void* copy = DEVKIT_CALLOC( alloc, array->length, array->typesize);
	memcpy( copy, array->items, array->length*array->typesize);
	return copy;
}


/* Sets a value of 'array' at 'index' to 'value' */
void array_set( Array *array, size_t index, void* value) {
	assert(array!=nullptr);
	memcpy( array + index, value, array->typesize);
}

/* Sorts an array using function 'func' */
inline void array_sort( Array *array, Comparator func) {
	assert( array != nullptr);
	qsort( array, array->length, array->typesize, func);
}



/* Returns a copy of the array with all elements from 'start' to 'end' */
Array devkit_array_slice( DEVKIT_ALLOCATOR *alloc, Array *array, size_t start, size_t end) {
	assert( array != nullptr);
	assert( end > start);

	size_t delta = end - start;
	void *slice = DEVKIT_MALLOC( alloc, array->typesize*delta);
	void *src = array->items + start*array->typesize;
	memcpy( slice, src, array->typesize*delta); 
	// Return slice
	return (Array) { alloc, array->typesize, delta, };
}


/* Returns a new array with concatenated items.
 * NOTE: arrays must be of same type */
Array devkit_array_concat( DEVKIT_ALLOCATOR *alloc, Array *array, Array *other) {
	assert( array->typesize == other->typesize);
	assert( array != nullptr && other != nullptr);

	size_t newlen = array->length + other->length;

	void *concat = DEVKIT_CALLOC( alloc, newlen, array->typesize);
	memcpy( concat, array->items, array->typesize*array->length);
	memcpy( concat + array->typesize*array->length, other->items, array->typesize*other->length);
	
	return (Array) { 
		.typesize=array->typesize, 
		.length=newlen, 
		.items=concat
	};
}


void devkit_array_free( DEVKIT_ALLOCATOR *alloc, Array *array) {
	DEVKIT_FREE( alloc, array->items, array->length*array->typesize),
		array->length = 0;
}

#endif

#endif
