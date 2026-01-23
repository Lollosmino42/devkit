#ifndef DEVKIT_STRING_STRUCT_H
#define DEVKIT_STRING_STRUCT_H

#include "../settings.h"
#include "iterable.h"
#include <stddef.h>

struct devkit_string {
	char *items;
	size_t length;
	bool heap_default;
};

Iterable devkit_string_asiterable( struct devkit_string *s) {
	return (Iterable) {
		.typesize=1,
		.items=s->items,
		.length=s->length,
	};
}


#endif
