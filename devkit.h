#ifndef _DEVKIT_H
#define _DEVKIT_H

#define DEVKIT_IMPLEMENTATION

/* 
 * ##########
 * # DEVKIT #
 * ##########
 *
 * Devkit optional flags:
 *
 * DEVKIT_DEBUG to enable additional safety checks (assertions) in functions
 * DEVKIT_STRIP_PREFIXES to strip 'devkit' prefix from functions and structs
 * DEVKIT_MATH to include math declarations and functions
 * DEVKIT_INTERFACING to disable features that wouldn't work when importing 
 *		this header with another language 
 * DEVKIT_ENABLE_EXTRA_ITERABLES (Read below for info)
 *
 * -----------------------------------------------------------------
 *
 * Devkit implementation flags:
 *
 * DEVKIT_STRING_IMPLEMENTATION
 * DEVKIT_LIST_IMPLEMENTATION
 * DEVKIT_POINTERS_IMPLEMENTATION
 * DEVKIT_ARRAY_IMPLEMENTATION
 * DEVKIT_IMPLEMENTATION (defines all implementations above)
 *
 * DEVKIT_MATH_IMPLEMENTATION (defines DEVKIT_MATH automatically)
 *
 */


#ifdef DEVKIT_ENABLE_EXTRA_ITERABLES

/*
 * ###################
 * # EXTRA ITERABLES #
 * ###################
 *
 * DktIterable definition:
 *	typedef struct devkit_iterable {
 *		void *items;
 *		size_t typesize;
 *		size_t length;
 *		size_t counter; <- ignore this (nothing changes if you touch it, so do not)
 *	} DktIterable;
 *
 * Unlike typical iterables in "higher" level lanuages,
 * this iterable can be reused! (Every iteration the counter is set to 0)
 *
 * --------------------------------------------------------------
 *
 * To make a structure iterable, make a function as such:
 *
 * DktIterable <func_name>( <struct_t> *<struct>)
 *
 * Then add below:
 * <struct_t> : <func_name>,
 * <struct_t2> : <func_name2>,
 *	.
 *	.
 * <struct_tN> : <func_nameN>
 *
 * Use commas for multiple entries. Last entry must not have a comma
 *
 */

// vvvv ADD YOUR ITERABLES HERE vvvv
#define _DEVKIT_EXTRA_ITERABLES			\



#define _DEVKIT_COMMA ,
#else
#define _DEVKIT_EXTRA_ITERABLES
#define _DEVKIT_COMMA
#endif

#define _DEVKIT_ITERABLES		\
	_DEVKIT_EXTRA_ITERABLES	_DEVKIT_COMMA\
	default: dkt_view_asiterable

#if defined(__STDC__) && __STDC_VERSION__ < 202311L
#define nullptr NULL
#include <stdbool.h>
#endif

/*
 * Anything below here should not be touched
 */

#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>



#ifdef DEVKIT_IMPLEMENTATION

#define DEVKIT_LIST_IMPLEMENTATION
#define DEVKIT_ARRAY_IMPLEMENTATION
#define DEVKIT_STRING_IMPLEMENTATION
#define DEVKIT_POINTERS_IMPLEMENTATION

#endif
#ifdef DEVKIT_MATH_IMPLEMENTATION
#define DEVKIT_MATH
#endif


/* Something useful i guess */

typedef __compar_fn_t DktComparator;
#ifdef DEVKIT_STRIP_PREFIXES
typedef DktComparator Comparator;
#endif


/*
 * #########
 * # FILES #
 * #########
 *
 * Quality macros that stdio.h should have 
 * to define file opening modes so you don't have
 * to memorize the string values 
 */

#define	F_READ		"r"
#define	F_WRITE		"w"
#define F_APPEND	"a"
#define F_READ_WRITE	"r+"
#define	F_WRITE_READ	"w+"
#define	F_APPEND_READ	"a+"
#define F_READ_B	"rb"
#define	F_WRITE_B	"wb"
#define F_APPEND_B	"ab"
#define F_READ_WRITE_B	"rb+"
#define F_WRITE_READ_B	"wb+"
#define F_APPEND_READ_B	"ab+"




/*
 * ########
 * # VIEW #
 * ########
 *
 * "Template" type for most collections in this library (except String)
 */

typedef void DktViewType;

#define DKT_VIEW_TEMPLATE	\
	void *items;			\
	size_t typesize;		\
	size_t length

#define DKT_VIEW_TEMPLATE_CONST \
	void *const items;			\
	const size_t typesize;		\
	const size_t length

typedef struct {
	DKT_VIEW_TEMPLATE_CONST;
} DktView;

/* Take the full view of a view_compatible collection */

/* Take a slice of a view_compatible collection */
extern DktView	dkt_view_all	(DktViewType *v);
extern DktView	dkt_view_of	(DktViewType *v, size_t start, size_t end);
/* Get and set values of a view */
extern void*	dkt_view_get	(DktViewType *v, size_t index);
extern void  	dkt_view_set	(DktViewType *restrict v, size_t index, const void *restrict value);
extern void  	dkt_view_set_all	(DktViewType *restrict v, const void *restrict value);
extern bool		dkt_view_contains	(DktViewType *restrict v, const void *restrict value);
extern void		dkt_view_sort	(DktViewType *restrict v, DktComparator func);
extern void		dkt_view_copy	(DktViewType *restrict dest, DktViewType *restrict src);
extern void  	dkt_view_copy_array	(DktViewType *restrict dest, unsigned nitems, const void *restrict values);


#ifdef DEVKIT_STRIP_PREFIXES
#define view_all		dkt_view_all
#define view_of			dkt_view_of
#define	view_get		dkt_view_get
#define	view_set		dkt_view_set
#define	view_set_all	dkt_view_set_all
#define view_contains	dkt_view_contains
#define view_sort		dkt_view_sort
#define view_copy		dkt_view_copy
#define view_copy_array	dkt_view_copy_array
#endif



/*
 * ############
 * # ITERABLE #
 * ############
 */

#ifndef DEVKIT_INTERFACING

/* 
 * Definition.
 * Used for 'foreach' loops 
 */

typedef struct {
	DKT_VIEW_TEMPLATE;
	size_t counter;
} DktIterable;


extern DktIterable dkt_view_asiterable( DktViewType *);
#endif



/*
 * ##########
 * # STRING #
 * ##########
 */

typedef struct {
	char *items;
	size_t length;
} DktString;

typedef const DktString DktStringView;

typedef struct {
	DktString *items;
	size_t count;
	size_t size;
	size_t capacity;
} DktStringBuilder;


/* Declarations */

extern DktString		dkt_str_new	( const char *text);
extern void				dkt_str_init ( DktString *s, const char *text);
extern DktString		dkt_str_copy	( DktStringView *s);

extern DktStringView	dkt_str_view	( DktStringView *s, size_t start, size_t end);
extern void 			dkt_str_reverse	( DktString *s);
extern void 			dkt_str_free	( DktString *s);
extern char*			dkt_str_cstring	( DktStringView *s);

extern DktStringBuilder	dkt_strb_new	(size_t char_capacity);
extern void				dkt_strb_append	( DktStringBuilder *, DktStringView *s);
extern void 			dkt_strb_cappend	( DktStringBuilder *, const char *s);
extern void 			dkt_strb_clear	( DktStringBuilder *);
extern void 			dkt_strb_realloc	( DktStringBuilder *, size_t new_capacity);
extern void 			dkt_strb_free	( DktStringBuilder *);

extern char*		dkt_strb_cmake	( DktStringBuilder *, char *separator);
extern DktString	dkt_strb_make	( DktStringBuilder *, char *separator);

#ifndef DEVKIT_INTERFACING
extern DktIterable	dkt_str_asiterable	( DktStringView *);
#endif

#ifdef DEVKIT_STRIP_PREFIXES

#define str_new			dkt_str_new
#define str_init		dkt_str_init
#define str_copy		dkt_str_copy
#define str_view		dkt_str_view
#define str_reverse 	dkt_str_reverse
#define str_free		dkt_str_free
#define str_cstring		dkt_str_cstring

#define strb_new		dkt_strb_new
#define strb_append		dkt_strb_append
#define strb_cappend 	dkt_strb_cappend
#define strb_clear	 	dkt_strb_clear	
#define strb_realloc 	dkt_strb_realloc
#define strb_free	 	dkt_strb_free
#define strb_cmake		dkt_strb_cmake
#define strb_make  		dkt_strb_make

#endif

/*
 * ########
 * # LIST #
 * ########
 */

/* An approach to variable length arrays in C. */

typedef struct {
	DKT_VIEW_TEMPLATE;
	size_t capacity;
} DktList;


#define DKT_LIST_SIZE(T, capacity) (sizeof(DktList) + sizeof(T)*(capacity))

#ifdef DEVKIT_STRIP_PREFIXES

#define LIST_SIZE DKT_LIST_SIZE

#define list_new	dkt_list_new
#define list_copy	dkt_list_copy
#define list_free	dkt_list_free

#define list_add	dkt_list_add
#define list_nadd	dkt_list_nadd
#define	list_insert	dkt_list_insert
#define list_ninsert	dkt_list_ninsert
#define list_remove		dkt_list_remove
#define	list_nremove	dkt_list_nremove
#define list_concat		dkt_list_concat
#define list_expand	dkt_list_expand
#define list_trim	dkt_list_trim

#endif


/* Creates a new list */
extern DktList _dkt_list_new( const size_t typesize, const size_t capacity);
#define dkt_list_new( type, capacity) _dkt_list_new( sizeof(type), (capacity))

/* Makes a copy of this list/slice */
extern DktList dkt_list_copy( DktViewType *);

/* List destructor: calls 'free' on items, struct is set to 0 */
extern void dkt_list_free( DktList *);

/* Add 'nitems' items from 'values' to 'list' */
extern void dkt_list_nadd( DktList *restrict l, size_t nitems, const void *const values);
#define dkt_list_add( l, var) dkt_list_nadd( (l), 1, (var))

/* Insert 'nitems' of 'values' in 'list' at 'index' */
extern void dkt_list_ninsert( DktList *restrict l, size_t index, size_t nitems, const void *const values);
#define dkt_list_insert( l, index, var) dkt_list_ninsert( (l), (index), 1, (var))

/* Remove item at 'index' from 'list' and copy it to 'dest'.
 * If 'dest' is null, the value isn't copied */
extern void dkt_list_remove( void *const dest, DktList *l, size_t index);

/* Remove 'nitems' items at 'indices' in 'list', copying them into 'dest' if not null */
extern void dkt_list_nremove( void *restrict dest, DktList *l, size_t nitems, const size_t *restrict indices);

/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
extern bool dkt_list_concat( DktList *restrict dest, DktViewType *restrict src);

/* Allocate more space for 'list' to increase its capacity to 'new_capacity' */
extern void dkt_list_expand( DktList *l, size_t new_capacity);

/* Reduce list capacity to its length to free unneeded memory */
extern void dkt_list_trim( DktList *l);


/*
 * #########
 * # ARRAY #
 * #########
 */

typedef struct {
	DKT_VIEW_TEMPLATE;
} DktArray;

#define DKT_ARR_SIZE(T, capacity) (sizeof(DktArray) + sizeof(T)*(capacity))

#ifdef DEVKIT_STRIP_PREFIXES

#define ARR_SIZE DKT_ARR_SIZE

#define arr_new			dkt_arr_new
#define arr_init		dkt_arr_init
#define arr_init_values	dkt_arr_init_values
#define arr_copy		dkt_arr_copy

#define arr_concat		dkt_arr_concat
#define arr_free		dkt_arr_free

#endif


/* Allocates a new DktArray on the heap */
extern DktArray _dkt_arr_new( size_t typesize, size_t length);
#define dkt_arr_new( type, length) _dkt_arr_new( sizeof(type), (length))

extern void dkt_arr_init( DktArray *a, const size_t length, const void *const values);
extern void dkt_arr_init_values( DktArray *a, const void *const values);

/* Makes a copy of this array/slice */
extern DktArray dkt_arr_copy( DktViewType *a);

/* Concatenates 'array' and 'other' and copies the buffer into 'dest'.
 * NOTE: arrays must be of same type */
extern DktArray dkt_arr_concat( DktViewType *a, DktViewType *other);

/* Deallocates item buffer of 'array' if allocated on heap using the standard library,
 * sets all array values to 0 */
extern void dkt_arr_free( DktArray *a);

/*
 * ############
 * # POINTERS #
 * ############
 */

#ifndef DEVKIT_INTERFACING

extern DktIterable _dkt_as_iterable( void* a, size_t length, size_t typesize);
#define dkt_as_iterable( arr, length) _dkt_as_iterable( (arr), (length), sizeof(*(arr)))

extern DktView _dkt_as_view( void *a, size_t length, size_t typesize);
#define dkt_as_view( arr, length) _dkt_as_view( (arr), (length), sizeof(*(arr)))

/* Returns an heap allocated array with 'steps' rational numbers from 'start' to 'end' */
extern void* _dkt_linspace( double start, double end, size_t steps, bool isfloat);
#define dkt_linspace( start, end, steps) _dkt_linspace( (start), (end), (steps), false)
#define dkt_flinspace( start, end, steps) _dkt_linspace( (start), (end), (steps), true)

/* Returns an heap allocated array with integer numbers from 'start' to 'end - 1' */
extern void* _dkt_range( long start, long end, bool islong);
#define dkt_range( start, end) _dkt_range( (start), (end), false)
#define dkt_lrange( start, end) _dkt_range( (start), (end), true)

/* Checks if an array contains a certain value */
extern bool _dkt_contains(const void *a, size_t len, size_t typesize, const void *value);
#define dkt_contains( arr, len, var_ref) _dkt_contains( (arr), (len), sizeof(*(arr)), (var_ref))
/* Unreferences to pointer after casting */
#define dkt_unref( type, value) (*(type*)(value))
/* Creates a stack array of elements of 'type' */
#define dkt_ref( type, ...) (type[]){__VA_ARGS__}

/* Convenience function to free multiple pointers at once */
extern void _dkt_free_all( void **ptrs);
#define dkt_free_all(...) _dkt_free_all( (void*[]) {__VA_ARGS__, nullptr})

#ifdef DEVKIT_STRIP_PREFIXES

#define as_iterable	dkt_as_iterable
#define as_view		dkt_as_view
#define linspace	dkt_linspace
#define flinspace	dkt_flinspace
#define range		dkt_range
#define lrange		dkt_lrange
#define contains	dkt_contains
#define unref		dkt_unref
#define ref			dkt_ref
#define free_all	dkt_free_all

#endif

#endif




// Math module flag condition
#ifdef DEVKIT_MATH

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
	const size_t length;
} DktVector;

typedef struct {
	double *items;
	unsigned length;
	unsigned columns, rows;
} DktMatrix;


#ifndef DEVKIT_INTERFACING
extern DktIterable dkt_vec_asiterable( DktVector *);

/* Returns an DktIterable that has the mat_new iterated ROW BY ROW */
extern DktIterable dkt_mat_asiterable( DktMatrix *);

#define _DEVKIT_MATH_ITERABLES \
	DktVector:	dkt_vec_asiterable, \
	DktMatrix:	dkt_mat_asiterable,
#endif


/*
 * NOTE:
 * While Vec2 and Vec3 are const and their functions make new structs,
 * heap structs modify the existing structs to avoid numerous heap allocations
 * that would be hard to keep track of.
 */

// VEC2
extern DktVec2		dkt_vec2_new	( double x, double y);
extern DktVec2		dkt_vec2_sum	( DktVec2 v, DktVec2 w);
extern DktVec2		dkt_vec2_sub	( DktVec2 v, DktVec2 w);
/* Returns the negative of this vector */
extern DktVec2		dkt_vec2_neg	( DktVec2 v);
extern DktVec2		dkt_vec2_scale	( DktVec2 v, double scale);
/* Returns the module of this vector */
extern double		dkt_vec2_mod	( DktVec2 v);

// VEC3
extern DktVec3		dkt_vec3_new	( double x,	double y, double z);
extern DktVec3 		dkt_vec3_sum	( DktVec3 v, DktVec3 w);
extern DktVec3 		dkt_vec3_sub	( DktVec3 v, DktVec3 w);
/* Returns the negative of this vector */
extern DktVec3		dkt_vec3_neg	( DktVec3 v);
extern DktVec3 		dkt_vec3_scale	( DktVec3 v, double scale);
/* Returns the module of this vector */
extern double		dkt_vec3_mod	( DktVec3 v);

// VECTOR

/* Creates a new vector of zeros */
extern DktVector	dkt_vec_new		( const size_t length);
/* Initializes allocation of vector with values.
 * Values must be of the same size as vector! */
extern void			dkt_vec_init	( DktVector, const size_t length, double values[]);
extern void			dkt_vec_init_values	( DktVector, double values[]);
/* Frees the vector data */
extern void			dkt_vec_free	( DktVector *);
/* Creates a copy of the vector */
extern DktVector	dkt_vec_copy	( const DktVector);
/* Take a view of the vector */
extern DktView		dkt_vec_view	( const DktVector);
extern DktView		dkt_vec_view_of	( const DktVector, const size_t start, const size_t end);
/* Get or set values of vector */
extern double		dkt_vec_get		( const DktVector, const size_t index);
extern void			dkt_vec_set		( DktVector, const double value, const size_t index);
/* Checks whether two vectors are equal */
extern bool			dkt_vec_equals	( const DktVector, const DktVector other);
/* In place sum-subtraction to this vector. Other is unchanged */
extern void			dkt_vec_sum		( DktVector, const DktVector other);
extern void 		dkt_vec_sub		( DktVector, const DktVector other);
/* In place scale function */
extern void			dkt_vec_scale	( DktVector, const double scalar);
/* Checks whether this vector is all zeros */
extern bool			dkt_vec_iszero	( const DktVector);

// MATRIX

/* Creates a ('rows'×'cols') matrix of zeros */
extern DktMatrix	dkt_mat_new	( size_t columns, size_t rows);
/* Initialises this matrix with 'values' */
extern void			dkt_mat_init	( DktMatrix, const size_t cols, const size_t rows, double values[]);
extern void			dkt_mat_init_values	( DktMatrix m, double values[]);

/* Deallocates block of memory allocated for matrix data.
 * If this matrix struct was created on the heap, the use of this function
 * will segfault */
extern void			dkt_mat_free	( DktMatrix *);
/* Clones the matrix with its data */
extern DktMatrix	dkt_mat_copy	( DktMatrix);
extern DktView		dkt_mat_view	( DktMatrix);

/* Gets or sets values of matrix at ('row', 'col') */
extern double		dkt_mat_get		( DktMatrix, size_t col, size_t row);
extern void			dkt_mat_set		( DktMatrix, double value, size_t col, size_t row);

/* Adds 'mat' to this matrix */
extern void			dkt_mat_sum	( DktMatrix this, DktMatrix mat);
/* Subtracts 'mat' to this matrix */
extern void			dkt_mat_sub	( DktMatrix dest, DktMatrix mat);
/* Creates a new matrix from the matrix multiplication of A and B */
extern DktMatrix	dkt_mat_mul	( DktMatrix A, DktMatrix B);

/* Transposes a matrix.
 * This function assumes the matrix is square-shaped. Be careful! */
extern void			dkt_mat_transpose	( DktMatrix);
/* Checks whether A and B are equal */
extern bool			dkt_mat_equals	( const DktMatrix A, const DktMatrix B);
/* Checks whether the matrix is full of zeros */
extern bool			dkt_mat_iszero	( const DktMatrix);

/* Matrix - Vector conversion functions */

// Creates a NEW matrix from vec2 or vec3 (copy semantics)
extern DktMatrix	dkt_vec2_tomat	( DktVec2);
extern DktMatrix	dkt_vec3_tomat	( DktVec3);
// Copy semantics also here
extern DktVec2		dkt_mat_tovec2	( DktMatrix);
extern DktVec3 		dkt_mat_tovec3	( DktMatrix);
extern DktVec2		dkt_vec_tovec2	( DktVector);
extern DktVec3 		dkt_vec_tovec3	( DktVector);
extern DktVector	dkt_vec2_tovec	( DktVec2 v);
extern DktVector	dkt_vec3_tovec	( DktVec3 v);
// These have REFERENCE semantics: the data is not copied
extern DktMatrix	dkt_vec_asmat	( DktVector);
extern DktVector	dkt_mat_asvec	( DktMatrix);


#ifdef DEVKIT_STRIP_PREFIXES

typedef DktVec2 Vec2;
typedef DktVec3 Vec3;
typedef DktVector Vector;
typedef DktMatrix Matrix;


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

#define vec_new			dkt_vec_new
#define vec_init		dkt_vec_init
#define vec_init_values	dkt_vec_init_values
#define vec_copy		dkt_vec_copy
#define dkt_vec_view   	dkt_vec_view   
#define dkt_vec_view_of	dkt_vec_view_of
#define vec_tomat		dkt_vec_asmat
#define vec_free		dkt_vec_free
#define vec_copyto		dkt_vec_copyto
#define vec_sum			dkt_vec_sum
#define vec_equals		dkt_vec_equals
#define vec_scale		dkt_vec_scale
#define vec_iszero		dkt_vec_iszero
#define vec_set			dkt_vec_set
#define vec_get			dkt_vec_get

#define mat_new			dkt_mat_new
#define mat_free		dkt_mat_free
#define mat_getrow		dkt_mat_getrow 
#define mat_getcol		dkt_mat_getcol 
#define mat_mul			dkt_mat_mul
#define mat_init		dkt_mat_init
#define mat_copyto		dkt_mat_copyto
#define mat_get			dkt_mat_get
#define mat_set 		dkt_mat_set
#define mat_equals		dkt_mat_equals
#define mat_transpose	dkt_mat_transpose
#define mat_sum			dkt_mat_sum
#define mat_iszero		dkt_mat_iszero

#define vec2_tomat	dkt_vec2_tomat
#define vec2_tovec	dkt_vec2_tovec
#define vec3_tomat	dkt_vec3_tomat
#define vec3_tovec	dkt_vec3_tovec

#define vec2_tovec	dkt_vec2_tovec
#define vec3_tovec	dkt_vec3_tovec

#define mat_tovec2	dkt_mat_tovec2
#define mat_tovec3	dkt_mat_tovec3
#define vec_asmat	dkt_vec_asmat 
#define mat_asvec	dkt_mat_asvec 
#endif

#endif


/* 
 * ###################################################################
 * # Loop pool implementation needed for nested 'enhanced for' loops #
 * ###################################################################
 */

#ifndef DEVKIT_INTERFACING

typedef struct {
	DktIterable **loops;
	size_t length;
	size_t capacity;
} DKT_LOOP_POOL;

DKT_LOOP_POOL _DKT_POOL = (DKT_LOOP_POOL) {.loops = nullptr};

extern void _dkt_loop_pool_destroy() {
	free( _DKT_POOL.loops);
}

extern void _dkt_loop_pool_init() {
	if (!_DKT_POOL.loops) {
		_DKT_POOL = (DKT_LOOP_POOL) {
			.loops = calloc( 4, sizeof(DktIterable*)),
			.length = 0,
			.capacity = 4
		};
		atexit( _dkt_loop_pool_destroy);
	}
}

#define _dkt_loop_current (_DKT_POOL.loops[_DKT_POOL.length - 1])

extern inline void _dkt_expand_loop_pool( size_t increment) {
	_DKT_POOL.capacity += increment;
	_DKT_POOL.loops = realloc( _DKT_POOL.loops, _DKT_POOL.capacity);
}

extern inline void _dkt_loop_new( DktIterable *iter) {
	if ( _DKT_POOL.length + 1 >= _DKT_POOL.capacity)
		_dkt_expand_loop_pool( _DKT_POOL.capacity);

	_DKT_POOL.loops[_DKT_POOL.length++] = iter;
}

#define _dkt_loop_close \
	if ( _DKT_POOL.length != 0) { _DKT_POOL.loops[--_DKT_POOL.length] = nullptr; }


/*
 * ################
 * # ENHANCED FOR #
 * ################
 *
 * Bypass for DktIterable in generic selection of _dkt_iterable. With this,
 * arrays can be converted to iterables and used in foreach loops.
 * It is recommended not to use stack arrays or, in general, stack allocations
 * as it may seg-fault (don't know how to fix it)
 */
extern inline DktIterable _dkt_dummy_asiterable(DktIterable *iter) {
	return *iter;
}

/* "Cast" to _dkt_iterable.
 * Works with Arrays, Lists, and other structures defined in devkit that have
 * a <...>_asiterable function.
 * Other structures can be compatible with 'foreach' if an 'as_iterable'-like function
 * is defined for them in the section EXTRA ITERABLES at the start of this header file */
#ifndef DEVKIT_MATH
#define _DEVKIT_MATH_ITERABLES
#endif

#define _dkt_iterable( structure) _Generic( (structure), \
		_DEVKIT_ITERABLES, \
		_DEVKIT_MATH_ITERABLES \
		DktIterable: _dkt_dummy_asiterable, \
		DktString: dkt_str_asiterable \
		)( &(structure))


/* 'foreach' macros for 'enhanced for' loops.
 * It is recommended not to use this with items allocated on the stack
 * as it may seg-fault. Most structures that have items allocated on the
 * heap should work just fine */

#define foreach( type, var, iter, ...) \
	foreach_in( type, var, iter, 0, -1, __VA_ARGS__)
	  
#define foreach_in( type, var, iter, start, end, ...) { \
	_dkt_loop_pool_init(); \
	_dkt_loop_new( (DktIterable[]){ _dkt_iterable(iter)} ); \
	if (end >= 0) _dkt_loop_current->length = end; \
	type var; \
	for (_dkt_loop_current->counter = (start>=0) ? start : 0; _dkt_loop_current->counter < _dkt_loop_current->length; _dkt_loop_current->counter++) { \
		var = ((type*) _dkt_loop_current->items)[_dkt_loop_current->counter]; \
		__VA_ARGS__; \
		memcpy( ((type*) _dkt_loop_current->items)+_dkt_loop_current->counter, &var, _dkt_loop_current->typesize); \
	} \
	_dkt_loop_close; \
}

#endif

/* 
 * ##########################
 * # TYPES PREFIX STRIPPING #
 * ##########################
 */

#ifdef DEVKIT_STRIP_PREFIXES

typedef DktView View;
typedef DktArray Array;
typedef DktString String;
typedef DktStringView StringView;
typedef DktStringBuilder StringBuilder;
typedef DktList List;

#endif

/* 
 * ###################
 * # IMPLEMENTATIONS #
 * ###################
 */

/* VIEW IMPLEMENTATION */

#if		defined(DEVKIT_LIST_IMPLEMENTATION)		\
	||	defined(DEVKIT_ARRAY_IMPLEMENTATION)	\
	||	defined(DEVKIT_STRING_IMPLEMENTATION)

#ifndef DEVKIT_INTERFACING
extern DktIterable dkt_view_asiterable( DktViewType *view) {
	DktView *v = (DktView*) view;
	return (DktIterable) {
		.items = v->items,
		.typesize = v->typesize,
		.length = v->length
	};
}
#endif

DktView dkt_view_all (DktViewType *_v) {
	DktView *v = (DktView*) _v;
#ifdef DEVKIT_DEBUG
	assert(v && v->items);
	assert(view->length != 0);
#endif
	return *v;
}

DktView dkt_view_of (DktViewType *v, size_t start, size_t end) {
	DktView *view = (DktView*)v;
#ifdef DEVKIT_DEBUG
	assert(end >= start);
	assert(view && view->items);
	assert(end - start < view->length);
#endif
	return (DktView) {
		.items = view->items + start*view->typesize,
		.length = end - start,
		.typesize = view->typesize
	};
}


extern void* dkt_view_get( DktViewType *v, size_t index) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
#endif
	return view->items + view->typesize*index;
}

extern void dkt_view_set( DktViewType *restrict v, size_t index, const void *restrict value) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
	assert(value);
#endif
	memcpy( view->items + index*view->typesize, value, view->typesize);
}

extern void dkt_view_set_all( DktViewType *restrict v, const void *restrict value) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
	assert(value);
#endif
	for (size_t i = 0; i < view->length; ++i)
		memcpy(view->items + i*view->typesize, value, view->typesize);
}

extern bool	dkt_view_contains( DktViewType *restrict v, const void *restrict value) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
	assert(value);
#endif
	for (size_t i = 0; i < view->length; ++i)
		if (memcmp(view->items + view->typesize*i, value, view->typesize) == 0)
			return true;
	return false;
}

void dkt_view_sort( DktViewType *restrict v, DktComparator func) {
	DktView *view = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(view && view->items);
	assert(func);
#endif
	qsort( view->items, view->length, view->typesize, func);
}

inline void dkt_view_copy (DktViewType *restrict dest, DktViewType *restrict src) {
	DktView
		*d = (DktView*) dest,
		*s = (DktView*) src;
#ifdef DEVKIT_DEBUG
	assert(d && d->items);
	assert(s && s->items);
#endif
	memcpy(d->items, s->items, s->typesize*s->length);
}

inline void dkt_view_copy_array (DktViewType *restrict _dest, unsigned nitems, const void *restrict _values) {
	DktView *dest = (DktView*) _dest;
#ifdef DEVKIT_DEBUG
	assert(dest && dest->items);
	assert(values);
	assert(nitems != 0);
#endif
	memcpy(dest->items, _values, nitems*dest->typesize);
}

#endif


/* STRING IMPLEMENTATION */

#ifdef DEVKIT_STRING_IMPLEMENTATION

#ifndef DEVKIT_INTERFACING
DktIterable dkt_str_asiterable( DktStringView *s) {
	return (DktIterable) {
		.typesize=1,
		.items=s->items,
		.length=s->length,
	};
}
#endif

DktString dkt_str_new( const char *text) {
#ifdef DEVKIT_DEBUG
	assert(text);
#endif
	size_t length = strlen(text);
	char *items = malloc( sizeof(char)*length);
	memcpy( items, text, length);
	return (DktString) {
		.length = length,
		.items = items
	};
}


void dkt_str_init ( DktString *s, const char *text) {
#ifdef DEVKIT_DEBUG
	assert(text);
#endif
	s->length = strlen(text);
	memcpy( s->items, text, sizeof(char)*s->length);
}


DktString dkt_str_copy( DktStringView *s) {
	char *items = malloc(s->length);
	memcpy( items, s->items, s->length);
	return (DktString) {
		.length = s->length,
		.items = items
	};
}


DktStringView dkt_str_view( DktStringView *s, size_t start, size_t end) {
#ifdef DEVKIT_DEBUG
	assert( s->items);
	assert( end > start);
	assert( start >= 0 && end <= s->length);
#endif
	DktStringView substr = {
		.items = malloc(end - start),
		.length = end - start
	};

	memcpy(substr.items, s->items + start, substr.length);
	return substr;
}


void dkt_str_reverse( DktString *s) {
#ifdef DEVKIT_INTERFACING
	char *buffer = malloc(s->length);
#else
	char buffer[s->length];
#endif
	memcpy( buffer, s->items, s->length);
	for (size_t idx = 0; idx < s->length; idx++) {
		s->items[idx] = buffer[s->length - idx - 1];
	}
#ifdef DEVKIT_INTERFACING
	free(buffer);
#endif
}

extern void dkt_str_free( DktString *s) {
	free(s->items);
	// Bypass const attribute
	memset(&s->items, 0, sizeof(size_t));
}

extern char* dkt_str_cstring( DktStringView *s) {
	size_t len = s->length + 1;
	char *cstr = malloc(len);
	memset(cstr, 0, len);
	strncpy(cstr, s->items, s->length);
	return cstr;
}


extern DktStringBuilder dkt_strb_new(size_t init_capacity) {
	DktString *items = calloc( init_capacity, sizeof(DktString));
	return (DktStringBuilder) {
		.count = 0,
		.size = 0,
		.capacity = init_capacity,
		.items = items
	};
}

extern void dkt_strb_append( DktStringBuilder *sb, DktStringView *s) {
#ifdef DEVKIT_DEBUG
	assert(sb && sb->items);
	assert(s && s->items);
#endif
	size_t mem_needed = sizeof(*s) + s->length;
	size_t new_size = sb->size + mem_needed;
	sb->count += 1;

	if ( new_size > sb->capacity) {
		size_t new_cap = sb->capacity;
		while (new_cap <= new_size) new_cap *= 2;
		dkt_strb_realloc( sb, new_cap);
	}

	char *cur = (char*)sb->items + sb->size;
	memcpy(cur, s, sizeof(*s));
	cur += sizeof(*s);
	memcpy(cur, s->items, s->length);
	
	sb->size = new_size;
}

inline void dkt_strb_cappend( DktStringBuilder *sb, const char *s) {
	DktStringView wrap = { .items = (char*)s, .length = strlen(s) };
	dkt_strb_append(sb, &wrap);
}

extern void dkt_strb_clear( DktStringBuilder *sb) {
#ifdef DEVKIT_DEBUG
	assert(sb && sb->items);
#endif
	memset(sb->items, 0, sb->size);
	sb->size = 0;
	sb->count = 0;
}

extern void dkt_strb_realloc( DktStringBuilder *sb, size_t new_capacity) {
#ifdef DEVKIT_DEBUG
	assert(sb && sb->items);
#endif
	// Clone data into buffer
#ifdef DEVKIT_INTERFACING
	char *buf = malloc(sb->size);
#else
	char buf[sb->size];
#endif
	memcpy(buf, sb->items, sb->size);

	// Reallocate items buffer and put data back
	sb->items = realloc(sb->items, new_capacity);

	memcpy(sb->items, buf, sb->size);
#ifdef DEVKIT_INTERFACING
	free(buf);
#endif
}

extern void dkt_strb_free( DktStringBuilder *sb) {
	free(sb->items);
	memset(sb, 0, sizeof(*sb));
}

extern char* dkt_strb_cmake( DktStringBuilder *sb, char *sep) {
	size_t build_size = sb->size + strlen(sep)*(sb->count-1) - sb->count*sizeof(DktString) + 1;
	char *build = malloc(build_size);
	memset(build, 0, build_size);

	char *cur = (char*)sb->items;
	for (int i = 0; i < sb->count; ++i) {
		DktString *src = (DktString*)cur;

		cur += sizeof(DktString);

		size_t amount = src->length;
		strncat( build, cur, amount);
		// Add sep between tokens
		if (i != sb->count - 1)
			strncat( build, sep, strlen(sep));

		cur += src->length;
	}
	return build;
}

extern DktString dkt_strb_make( DktStringBuilder *sb, char *sep) {
	char *raw_text = dkt_strb_cmake(sb, sep);
	DktString wrapped = dkt_str_new( raw_text);
	free(raw_text);
	return wrapped;
}

#endif


/* LIST IMPLEMENTATION */

#ifdef DEVKIT_LIST_IMPLEMENTATION

DktList _dkt_list_new( size_t typesize, size_t capacity) {
	return (DktList) {
		.typesize = typesize,
		.length = 0,
		.capacity = capacity,
		.items = calloc(capacity,typesize),
	};
}

extern DktList dkt_list_copy( DktViewType *v) {
	DktView *l = (DktView*) v;
	char *items = calloc( l->length, l->typesize);
	memcpy( items, l->items, l->length*l->typesize);
	return (DktList) {
		.capacity = l->length,
		.length = l->length,
		.typesize = l->typesize,
		.items = items
	};
}

void dkt_list_free( DktList *this) {
#ifdef DEVKIT_DEBUG
	assert(this);
#endif
	free( this->items);
	memset(this, 0, sizeof(*this));
}


void dkt_list_nadd( DktList *restrict this, size_t nitems, const void *const values) {
#ifdef DEVKIT_DEBUG
	assert( this && values );
#endif

	size_t ptr = this->length; // Needed later
	this->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( this->length > this->capacity) dkt_list_expand( this, this->length);
	// Copy values in pointers
	memcpy( this->items + ptr*this->typesize, values, nitems*this->typesize);
}


void dkt_list_ninsert( DktList *restrict this, size_t index, size_t nitems, const void *const values) {
#ifdef DEVKIT_DEBUG
	assert( this && values );
#endif

	this->length += nitems;

	// Allocate more memory if needed
	if (this->length > this->capacity) dkt_list_expand( this, this->length);
	// Move following items forward, if there are any
	if (index < this->length) {
		memmove( this->items + (index+nitems)*this->typesize, this->items + index*this->typesize, this->typesize*(this->length-nitems - index) );
	}
	// Insert values at index
	memcpy( this->items + (index)*this->typesize, values, nitems*this->typesize);
}


void dkt_list_remove( void *restrict dest, DktList *this, size_t index) {
#ifdef DEVKIT_DEBUG
	assert( this && index <= this->length);
#endif

	if (dest) memcpy( dest, this->items + index, this->typesize);

	// If the item isn't last, every following item must be shifted backwards.
	if (index != --this->length) {
		void *_dest = this->items + index*this->typesize;
		void *src = _dest + this->typesize; // list->items + (index+1)*list->typesize
		memmove( _dest, src, this->typesize * (this->length - 1 - index) );
	}
}


int _devkit_list_cmp(const void *a, const void*b) {
	return memcmp(a,b, sizeof(size_t));
}

void dkt_list_nremove( 
		void *restrict dest, 
		DktList *l, 
		size_t nitems, 
		const size_t *restrict indices) 
{
#ifdef DEVKIT_DEBUG
	assert( l && indices );
#endif

#ifdef DEVKIT_INTERFACING
	size_t *sorted = calloc(nitems, sizeof(size_t));
#else
	size_t sorted[nitems];
#endif
	memcpy( sorted, indices, sizeof(size_t)*nitems);
	qsort( sorted, nitems, sizeof(size_t), _devkit_list_cmp);

	for (size_t item = 0; item < nitems; item++) {
		size_t index = indices[item] - item;
		if (dest) memcpy( dest + item*l->typesize, l->items + index*l->typesize, l->typesize);
		
		// If the item isn't last, every following item must be shifted backwards.
		if ( index != --l->length) {
			void *_dest = l->items + index*l->typesize, 
				 *src = l->items + (index+1)*l->typesize;
			memmove( _dest, src, l->typesize * (l->length - index));
		}
	}
#ifdef DEVKIT_INTERFACING
	free(sorted);
#endif
}

bool dkt_list_concat( DktList *restrict l, DktViewType *restrict v) {
	DktView *other = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert(l && l->items);
	assert(other && other->items);
#endif

	// Exit if sizes are different
	if ( l->typesize != other->typesize) return false;
	// Index of concatenation
	size_t concat_pos = l->length * l->typesize;
	l->length += other->length;

	if ( l->length > l->capacity) dkt_list_expand(l, l->length);
	// Copy items
	memcpy( l->items + concat_pos, other->items, other->length*l->typesize);

	return true;
}


void dkt_list_expand( DktList *l, size_t new_capacity) {
#ifdef DEVKIT_DEBUG
	assert( l );
#endif
	size_t prev_size = l->capacity*l->typesize;

	char *new_items = calloc( new_capacity, l->typesize);
#ifdef DEVKIT_DEBUG
	assert(new_items);
#endif
	memcpy( new_items, l->items, prev_size);
	free(l->items);
	l->items = new_items;
	l->capacity = new_capacity;
}

void dkt_list_trim( DktList *l) {
#ifndef DEVKIT_DEBUG
	assert( l );
#endif

	if (l->capacity == l->length) return;
	
	char *trim = malloc( l->length * l->typesize);
#ifdef DEVKIT_DEBUG
	assert(trim);
#endif
	memcpy( trim, l->items, l->length*l->typesize);
	free(l->items);

	l->items = trim;
	l->capacity = l->length;
}
#endif


/* ARRAY IMPLEMENTATION */

#ifdef DEVKIT_ARRAY_IMPLEMENTATION

DktArray _dkt_arr_new( size_t typesize, size_t length) {
	void *items = calloc(length, typesize);
	memset(items, 0, length * typesize);
	return (DktArray) {
		.items = items,
		.length = length,
		.typesize = typesize
	};
}

extern DktArray dkt_arr_copy( DktViewType *v) {
	DktView *a = (DktView*) v;
#ifdef DEVKIT_DEBUG
	assert( a && a->items );
#endif
	void *items = calloc( a->length, a->typesize);
	memcpy( items, a->items, a->typesize*a->length);
	return (DktArray) {
		.items = items,
		.typesize = a->typesize,
		.length = a->length
	};
}

void dkt_arr_init_values( DktArray *a, const void *const values) {
#ifdef DEVKIT_DEBUG
	assert(a && a->items);
	assert(values);
#endif
	memcpy(a->items, values, a->typesize*a->length);
}


void dkt_arr_init( DktArray *a, const size_t length, const void *const values) {
#ifdef DEVKIT_DEBUG
	assert(a && a->items);
	assert(values);
	assert(length != 0);
#endif
	a->length = length;
	memcpy(a->items, values, a->typesize*length);
}


DktArray dkt_arr_concat( DktViewType *v, DktViewType *w) {
	DktArray
		*a = (DktArray*) v,
		*other = (DktArray*) w;
#ifdef DEVKIT_DEBUG
	assert( a->typesize == other->typesize);
	assert( a && a->items );
	assert( other && other->items );
#endif
	size_t newlen = a->length + other->length;

	void *concat = calloc( newlen, a->typesize);
	memcpy( concat, a->items, a->length*a->typesize);
	memcpy( concat + a->length*a->typesize, other->items, other->length*a->typesize);

	return (DktArray) {
		.items = concat,
		.length = newlen,
		.typesize = a->typesize
	};
}


void dkt_arr_free( DktArray *a) {
	a->length = 0, a->typesize = 0;
	free(a->items);
}

#endif


/* POINTERS IMPLEMENTATION */

#if defined(DEVKIT_POINTERS_IMPLEMENTATION) && !defined(DEVKIT_INTERFACING)

/* Returns true if 'array' contains 'value' */
extern bool _dkt_contains( 
		const void *a, 
		size_t len, 
		size_t typesize, 
		const void *value) 
{
	for ( size_t idx = 0; idx < len; idx++) {
		if ( memcmp( a + idx*typesize, value, typesize) == 0)
			return true;
	}
	return false;
}


/* Creates an iterable object associated with the 'array' of 'length' items of 'typesize' */
DktIterable _dkt_as_iterable( void* a, size_t length, size_t typesize) {
#ifdef DEVKIT_DEBUG
	assert( a);
#endif
	return (DktIterable) { 
		.typesize=typesize, 
		.length=length, 
		.items=a
	};
}

/* Gives a set of numbers from 'start' to 'end' - 1 */
extern void* _dkt_range( long start, long end, bool islong) {
#ifdef DEVKIT_DEBUG
	assert(start != end);
#endif
	int sign = (start < end) ? 1 : -1;
	long delta = labs(end - start);
	if (islong) {
		long *dest = calloc( delta, sizeof(long));
		for (long l = 0; l != delta; l++, start += sign) {
			dest[l] = start;
		}
		return dest;
	}
	else {
		int *dest = calloc( delta, sizeof(int));
		for (long l = 0; l != delta; l++, start += sign) {
			dest[l] = (int) start;
		}
		return dest;
	}
}


/* Returns a uniform dkt_range of 'steps' values between 'start' and 'end'.
 * NOTE: steps must be larger or equal than 2 */
extern void* _dkt_linspace( double start, double end, size_t steps, bool isfloat) {
#ifdef DEVKIT_DEBUG
	assert( steps >= 2);
#endif

	if (isfloat) {
		float delta = (end - start) / (steps - 1);
		float *values = calloc( end - start, sizeof(float));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
	else {
		double delta = (end - start) / (steps - 1);
		double *values = calloc( end - start, sizeof(double));
		for ( size_t step = 0; step < steps; step++) values[step] = start + delta*step;
		return values;
	}
}


extern void _dkt_free_all( void **ptrs) {
	for (int i = 0; ptrs[i]; ++i)
		free(ptrs[i]);
}

#endif



/* MATH IMPLEMENTATION */

#ifdef DEVKIT_MATH_IMPLEMENTATION

#ifndef DEVKIT_INTERFACING
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


DktVec2 dkt_vec2_new( double x, double y) {
	return (DktVec2) { x, y};
}
DktVec2 dkt_vec2_sum( DktVec2 v, DktVec2 w) {
	return (DktVec2) { v.x + w.x, v.y + w.y};
}
DktVec2 dkt_vec2_sub( DktVec2 v, DktVec2 w) {
	return (DktVec2) { v.x - w.x, v.y - w.y};
}
DktVec2 dkt_vec2_neg( DktVec2 v) {
	return (DktVec2) { -v.x, -v.y};
}
DktVec2 dkt_vec2_scale( DktVec2 v, double scale) {
	return (DktVec2) { v.x * scale, v.y * scale};
}
double dkt_vec2_mod( DktVec2 v) {
	return sqrt( pow(v.x,2) + pow(v.y,2));
}
DktVector dkt_vec2_tovec( DktVec2 v) {
	double *items = calloc(2, sizeof(double));
	return (DktVector) {
		.items = items,
		.length = 2
	};
}

extern DktMatrix dkt_vec2_tomat( DktVec2 v) {
	DktMatrix m = dkt_mat_new(1, 2);
	double vals[] = {v.x, v.y};
	dkt_mat_init_values(m, vals);
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
	double vals[] = {v.x, v.y, v.z};
	dkt_mat_init_values(m, vals);
	return m;
}


DktVector dkt_vec_new( const size_t length) {
	return (DktVector) {
		.items = calloc( length, sizeof(double)),
		.length = length
	};
}

inline void dkt_vec_init( DktVector v, const size_t length, double values[]) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(values);
	assert(length != 0);
#endif
	memcpy((size_t*)&v.length, &length, sizeof(size_t));
	memcpy(v.items, values, length*sizeof(double));
}

inline void dkt_vec_init_values ( DktVector v, double values[]) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(values);
#endif
	memcpy(v.items, values, v.length*sizeof(double));
}


inline void dkt_vec_free( DktVector *v) {
#ifdef DEVKIT_DEBUG
	assert(v && v->items);
#endif
	free( v->items);
	memset(v, 0, sizeof(*v));
}


extern DktMatrix dkt_vec_asmat( DktVector v) {
	return (DktMatrix) {
		.length = v.length,
		.items = v.items,
		.rows = v.length,
		.columns = 1
	};
}

extern DktVector dkt_vec_copy( const DktVector v) {
	double *copy = calloc(v.length, sizeof(double));
	memcpy(copy, v.items, v.length * sizeof(double));
	return (DktVector) {
		.items = copy,
		.length = v.length
	};
}


extern DktView dkt_vec_view ( const DktVector v) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
#endif
	return (DktView) {
		.items = v.items,
		.length = v.length,
		.typesize = sizeof(double)
	};
}

DktView	dkt_vec_view_of( const DktVector v, const size_t start, const size_t end) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(end >= start);
	assert(end - start < v.length);
#endif
	return (DktView) {
		.items = v.items + start,
		.length = end - start,
		.typesize = sizeof(double)
	};
}


double dkt_vec_get( const DktVector v, size_t index) {
	return v.items[index];
}

inline void dkt_vec_set( DktVector v, const double value, const size_t index) {
	v.items[index] = value;
}


bool dkt_vec_equals( const DktVector v, const DktVector other) {
	if (v.length != other.length) return false;
	for (size_t idx = 0; idx < v.length; idx++) {
		if ( v.items[idx] != other.items[idx]) 
			return false;
	}
	return true;
}


inline void dkt_vec_sum( DktVector v, const DktVector other) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(other.items);
	assert(v.length == other.length);
#endif
	for (size_t idx = 0; idx < v.length; idx++)
		v.items[idx] += other.items[idx];
}

inline void dkt_vec_sub( DktVector v, const DktVector other) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
	assert(other.items);
	assert(v.length == other.length);
#endif
	for (size_t idx = 0; idx < v.length; idx++)
		v.items[idx] -= other.items[idx];
}


inline void dkt_vec_scale( DktVector v, const double scalar) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
#endif
	for (size_t idx = 0; idx < v.length; idx++) {
		v.items[idx] *= scalar;
	}
}


bool dkt_vec_iszero( const DktVector v) {
#ifdef DEVKIT_DEBUG
	assert(v.items);
#endif
	for ( size_t idx = 0; idx < v.length; idx++) {
		if ( v.items[idx] != 0)
			return false;
	}
	return true;
}


DktMatrix dkt_mat_new( size_t columns, size_t rows) {
	return (DktMatrix) {
		.columns = columns,
		.rows = rows,
		.length = rows*columns,
		.items = calloc( rows*columns, sizeof(long))
	};
}


extern void dkt_mat_init (DktMatrix m, const size_t cols, const size_t rows, double values[]) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
	assert(cols != 0 && rows != 0);
	assert(values);
#endif
	m.length = cols*rows;
	m.columns = cols;
	m.rows = rows;
	memcpy(m.items, values, sizeof(double)*cols*rows);
}

extern void dkt_mat_init_values (DktMatrix m, double values[]) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
	assert(m.length != 0);
	assert(values);
#endif
	memcpy(m.items, values, sizeof(double)*m.length);
}


extern void dkt_mat_free( DktMatrix *m) {
#ifdef DEVKIT_DEBUG
	assert(m && m->items);
#endif
	free(m->items);
	memset(m, 0, sizeof(*m));
}

inline DktVector dkt_mat_asvec( DktMatrix m) {
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


DktView dkt_mat_view (DktMatrix m) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
#endif
	return (DktView) {
		.items = m.items,
		.length = m.length,
		.typesize = sizeof(double)
	};
}


inline double dkt_mat_get( DktMatrix m, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(m.items);
	assert(col < m.columns);
	assert(row < m.rows);
#endif
	return m.items[m.columns*row + col];
}


inline void dkt_mat_set( DktMatrix m, double value, size_t col, size_t row) {
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
#ifdef DEVKIT_DEBUG
	assert(m.rows == m.columns);
	assert(m.items);
#endif
	double *buffer = calloc(m.length, sizeof(double));
	memcpy( buffer, m.items, m.length*sizeof(double));

	for ( size_t col = 0; col < m.columns; col++)
	for ( size_t row = 0; row < m.columns; row++)
		m.items[m.rows*row + col] = buffer[m.rows*col + row];

	free(buffer);
}


inline void dkt_mat_sum( DktMatrix dest, DktMatrix mat) {
#ifdef DEVKIT_DEBUG
	assert(dest.items);
	assert(mat.items);
	assert(dest.rows == mat.rows && dest.columns == mat.columns);
#endif
	for (size_t i = 0; i < dest.length; ++i)
		dest.items[i] += mat.items[i];
}

inline void dkt_mat_sub( DktMatrix dest, DktMatrix mat) {
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
	for (size_t col = 0; col < result.columns; col++) {
		for (size_t row = 0; row < result.rows; row++) {
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
