#ifndef __DEVKIT_LIST_STRUCT_H
#define __DEVKIT_LIST_STRUCT_H

#include "iterable.h"
#include <stddef.h>

/*
	-----------
	LIST STRUCT
	-----------
*/

/* An approach to variable length arrays in C. */

struct devkit_list {
	void *items;
	size_t length;
	size_t capacity;
	size_t typesize;
	DEVKIT_ALLOCATOR *alloc;
};

Iterable devkit_list_asiterable( DEVKIT_ALLOCATOR*, struct devkit_list*);

#endif
