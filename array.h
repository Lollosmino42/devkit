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


#if DEVKIT_STRIP_PREFIXES

#define array_new devkit_array_new
#define array_of devkit_array_of
#define array_fromptr devkit_array_fromptr
#define array_fromlist devkit_array_fromlist
#define array_getref devkit_array_getref
#define array_get devkit_array_get
#define array_copyitems devkit_array_copyitems
#define array_concat devkit_array_concat
#define array_slice devkit_array_slice
#define array_set devkit_array_set
#define array_sort devkit_array_sort
#define array_free devkit_array_free

#endif


#if DEVKIT_USE_CUSTOM_ALLOCATOR

/* Custom allocator version */
// Is there a way to fix this macro hell? Is it so bad it has to be fixed?

#define devkit_array_new( alloc, type, length) _devkit_array_new( (alloc), (length), sizeof(type))
#define devkit_array_of( alloc, type, length, ...) _devkit_array_from( (alloc), (length), sizeof(type), (type[]) {__VA_ARGS__})
#define devkit_array_fromptr( alloc, length, ptr) _devkit_array_from( (alloc), (length), sizeof((ptr)[0]), (ptr) )
#define devkit_array_fromlist( alloc, list) _devkit_array_from( (alloc), (list).length, (list).typesize, (list).items)
#define devkit_array_fromstr( alloc, str) _devkit_array_from( (alloc), strlen(str), 1, (str))

#define devkit_array_getref devkit_array_get
#define devkit_array_get( alloc, type, array, index) (*(type*) array_getref( (alloc), (array), (index)) )

#else

/* Standard allocator version */
#define devkit_array_new( type, length) _devkit_array_new( nullptr, (length), sizeof(type))
#define devkit_array_of( type, length, ...) _devkit_array_from( nullptr, (length), sizeof(type), (type[]) {__VA_ARGS__})
#define devkit_array_fromptr( ptr, length) _devkit_array_from( nullptr, (length), sizeof((ptr)[0]), (ptr) )
#define devkit_array_fromlist( list) _devkit_array_from( nullptr, (list).length, (list).typesize, (list).items)
#define devkit_array_fromstr( str) _devkit_array_from( nullptr, strlen(str), 1, (str))

#define devkit_array_getref( array, index) devkit_array_get( nullptr, (array), (index)) )
#define devkit_array_get( type, array, index) (*(type*) array_getref( (array), (index) ) )

#endif

extern Array _devkit_array_new( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize);
extern Array _devkit_array_from( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize, void *items);
extern void* _devkit_array_get( DEVKIT_ALLOCATOR *alloc, Array *array, size_t index);


/* Other functions that survived macro hell */
extern void devkit_array_copyitems( void *dest, Array *array);
extern void devkit_array_slice( Array *restrict dest, Array *restrict array, size_t start, size_t end);
extern void devkit_array_concat( Array *dest, Array *array, Array *other);
extern void devkit_array_set( Array *array, size_t index, void* value);
extern inline void devkit_array_sort( Array *array, Comparator func);

extern void devkit_array_free( Array *array);



/* IMPLEMENTATION */

//#define DEVKIT_ARRAY_IMPLEMENTATION
#ifdef DEVKIT_ARRAY_IMPLEMENTATION

/* Creates a new list */
Array _devkit_array_new( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize) {
	return (Array) { 
		.allocator=alloc,
		.typesize=typesize, 
		.length=length, 
		.items=DEVKIT_CALLOC( alloc, length, typesize)
	};
}


/* Makes an array with 'nitems' in it. Values must be passed by reference */
Array _devkit_array_from( DEVKIT_ALLOCATOR *alloc, size_t length, size_t typesize, void *items) {
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


void* _devkit_array_get( DEVKIT_ALLOCATOR *alloc, Array *array, size_t index) {
	void *item = DEVKIT_MALLOC( alloc, array->typesize);
	index *= array->typesize,
	memcpy( item, array->items + index, array->typesize);
	return item;
}


/* Returns a copy of the items */
void devkit_array_copyitems( void *dest, Array *array) {
	assert( dest && array );
	memcpy( dest, array->items, array->length*array->typesize);
}


/* Sets a value of 'array' at 'index' to 'value' */
void devkit_array_set( Array *array, size_t index, void* value) {
	assert(array!=nullptr);
	memcpy( array + index, value, array->typesize);
}

/* Sorts an array using function 'func' */
inline void devkit_array_sort( Array *array, Comparator func) {
	assert( array != nullptr);
	qsort( array, array->length, array->typesize, func);
}



/* Returns a copy of the array with all elements from 'start' to 'end' */
void devkit_array_slice( Array *restrict dest, Array *restrict array, size_t start, size_t end) {
	size_t delta = end - start;

	assert( array != nullptr);
	assert( delta >= 0 && dest->length >= end - start);

	void *slice = dest->items;
	void *src = array->items + start*array->typesize;
	memcpy( slice, src, array->typesize*delta); 
}


/* Concatenates 'array' and 'other' and copies the buffer into 'dest'.
 * NOTE: arrays must be of same type */
void devkit_array_concat( Array *dest, Array *array, Array *other) {
	assert( array->typesize == other->typesize);
	assert( dest->length >= array->length + other->length);
	assert( array && other );

	size_t newlen = array->length + other->length;

	void *concat = dest->items;
	memmove( concat, array->items, array->typesize*array->length);
	memmove( concat + array->typesize*array->length, other->items, array->typesize*other->length);
}


void devkit_array_free( Array *array) {
	DEVKIT_FREE( array->allocator, array->items, array->length*array->typesize);
	array->length = 0;
}

#endif

#endif
