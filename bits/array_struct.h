#ifndef __DEVKIT_ARRAY_STRUCT_H
#define __DEVKIT_ARRAY_STRUCT_H

#include <stddef.h>
#include "iterable.h"

struct devkit_array {
	void* items;
	size_t length;
	size_t typesize;
	//DEVKIT_ALLOCATOR *alloc;
};

Iterable devkit_array_asiterable(DEVKIT_ALLOCATOR*, struct devkit_array*);

#endif
