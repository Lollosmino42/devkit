#ifndef __DEVKIT_ITERABLE_H
#define __DEVKIT_ITERABLE_H

#include <stdlib.h>

/* NOTE: this header should not be included on its own as it
 * is useless without iterable structs */

/* Map: a function that takes a parameter and does something with it */
typedef void (*Map)(void*);

typedef struct {
	const size_t typesize;
	const size_t length;
	void *items;
} Iterable;



/* Cast to iterable.
 * Only works with Arrays, Lists and normal pointers/arrays */
#define iterable( structure) _Generic( (structure), \
		Array: array_asiterable, \
		List: list_asiterable, \
		char*: asiterable( (structure), strlen((structure)), 1), \
		default: (structure) ) ( &(structure) )

#define foreach_range( iter, map, start, end) \
	__devkit_foreach( (Iterable[]) {iterable(iter)}, (Map) (map), (start), (end) )

#define foreach( iter, map) foreach_range( (iter), (map), 0, (iter).length )

/* 'Unlinks' the Iterator from the items referenced to avoid any
 * unwanted modifications. Use this only after you are done using the iterator,
 * because it is unusable after this function call*/
#define iterator_unlink( iterable) (iterable->items) = nullptr


/* For loop that iterates directly over the Iterable objects */
extern void __devkit_foreach( Iterable *iter, Map map, const size_t start, const size_t end) {
	for ( size_t index = start; index < end; index++ ) {
		void* current = iter->items + iter->typesize*index;
		map(current);
	}
}

#endif
