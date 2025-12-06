#ifndef __DEVKIT_LIST_STRUCT_H
#define __DEVKIT_LIST_STRUCT_H

#include <stddef.h>
#include <assert.h>

#include "iterable.h"

/*
	-----------
	LIST STRUCT
	-----------
*/

/* An approach to variable length arrays in C. */

struct devkit_list {
	void *items;
	union { size_t length, size; };
	size_t capacity;
	size_t typesize;
	DEVKIT_ALLOCATOR *alloc;
};


/* Returns a new iterable associated to the list.
 * NOTE: this iterable has direct REFERENCES to the list data! Be careful!*/
Iterable devkit_list_asiterable( DEVKIT_ALLOCATOR *alloc, struct devkit_list *list) {
	assert( list != nullptr);
	return (Iterable) { 
		.alloc=alloc, 
		.typesize=list->typesize, 
		.length=list->length, 
		.items=list->items
	};
}

#endif
