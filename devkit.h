#ifndef _DEVKIT_H
#define _DEVKIT_H

// Remove once done editing
#define DEVKIT_IMPLEMENTATION
#define DEVKIT_STRIP_PREFIXES
#define DEVKIT_DEBUG

/* 
 * #######################
 * # SETTINGS FOR DEVKIT #
 * #######################
 */


// Enable support for custom iterables
#define DEVKIT_EXTRA_ITERABLES 0
// and add them below

#if DEVKIT_EXTRA_ITERABLES

/*
 * ###################
 * # EXTRA ITERABLES #
 * ###################
 */

/* DevkitIterable definition
	typedef struct devkit_iterable {
		void *items;
		size_t length;
		size_t typesize;
		size_t counter; <- ignore this (nothing changes if you touch it, so do not)
	} DevkitIterable;
*/

/* 
 * To make a structure iterable, enable DEVKIT_EXTRA_ITERABLES,
 * then make a function as such:
 *
 * DevkitIterable <func_name>( <struct_t> *<struct>)
 *
 * Then add below:
 * <struct_t> : <func_name>,
 * <struct_t2> : <func_name2>,
 *	.
 *	.
 * <struct_tN> : <func_nameN>
 * Use commas for multiple entries. Last entry must not have a comma
 */
#define _DEVKIT_ITERABLES \
	/* ADD YOUR ITERABLES HERE */

#else
#define _DEVKIT_ITERABLES default:nullptr
#endif

#if defined(__STDC__) && __STDC_VERSION__ < 202311L
#define nullptr NULL
#include <stdbool.h>
#endif


/* 
 * ################
 * # SETTINGS END #
 * ################
 */

// Anything below should not be touched



#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <stdarg.h>

#ifdef DEVKIT_IMPLEMENTATION

#define DEVKIT_LIST_IMPLEMENTATION
#define DEVKIT_MATH_IMPLEMENTATION
#define DEVKIT_ARRAY_IMPLEMENTATION
#define DEVKIT_STRING_IMPLEMENTATION

#define DEVKIT_POINTERS_IMPLEMENTATION

#endif

/* Something useful i guess */

typedef __compar_fn_t DevkitComparator;
#ifdef DEVKIT_STRIP_PREFIXES
typedef DevkitComparator Comparator;
#endif


/*
#########
# FILES #
#########
*/

/* Quality macros that stdio.h should have 
 * to define file opening modes so you don't have
 * to memorize the string values */

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
 * ############
 * # ITERABLE #
 * ############
 */

/* Definition */
/* Used for 'foreach' loops */

typedef struct devkit_iterable {
	void *items;
	size_t length;
	size_t typesize;
	size_t counter;
} DevkitIterable;


/*
 * ##########
 * # STRING #
 * ##########
 */

typedef struct {
	char *items;
	size_t length;
	bool on_heap;
} DevkitString;

#define DEVKIT_STR_SIZE(length) (sizeof(DevkitString) + sizeof(double)*(length))

#ifdef DEVKIT_STRIP_PREFIXES

#define STR_SIZE DEVKIT_STR_SIZE

#define str_slice	devkit_str_slice
#define str_reverse devkit_str_reverse
#define str		devkit_str
#define str_stack	devkit_str_stack
#define str_free	devkit_str_free

#endif

/* Declarations */

extern DevkitString* devkit_str( const char *text);
extern DevkitString devkit_str_stack( const char *text);
extern char* devkit_str_slice( const DevkitString *restrict s, size_t start, size_t end);
extern void devkit_str_reverse( DevkitString *s);
extern void devkit_str_free( DevkitString *s);

extern DevkitIterable devkit_string_asiterable( DevkitString *);



/*
 * ########
 * # LIST #
 * ########
 */

/* An approach to variable length arrays in C. */

typedef struct {
	union { size_t length, size; };
	size_t capacity;
	size_t typesize;
	void *items;
	bool on_heap;
} DevkitList;

DevkitIterable devkit_list_asiterable( DevkitList *);

#define DEVKIT_LIST_SIZE(T, capacity) (sizeof(DevkitList) + sizeof(T)*(capacity))

#ifdef DEVKIT_STRIP_PREFIXES

#define LIST_SIZE DEVKIT_LIST_SIZE

#define list	devkit_list
#define list_stack	devkit_list_stack

#define list_contains	devkit_list_contains
#define list_itemat	devkit_itemat
#define list_add	devkit_list_add
#define list_nadd	devkit_list_nadd
#define	list_insert	devkit_list_insert
#define list_ninsert	devkit_list_ninsert
#define list_remove		devkit_list_remove
#define	list_nremove	devkit_list_nremove
#define list_concat		devkit_list_concat
#define list_sort	devkit_list_sort
#define list_sliceinto	devkit_list_sliceinto
#define list_copyto	devkit_list_copyto
#define list_expand	devkit_list_expand
#define list_trim	devkit_list_trim
#define list_free	devkit_list_free

#endif


/* Allocates a new list on the heap */
extern DevkitList* _devkit_list( const size_t typesize, const size_t capacity);
#define devkit_list( type, capacity) _devkit_list( sizeof(type), (capacity))

/* Creates a new list whose struct is on the stack (not the items) */
extern DevkitList _devkit_list_stack( const size_t typesize, const size_t capacity);
#define devkit_list_stack( type, capacity) _devkit_list_stack( sizeof(type), (capacity))


/* Deallocates the items from memory and sets all list values to 0 */
extern void devkit_list_free( DevkitList *);

/* Gives a reference to the item at 'index' in 'list' */
extern void* devkit_list_itemat( const DevkitList *, const size_t index);

/* Set item at 'index' of 'list' to 'value', if index is in bounds.
 * 'value' is COPIED to the list */
extern void devkit_list_set( DevkitList *restrict l, size_t index, const void *restrict value);

/* Add 'nitems' items from 'values' to 'list' */
extern void devkit_list_nadd( DevkitList *restrict l, size_t nitems, void *const values);
#define devkit_list_add( l, var) devkit_list_nadd( (l), 1, (var))

/* Insert 'nitems' of 'values' in 'list' at 'index' */
extern void devkit_list_ninsert( DevkitList *l, size_t index, size_t nitems, void *values);
#define devkit_list_insert( l, index, var) devkit_list_ninsert( (l), (index), 1, (var))

/* Remove item at 'index' from 'list' and copy it to 'dest'.
 * If 'dest' is null, the value isn't copied */
extern void devkit_list_remove( void *dest, DevkitList *l, size_t index);

/* Remove 'nitems' items at 'indices' in 'list', copying them into 'dest' if not null */
extern void devkit_list_nremove( void *dest, DevkitList *l, const size_t nitems, const size_t *indices);

/* Checks is value is contained in list */
extern bool devkit_list_contains( const DevkitList *l, const void *const value);

/* Qsort adaptation for DevkitList. Sorts the list */
extern void devkit_list_sort( DevkitList *restrict l, DevkitComparator func);


/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
extern bool devkit_list_concat( DevkitList *restrict dest, const DevkitList *restrict src);

/* Copies a section of the items into a buffer */
extern void devkit_list_sliceinto( void *restrict dest, DevkitList *restrict l, const size_t start, const size_t end);

/* Copies list items to 'dest' buffer, ovverriding its content */
extern void devkit_list_copyto( void *restrict dest, DevkitList *l);


/* Allocate more space for 'list' to increase its capacity to 'new_capacity' */
extern void devkit_list_expand( DevkitList *l, size_t new_capacity);

/* Reduce list capacity to its length to free unneeded memory */
extern void devkit_list_trim( DevkitList *l);


/*
 * #########
 * # ARRAY #
 * #########
 */

typedef struct {
	size_t length;
	size_t typesize;
	void* items;
	bool on_heap;
} DevkitArray;

extern DevkitIterable devkit_arr_asiterable( DevkitArray *);

#define DEVKIT_ARR_SIZE(T, capacity) (sizeof(DevkitArray) + sizeof(T)*(capacity))

#ifdef DEVKIT_STRIP_PREFIXES

#define ARR_SIZE DEVKIT_ARR_SIZE

#define arr	devkit_arr
#define arr_stack	devkit_arr_stack

#define arr_get	devkit_arr_get
#define arr_copyto	devkit_arr_copyto
#define arr_sliceinto	devkit_arr_sliceinto
#define arr_set		devkit_arr_set
#define arr_sort	devkit_arr_sort
#define arr_concat	devkit_arr_concat
#define arr_free	devkit_arr_free

#endif


/* Allocates a new DevkitArray on the heap */
extern DevkitArray* _devkit_arr( size_t typesize, size_t length);
#define devkit_arr( type, length) _devkit_arr( sizeof(type), (length))

/* Allocates a new DevkitArray on the stack (with items in heap) */
extern DevkitArray _devkit_arr_stack( size_t typesize, size_t length);
#define devkit_arr_stack( type, length) _devkit_arr_stack( sizeof(type), (length))


/* Gets a reference to the item at 'index' in 'arr' */
extern void* devkit_arr_get( DevkitArray *a, size_t index);

/* Sets 'arr' item at 'index' to 'value' */
extern void devkit_arr_set( DevkitArray *restrict a, size_t index, void *restrict value);

/* Copy 'arr' items to buffer 'dest' */
extern void devkit_arr_copyto( void *restrict dest, DevkitArray *restrict a);

/* Copy 'arr' items from 'start' to 'end' into buffer 'dest' */
extern void devkit_arr_sliceinto( void *restrict dest, DevkitArray *restrict a, const size_t start, const size_t end);

/* Concatenate 'arr' and 'other', copying items into buffer 'dest' */
extern void devkit_arr_concat( void *restrict dest, DevkitArray *a, DevkitArray *other);

/* Qsort adaptation for DevkitArray */
extern void devkit_arr_sort( DevkitArray *a, DevkitComparator func);

/* Deallocates item buffer of 'arr' if allocated on heap using the standard library,
 * sets all arr values to 0 */
extern void devkit_arr_free( DevkitArray *a);

/*
 * ############
 * # POINTERS #
 * ############
 */

#ifdef DEVKIT_STRIP_PREFIXES

#define asiterable	devkit_asiterable
#define linspace	devkit_linspace
#define flinspace	devkit_flinspace
#define range	devkit_range
#define lrange	devkit_lrange
#define contains	devkit_contains
#define unref	devkit_unref
#define ref	devkit_ref

#endif


#define devkit_asiterable( a, length, typesize) _devkit_asiterable( (a), (length), typesize)	
#define devkit_linspace( start, end, steps) _devkit_linspace( (start), (end), (steps), false)
#define devkit_flinspace( start, end, steps) _devkit_linspace( (start), (end), (steps), true)
#define devkit_range( start, end) _devkit_range( (start), (end), false)
#define devkit_lrange( start, end) _devkit_range( (start), (end), true)

/* Checks if an arr contains a certain value */
#define devkit_contains( a, len, var) _devkit_contains( (a), (len), sizeof(*(a)), &(var))
/* Unreferences to pointer after casting */
#define devkit_unref( type, value) (*(type*)(value))
#define devkit_ref( type, value) (type[]){(value)}


/*
 * ########
 * # MATH #
 * ########
 */

typedef struct DevkitVector {
	double *items;
	size_t length;
	bool on_heap;
} DevkitVector;

#define DEVKIT_VECTOR_SIZE(length) (sizeof(DevkitVector) + sizeof(double)*(length))

/* Note that DevkitMatrix can be converted to vec, as they have the same
 * variables and structure, so DevkitMatrix can use DevkitVector functions if cast to DevkitVector */

typedef struct DevkitMatrix {
	double *items;
	size_t length;
	size_t columns, rows;
	bool on_heap;
} DevkitMatrix;

#define DEVKIT_MAT_SIZE(length) (sizeof(DevkitMatrix) + sizeof(double)*(length))

extern DevkitIterable devkit_vec_asiterable( DevkitVector *);

/* Returns an DevkitIterable that has the mat iterated ROW BY ROW */
extern DevkitIterable devkit_mat_asiterable( DevkitMatrix *);


#ifdef DEVKIT_STRIP_PREFIXES

#define MAT_SIZE DEVKIT_MAT_SIZE
#define VEC_SIZE DEVKIT_VECTOR_SIZE

#define vec	devkit_vec
#define vec_stack	devkit_vec_stack
#define vec_free	devkit_vec_free
#define vec_copyto	devkit_vec_copyto
#define vec_sum		devkit_vec_sum
#define vec_equals	devkit_vec_equals
#define vec_scale	devkit_vec_scale
#define vec_iszero	devkit_vec_iszero
#define vec_nonzero	devkit_vec_nonzero
#define vec_set	devkit_vec_set
#define vec_get	devkit_vec_get

#define mat	devkit_mat
#define mat_stack	devkit_mat_stack
#define mat_free	devkit_mat_free
#define mat_copyto	devkit_mat_copyto
#define mat_get	devkit_mat_get
#define mat_set 	devkit_mat_set
#define mat_equals	devkit_mat_equals
#define mat_transpose	devkit_mat_transpose
#define mat_sum	devkit_mat_sum
#define mat_iszero	devkit_mat_iszero
#define mat_nonzero	devkit_mat_nonzero

#endif


extern DevkitVector* devkit_vec( size_t length);
extern DevkitVector devkit_vec_stack( size_t length);

extern void devkit_vector_of( DevkitVector *, double values[]);
extern void devkit_vec_free( DevkitVector *);

extern double devkit_vec_get( DevkitVector *, size_t index);
extern void devkit_vec_set( DevkitVector *, double value, size_t index);
extern void devkit_vec_copyto( void *restrict dest, DevkitVector *restrict );
extern bool devkit_vec_equals( const DevkitVector *, const DevkitVector *other);
extern void devkit_vec_sum( DevkitVector *this, size_t nvecs, DevkitVector *vecs);
extern void devkit_vec_scale( DevkitVector *, double scalar);
extern bool devkit_vec_iszero( DevkitVector *);
#define devkit_vec_nonzero( v) ( assert(!devkit_vec_iszero(&v)), v)


extern DevkitMatrix* devkit_mat( size_t columns, size_t rows);
extern DevkitMatrix devkit_mat_stack( size_t columns, size_t rows);

extern void devkit_matrix_of( DevkitMatrix *, double values[]);
extern void devkit_mat_free( DevkitMatrix *);

extern double* devkit_mat_get( DevkitMatrix *, size_t col, size_t row);
extern void devkit_mat_set( DevkitMatrix *, double value, size_t col, size_t row);
extern void devkit_mat_copyto( void *restrict dest, DevkitMatrix *restrict );
extern void devkit_matrix_multiply( DevkitMatrix *restrict dest, DevkitMatrix *restrict A, DevkitMatrix *restrict B);
extern bool devkit_mat_equals( const DevkitMatrix *A, const DevkitMatrix *B);
extern void devkit_mat_transpose( DevkitMatrix *);
extern void devkit_mat_sum( DevkitMatrix *this, size_t nmats, DevkitMatrix *mats);
extern bool devkit_mat_iszero( const DevkitMatrix *);
#define devkit_mat_nonzero( m) ( assert(!devkit_mat_iszero(&m)), m)




/* 
 * ###################################################################
 * # Loop pool implementation needed for nested 'enhanced for' loops #
 * ###################################################################
 */

typedef struct {
	DevkitIterable **loops;
	size_t length;
	size_t capacity;
} DEVKIT_LOOP_POOL;

DEVKIT_LOOP_POOL _DEVKIT_POOL = (DEVKIT_LOOP_POOL) {.loops = nullptr};

extern void _devkit_loop_pool_destroy() {
	free( _DEVKIT_POOL.loops);
}

extern void _devkit_loop_pool_init() {
	if (!_DEVKIT_POOL.loops) {
		_DEVKIT_POOL = (DEVKIT_LOOP_POOL) {
			.loops = calloc( 4, sizeof(DevkitIterable*)),
			.length = 0,
			.capacity = 4
		};
		atexit( _devkit_loop_pool_destroy);
	}
}

#define _devkit_loop_current (_DEVKIT_POOL.loops[_DEVKIT_POOL.length - 1])

extern inline void __devkit_expand_loop_pool( size_t increment) {
	_DEVKIT_POOL.capacity += increment;
	_DEVKIT_POOL.loops = realloc( _DEVKIT_POOL.loops, _DEVKIT_POOL.capacity);
}

extern inline void _devkit_loop_new( DevkitIterable *iter) {
	if ( _DEVKIT_POOL.length + 1 >= _DEVKIT_POOL.capacity)
		__devkit_expand_loop_pool( _DEVKIT_POOL.capacity);

	_DEVKIT_POOL.loops[_DEVKIT_POOL.length++] = iter;
}

#define _devkit_loop_close \
	if ( _DEVKIT_POOL.length != 0) { _DEVKIT_POOL.loops[--_DEVKIT_POOL.length] = nullptr; }


/*
 * ################
 * # ENHANCED FOR #
 * ################
 */

/* Bypass for DevkitIterable in generic selection of _devkit_iterable. With this,
 * arrays can be converted to iterables and used in foreach loops.
 * It is recommended not to use stack arrays or in general stack allocations
 * as it may segfault */
extern inline DevkitIterable devkit_dummy_asiterable(DevkitIterable *iter) {
	return *iter;
}

/* "Cast" to _devkit_iterable.
 * Works with Arrays, Lists, and other structures defined in devkit that have
 * a <...>_asiterable function.
 * Other structures can be compatible with 'foreach' if an 'asiterable'-like function
 * is defined for them in the section EXTRA ITERABLES and the start of this header file */

#define _devkit_iterable( structure) _Generic( (structure), \
		_DEVKIT_ITERABLES, \
		DevkitArray: devkit_arr_asiterable, \
		DevkitList: devkit_list_asiterable, \
		DevkitVector: devkit_vec_asiterable, \
		DevkitMatrix: devkit_mat_asiterable, \
		DevkitIterable: devkit_dummy_asiterable, \
		DevkitString: devkit_string_asiterable \
		)( &(structure))


/* 'foreach' macros for 'enhanced for' loops.
 * It is recommended not to use this with items allocated on the stack
 * as it may segfault. Most structures that have items allocated on the
 * heap should work just fine */

#define foreach( type, var, iter, ...) \
	foreach_in( type, var, iter, 0, -1, __VA_ARGS__)
	  
#define foreach_in( type, var, iter, start, end, ...) { \
	_devkit_loop_pool_init(); \
	_devkit_loop_new( (DevkitIterable[]){ _devkit_iterable(iter)} ); \
	if (end >= 0) _devkit_loop_current->length = end; \
	type var; \
	for (_devkit_loop_current->counter = (start>=0) ? start : 0; _devkit_loop_current->counter < _devkit_loop_current->length; _devkit_loop_current->counter++) { \
		var = ((type*) _devkit_loop_current->items)[_devkit_loop_current->counter]; \
		__VA_ARGS__; \
		memcpy( ((type*) _devkit_loop_current->items)+_devkit_loop_current->counter, &var, _devkit_loop_current->typesize); \
	} \
	_devkit_loop_close; \
}


/* Prefix stripping */

#ifdef DEVKIT_STRIP_PREFIXES
typedef DevkitArray Array;
typedef DevkitString String;
typedef DevkitList List;
typedef DevkitVector Vector;
typedef DevkitMatrix Matrix;
#endif

/* 
 * ###################
 * # IMPLEMENTATIONS #
 * ###################
 */


/* STRING IMPLEMENTATION */

//#define DEVKIT_STRING_IMPLEMENTATION
#ifdef DEVKIT_STRING_IMPLEMENTATION

DevkitIterable devkit_string_asiterable( DevkitString *s) {
	return (DevkitIterable) {
		.typesize=1,
		.items=s->items,
		.length=s->length,
	};
}

DevkitString* devkit_str( const char *text) {
	size_t length = strlen(text);
	DevkitString *this = malloc( sizeof(*this) + length);
	this->length = length;
	this->on_heap = true;
	this->items = (char*)(this + 1);
	memcpy( this->items, text, length);
	return this;
}

DevkitString devkit_str_stack( const char *text) {
	size_t length = strlen(text);
	char *items = malloc( length);
	memcpy( items, text, length);
	return (DevkitString) {
		.length = length,
		.items = items,
		.on_heap = false
	};
}


/* Substring of DevkitString 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 */
char* devkit_str_slice( const DevkitString *restrict s, size_t start, size_t end) {
	assert( end > start);

	size_t substr_len = end - start;
	char *temp = malloc(substr_len + 1);
	strncpy( temp, s->items + start, substr_len);
	temp[substr_len] = '\0';
	return temp;
}

/* Returns DevkitString 's' reversed */
void devkit_str_reverse( DevkitString *s) {
	char buffer[s->length];
	memcpy( buffer, s->items, s->length);
	for (size_t idx = 0; idx < s->length; idx++) {
		s->items[idx] = buffer[s->length - idx - 1];
	}
}

extern void devkit_str_free( DevkitString *s) {
	if (s->on_heap) free(s);
	else {
		free(s->items);
		s->length = 0;
	}
}

#endif


/* LIST IMPLEMENTATION */

//#define DEVKIT_LIST_IMPLEMENTATION
#ifdef DEVKIT_LIST_IMPLEMENTATION

DevkitIterable devkit_list_asiterable( DevkitList *list) {
#ifdef DEVKIT_DEBUG
	assert( list != nullptr);
#endif
	return (DevkitIterable) { 
		.typesize=list->typesize, 
		.length=list->length, 
		.items=list->items
	};
}

DevkitList* _devkit_list( size_t typesize, size_t capacity) {
	DevkitList *this = malloc( sizeof(*this) + typesize * capacity);
	this->items = this + 1;
	this->typesize = typesize;
	this->capacity = capacity;
	this->length = 0;
	this->on_heap = true;
	return this;
}

DevkitList _devkit_list_stack( size_t typesize, size_t capacity) {
	return (DevkitList) {
		.typesize = typesize,
		.length = 0,
		.capacity = capacity,
		.items = calloc(capacity,typesize),
		.on_heap = false
	};
}


void devkit_list_free( DevkitList *this) {
#ifdef DEVKIT_DEBUG
	assert(this);
#endif
	if (this->on_heap) free(this);
	else {
		this->length = 0, this->capacity = 0, this->typesize = 0;
		free( this->items);
	}
}


void* devkit_list_itemat( const DevkitList *this, const size_t index) {
	return this->items + index*this->typesize;
}

void devkit_list_set( DevkitList *restrict this, size_t index, const void *restrict value) {
#ifdef DEVKIT_DEBUG
	assert( this && value && index < this->length);
#endif

	memcpy( this->items + index*this->typesize, value, this->typesize);
}



void devkit_list_nadd( DevkitList *restrict this, size_t nitems, void *values) {
#ifdef DEVKIT_DEBUG
	assert( this && values );
#endif

	size_t ptr = this->length; // Needed later
	this->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( this->length > this->capacity) devkit_list_expand( this, this->length);
	// Copy values in pointers
	memcpy( this->items + ptr*this->typesize, values, nitems*this->typesize);
}


void devkit_list_ninsert( DevkitList *this, size_t index, size_t nitems, void *values) {
#ifdef DEVKIT_DEBUG
	assert( this && values );
#endif

	this->length += nitems;

	// Allocate more memory if needed
	if (this->length > this->capacity) devkit_list_expand( this, this->length);
	// Move following items forward, if there are any
	if (index < this->length) {
		memmove( this->items + (index+nitems)*this->typesize, this->items + index*this->typesize, this->typesize*(this->length-nitems - index) );
	}
	// Insert values at index
	memcpy( this->items + (index)*this->typesize, values, nitems*this->typesize);
}


void devkit_list_remove( void *dest, DevkitList *this, size_t index) {
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

void devkit_list_nremove( void *dest, DevkitList *list, const size_t nitems, const size_t *indices) {
#ifdef DEVKIT_DEBUG
	assert( list && indices );
#endif

	size_t sorted[nitems];
	memcpy( sorted, indices, sizeof(size_t)*nitems);
	qsort( sorted, nitems, sizeof(size_t), _devkit_list_cmp);

	for (size_t item = 0; item < nitems; item++) {
		size_t index = indices[item] - item;
		if (dest) memcpy( dest + item*list->typesize, list->items + index*list->typesize, list->typesize);
		
		// If the item isn't last, every following item must be shifted backwards.
		if ( index != --list->length) {
			void *_dest = list->items + index*list->typesize, 
				 *src = list->items + (index+1)*list->typesize;
			memmove( _dest, src, list->typesize * (list->length - index));
		}
	}
}

bool devkit_list_contains( const DevkitList *list, const void *const value) {
	for (size_t idx = 0; idx < list->length; idx++) {
		if ( memcmp(list->items + idx*list->typesize, value, list->typesize) == 0)
			return true;
	}
	return false;
}

void devkit_list_sort( DevkitList *restrict list, DevkitComparator func) {
#ifdef DEVKIT_DEBUG
	assert( list && func);
#endif

	qsort( list->items, list->length, list->typesize, func);
}


bool devkit_list_concat( DevkitList *restrict list, const DevkitList *restrict other) {
#ifdef DEVKIT_DEBUG
	assert(list && other);
#endif

	// Exit if sizes are different
	if ( list->typesize != other->typesize) return false;
	// Index of concatenation
	size_t concat_pos = list->length * list->typesize;
	list->length += other->length;

	if ( list->length > list->capacity) devkit_list_expand(list, list->length);
	// Copy items
	memcpy( list->items + concat_pos, other->items, other->length*list->typesize);

	return true;
}

extern void devkit_list_sliceinto( void *restrict dest, DevkitList *list, const size_t start, const size_t end) {
	const size_t delta = end - start;

#ifdef DEVKIT_DEBUG
	assert(list);
	assert( delta >= 0 && delta < list->length);
#endif

	// Copy data to slice
	void *restrict src = list->items + start * list->typesize;
	memcpy( dest, src, delta*list->typesize);
}

void devkit_list_copyto( void *restrict dest, DevkitList *list) {
	memcpy( dest, list->items, list->length*list->typesize);
}


void devkit_list_expand( DevkitList *list, size_t new_capacity) {
#ifdef DEVKIT_DEBUG
	assert( list );
#endif

	size_t prev_size = list->capacity*list->typesize;
	char buf[ prev_size];
	memcpy( buf, list->items, prev_size);

	void *new_items = calloc( new_capacity, list->typesize);
#ifdef DEVKIT_DEBUG
	assert(new_items);
#endif
	memcpy( new_items, buf, prev_size);
	list->items = new_items;
	list->capacity = new_capacity;
}

void devkit_list_trim( DevkitList *list) {
#ifndef DEVKIT_DEBUG
	assert( list );
#endif

	if (list->capacity == list->length) return;
	
	char buf[list->length];
	memcpy( buf, list->items, list->length*list->typesize);
	devkit_list_free( list->items);

	void *trim = malloc( list->length * list->typesize);
#ifdef DEVKIT_DEBUG
	assert(trim);
#endif
	memcpy( trim, buf, list->length*list->typesize);

	list->items = trim;
	list->capacity = list->length;
}
#endif


/* ARRAY IMPLEMENTATION */

//#define DEVKIT_ARRAY_IMPLEMENTATION
#ifdef DEVKIT_ARRAY_IMPLEMENTATION

DevkitIterable devkit_arr_asiterable( DevkitArray *a) {
	assert( a != nullptr);
	return (DevkitIterable) { 
		.typesize=a->typesize, 
		.length=a->length, 
		.items=a->items
	};
}

DevkitArray* _devkit_arr( size_t typesize, size_t length) {
	DevkitArray *this = malloc( sizeof(*this) + typesize*length);
	this->typesize = typesize;
	this->length = length;
	this->items = this + 1;
	this->on_heap = true;
	return this;
}

DevkitArray _devkit_arr_stack( size_t typesize, size_t length) {
	void *items = calloc( length, typesize);
#ifdef DEVKIT_DEBUG
	assert(items);
#endif
	return (DevkitArray) { 
		.typesize=typesize, 
		.length=length, 
		.items=items,
		.on_heap = false
	};
}


void* devkit_arr_get( DevkitArray *a, size_t index) {
#ifdef DEVKIT_DEBUG
	assert(a);
	assert( index < a->length);
#endif
	return a->items + index*a->typesize;
}


void devkit_arr_copyto( void *dest, DevkitArray *a) {
#ifdef DEVKIT_DEBUG
	assert( dest && a );
#endif
	memcpy( dest, a->items, a->length*a->typesize);
}


void devkit_arr_set( DevkitArray *a, size_t index, void* value) {
#ifdef DEVKIT_DEBUG
	assert(a);
	assert(index < a->length);
#endif
	memcpy( a->items + index*a->typesize, value, a->typesize);
}

void devkit_arr_sort( DevkitArray *a, DevkitComparator func) {
#ifdef DEVKIT_DEBUG
	assert( a != nullptr);
#endif
	qsort( a, a->length, a->typesize, func);
}



void devkit_arr_sliceinto( void *restrict dest, DevkitArray *restrict a, size_t start, size_t end) {
	size_t delta = end - start;

#ifdef DEVKIT_DEBUG
	assert( a );
	assert( delta >= 0 && delta < a->length);
#endif
	void *slice = dest;
	void *src = a->items + start*a->typesize;
	memcpy( slice, src, a->typesize*delta);
}


/* Concatenates 'arr' and 'other' and copies the buffer into 'dest'.
 * NOTE: arrays must be of same type */
void devkit_arr_concat( void *restrict dest, DevkitArray *a, DevkitArray *other) {
#ifdef DEVKIT_DEBUG
	assert( a->typesize == other->typesize);
	assert( a && other );
#endif

	size_t newlen = a->length + other->length;

	void *concat = dest;
	memmove( concat, a->items, a->typesize*a->length);
	memmove( concat + a->typesize*a->length, other->items, a->typesize*other->length);
}


void devkit_arr_free( DevkitArray *a) {
	if (a->on_heap) free(a);
	else {
		a->length = 0, a->typesize = 0;
		free(a->items);
	}
}

#endif


/* POINTERS IMPLEMENTATION */

//#define DEVKIT_POINTERS_IMPLEMENTATION
#ifdef DEVKIT_POINTERS_IMPLEMENTATION

/* Returns true if 'arr' contains 'value' */
extern bool _devkit_contains( 
		const void *const a, 
		const size_t len, 
		const size_t typesize, 
		const void *value) 
{
	for ( size_t idx = 0; idx < len; idx++) {
		if ( memcmp( a + idx*typesize, value, typesize) == 0)
			return true;
	}
	return false;
}


/* Creates an iterable object associated with the 'arr' of 'length' items of 'typesize' */
extern inline DevkitIterable _devkit_asiterable( void* a, size_t length, size_t typesize) {
#ifdef DEVKIT_DEBUG
	assert( a);
#endif
	return (DevkitIterable) { 
		.typesize=typesize, 
		.length=length, 
		.items=a
	};
}

/* Gives a set of numbers from 'start' to 'end' - 1 */
extern void* _devkit_range( long start, long end, bool islong) {
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


/* Returns a uniform devkit_range of 'steps' values between 'start' and 'end'.
 * NOTE: steps must be larger or equal than 2 */
extern void* _devkit_linspace( double start, double end, size_t steps, bool isfloat) {
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
#endif


/* MATH IMPLEMENTATION */

//#define DEVKIT_MATH_IMPLEMENTATION
#ifdef DEVKIT_MATH_IMPLEMENTATION

extern DevkitIterable devkit_vec_asiterable( DevkitVector *this) {
	return (DevkitIterable) {
		.typesize=sizeof(double),
		.length=this->length,
		.items=this->items
	};
}

DevkitVector* devkit_vec( size_t length) {
	DevkitVector *this = malloc( sizeof(*this) + length * sizeof(double));
	this->length = length;
	this->items = (double*)(this + 1);
	this->on_heap = true;

	return this;
}

DevkitVector devkit_vec_stack( size_t length) {
	return (DevkitVector) {
		.items = calloc( length, sizeof(double)),
		.length = length,
		.on_heap = false
	};
}

extern void devkit_vector_of( DevkitVector *v, double values[]) {
	for (size_t i = 0; i < v->length; ++i) {
		v->items[i] = values[i];
	}
}

extern void devkit_vec_free( DevkitVector *v) {
	if (v->on_heap) free(v);
	else {
		free( v->items);
		v->length = 0;
	}
}

extern double devkit_vec_get( DevkitVector *v, size_t index) {
	return v->items[index];
}
extern void devkit_vec_set( DevkitVector *v, double value, size_t index) {
	v->items[index] = value;
}


void devkit_vec_copyto( void *restrict dest, DevkitVector *restrict v) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(v && v->items);
#endif
	memcpy( dest, v->items, v->length*sizeof(double));
}



bool devkit_vec_equals( const DevkitVector *v, const DevkitVector *other) {
	if (v->length != other->length) return false;
	for (size_t idx = 0; idx < v->length; idx++) {
		if ( v->items[idx] != other->items[idx]) 
			return false;
	}
	return true;
}


void devkit_vec_sum( DevkitVector *v, size_t nvecs, DevkitVector *args) {
#ifdef DEVKIT_DEBUG
	assert(v);
	assert(args);
#endif
	for ( size_t idx = 0; idx < nvecs; idx++, args++) {
#ifdef DEVKIT_DEBUG
		assert( args->length == v->length);
#endif
		for (size_t idx = 0; idx < v->length; idx++)
			v->items[idx] += args->items[idx];
	}
}


void devkit_vec_scale( DevkitVector *v, double scalar) {
#ifdef DEVKIT_DEBUG
	assert(v);
#endif
	for (size_t idx = 0; idx < v->length; idx++) {
		v->items[idx] *= scalar;
	}
}


bool devkit_vec_iszero( DevkitVector *v) {
#ifdef DEVKIT_DEBUG
	assert(v);
#endif
	for ( size_t idx = 0; idx < v->length; idx++) {
		if ( v->items[idx] != 0)
			return false;
	}
	return true;
}


extern DevkitIterable devkit_mat_asiterable( DevkitMatrix *m) {
	return (DevkitIterable) {
		.items=m->items,
		.length=m->length,
		.typesize=sizeof(double)
	};
}

DevkitMatrix* devkit_mat( size_t columns, size_t rows) {
	DevkitMatrix *this = malloc( sizeof(*this) + columns*rows*sizeof(double));
	this->items = (double*)(this + 1);
	this->columns = columns;
	this->rows = rows;
	this->length = columns*rows;
	this->on_heap = true;
	return this;
}

DevkitMatrix devkit_mat_stack( size_t columns, size_t rows) {
	return (DevkitMatrix) {
		.columns = columns,
		.rows = rows,
		.length = rows*columns,
		.items = calloc( rows*columns, sizeof(long)),
		.on_heap = false
	};
}


extern void devkit_matrix_of( DevkitMatrix *m, double values[]) {
	for (size_t i = 0; i < m->length; ++i)
		m->items[i] = values[i];
}


extern void devkit_mat_free( DevkitMatrix *m) {
	if (m->on_heap) free(m);
	else {
		free(m->items);
		m->length = 0, m->columns = 0, m->rows = 0;
	}
}


void devkit_mat_copyto( void *restrict dest, DevkitMatrix *restrict m) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(m);
#endif
	size_t size = m->columns*m->rows*sizeof(double);
	memcpy( dest, m->items, size);
}


double* devkit_mat_get( DevkitMatrix *m, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(m);
	assert(col < m->columns);
	assert(row < m->rows);
#endif
	return &m->items[m->columns*row + col];
}


void devkit_mat_set( DevkitMatrix *m, double value, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(m);
	assert(col < m->columns);
	assert(row < m->rows);
#endif
	m->items[m->columns*row + col] = value;
}


bool devkit_mat_equals( const DevkitMatrix *A, const DevkitMatrix *B) {
	return ( A->rows == B->rows && A->columns == B->columns)
		? ( memcmp( A->items, B->items, A->length))
			? true
			: false
		: false;
}


void devkit_mat_transpose( DevkitMatrix *m) {
	double buffer[m->length];
	memcpy( buffer, m->items, m->length*sizeof(double));

	for ( size_t col = 0; col < m->columns; col++)
	for ( size_t row = 0; row < m->rows; row++)
		m->items[m->rows*row + col] = buffer[m->rows*col + row];
	
	double temp = m->rows;
	m->rows = m->columns;
	m->columns = temp;
}


void devkit_mat_sum( DevkitMatrix *dest, size_t nmats, DevkitMatrix *mats) {
	for (size_t m = 0; m < nmats; m++, mats++) {
		for (size_t idx = 0; idx < dest->length; idx++) {
			dest->items[idx] += mats->items[idx];
		}
	}
}


void devkit_matrix_multiply( DevkitMatrix *restrict dest, DevkitMatrix *restrict A, DevkitMatrix *restrict B) {
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


bool devkit_mat_iszero( const DevkitMatrix *m) {
	for ( size_t idx = 0; idx < m->length; idx++) {
		if ( m->items[idx] != 0)
			return false;
	}
	return true;
}
#endif



#endif
