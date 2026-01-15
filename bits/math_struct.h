#ifndef _DEVKIT_MATH_STRUCT_H
#define _DEVKIT_MATH_STRUCT_H

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
	size_t columns, rows;
};


extern Iterable devkit_vector_asiterable( struct devkit_vector *vec) {
	return (Iterable) {
		.typesize=sizeof(double),
		.length=vec->length,
		.items=vec->items
	};
}

/* Returns an Iterable that has the matrix iterated ROW BY ROW */
extern Iterable devkit_matrix_asiterable( struct devkit_matrix *mat) {
	return (Iterable) {
		.items=mat->items,
		.length=mat->length,
		.typesize=sizeof(double)
	};
}


#endif
