#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "comparators.h"


typedef struct {
	const size_t typesize;
	const size_t size;
	void* items;
} Array;


#define new_array( type, size) { sizeof(type), size, calloc( size, sizeof(type)) }


/* Makes an array with 'nitems' in it. Values must be passed by reference */
extern Array array_of( size_t typesize, size_t nitems, void* first, ...) {
	va_list args;
	va_start( args);

	Array array = { typesize, nitems, calloc( nitems, typesize) };
	// Copy first value
	memcpy( array.items, first, typesize);
	// Copy other values
	for (size_t times = 1; times < nitems; times++) {
		memcpy( array.items + times*array.typesize, va_arg( args, void*), typesize);
	}

	return array;
}


/* Sets a value of 'array' at 'index' to 'value' */
extern void array_set( Array *array, size_t index, void* value) {
	memcpy( array + index, value, array->typesize);
}


#define array_get( type, array, index) \
	*((type)*) __devkit_array_get( array, index)

/* Get an item of the array at 'index' of 'type' */
extern inline void* __devkit_array_get( Array *array, size_t index) {
	return array->items + index*array->typesize;
}

/* Sorts an array using function 'func' */
extern void array_sort( Array *array, Comparator func) {
	qsort( array, array->size, array->typesize, func);
}

