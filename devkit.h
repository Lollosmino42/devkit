#ifndef _DEVKIT_H
#define _DEVKIT_H

/* 
 * #################################
 * # SETTINGS FOR DEVKIT ITERABLES #
 * #################################
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

#ifdef DEVKIT_IMPLEMENTATION

#define DEVKIT_LIST_IMPLEMENTATION
#define DEVKIT_MATH_IMPLEMENTATION
#define DEVKIT_ARRAY_IMPLEMENTATION
#define DEVKIT_STRING_IMPLEMENTATION

#define DEVKIT_POINTERS_IMPLEMENTATION

#endif

/* Something useful i guess */

typedef __compar_fn_t CompareFunc;

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
	bool heap_default;
} DevkitString;


#ifdef DEVKIT_STRIP_PREFIXES

#define string_slice devkit_string_slice
#define string_reverse devkit_string_reverse
#define string_new devkit_string_new

#endif

/* Declarations */

extern DevkitString* devkit_string_new( const char *text);
extern DevkitString devkit_string_new_stack( const char *text);
extern char* devkit_string_slice( const DevkitString *restrict s, size_t start, size_t end);
extern void devkit_string_reverse( DevkitString *s);

DevkitIterable devkit_string_asiterable( DevkitString *);



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
} DevkitList;

DevkitIterable devkit_list_asiterable( DevkitList *);

#ifdef DEVKIT_STRIP_PREFIXES

#define list_new	devkit_list_new
#define list_new_stack	devkit_list_new_stack

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
#define list_freeitems	devkit_list_freeitems

#endif


/* Allocates a new list on the heap */
extern DevkitList* _devkit_list_new( const size_t typesize, const size_t capacity);
#define devkit_list_new( type, capacity) _devkit_list_new( sizeof(type), (capacity))

/* Creates a new list whose struct is on the stack (not the items) */
extern DevkitList _devkit_list_new_stack( const size_t typesize, const size_t capacity);
#define devkit_list_new_stack( type, capacity) _devkit_list_new_stack( sizeof(type), (capacity))


/* Deallocates the items from memory if those items where allocated using stdlib,
 * sets all list values to 0 */
extern void devkit_list_freeitems( DevkitList *list);

/* Gives a reference to the item at 'index' in 'list' */
extern void* devkit_list_itemat( const DevkitList *list, const size_t index);

/* Set item at 'index' of 'list' to 'value', if index is in bounds.
 * 'value' is COPIED to the list */
extern void devkit_list_set( DevkitList *restrict list, size_t index, const void *restrict value);

/* Add 'nitems' items from 'values' to 'list' */
extern void devkit_list_nadd( DevkitList *restrict list, size_t nitems, void *const values);
#define devkit_list_add( list, var) devkit_list_nadd( (list), 1, (var))

/* Insert 'nitems' of 'values' in 'list' at 'index' */
extern void devkit_list_ninsert( DevkitList *list, size_t index, size_t nitems, void *values);
#define devkit_list_insert( list, index, var) devkit_list_ninsert( (list), (index), 1, (var))

/* Remove item at 'index' from 'list' and copy it to 'dest'.
 * If 'dest' is null, the value isn't copied */
extern void devkit_list_remove( void *dest, DevkitList *list, size_t index);

/* Remove 'nitems' items at 'indices' in 'list', copying them into 'dest' if not null */
extern void devkit_list_nremove( void *dest, DevkitList *list, const size_t nitems, const size_t *indices);

/* Checks is value is contained in list */
extern bool devkit_list_contains( const DevkitList *list, const void *const value);

/* Qsort adaptation for DevkitList. Sorts the list */
extern void devkit_list_sort( DevkitList *restrict list, CompareFunc func);


/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
extern bool devkit_list_concat( DevkitList *restrict dest, const DevkitList *restrict src);

/* Copies a section of the items into a buffer */
extern void devkit_list_sliceinto( void *restrict dest, DevkitList *restrict list, const size_t start, const size_t end);

/* Copies list items to 'dest' buffer, ovverriding its content */
extern void devkit_list_copyto( void *restrict dest, DevkitList *list);


/* Allocate more space for 'list' to increase its capacity to 'new_capacity' */
extern void devkit_list_expand( DevkitList *list, size_t new_capacity);

/* Reduce list capacity to its length to free unneeded memory */
extern void devkit_list_trim( DevkitList *list);


/*
 * #########
 * # ARRAY #
 * #########
 */

typedef struct {
	union { size_t length, size; };
	size_t typesize;
	void* items;
} DevkitArray;

DevkitIterable devkit_array_asiterable( DevkitArray *);


#ifdef DEVKIT_STRIP_PREFIXES

#define array_new	devkit_array_new
#define array_new_stack	devkit_array_new_stack

#define array_itemat	devkit_array_itemat
#define array_copyto	devkit_array_copyto
#define array_sliceinto	devkit_array_sliceinto
#define array_set	devkit_array_set
#define array_sort	devkit_array_sort
#define array_free	devkit_array_freeitems

#endif


/* Allocates a new DevkitArray on the heap */
extern DevkitArray* _devkit_array_new( size_t typesize, size_t length);
#define devkit_array_new( type, length) _devkit_array_new( sizeof(type), (length))

/* Allocates a new DevkitArray on the stack (with items in heap) */
extern DevkitArray _devkit_array_new_stack( size_t typesize, size_t length);
#define devkit_array_new_stack( type, length) _devkit_array_new_stack( sizeof(type), (length))


/* Gets a reference to the item at 'index' in 'array' */
extern void* devkit_array_itemat( DevkitArray *array, size_t index);

/* Sets 'array' item at 'index' to 'value' */
extern void devkit_array_set( DevkitArray *restrict array, size_t index, void *restrict value);

/* Copy 'array' items to buffer 'dest' */
extern void devkit_array_copyto( void *restrict dest, DevkitArray *restrict array);

/* Copy 'array' items from 'start' to 'end' into buffer 'dest' */
extern void devkit_array_sliceinto( void *restrict dest, DevkitArray *restrict array, const size_t start, const size_t end);

/* Concatenate 'array' and 'other', copying items into buffer 'dest' */
extern void devkit_array_concat( void *restrict dest, DevkitArray *array, DevkitArray *other);

/* Qsort adaptation for DevkitArray */
extern void devkit_array_sort( DevkitArray *array, CompareFunc func);

/* Deallocates item buffer of 'array' if allocated on heap using the standard library,
 * sets all array values to 0 */
extern void devkit_array_freeitems( DevkitArray *array);

/*
 * ############
 * # POINTERS #
 * ############
 */

#ifdef DEVKIT_STRIP_PREFIXES

#define asiterable devkit_asiterable
#define linspace devkit_linspace
#define flinspace devkit_flinspace
#define range devkit_range
#define lrange devkit_lrange
#define contains devkit_contains
#define unref devkit_unref

#endif


#define devkit_asiterable( array, length, typesize) _devkit_asiterable( (array), (length), typesize)	
#define devkit_linspace( start, end, steps) _devkit_linspace( (start), (end), (steps), false)
#define devkit_flinspace( start, end, steps) _devkit_linspace( (start), (end), (steps), true)
#define devkit_range( start, end) _devkit_range( (start), (end), false)
#define devkit_lrange( start, end) _devkit_range( (start), (end), true)

/* Checks if an array contains a certain value */
#define devkit_contains( array, len, var) _devkit_contains( (array), (len), sizeof(*(array)), &(var))
/* Unreferences to pointer after casting */
#define devkit_unref( type) *(type*)


/*
 * ########
 * # MATH #
 * ########
 */

typedef struct DevkitVector {
	double *items;
	size_t length;
} DevkitVector;

/* Note that DevkitMatrix can be converted to vector, as they have the same
 * variables and structure, so DevkitMatrix can use DevkitVector functions if cast to DevkitVector */

typedef struct DevkitMatrix {
	double *items;
	size_t length;
	size_t columns, rows;
} DevkitMatrix;

extern DevkitIterable devkit_vector_asiterable( DevkitVector *);

extern DevkitIterable devkit_vector_asiterable( DevkitVector *vec) {
	return (DevkitIterable) {
		.typesize=sizeof(double),
		.length=vec->length,
		.items=vec->items
	};
}

/* Returns an DevkitIterable that has the matrix iterated ROW BY ROW */
extern DevkitIterable devkit_matrix_asiterable( DevkitMatrix *);

extern DevkitIterable devkit_matrix_asiterable( DevkitMatrix *mat) {
	return (DevkitIterable) {
		.items=mat->items,
		.length=mat->length,
		.typesize=sizeof(double)
	};
}

#ifdef DEVKIT_STRIP_PREFIXES

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


extern DevkitVector* devkit_vector_new( size_t length);
extern DevkitVector devkit_vector_new_stack( size_t length);

// Access items through 'vector'->items

extern void devkit_vector_copyto( void *restrict dest, DevkitVector *restrict vec);
extern bool devkit_vector_equals( const DevkitVector *vec, const DevkitVector *other);
extern void devkit_vector_sum( DevkitVector *this, size_t nvecs, DevkitVector *vecs);
extern void devkit_vector_multiply_scalar( DevkitVector *vec, double scalar);
extern bool devkit_vector_iszero( DevkitVector *vec);
#define devkit_vector_nonzero( vec) ( assert(!devkit_vector_iszero(&vec)), vec)


extern DevkitMatrix* devkit_matrix_new( size_t columns, size_t rows);
extern DevkitMatrix devkit_matrix_new_stack( size_t columns, size_t rows);

extern double* devkit_matrix_get( DevkitMatrix *mat, size_t col, size_t row);
extern void devkit_matrix_set( DevkitMatrix *mat, double value, size_t col, size_t row);
extern void devkit_matrix_copyto( void *restrict dest, DevkitMatrix *restrict mat);
extern void devkit_matrix_multiply( DevkitMatrix *restrict dest, DevkitMatrix *restrict A, DevkitMatrix *restrict B);
extern bool devkit_matrix_equals( const DevkitMatrix *A, const DevkitMatrix *B);
extern void devkit_matrix_transpose( DevkitMatrix *mat);
extern void devkit_matrix_sum( DevkitMatrix *this, size_t nmats, DevkitMatrix *mats);
extern bool devkit_matrix_iszero( const DevkitMatrix *mat);
#define devkit_matrix_nonzero( mat) ( assert(!devkit_matrix_iszero(&mat)), mat)




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
	if ( _DEVKIT_POOL.length != 0) _DEVKIT_POOL.length--;


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
		DevkitArray: devkit_array_asiterable, \
		DevkitList: devkit_list_asiterable, \
		DevkitVector: devkit_vector_asiterable, \
		DevkitMatrix: devkit_matrix_asiterable, \
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

#define DEVKIT_STRING_IMPLEMENTATION
#ifdef DEVKIT_STRING_IMPLEMENTATION

DevkitIterable devkit_string_asiterable( DevkitString *s) {
	return (DevkitIterable) {
		.typesize=1,
		.items=s->items,
		.length=s->length,
	};
}

DevkitString* devkit_string_new( const char *text) {
	size_t length = strlen(text);
	DevkitString *this = malloc( sizeof(*this) + length);
	this->length = length;
	this->heap_default = true;
	this->items = (char*)(this + 1);
	memcpy( this->items, text, length);
	return this;
}

DevkitString devkit_string_new_stack( const char *text) {
	size_t length = strlen(text);
	char *items = malloc( length);
	return (DevkitString) {
		.length = length,
		.items = items,
		.heap_default = false
	};
}


/* Substring of DevkitString 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 */
char* devkit_string_slice( const DevkitString *restrict s, size_t start, size_t end) {
	assert( end > start);

	size_t substr_len = end - start;
	char *temp = malloc(substr_len + 1);
	strncpy( temp, s->items + start, substr_len);
	temp[substr_len] = '\0';
	return temp;
}

/* Returns DevkitString 's' reversed */
void devkit_string_reverse( DevkitString *s) {
	char buffer[s->length];
	memcpy( buffer, s->items, s->length);
	for (size_t idx = 0; idx < s->length; idx++) {
		s->items[idx] = buffer[s->length - idx - 1];
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

DevkitList* _devkit_list_new( size_t typesize, size_t capacity) {
	DevkitList *this = malloc( sizeof(*this) + typesize * capacity);
	this->items = this + 1;
	this->typesize = typesize;
	this->capacity = capacity;
	this->length = 0;
	return this;
}

DevkitList _devkit_list_new_stack( size_t typesize, size_t capacity) {
	return (DevkitList) {
		.typesize = typesize,
		.length = 0,
		.capacity = capacity,
		.items = calloc(capacity,typesize),
	};
}


void devkit_list_freeitems( DevkitList *list) {
#ifdef DEVKIT_DEBUG
	assert(list);
#endif
	list->length = 0, list->capacity = 0, 
	list->typesize = 0;
	free( list->items);
}


void* devkit_list_itemat( const DevkitList *list, const size_t index) {
	return list->items + index*list->typesize;
}

void devkit_list_set( DevkitList *restrict list, size_t index, const void *restrict value) {
#ifdef DEVKIT_DEBUG
	assert( list && value && index < list->length);
#endif

	memcpy( list->items + index*list->typesize, value, list->typesize);
}



void devkit_list_nadd( DevkitList *restrict list, size_t nitems, void *values) {
#ifdef DEVKIT_DEBUG
	assert( list && values );
#endif

	size_t ptr = list->length; // Needed later
	list->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( list->length > list->capacity) devkit_list_expand( list, list->length);
	// Copy values in pointers
	memcpy( list->items + ptr*list->typesize, values, nitems*list->typesize);
}


void devkit_list_ninsert( DevkitList *list, size_t index, size_t nitems, void *values) {
#ifdef DEVKIT_DEBUG
	assert( list && values );
#endif

	list->length += nitems;

	// Allocate more memory if needed
	if (list->length > list->capacity) devkit_list_expand( list, list->length);
	// Move following items forward, if there are any
	if (index < list->length) {
		memmove( list->items + (index+nitems)*list->typesize, list->items + index*list->typesize, list->typesize*(list->length-nitems - index) );
	}
	// Insert values at index
	memcpy( list->items + (index)*list->typesize, values, nitems*list->typesize);
}


void devkit_list_remove( void *dest, DevkitList *list, size_t index) {
#ifdef DEVKIT_DEBUG
	assert( list && index <= list->length);
#endif

	if (dest) memcpy( dest, list->items + index, list->typesize);

	// If the item isn't last, every following item must be shifted backwards.
	if (index != --list->length) {
		void *_dest = list->items + index*list->typesize;
		void *src = _dest + list->typesize; // list->items + (index+1)*list->typesize
		memmove( _dest, src, list->typesize * (list->length - 1 - index) );
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

void devkit_list_sort( DevkitList *restrict list, CompareFunc func) {
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
	devkit_list_freeitems( list->items);

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

DevkitIterable devkit_array_asiterable( DevkitArray *array) {
	assert( array != nullptr);
	return (DevkitIterable) { 
		.typesize=array->typesize, 
		.length=array->length, 
		.items=array->items
	};
}

DevkitArray* _devkit_array_new( size_t typesize, size_t length) {
	DevkitArray *this = malloc( sizeof(*this) + typesize*length);
	this->typesize = typesize;
	this->length = length;
	this->items = this + 1;
	return this;
}

DevkitArray _devkit_array_new_stack( size_t typesize, size_t length) {
	void *items = calloc( length, typesize);
#ifdef DEVKIT_DEBUG
	assert(items);
#endif
	return (DevkitArray) { 
		.typesize=typesize, 
		.length=length, 
		.items=items
	};
}


void* devkit_array_itemat( DevkitArray *array, size_t index) {
#ifdef DEVKIT_DEBUG
	assert(array);
	assert( index < array->length);
#endif
	return array->items + index*array->typesize;
}


void devkit_array_copyto( void *dest, DevkitArray *array) {
#ifdef DEVKIT_DEBUG
	assert( dest && array );
#endif
	memcpy( dest, array->items, array->length*array->typesize);
}


void devkit_array_set( DevkitArray *array, size_t index, void* value) {
#ifdef DEVKIT_DEBUG
	assert(array!=nullptr);
#endif
	memcpy( array + index, value, array->typesize);
}

void devkit_array_sort( DevkitArray *array, CompareFunc func) {
#ifdef DEVKIT_DEBUG
	assert( array != nullptr);
#endif
	qsort( array, array->length, array->typesize, func);
}



void devkit_array_sliceinto( void *restrict dest, DevkitArray *restrict array, size_t start, size_t end) {
	size_t delta = end - start;

#ifdef DEVKIT_DEBUG
	assert( array );
	assert( delta >= 0 && delta < array->length);
#endif
	void *slice = dest;
	void *src = array->items + start*array->typesize;
	memcpy( slice, src, array->typesize*delta);
}


/* Concatenates 'array' and 'other' and copies the buffer into 'dest'.
 * NOTE: arrays must be of same type */
void devkit_array_concat( void *restrict dest, DevkitArray *array, DevkitArray *other) {
#ifdef DEVKIT_DEBUG
	assert( array->typesize == other->typesize);
	assert( array && other );
#endif

	size_t newlen = array->length + other->length;

	void *concat = dest;
	memmove( concat, array->items, array->typesize*array->length);
	memmove( concat + array->typesize*array->length, other->items, array->typesize*other->length);
}


void devkit_array_freeitems( DevkitArray *array) {
	array->length = 0, array->typesize = 0;
	free(array->items);
}

#endif


/* POINTERS IMPLEMENTATION */

#define DEVKIT_POINTERS_IMPLEMENTATION
#ifdef DEVKIT_POINTERS_IMPLEMENTATION

/* Returns true if 'array' contains 'value' */
extern bool _devkit_contains( 
		const void *const array, 
		const size_t len, 
		const size_t typesize, 
		const void *value) 
{
	for ( size_t idx = 0; idx < len; idx++) {
		if ( memcmp( array + idx*typesize, value, typesize) == 0)
			return true;
	}
	return false;
}


/* Creates an iterable object associated with the 'array' of 'length' items of 'typesize' */
extern inline DevkitIterable _devkit_asiterable( void* array, size_t length, size_t typesize) {
#ifdef DEVKIT_DEBUG
	assert( array);
#endif
	return (DevkitIterable) { 
		.typesize=typesize, 
		.length=length, 
		.items=array
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

DevkitVector* devkit_vector_new( size_t length) {
	DevkitVector *this = malloc( sizeof(*this) + length * sizeof(double));
	this->length = length;
	this->items = (double*)(this + 1);
	return this;
}

DevkitVector devkit_vector_new_stack( size_t length) {
	return (DevkitVector) {
		.items = calloc( length, sizeof(double)),
		.length = length
	};
}


void devkit_vector_copyto( void *restrict dest, DevkitVector *restrict vec) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(vec && vec->items);
#endif
	memcpy( dest, vec->items, vec->length*sizeof(double));
}



bool devkit_vector_equals( const DevkitVector *vec, const DevkitVector *other) {
	if (vec->length != other->length) return false;
	for (size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != other->items[idx]) 
			return false;
	}
	return true;
}


void devkit_vector_sum( DevkitVector *vec, size_t nvecs, DevkitVector *args) {
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


void devkit_vector_multiply_scalar( DevkitVector *vec, double scalar) {
#ifdef DEVKIT_DEBUG
	assert(vec);
#endif
	for (size_t idx = 0; idx < vec->length; idx++) {
		vec->items[idx] *= scalar;
	}
}


bool devkit_vector_iszero( DevkitVector *vec) {
#ifdef DEVKIT_DEBUG
	assert(vec);
#endif
	for ( size_t idx = 0; idx < vec->length; idx++) {
		if ( vec->items[idx] != 0)
			return false;
	}
	return true;
}



DevkitMatrix* devkit_matrix_new( size_t columns, size_t rows) {
	DevkitMatrix *this = malloc( sizeof(*this) + columns*rows*sizeof(double));
	this->items = (double*)(this + 1);
	this->columns = columns;
	this->rows = rows;
	this->length = columns*rows;
	return this;
}

DevkitMatrix devkit_matrix_new_stack( size_t columns, size_t rows) {
	return (DevkitMatrix) {
		.columns = columns,
		.rows = rows,
		.length = rows*columns,
		.items = calloc( rows*columns, sizeof(long))
	};
}


void devkit_matrix_copyto( void *restrict dest, DevkitMatrix *restrict mat) {
#ifdef DEVKIT_DEBUG
	assert(dest);
	assert(mat);
#endif
	size_t size = mat->columns*mat->rows*sizeof(double);
	memcpy( dest, mat->items, size);
}


double* devkit_matrix_get( DevkitMatrix *mat, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(mat);
	assert(col < mat->columns);
	assert(row < mat->rows);
#endif
	return &mat->items[mat->columns*row + col];
}


void devkit_matrix_set( DevkitMatrix *mat, double value, size_t col, size_t row) {
#ifdef DEVKIT_DEBUG
	assert(mat);
	assert(col < mat->columns);
	assert(row < mat->rows);
#endif
	mat->items[mat->columns*row + col] = value;
}


bool devkit_matrix_equals( const DevkitMatrix *A, const DevkitMatrix *B) {
	return ( A->rows == B->rows && A->columns == B->columns)
		? ( memcmp( A->items, B->items, A->length))
			? true
			: false
		: false;
}


void devkit_matrix_transpose( DevkitMatrix *mat) {
	double buffer[mat->length];
	memcpy( buffer, mat->items, mat->length*sizeof(double));

	for ( size_t col = 0; col < mat->columns; col++)
	for ( size_t row = 0; row < mat->rows; row++)
		mat->items[mat->rows*row + col] = buffer[mat->rows*col + row];
	
	double temp = mat->rows;
	mat->rows = mat->columns;
	mat->columns = temp;
}


void devkit_matrix_sum( DevkitMatrix *dest, size_t nmats, DevkitMatrix *mats) {
	for (size_t mat = 0; mat < nmats; mat++, mats++) {
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


bool devkit_matrix_iszero( const DevkitMatrix *mat) {
	for ( size_t idx = 0; idx < mat->length; idx++) {
		if ( mat->items[idx] != 0)
			return false;
	}
	return true;
}
#endif



#endif
