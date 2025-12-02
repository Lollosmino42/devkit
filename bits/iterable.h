#ifndef __DEVKIT_ITERABLE_H
#define __DEVKIT_ITERABLE_H

#include <stddef.h>

#include "../settings.h"


/* NOTE: this header should not be included on its own as it
 * is useless without iterable structs */

/* Map: a function that takes a parameter and does something with it */
typedef void (*Map)(void*);

typedef struct {
	DEVKIT_ALLOCATOR *alloc;
	const size_t typesize;
	const size_t length;
	void *items;
} Iterable;



/* Cast to iterable.
 * Only works with Arrays, Lists and normal pointers/arrays */

Iterable *__DEVKIT_ITERPTR;

#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define iterable( alloc, structure) _Generic( (structure), \
		Array: devkit_array_asiterable, \
		List: devkit_list_asiterable, \
		char*: devkit_str_asiterable, \
		default: (structure) ) ( (alloc), (&(structure)) )

#define foreach_in( alloc, iter, map, start, end) \
	devkit_foreach( (Iterable[]) {iterable((alloc), (iter))}, (Map) (map), (start), (end) )

#define foreach( alloc, iter, map) \
	( __DEVKIT_ITERPTR = (Iterable[]) {iterable( (alloc), (iter))}, \
	  devkit_foreach( __DEVKIT_ITERPTR, (Map) (map), 0, __DEVKIT_ITERPTR->length) )

#else
#define iterable( structure) _Generic( (structure), \
		Array: devkit_array_asiterable, \
		List: devkit_list_asiterable, \
		char*: devkit_str_asiterable, \
		default: (structure) ) ( nullptr, (&(structure)) )

#define foreach_in( iter, map, start, end) \
	devkit_foreach( (Iterable[]) {iterable((iter))}, (Map) (map), (start), (end) )

#define foreach( iter, map) \
	( __DEVKIT_ITERPTR = (Iterable[]) {iterable(iter)}, \
	  devkit_foreach( __DEVKIT_ITERPTR, (Map) (map), 0, __DEVKIT_ITERPTR->length) )

#endif


/* 'Unlinks' the Iterator from the items referenced to avoid any
 * unwanted modifications. Use this only after you are done using the iterator,
 * because it is unusable after this function call*/
#define iterable_unlink( iter_ref) (iter_ref->items) = nullptr


/* For loop that iterates directly over the Iterable objects */
extern void devkit_foreach( Iterable *iter, Map map, const size_t start, const size_t end) {
	for ( size_t index = start; index < end; index++ ) {
		void* current = iter->items + iter->typesize*index;
		map(current);
	}
	iterable_unlink( iter);
}

#endif
