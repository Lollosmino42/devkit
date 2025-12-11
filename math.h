#ifndef __DEVKIT_MATH_H
#define __DEVKIT_MATH_H

#include <stddef.h>
#include <string.h>
#include <stdarg.h>
#include <assert.h>

#include "settings.h"


#include "bits/math_struct.h"

typedef struct devkit_vector Vector;
typedef struct devkit_matrix Matrix;

constexpr Vector NULL_VECTOR = { .length=0, .items=nullptr };


// "Raw" functions

extern Vector devkit_vector_new( DEVKIT_ALLOCATOR *alloc, size_t length);
extern Vector devkit_vector_from( DEVKIT_ALLOCATOR *alloc, size_t length, double *items);
extern Vector devkit_vector_copy( DEVKIT_ALLOCATOR *alloc, Vector *vec);

extern Matrix devkit_matrix_new( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows);
extern Matrix devkit_matrix_copy( DEVKIT_ALLOCATOR *alloc, Matrix *mat);
extern Matrix devkit_matrix_from( DEVKIT_ALLOCATOR *alloc, size_t columns, 
									size_t rows, double *nums);

// Macros for raw functions

#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define vector_new devkit_vector_new
#define vector_of( alloc, length, ...) devkit_vector_from( (alloc), (length), (double[]){__VA_ARGS__})
#define vector_fromptr devkit_vector_from
#define vector_copy devkit_vector_copy

#define matrix_new devkit_matrix_new
#define matrix_copy devkit_matrix_copy
#define matrix_fromptr devkit_matrix_from

#else
#define vector_new( length) devkit_vector_new( nullptr, (length))
#define vector_of( length, ...) devkit_vector_from( nullptr, (length), (double[]){__VA_ARGS__})
#define vector_fromptr( length, items) devkit_vector_from( nullptr, (length), (items))
#define vector_copy( vec) devkit_vector_copy( nullptr, (vec))

#define matrix_new( columns, rows) devkit_matrix_new( nullptr, (columns), (rows))
#define matrix_copy( mat) devkit_matrix_copy( nullptr, (mat))
#define matrix_fromptr( columns, rows, ptr) devkit_matrix_from( nullptr, (columns), (rows), (ptr))

#endif

// Functions that don't require a raw form

extern bool vector_equals( const Vector *vec, const Vector *other);
extern inline void vector_sum( Vector *dest, size_t nvecs, Vector *vecs);
extern void vector_multiply_scalar( Vector *vec, double scalar);
extern inline double vector_get( Vector *vec, size_t idx);
extern inline void vector_set( Vector *vec, double value, size_t idx);

extern inline double matrix_get( Matrix *mat, size_t col, size_t row);
extern inline void matrix_set( Matrix *mat, double value, size_t col, size_t row);
extern bool matrix_equals( const Matrix *mat, const Matrix *other);
extern void matrix_transpose( Matrix *mat);
extern inline void matrix_sum( Matrix *dest, size_t nmats, Matrix *mats);
extern inline double* matrix_getref( Matrix *mat, size_t col, size_t row);



/*
	IMPLEMENTATION
*/

Vector devkit_vector_new( DEVKIT_ALLOCATOR *alloc, size_t length) {
	Vector vec = {
		.length = length,
		.items = DEVKIT_CALLOC( alloc, length, sizeof(double))
	};
	return vec;
}

Vector devkit_vector_from( DEVKIT_ALLOCATOR *alloc, size_t length, double *items) {
	Vector vec = {
		.length = length,
		.items = DEVKIT_CALLOC( alloc, length, sizeof(double))
	};
	memcpy( vec.items, items, length*sizeof(double));

	return vec;
}


Vector devkit_vector_copy( DEVKIT_ALLOCATOR *alloc, Vector *vec) {
	void *items = DEVKIT_MALLOC( alloc, vec->length * sizeof(double));
	memcpy( items, vec->items, vec->length*sizeof(double));
	return (Vector) {
		.items=items,
		.length=vec->length,
	};
}



bool vector_equals( const Vector *vec, const Vector *other) {
	if (vec->length != other->length) return false;
	for (size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != other->items[idx]) 
			return false;
	}
	return true;
}


inline void vector_sum( Vector *vec, size_t nvecs, Vector *args) {
	for ( size_t idx = 0; idx < nvecs; idx++, args++) {
		assert( args->length == vec->length);
		for (size_t idx = 0; idx < vec->length; idx++)
			vec->items[idx] += args->items[idx];
	}
}


void vector_multiply_scalar( Vector *vec, double scalar) {
	for (size_t idx = 0; idx < vec->length; idx++) {
		vec->items[idx] *= scalar;
	}
}


inline double vector_get( Vector *vec, size_t idx) {
	return vec->items[idx];
}


inline void vector_set( Vector *vec, double value, size_t idx) {
	vec->items[idx] = value;
}



Matrix devkit_matrix_new( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows) {
	double *items = DEVKIT_CALLOC( alloc, columns*rows, sizeof(double));
	return (Matrix) {
		.items=items,
		.rows=rows,
		.columns=columns,
		.length=rows*columns
	};
}


Matrix devkit_matrix_copy( DEVKIT_ALLOCATOR *alloc, Matrix *mat) {
	size_t size = mat->columns*mat->rows*sizeof(double);
	double *items = DEVKIT_MALLOC( alloc, size);
	memcpy( items, mat->items, size);
	return (Matrix) {
		.items=items,
		.length=mat->length,
		.columns=mat->columns,
		.rows=mat->rows
	};
}


inline double matrix_get( Matrix *mat, size_t col, size_t row) {
	return mat->items[mat->columns*row + col];
}


inline void matrix_set( Matrix *mat, double value, size_t col, size_t row) {
	// Check for index out of bounds
	if ( row < 0 || row >= mat->rows || col < 0 || col >= mat->columns)
		return;
	else mat->items[mat->columns*row + col] = value;
}


bool matrix_equals( const Matrix *mat, const Matrix *other) {
	return ( mat->rows == other->rows && mat->columns == other->columns)
		? false
		: ( memcmp( mat->items, other->items, mat->length))
			? true
			: false;
}


void matrix_transpose( Matrix *mat) {
	double buffer[mat->length];
	memcpy( buffer, mat->items, mat->length*sizeof(double));

	for ( size_t col = 0; col < mat->columns; col++)
	for ( size_t row = 0; row < mat->rows; row++)
		mat->items[mat->rows*row + col] = buffer[mat->rows*col + row];
	
	double temp = mat->rows;
	mat->rows = mat->columns;
	mat->columns = temp;
}


inline void matrix_sum( Matrix *dest, size_t nmats, Matrix *mats) {
	for (size_t mat = 0; mat < nmats; mat++, mats++) {
		for (size_t idx = 0; idx < dest->length; idx++) {
			dest->items[idx] += mats->items[idx];
		}
	}
}


Matrix devkit_matrix_from( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows, double *nums) {
	const size_t size = sizeof(double)*columns*rows;
	double *items = DEVKIT_MALLOC( alloc, size);
	memcpy( items, nums, size);

	return (Matrix) {
		.items=items,
		.length=columns*rows,
		.rows=rows,
		.columns=columns
	};
}


inline double* matrix_getref( Matrix *mat, size_t col, size_t row) {
	// Check for index out of bounds
	return ( row < 0 || row >= mat->rows || col < 0 || col >= mat->columns)
		? nullptr
		: &mat->items[mat->columns*row + col];
}


#endif
