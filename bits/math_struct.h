#ifndef __DEVKIT_MATH_STRUCT_H
#define __DEVKIT_MATH_STRUCT_H

#include "../settings.h"
#include "iterable.h"


/*
	-------------------------
	VECTOR AND MATRIX STRUCTS
	-------------------------
*/

struct devkit_vector {
	double *items;
	size_t length;
};

struct devkit_matrix {
	union { struct devkit_vector *columns, *items; };
	size_t length;
};


Iterable devkit_vector_asiterable( DEVKIT_ALLOCATOR *alloc, struct devkit_vector *vec) {
	return (Iterable) {
		.alloc=alloc,
		.typesize=sizeof(double),
		.length=vec->length,
		.items=(char*)vec->items
	};
}

// To be implemented
Iterable devkit_matrix_asiterable( DEVKIT_ALLOCATOR *alloc, struct devkit_matrix *mat);


#endif
