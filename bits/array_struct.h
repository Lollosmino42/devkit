#ifndef __DEVKIT_ARRAY_STRUCT_H
#define __DEVKIT_ARRAY_STRUCT_H

#include <stddef.h>
#include <assert.h>

#include "../settings.h"
#include "iterable.h"

struct devkit_array {
	void* items;
	union { size_t length, size; };
	size_t typesize;
};


/* Returns an Iterable object REFERENCING the items of the array.
 * WARNING: the items of this object are REFERENCES to the array items!
 * If you use a map on the items, the items of the array will also be modified,
 * because they ARE the same items! This is intentional, but caution must be used
 * to avoid any unwanted side-effects! */
Iterable devkit_array_asiterable( DEVKIT_ALLOCATOR *alloc, struct devkit_array *array) {
	assert( array != nullptr);
	return (Iterable) { 
		.alloc=alloc, 
		.typesize=array->typesize, 
		.length=array->length, 
		.items=array->items
	};
}

#endif
