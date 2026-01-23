#ifndef _DEVKIT_MATH_H
#define _DEVKIT_MATH_H

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>


/* Note that Matrix can be converted to vector, as they have the same
 * variables and structure, so Matrix can use Vector functions if cast to Vector */
#include "bits/math_struct.h"

typedef struct devkit_vector Vector;
typedef struct devkit_matrix Matrix;


#if DEVKIT_STRIP_PREFIXES

#define vector_new	devkit_vector_new
#define vector_new_stack	devkit_vector_new_stack
#define vector_copyto	devkit_vector_copyto
#define vector_sum	devkit_vector_sum
#define vector_equals	devkit_vector_equals
#define vector_multiply_scalar	devkit_vector_multiply_scalar
#define vector_iszero	devkit_vector_iszero
#define vector_nonzero	devkit_vector_nonzero

#define matrix_new devkit_matrix_new
#define matrix_new_stack devkit_matrix_new_stack
#define matrix_copyto devkit_matrix_copyto
#define matrix_get devkit_matrix_get
#define matrix_set devkit_matrix_set
#define matrix_equals devkit_matrix_equals
#define matrix_transpose devkit_matrix_transpose
#define matrix_sum devkit_matrix_sum
#define matrix_iszero devkit_matrix_iszero
#define matrix_nonzero devkit_matrix_nonzero

#endif


extern Vector* devkit_vector_new( size_t length);
extern Vector devkit_vector_new_stack( size_t length);

// Access items through 'vector'->items

extern void devkit_vector_copyto( void *restrict dest, Vector *restrict vec);
extern bool devkit_vector_equals( const Vector *vec, const Vector *other);
extern void devkit_vector_sum( Vector *this, size_t nvecs, Vector *vecs);
extern void devkit_vector_multiply_scalar( Vector *vec, double scalar);
extern bool devkit_vector_iszero( Vector *vec);
#define devkit_vector_nonzero( vec) ( assert(!devkit_vector_iszero(&vec)), vec)


extern Matrix* devkit_matrix_new( size_t columns, size_t rows);
extern Matrix devkit_matrix_new_stack( size_t columns, size_t rows);

extern double* devkit_matrix_get( Matrix *mat, size_t col, size_t row);
extern void devkit_matrix_set( Matrix *mat, double value, size_t col, size_t row);
extern void devkit_matrix_copyto( void *restrict dest, Matrix *restrict mat);
extern void devkit_matrix_multiply( Matrix *restrict dest, Matrix *restrict A, Matrix *restrict B);
extern bool devkit_matrix_equals( const Matrix *A, const Matrix *B);
extern void devkit_matrix_transpose( Matrix *mat);
extern void devkit_matrix_sum( Matrix *this, size_t nmats, Matrix *mats);
extern bool devkit_matrix_iszero( const Matrix *mat);
#define devkit_matrix_nonzero( mat) ( assert(!devkit_matrix_iszero(&mat)), mat)



/*
	IMPLEMENTATION
*/

//#define DEVKIT_MATH_IMPLEMENTATION
#ifdef DEVKIT_MATH_IMPLEMENTATION

Vector* devkit_vector_new( size_t length) {
	Vector *this = malloc( sizeof(*this) + length * sizeof(double));
	this->length = length;
	this->items = (double*)(this + 1);
	return this;
}

Vector devkit_vector_new_stack( size_t length) {
	return (Vector) {
		.items = calloc( length, sizeof(double)),
		.length = length
	};
}


void devkit_vector_copyto( void *restrict dest, Vector *restrict vec) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(vec && vec->items);
#endif
	memcpy( dest, vec->items, vec->length*sizeof(double));
}



bool devkit_vector_equals( const Vector *vec, const Vector *other) {
	if (vec->length != other->length) return false;
	for (size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != other->items[idx]) 
			return false;
	}
	return true;
}


void devkit_vector_sum( Vector *vec, size_t nvecs, Vector *args) {
#ifdef DEVKIT_DEBUG
	assert(vec);
	assert(args);
#endif
	for ( size_t idx = 0; idx < nvecs; idx++, args++) {
#ifdef DEVKIT_DEBUG
		assert( args->length == vec->length);
#endif
		for (size_t idx = 0; idx < vec->length; idx++)
			vec->items[idx] += args->items[idx];
	}
}


void devkit_vector_multiply_scalar( Vector *vec, double scalar) {
#ifdef DEVKIT_DEBUG
	assert(vec);
#endif
	for (size_t idx = 0; idx < vec->length; idx++) {
		vec->items[idx] *= scalar;
	}
}


bool devkit_vector_iszero( Vector *vec) {
#ifdef DEVKIT_DEBUG
	assert(vec);
#endif
	for ( size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != 0)
			return false;
	}
	return true;
}



Matrix* devkit_matrix_new( size_t columns, size_t rows) {
	Matrix *this = malloc( sizeof(*this) + columns*rows*sizeof(double));
	this->items = (double*)(this + 1);
	this->columns = columns;
	this->rows = rows;
	this->length = columns*rows;
	return this;
}

Matrix devkit_matrix_new_stack( size_t columns, size_t rows) {
	return (Matrix) {
		.columns = columns,
		.rows = rows,
		.length = rows*columns,
		.items = calloc( rows*columns, sizeof(long))
	};
}


void devkit_matrix_copyto( void *restrict dest, Matrix *restrict mat) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(mat);
#endif
	size_t size = mat->columns*mat->rows*sizeof(double);
	memcpy( dest, mat->items, size);
}


double* devkit_matrix_get( Matrix *mat, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(mat)
	assert(col < mat->columns);
	assert(row < mat->rows);
#endif
	return &mat->items[mat->columns*row + col];
}


void devkit_matrix_set( Matrix *mat, double value, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(mat);
	assert(col < mat->columns);
	assert(row < mat->rows);
#endif
	mat->items[mat->columns*row + col] = value;
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


void devkit_matrix_sum( Matrix *dest, size_t nmats, Matrix *mats) {
	for (size_t mat = 0; mat < nmats; mat++, mats++) {
		for (size_t idx = 0; idx < dest->length; idx++) {
			dest->items[idx] += mats->items[idx];
		}
	}
}


void devkit_matrix_multiply( Matrix *restrict dest, Matrix *restrict A, Matrix *restrict B) {
#ifdef DEVKIT_DEBUG
	assert(A);
	assert(B);
	assert( A->columns == B->rows);
#endif
	double *r;
	for (size_t row = 0; row < dest->rows; row++) {
		for (size_t col = 0; col < dest->columns; col++) {
			r = dest->items+(dest->columns*row + col);

			for (size_t idx = 0; idx < A->columns; idx++) {
					*r += A->items[row*A->columns + idx]
						* B->items[col + idx*B->columns];
			}
		}
	}
}


bool devkit_matrix_iszero( Matrix *mat) {
	for ( size_t idx = 0; idx < mat->length; idx++) {
		if ( mat->items[idx] != 0)
			return false;
	}
	return true;
}


#endif

#endif
