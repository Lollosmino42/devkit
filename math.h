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
constexpr Matrix NULL_MATRIX = { .length=0, .columns=nullptr };


// "Raw" functions

Vector devkit_vector_new( DEVKIT_ALLOCATOR *alloc, size_t length, ...);
Vector devkit_vector_fromptr( DEVKIT_ALLOCATOR *alloc, size_t length, double *items);
Vector devkit_vector_copy( DEVKIT_ALLOCATOR *alloc, Vector *vec);

Matrix devkit_matrix_new( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows);
Matrix devkit_matrix_copy( DEVKIT_ALLOCATOR *alloc, Matrix *mat);

// Macros for raw functions

#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define vector_new devkit_vector_new
#define vector_fromptr devkit_vector_fromptr
#define vector_copy devkit_vector_copy

#define matrix_new devkit_matrix_new

#else
#define vector_new( length, ...) devkit_vector_new( nullptr, (length), __VA_ARGS__)
#define vector_fromptr( length, items) devkit_vector_fromptr( nullptr, (length), (items))
#define vector_copy( vec) devkit_vector_copy( nullptr, (vec))

#define matrix_new( columns, rows) devkit_matrix_new( nullptr, (columns), (rows))

#endif

// Functions that don't require a raw form

bool vector_equals( const Vector *vec, const Vector *other);
void vector_sum( Vector *dest, size_t nvecs, Vector *vecs);
void vector_multiply_scalar( Vector *vec, double scalar);
double vector_get( Vector *vec, size_t idx);

void matrix_sum( Matrix *dest, size_t nmats, Matrix *mats);
bool matrix_equals( const Matrix *mat, const Matrix *other);
double matrix_get( Matrix *mat, size_t column, size_t row);
void matrix_transpose( Matrix *mat);
void matrix_set( Matrix *mat, double value, size_t col, size_t row);



/*
	IMPLEMENTATION
*/

#define MEMSIZE( veclen) (sizeof(double)*(veclen))
#define VEC_MEMSIZE( veclen) (sizeof(Vector) + MEMSIZE( veclen))
#define MAT_DATA( colptr, matlen) (char*)( (colptr) + matlen)
#define ROW_LEN( matrix) (matrix->columns[0].length)
#define MAT_GET( matrix, col, row) (matrix->columns[col].items+row)


Vector devkit_vector_new( DEVKIT_ALLOCATOR *alloc, size_t length, ...) {
	Vector vec = {
		.length = length,
		.items = DEVKIT_MALLOC( alloc, length * sizeof(double))
	};
	va_list args;
	va_start( args);
	for (size_t idx = 0; idx < length; idx++)
		vec.items[idx] = va_arg( args, double);

	return va_end(args), vec;
}

Vector devkit_vector_fromptr( DEVKIT_ALLOCATOR *alloc, size_t length, double *items) {
	double *copy = DEVKIT_MALLOC( alloc, length * sizeof(double));
	memcpy( copy, items, length * sizeof(double));
	return (Vector) {
		.length = length,
		.items = copy
	};
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


void vector_sum( Vector *vec, size_t nvecs, Vector *args) {
	Vector *arg;
	for ( size_t idx = 0; idx < nvecs; idx++) {
		arg = args+idx;
		assert( arg->length == vec->length);
		for (size_t idx = 0; idx < vec->length; idx++)
			vec->items[idx] += arg->items[idx];
	}
}


void vector_multiply_scalar( Vector *vec, double scalar) {
	for (size_t idx = 0; idx < vec->length; idx++) {
		vec->items[idx] *= scalar;
	}
}


double vector_get( Vector *vec, size_t idx) {
	return vec->items[idx];
}


Matrix devkit_matrix_new( DEVKIT_ALLOCATOR *alloc, size_t columns, size_t rows) {
	// All of this mess to pack everything into a single memory block for efficiency
	// [Vector struct info][vector items] * columns
	// [double* + size_t][double * rows] * columns ==> memory needed
	Vector *columns_buf = DEVKIT_CALLOC( alloc, columns, VEC_MEMSIZE(rows));

	char *memstart = MAT_DATA( columns_buf, columns);
	for (size_t col = 0; col < columns; col++) {
		columns_buf[col] = (Vector) { 
			.items=(double*)(memstart + col*MEMSIZE(rows)),
			.length=rows
		};
	}

	return (Matrix) {
		.columns=columns_buf,
		.length=columns,
	};
}


bool matrix_equals( const Matrix *mat, const Matrix *other) {
	if (mat->length != other->length) return false;
	for (size_t idx = 0; idx < mat->length; idx++) {
		if ( !vector_equals( mat->columns+idx, other->columns+idx)) return false;
	}
	return true;
}


void matrix_sum( Matrix *dest, size_t nmats, Matrix *args) {
	assert( dest != nullptr && args != nullptr 
			&& dest->length == args->length
			&& ROW_LEN(dest) == ROW_LEN(args));

	Vector columns[nmats];
	size_t arg;
	for (size_t idx = 0; idx < dest->length; idx++) {
		for (arg = 0; arg < dest->length; arg++)
			columns[arg] = args[arg].columns[idx];

		vector_sum( dest->columns+idx, ++arg, columns);
	}
}


double matrix_get( Matrix *mat, size_t column, size_t row) {
	return mat->columns[column].items[row];
}


Matrix devkit_matrix_copy( DEVKIT_ALLOCATOR *alloc, Matrix *mat) {
	const size_t rows = ROW_LEN(mat),
				 columns = mat->length;

	Vector *columns_buf = DEVKIT_MALLOC( alloc, columns * VEC_MEMSIZE(rows)); // To be defined
	char *memstart = MAT_DATA( columns_buf, columns);
	
	for (size_t col = 0; col < mat->length; col++) {
		columns_buf[col] = (Vector) { 
			.items=(double*)( memstart + col*MEMSIZE(rows)), 
			.length=rows 
		};

		memcpy( columns_buf[col].items, mat->columns[col].items, MEMSIZE(rows));
	}

	return (Matrix) {
		.columns=columns_buf,
		.length=columns
	};
}


void matrix_transpose( Matrix *mat) {
	const size_t rows = ROW_LEN(mat);
	double buffer[mat->length*rows];
	memcpy( buffer, MAT_DATA( mat->columns, mat->length), MEMSIZE(rows)*mat->length);

	for (size_t col = 0; col < mat->length; col++) {
		for (size_t row = 0; row < rows; row++) {
			memcpy( MAT_GET(mat, row, col), &buffer[col*rows+row], sizeof(double));
		}
	}
}


void matrix_set( Matrix *mat, double value, size_t col, size_t row) {
	memcpy( MAT_GET(mat, col, row), &value, sizeof(double));
}


#undef MEMSIZE
#undef VEC_MEMSIZE
#undef MAT_DATA
#undef ROW_LEN
#undef MAT_GET

#endif
