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
	double *items;
	size_t length;
	size_t columns,
		   rows;
};


Iterable devkit_vector_asiterable( DEVKIT_ALLOCATOR *alloc, struct devkit_vector *vec) {
	return (Iterable) {
		.alloc=alloc,
		.typesize=sizeof(double),
		.length=vec->length,
		.items=(char*)vec->items
	};
}

/* Returns an Iterable that has the matrix iterated ROW BY ROW */
Iterable devkit_matrix_asiterable( DEVKIT_ALLOCATOR *alloc, struct devkit_matrix *mat) {
	return (Iterable) {
		.items=(char*) mat->items,
		.length=mat->length,
		.typesize=sizeof(double)
	};
}


#endif
