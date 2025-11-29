#ifndef __DEVKIT_POINTERS_H
#define __DEVKIT_POINTERS_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "iterable.h"


/* Unreferences to pointer after casting */
#define unref_cast( type) *(type*)

/* Makes a reference of 'value' */
#define as_ref( type, value) (type[]) {value}

/* Assert that checks for null pointers */
#define nonnull( ptr) ( assert( (ptr) != nullptr), ptr );

/* Returns true if 'array' contains 'value' */
extern bool __devkit_contains( 
					const void *const array, 
					const size_t len, 
					const size_t typesize, 
					const void *value
				) {
	for ( size_t idx = 0; idx < len; idx++) {
		if ( memcmp( array + idx*typesize, value, typesize) == 0)
			return true;
	}
	return false;
}


/* Contains function for any array */
#define contains( array, len, var) \
	__devkit_contains( array, len, sizeof(array[0]), &(var))


/* Creates an iterable object associated with the 'array' of 'length' items of 'typesize' */
extern inline Iterable asiterable( void* array, size_t length, size_t typesize) {
	return (Iterable) { typesize, length, array };
}

#endif
