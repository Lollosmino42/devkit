#ifndef _DEVKIT_MATH_H
#define _DEVKIT_MATH_H

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "settings.h"


#include "bits/math_struct.h"

typedef struct devkit_vector Vector;
typedef struct devkit_matrix Matrix;


// Macros for raw functions

#if DEVKIT_USE_CUSTOM_ALLOCATOR
#define devkit_vector_new _devkit_vector_new
#define devkit_vector_of( alloc, length, ...) _devkit_vector_from( (alloc), (length), (double[]){__VA_ARGS__})
#define devkit_vector_fromptr _devkit_vector_from
#define devkit_vector_copy _devkit_vector_copy

#define devkit_matrix_new _devkit_matrix_new
#define devkit_matrix_copy _devkit_matrix_copy
#define devkit_matrix_fromptr _devkit_matrix_from
#define devkit_matrix_multiply _devkit_matrix_multiply

#else
#define devkit_vector_new( length) _devkit_vector_new( nullptr, (length))
#define devkit_vector_of( length, ...) _devkit_vector_from( nullptr, (length), (double[]){__VA_ARGS__})
#define devkit_vector_fromptr( length, items) _devkit_vector_from( nullptr, (length), (items))
#define devkit_vector_copy( vec) _devkit_vector_copy( nullptr, (vec))

#define devkit_matrix_new( columns, rows) _devkit_matrix_new( nullptr, (columns), (rows))
#define devkit_matrix_copy( mat) _devkit_matrix_copy( nullptr, (mat))
#define devkit_matrix_fromptr( columns, rows, ptr) _devkit_matrix_from( nullptr, (columns), (rows), (ptr))
#define devkit_matrix_multiply( A, B) _devkit_matrix_multiply( nullptr, (A), (B));

#endif

#if DEVKIT_STRIP_PREFIXES

#define vector_new	devkit_vector_new
#define vector_of	devkit_vector_of
#define vector_fromptr	devkit_vector_fromptr
#define vector_copy	devkit_vector_copy
#define vector_sum	devkit_vector_sum
#define vector_equals	devkit_vector_equals
#define vector_multiply_scalar	devkit_vector_multiply_scalar
#define vector_get	devkit_vector_get
#define vector_set	devkit_vector_set
#define vector_iszero	devkit_vector_iszero
#define vector_nonzero	devkit_vector_nonzero

#define matrix_new devkit_matrix_new
#define matrix_copy devkit_matrix_copy
#define matrix_fromptr devkit_matrix_fromptr
#define matrix_get devkit_matrix_get
#define matrix_set devkit_matrix_set
#define matrix_equals devkit_matrix_equals
#define matrix_transpose devkit_matrix_transpose
#define matrix_sum devkit_matrix_sum
#define matrix_getref devkit_matrix_getref
#define matrix_iszero devkit_matrix_iszero
#define matrix_issquare devkit_matrix_issquare
#define matrix_nonzero devkit_matrix_nonzero

#endif

// "Raw" functions

extern Vector _devkit_vector_new( DEVKIT_ALLOCATOR *alloc, size_t length);
extern Vector _devkit_vector_from( DEVKIT_ALLOCATOR *alloc, size_t length, double *items);
extern Vector _devkit_vector_copy( DEVKIT_ALLOCATOR *alloc, Vector *vec);

extern Matrix _devkit_matrix_new( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows);
extern Matrix _devkit_matrix_copy( DEVKIT_ALLOCATOR *alloc, Matrix *mat);
extern Matrix _devkit_matrix_from( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows, double *nums);


// Functions that don't require a raw form

extern Matrix _devkit_matrix_multiply( DEVKIT_ALLOCATOR *alloc, Matrix *A, Matrix *B);
extern bool devkit_vector_equals( const Vector *vec, const Vector *other);
extern inline void devkit_vector_sum( Vector *dest, size_t nvecs, Vector *vecs);
extern void devkit_vector_multiply_scalar( Vector *vec, double scalar);
extern inline double devkit_vector_get( Vector *vec, size_t idx);
extern inline bool devkit_vector_set( Vector *vec, double value, size_t idx);
extern bool devkit_vector_iszero( Vector *vec);
#define devkit_vector_nonzero( vec) ( assert(!devkit_vector_iszero(&vec)), vec)

extern inline double devkit_matrix_get( Matrix *mat, size_t col, size_t row);
extern inline bool devkit_matrix_set( Matrix *mat, double value, size_t col, size_t row);
extern bool devkit_matrix_equals( const Matrix *A, const Matrix *B);
extern void devkit_matrix_transpose( Matrix *mat);
extern inline void devkit_matrix_sum( Matrix *dest, size_t nmats, Matrix *mats);
extern inline double* devkit_matrix_getref( Matrix *mat, size_t col, size_t row);
extern bool devkit_matrix_iszero( Matrix *mat);
extern bool devkit_matrix_issquare( Matrix *mat);
#define devkit_matrix_nonzero( mat) ( assert(!devkit_matrix_iszero(&mat)), mat)



/*
	IMPLEMENTATION
*/

//#define DEVKIT_MATH_IMPLEMENTATION
#ifdef DEVKIT_MATH_IMPLEMENTATION

Vector _devkit_vector_new( DEVKIT_ALLOCATOR *alloc, size_t length) {
	Vector vec = {
		.length = length,
		.items = DEVKIT_CALLOC( alloc, length, sizeof(double))
	};
	return vec;
}

Vector _devkit_vector_from( DEVKIT_ALLOCATOR *alloc, size_t length, double *items) {
	Vector vec = {
		.length = length,
		.items = DEVKIT_CALLOC( alloc, length, sizeof(double))
	};
	memcpy( vec.items, items, length*sizeof(double));

	return vec;
}


Vector _devkit_vector_copy( DEVKIT_ALLOCATOR *alloc, Vector *vec) {
	void *items = DEVKIT_ALLOC( alloc, vec->length * sizeof(double));
	memcpy( items, vec->items, vec->length*sizeof(double));
	return (Vector) {
		.items=items,
		.length=vec->length,
	};
}



bool devkit_vector_equals( const Vector *vec, const Vector *other) {
	if (vec->length != other->length) return false;
	for (size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != other->items[idx]) 
			return false;
	}
	return true;
}


inline void devkit_vector_sum( Vector *vec, size_t nvecs, Vector *args) {
	for ( size_t idx = 0; idx < nvecs; idx++, args++) {
		assert( args->length == vec->length);
		for (size_t idx = 0; idx < vec->length; idx++)
			vec->items[idx] += args->items[idx];
	}
}


void devkit_vector_multiply_scalar( Vector *vec, double scalar) {
	for (size_t idx = 0; idx < vec->length; idx++) {
		vec->items[idx] *= scalar;
	}
}


inline double devkit_vector_get( Vector *vec, size_t idx) {
	return vec->items[idx];
}


inline bool devkit_vector_set( Vector *vec, double value, size_t idx) {
	// Return false if index is out of bounds
	return ( idx >= 0 && idx < vec->length)
		? vec->items[idx] = value, true
		: false;
}


extern bool devkit_vector_iszero( Vector *vec) {
	for ( size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != 0)
			return false;
	}
	return true;
}



Matrix _devkit_matrix_new( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows) {
	double *items = DEVKIT_CALLOC( alloc, columns*rows, sizeof(double));
	return (Matrix) {
		.items=items,
		.rows=rows,
		.columns=columns,
		.length=rows*columns
	};
}


Matrix _devkit_matrix_copy( DEVKIT_ALLOCATOR *alloc, Matrix *mat) {
	size_t size = mat->columns*mat->rows*sizeof(double);
	double *items = DEVKIT_ALLOC( alloc, size);
	memcpy( items, mat->items, size);
	return (Matrix) {
		.items=items,
		.length=mat->length,
		.columns=mat->columns,
		.rows=mat->rows
	};
}


inline double devkit_matrix_get( Matrix *mat, size_t col, size_t row) {
	return mat->items[mat->columns*row + col];
}


inline bool devkit_matrix_set( Matrix *mat, double value, size_t col, size_t row) {
	// Do nothing when index out of bounds and return false
	if ( row < 0 || row >= mat->rows || col < 0 || col >= mat->columns)
		return false;
	else 
		return mat->items[mat->columns*row + col] = value,
			   true;
}


bool devkit_matrix_equals( const Matrix *A, const Matrix *B) {
	return ( A->rows == B->rows && A->columns == B->columns)
		? ( memcmp( A->items, B->items, A->length))
			? true
			: false
		: false;
}


void devkit_matrix_transpose( Matrix *mat) {
	double buffer[mat->length];
	memcpy( buffer, mat->items, mat->length*sizeof(double));

	for ( size_t col = 0; col < mat->columns; col++)
	for ( size_t row = 0; row < mat->rows; row++)
		mat->items[mat->rows*row + col] = buffer[mat->rows*col + row];
	
	double temp = mat->rows;
	mat->rows = mat->columns;
	mat->columns = temp;
}


inline void devkit_matrix_sum( Matrix *dest, size_t nmats, Matrix *mats) {
	for (size_t mat = 0; mat < nmats; mat++, mats++) {
		for (size_t idx = 0; idx < dest->length; idx++) {
			dest->items[idx] += mats->items[idx];
		}
	}
}


Matrix _devkit_matrix_from( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows, double *nums) {
	const size_t size = sizeof(double)*columns*rows;
	double *items = DEVKIT_ALLOC( alloc, size);
	memcpy( items, nums, size);

	return (Matrix) {
		.items=items,
		.length=columns*rows,
		.rows=rows,
		.columns=columns
	};
}


inline double* devkit_matrix_getref( Matrix *mat, size_t col, size_t row) {
	// Check for index out of bounds
	return ( row < 0 || row >= mat->rows || col < 0 || col >= mat->columns)
		? nullptr
		: &mat->items[mat->columns*row + col];
}


Matrix _devkit_matrix_multiply( DEVKIT_ALLOCATOR *alloc, Matrix *A, Matrix *B) {
	assert( A->columns == B->rows);

	Matrix result = (Matrix) {
		.items=DEVKIT_CALLOC( alloc, A->rows*B->columns, sizeof(double)),
		.length=A->rows*B->columns,
		.rows=A->rows,
		.columns=B->columns
	};

	double *r;
	for (size_t row = 0; row < result.rows; row++) {
		for (size_t col = 0; col < result.columns; col++) {
			r = result.items+(result.columns*row + col);

			for (size_t idx = 0; idx < A->columns; idx++) {
					*r += A->items[row*A->columns + idx]
						* B->items[col + idx*B->columns];
			}
		}
	}

	return result;
}


extern bool devkit_matrix_iszero( Matrix *mat) {
	for ( size_t idx = 0; idx < mat->length; idx++) {
		if ( mat->items[idx] != 0)
			return false;
	}
	return true;
}


extern bool devkit_matrix_issquare( Matrix *mat) {
	return mat->rows == mat->columns;
}

#endif

#endif
