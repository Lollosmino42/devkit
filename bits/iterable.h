#ifndef __DEVKIT_ITERABLE_H
#define __DEVKIT_ITERABLE_H

#include <stddef.h>

#include "../settings.h"

/* NOTE: this header should not be included on its own as it
 * is useless without __devkit_iterable structs */

/* Map: a function that takes a parameter and does something with it */
typedef void (*Map)(void*);

/* Macro to quickly make a Map-like function for a 'foreach' loop */
#define map_func( name, par_type, par, expression) \
	void name ( void *__devkit_par) { \
		par_type par = *(par_type*)__devkit_par; \
		do { expression; } while(0); \
		memcpy( __devkit_par, &par, sizeof(par_type)); \
	}


typedef struct devkit_iterable {
	void *items;
	union {
		const size_t length, size;
	};
	const size_t typesize;
	DEVKIT_ALLOCATOR *alloc;
} Iterable;


#include "array_struct.h"
#include "list_struct.h"
#include "math_struct.h"

/* Cast to __devkit_iterable.
 * Works with Arrays, Lists, and other structures defined in devkit that have
 * a <...>_asiterable function.
 * Other structures can be compatible with 'foreach' if an 'asiterable'-like function
 * is defined for them and if they are added in the 'settings.h' file in the macro
 * '__DEVKIT_EXTRA_ITERABLES' */

Iterable *__DEVKIT_ITERPTR;

#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define __devkit_iterable( alloc, structure) _Generic( (structure), \
		DEVKIT_ITERABLES, \
		struct devkit_array: devkit_array_asiterable, \
		struct devkit_list: devkit_list_asiterable, \
		struct devkit_vector: devkit_vector_asiterable, \
		char*: devkit_str_asiterable, \
		struct devkit_matrix: devkit_matrix_asiterable \
		) ( (alloc), (&(structure)) )

#define foreach_in( alloc, iter, map, start, end) \
	devkit_foreach( (Iterable[]) {__devkit_iterable((alloc), (iter))}, (Map) (map), (start), (end) )

#define foreach( alloc, iter, map) \
	( __DEVKIT_ITERPTR = (Iterable[]) {__devkit_iterable( (alloc), (iter))}, \
	  devkit_foreach( __DEVKIT_ITERPTR, (Map) (map), 0, __DEVKIT_ITERPTR->length) )

#else
#define __devkit_iterable( structure) _Generic( (structure), \
		DEVKIT_ITERABLES, \
		struct devkit_array: devkit_array_asiterable, \
		struct devkit_list: devkit_list_asiterable, \
		struct devkit_vector: devkit_vector_asiterable, \
		char*: devkit_str_asiterable, \
		struct devkit_matrix: devkit_matrix_asiterable \
		) ( nullptr, (&(structure)) )

#define foreach_in( iter, map, start, end) \
	devkit_foreach( (Iterable[]) {__devkit_iterable((iter))}, (Map) (map), (start), (end) )

#define foreach( iter, map) \
	( __DEVKIT_ITERPTR = (Iterable[]) {__devkit_iterable(iter)}, \
	  devkit_foreach( __DEVKIT_ITERPTR, (Map) (map), 0, __DEVKIT_ITERPTR->length) )

#endif


/* 'Unlinks' the Iterator from the items referenced to avoid any
 * unwanted modifications. Use this only after you are done using the iterator,
 * because it is unusable after this function call*/
#define iterable_unlink( iter_ref) (iter_ref->items) = nullptr


/* For loop that iterates directly over the Iterable objects */
extern void devkit_foreach( Iterable *iter, Map map, const size_t start, const size_t end) {
	for ( size_t devkit_index = start; devkit_index < end; devkit_index++ ) {
		char* current = (char*) iter->items + iter->typesize*devkit_index;
		map(current);
	}
	iterable_unlink( iter);
}

#endif
