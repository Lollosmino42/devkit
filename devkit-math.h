#ifndef _DEVKIT_MATH_H
#define _DEVKIT_MATH_H

// Remove once done editing
#define DEVKIT_MATH_IMPLEMENTATION

#ifdef DEVKIT_IMPLEMENTATION
#define DEVKIT_MATH_IMPLEMENTATION
#endif

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>


/*
 * ########
 * # MATH #
 * ########
 */

/*
 * Stack allocated (const) types
 */
typedef const struct {
	double x, y;
} DktVec2;

typedef const struct {
	double x, y, z;
} DktVec3;

/*
 * Heap allocated types
 */
typedef struct {
	double *items;
	size_t length;
} DktVector;


typedef struct {
	double *items;
	size_t length;
	size_t columns, rows;
} DktMatrix;

#ifdef _DEVKIT_H
#if !DEVKIT_INTERFACING
extern DktIterable dkt_vec_asiterable( DktVector *);

/* Returns an DktIterable that has the mat_new iterated ROW BY ROW */
extern DktIterable dkt_mat_asiterable( DktMatrix *);
#endif
#endif

#ifdef DEVKIT_STRIP_PREFIXES

typedef DktVec2 Vec2;
typedef DktVec3 Vec3;
typedef DktVector Vector;
typedef DktMatrix Matrix;

#endif


#ifdef DEVKIT_STRIP_PREFIXES

#define vec2_new	dkt_vec2_new
#define vec2_sum 	dkt_vec2_sum
#define vec2_sub 	dkt_vec2_sub
#define vec2_neg 	dkt_vec2_neg
#define vec2_scale	dkt_vec2_scale
#define vec2_mod	dkt_vec2_mod

#define vec3_new   dkt_vec3_new
#define vec3_sum   dkt_vec3_sum
#define vec3_sub   dkt_vec3_sub
#define vec3_neg   dkt_vec3_neg
#define vec3_scale dkt_vec3_scale
#define vec3_mod   dkt_vec3_mod

#define vec_new		dkt_vec_new
#define vec_init	dkt_vec_init
#define vec_tomat	dkt_vec_asmat
#define vec_free	dkt_vec_free
#define vec_copyto	dkt_vec_copyto
#define vec_sum		dkt_vec_sum
#define vec_equals	dkt_vec_equals
#define vec_scale	dkt_vec_scale
#define vec_iszero	dkt_vec_iszero
#define vec_nonzero	dkt_vec_nonzero
#define vec_set		dkt_vec_set
#define vec_get		dkt_vec_get

#define mat_new	dkt_mat_new
#define mat_free	dkt_mat_free
#define mat_getrow	dkt_mat_getrow 
#define mat_getcol	dkt_mat_getcol 
#define mat_mul	dkt_mat_mul
#define mat_init	dkt_mat_init
#define mat_copyto	dkt_mat_copyto
#define mat_get	dkt_mat_get
#define mat_set 	dkt_mat_set
#define mat_equals	dkt_mat_equals
#define mat_transpose	dkt_mat_transpose
#define mat_sum	dkt_mat_sum
#define mat_iszero	dkt_mat_iszero
#define mat_nonzero	dkt_mat_nonzero

#define vec2_tomat dkt_vec2_tomat
#define vec2_tovec	dkt_vec2_tovec
#define vec3_tomat dkt_vec3_tomat
#define vec3_tovec dkt_vec3_tovec

#define vec2_tovec dkt_vec2_tovec
#define vec3_tovec dkt_vec3_tovec

#define mat_tovec2 dkt_mat_tovec2
#define mat_tovec3 dkt_mat_tovec3
#define vec_asmat  dkt_vec_asmat 
#define mat_asvec  dkt_mat_asvec 
#endif


/*
 * NOTE:
 * While Vec2 and Vec3 are const and their functions make new structs,
 * heap structs modify the existing structs to avoid numerous heap allocations
 * that would be hard to keep track of.
 */

// VEC2
extern DktVec2	dkt_vec2_new	( double x, double y);
extern DktVec2	dkt_vec2_sum	( DktVec2 v, DktVec2 w);
extern DktVec2	dkt_vec2_sub	( DktVec2 v, DktVec2 w);
extern DktVec2	dkt_vec2_neg	( DktVec2 v);
extern DktVec2	dkt_vec2_scale	( DktVec2 v, double scale);
extern double	dkt_vec2_mod	( DktVec2 v);

// VEC3
extern DktVec3	dkt_vec3_new	( double x, double y, double z);
extern DktVec3 	dkt_vec3_sum	( DktVec3 v, DktVec3 w);
extern DktVec3 	dkt_vec3_sub	( DktVec3 v, DktVec3 w);
extern DktVec3 	dkt_vec3_neg	( DktVec3 v);
extern DktVec3 	dkt_vec3_scale	( DktVec3 v, double scale);
extern double	dkt_vec3_mod	( DktVec3 v);

// VECTOR
extern DktVector	dkt_vec_new	( size_t length);
extern void	dkt_vec_init( DktVector, double values[]);
extern void dkt_vec_free( DktVector *);
extern DktVector 	dkt_vec_copy( DktVector);
extern double	dkt_vec_get	( DktVector, size_t index);
extern void		dkt_vec_set	( DktVector, double value, size_t index);
extern void	dkt_vec_copyto	( void *restrict dest, DktVector);
extern bool dkt_vec_equals	( const DktVector, const DktVector other);
extern void dkt_vec_sum	( DktVector this, DktVector other);
extern void dkt_vec_sub	( DktVector this, DktVector other);
extern void dkt_vec_scale	( DktVector, double scalar);
extern bool dkt_vec_iszero	( DktVector);
#define dkt_vec_nonzero( v) ( assert(!dkt_vec_iszero(v)), v)

// MATRIX
/* Creates a ('rows'×'cols') matrix of zeros */
extern DktMatrix dkt_mat_new	( size_t columns, size_t rows);
/* Initialises this matrix with 'values' */
extern void		dkt_mat_init	( DktMatrix, double values[]);
/* Deallocates block of memory allocated for matrix data.
 * NOTE: if this matrix struct was created on the heap, the use of this function
 * will segfault */
extern void		dkt_mat_free	( DktMatrix *);
/* Clones the matrix with its data */
extern DktMatrix dkt_mat_copy	( DktMatrix);
/* Copy row 'row' of matrix in buffer 'dest' */
extern void		dkt_mat_getrow	( double *dest, DktMatrix, size_t row);
/* Copy col 'col' of matrix in buffer 'dest' */
extern void		dkt_mat_getcol	( double *dest, DktMatrix, size_t col);
/* Gets or sets values of matrix at ('row', 'col') */
extern double	dkt_mat_get	( DktMatrix, size_t col, size_t row);
extern void		dkt_mat_set	( DktMatrix, double value, size_t col, size_t row);
/* Copies matrix data into buffer 'dest' */
extern void		dkt_mat_copyto	( void *restrict dest, DktMatrix);
/* Creates a new matrix from the matrix multiplication of A and B */
extern DktMatrix dkt_mat_mul	( DktMatrix A, DktMatrix B);
/* Checks whether A and B are equal */
extern bool		dkt_mat_equals	( const DktMatrix A, const DktMatrix B);

/* Transposes a matrix.
 * NOTE: this function assumes the matrix is square-shaped. Be careful! */
extern void dkt_mat_transpose	( DktMatrix);
/* Adds 'mat' to this matrix */
extern void dkt_mat_sum	( DktMatrix this, DktMatrix mat);
/* Subtracts 'mat' to this matrix */
extern void dkt_mat_sub	( DktMatrix dest, DktMatrix mat);
/* Checks whether the matrix is full of zeros */
extern bool dkt_mat_iszero	( const DktMatrix);
#define	dkt_mat_nonzero( m) ( assert(!dkt_mat_iszero(m)), m)

/* Matrix - Vector conversion functions */

// Creates a NEW matrix from vec2 or vec3 (copy semantics)
extern DktMatrix	dkt_vec2_tomat	( DktVec2);
extern DktMatrix	dkt_vec3_tomat	( DktVec3);
// Copy semantics also here
extern DktVec2	dkt_mat_tovec2	( DktMatrix);
extern DktVec3 	dkt_mat_tovec3	( DktMatrix);
extern DktVec2	dkt_vec_tovec2	( DktVector);
extern DktVec3 	dkt_vec_tovec3	( DktVector);
extern DktVector	dkt_vec2_tovec	( DktVec2 v);
extern DktVector	dkt_vec3_tovec	( DktVec3 v);
// These have REFERENCE semantics: the data is not copied
extern DktMatrix	dkt_vec_asmat	( DktVector);
extern DktVector	dkt_mat_asvec	( DktMatrix);



/*
 * IMPLEMENTATION
 */

/* MATH IMPLEMENTATION */

#define DEVKIT_MATH_IMPLEMENTATION
#ifdef DEVKIT_MATH_IMPLEMENTATION

#ifdef _DEVKIT_H
#if !DEVKIT_INTERFACING
extern DktIterable dkt_vec_asiterable( DktVector *this) {
	return (DktIterable) {
		.typesize=sizeof(double),
		.length=this->length,
		.items=this->items
	};
}

extern DktIterable dkt_mat_asiterable( DktMatrix *m) {
	return (DktIterable) {
		.typesize=sizeof(double),
		.length=m->length,
		.items=m->items
	};
}
#endif
#endif


extern DktVec2 dkt_vec2_new( double x, double y) {
	return (DktVec2) { x, y};
}
extern DktVec2 dkt_vec2_sum( DktVec2 v, DktVec2 w) {
	return (DktVec2) { v.x + w.x, v.y + w.y};
}
extern DktVec2 dkt_vec2_sub( DktVec2 v, DktVec2 w) {
	return (DktVec2) { v.x - w.x, v.y - w.y};
}
extern DktVec2 dkt_vec2_neg( DktVec2 v) {
	return (DktVec2) { -v.x, -v.y};
}
extern DktVec2 dkt_vec2_scale( DktVec2 v, double scale) {
	return (DktVec2) { v.x * scale, v.y * scale};
}
extern double dkt_vec2_mod( DktVec2 v) {
	return sqrt( pow(v.x,2) + pow(v.y,2));
}
extern DktVector dkt_vec2_tovec( DktVec2 v) {
	double *items = calloc(2, sizeof(double));
	return (DktVector) {
		.items = items,
		.length = 2
	};
}

extern DktMatrix dkt_vec2_tomat( DktVec2 v) {
	DktMatrix m = dkt_mat_new(1, 2);
	dkt_mat_init(m, (double[]) {v.x, v.y});
	return m;
}

extern DktVec3 dkt_vec3_new( double x, double y, double z) {
	return (DktVec3) { x, y, z};
}
extern DktVec3 dkt_vec3_sum( DktVec3 v, DktVec3 w) {
	return (DktVec3) { v.x+w.x, v.y+w.y, v.z+w.z};
}
extern DktVec3 dkt_vec3_sub( DktVec3 v, DktVec3 w) {
	return (DktVec3) { v.x-w.x, v.y-w.y, v.z-w.z};
}
extern DktVec3 dkt_vec3_neg( DktVec3 v) {
	return (DktVec3) { -v.x, -v.y, -v.z};
}
extern DktVec3 dkt_vec3_scale( DktVec3 v, double scale) {
	return (DktVec3) { v.x*scale, v.y*scale, v.z*scale};
}
extern double dkt_vec3_mod( DktVec3 v) {
	return rootn( pow(v.x,3) + pow(v.y,3) + pow(v.z,3), 3);
}
extern DktVector dkt_vec3_tovec( DktVec3 v) {
	double *items = calloc(3, sizeof(double));
	memcpy(items, &v, sizeof(v));
	return (DktVector) {
		.items = items,
		.length = 3
	};
}
extern DktMatrix dkt_vec3_tomat( DktVec3 v) {
	DktMatrix m = dkt_mat_new(1, 3);
	dkt_mat_init(m, (double[]) {v.x, v.y, v.z});
	return m;
}


DktVector dkt_vec_new( size_t length) {
	return (DktVector) {
		.items = calloc( length, sizeof(double)),
		.length = length
	};
}

extern void dkt_vec_init( DktVector v, double values[]) {
	for (size_t i = 0; i < v.length; ++i) {
		v.items[i] = values[i];
	}
}

extern void dkt_vec_free( DktVector *v) {
	free( v->items);
	v->length = 0;
}


extern DktMatrix dkt_vec_asmat( DktVector v) {
	return (DktMatrix) {
		.length = v.length,
		.items = v.items,
		.rows = v.length,
		.columns = 1
	};
}

extern DktVector dkt_vec_copy( DktVector v) {
	double *copy = calloc(v.length, sizeof(double));
	memcpy(copy, v.items, v.length * sizeof(double));
	return (DktVector) {
		.items = copy,
		.length = v.length
	};
}


extern double dkt_vec_get( DktVector v, size_t index) {
	return v.items[index];
}

extern void dkt_vec_set( DktVector v, double value, size_t index) {
	v.items[index] = value;
}


void dkt_vec_copyto( void *restrict dest, DktVector v) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(v.items);
#endif
	memcpy( dest, v.items, v.length*sizeof(double));
}



bool dkt_vec_equals( const DktVector v, const DktVector other) {
	if (v.length != other.length) return false;
	for (size_t idx = 0; idx < v.length; idx++) {
		if ( v.items[idx] != other.items[idx]) 
			return false;
	}
	return true;
}


void dkt_vec_sum( DktVector v, DktVector other) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(other.items);
	assert(v.length == other.length);
#endif
	for (size_t idx = 0; idx < v.length; idx++)
		v.items[idx] += other.items[idx];
}

void dkt_vec_sub( DktVector v, DktVector other) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(other.items);
	assert(v.length == other.length);
#endif
	for (size_t idx = 0; idx < v.length; idx++)
		v.items[idx] -= other.items[idx];
}


void dkt_vec_scale( DktVector v, double scalar) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
#endif
	for (size_t idx = 0; idx < v.length; idx++) {
		v.items[idx] *= scalar;
	}
}


bool dkt_vec_iszero( DktVector v) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
#endif
	for ( size_t idx = 0; idx < v.length; idx++) {
		if ( v.items[idx] != 0)
			return false;
	}
	return true;
}


#ifdef _DEVKIT_H
#if !DEVKIT_INTERFACING
extern DktIterable dkt_mat_asiterable( DktMatrix *m) {
	return (DktIterable) {
		.items=m->items,
		.length=m->length,
		.typesize=sizeof(double)
	};
}
#endif
#endif


DktMatrix dkt_mat_new( size_t columns, size_t rows) {
	return (DktMatrix) {
		.columns = columns,
		.rows = rows,
		.length = rows*columns,
		.items = calloc( rows*columns, sizeof(long))
	};
}


extern void dkt_mat_init( DktMatrix m, double values[]) {
	for (size_t i = 0; i < m.length; ++i)
		m.items[i] = values[i];
}


extern void dkt_mat_free( DktMatrix *m) {
#ifdef DEVKIT_DEBUG
	assert(m->items);
#endif
	free(m->items);
	memset(m, 0, sizeof(*m));
}

extern DktVector dkt_mat_asvec( DktMatrix m) {
	return (DktVector) {
		.length = m.length,
		.items = m.items
	};
}

extern DktMatrix dkt_mat_copy( DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
#endif
	double *copy = calloc(m.length, sizeof(double));
	memcpy(copy, m.items, m.length * sizeof(double));
	return (DktMatrix) {
		.items = copy,
		.length = m.length,
		.columns = m.columns,
		.rows = m.rows
	};
}

extern void dkt_mat_getrow( double *dest, DktMatrix m, size_t row) {
	for (size_t i = 0; i < m.columns; ++i) {
		dest[i] = m.items[row*m.columns + i];
	}
}

extern void dkt_mat_getcol( double *dest, DktMatrix m, size_t col) {
	for (size_t i = 0; i < m.rows; ++i) {
		dest[i] = m.items[i*m.columns + col];
	}
}


void dkt_mat_copyto( void *restrict dest, DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(m.items);
#endif
	size_t size = m.columns*m.rows*sizeof(double);
	memcpy( dest, m.items, size);
}


double dkt_mat_get( DktMatrix m, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
	assert(col < m.columns);
	assert(row < m.rows);
#endif
	return m.items[m.columns*row + col];
}


void dkt_mat_set( DktMatrix m, double value, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
	assert(col < m.columns);
	assert(row < m.rows);
#endif
	m.items[m.columns*row + col] = value;
}


bool dkt_mat_equals( const DktMatrix A, const DktMatrix B) {
#ifdef DEVKIT_DEBUG
	assert(A.items);
	assert(B.items);
#endif
	return ( A.rows == B.rows && A.columns == B.columns)
		? ( memcmp( A.items, B.items, A.length))
			? true
			: false
		: false;
}


void dkt_mat_transpose( DktMatrix m) {
	double buffer[m.length];
	memcpy( buffer, m.items, m.length*sizeof(double));

	for ( size_t col = 0; col < m.columns; col++)
	for ( size_t row = 0; row < m.columns; row++)
		m.items[m.rows*row + col] = buffer[m.rows*col + row];
}


extern void dkt_mat_sum( DktMatrix dest, DktMatrix mat) {
#ifdef DEVKIT_DEBUG
	assert(dest.items);
	assert(mat.items);
	assert(dest.rows == mat.rows && dest.columns == mat.columns);
#endif
	for (size_t i = 0; i < dest.length; ++i)
		dest.items[i] += mat.items[i];
}

extern void dkt_mat_sub( DktMatrix dest, DktMatrix mat) {
#ifdef DEVKIT_DEBUG
	assert(dest.items);
	assert(mat.items);
	assert(dest.rows == mat.rows && dest.columns == mat.columns);
#endif
	for (size_t i = 0; i < dest.length; ++i)
		dest.items[i] -= mat.items[i];
}


extern DktMatrix dkt_mat_mul( DktMatrix A, DktMatrix B) {
#ifdef DEVKIT_DEBUG
	assert(A.items);
	assert(B.items);
	assert(A.columns == B.rows);
#endif
	DktMatrix result = dkt_mat_new(A.rows, B.columns);
	double *r;
	for (size_t row = 0; row < result.rows; row++) {
		for (size_t col = 0; col < result.columns; col++) {
			r = result.items+(result.columns*row + col);

			for (size_t idx = 0; idx < A.columns; idx++) {
					*r += A.items[row*A.columns + idx]
						* B.items[col + idx*B.columns];
			}
		}
	}
	return result;
}

bool dkt_mat_iszero( const DktMatrix m) {
	for ( size_t idx = 0; idx < m.length; idx++) {
		if ( m.items[idx] != 0)
			return false;
	}
	return true;
}

extern DktVec2 dkt_mat_tovec2	( DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(m.items && m.length == 2);
#endif
	return (DktVec2) {
		.x = m.items[0],
		.y = m.items[1]
	};
}
extern DktVec3 dkt_mat_tovec3	( DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(m.items && m.length == 3);
#endif
	return (DktVec3) {
		.x = m.items[0],
		.y = m.items[1],
		.z = m.items[2]
	};
}

extern DktVec2	dkt_vec_tovec2( DktVector v) {
#ifdef DEVKIT_DEBUG
	assert(v.items && v.length == 2);
#endif
	return (DktVec2) {
		.x = v.items[0],
		.y = v.items[1]
	};
}
extern DktVec3 	dkt_vec_tovec3( DktVector v) {
#ifdef DEVKIT_DEBUG
	assert(v.items && v.length == 2);
#endif
	return (DktVec3) {
		.x = v.items[0],
		.y = v.items[1],
		.z = v.items[2]
	};
}

#endif
#endif
