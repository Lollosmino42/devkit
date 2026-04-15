#ifndef _DEVKIT_H
#define _DEVKIT_H

// Remove once done with editing
#define DEVKIT_IMPLEMENTATION
#define DEVKIT_STRIP_PREFIXES

/* 
 * #######################
 * # SETTINGS FOR DEVKIT #
 * #######################
 */

//#define DEVKIT_DEBUG to enable additional safety checks in functions
//#define DEVKIT_STRIP_PREFIXES to strip 'devkit' prefix from functions and structs

// Enable to disable features that won't work if this header is imported 
// with a language that isn't C
#define DEVKIT_INTERFACING 0

// Enable support for custom iterables
#define DEVKIT_EXTRA_ITERABLES 0
// and add them below

#if DEVKIT_EXTRA_ITERABLES

/*
 * ###################
 * # EXTRA ITERABLES #
 * ###################
 */

/* DktIterable definition
	typedef struct devkit_iterable {
		void *items;
		size_t length;
		size_t typesize;
		size_t counter; <- ignore this (nothing changes if you touch it, so do not)
	} DktIterable;
*/

// Unlike typical iterables in high level lanuages,
// this iterable can be reused! (Every iteration the counter is set to 0)

/* 
 * To make a structure iterable, enable DEVKIT_EXTRA_ITERABLES,
 * then make a function as such:
 *
 * DktIterable <func_name>( <struct_t> *<struct>)
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
#define DEVKIT_ARRAY_IMPLEMENTATION
#define DEVKIT_STRING_IMPLEMENTATION

#define DEVKIT_POINTERS_IMPLEMENTATION

#endif

/* Something useful i guess */

typedef __compar_fn_t DktComparator;
#ifdef DEVKIT_STRIP_PREFIXES
typedef DktComparator Comparator;
#endif


/*
#########
# FILES #
#########
*/

/* Quality macros that stdio.h should have 
 * to define file opening modes so you don't have
 * to memorize the str_new values */

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

#if !DEVKIT_INTERFACING

/* Definition */
/* Used for 'foreach' loops */

typedef struct {
	size_t typesize;
	size_t length;
	size_t counter;
	void *items;
} DktIterable;

#endif

/*
 * ##########
 * # STRING #
 * ##########
 */

typedef struct {
	size_t length;
	char *items;
} DktString;

typedef struct {
	DktString *items;
	size_t count;
	size_t size;
	size_t capacity;
} DktStringBuilder;

#ifdef DEVKIT_STRIP_PREFIXES

#define str_new		dkt_str_new
#define str_slice	dkt_str_cslice
#define str_reverse dkt_str_reverse
#define str_free	dkt_str_free
#define str_cstr	dkt_str_cstr

#endif

/* Declarations */

extern DktString dkt_str_new( const char *text);
extern DktString dkt_str_slice( const DktString *restrict s, size_t start, size_t end);
/* Null terminated c-substring of DktString 's'. */
extern void dkt_str_cslice( char *restrict dest, const DktString *restrict s, size_t start, size_t end);
/* Returns DktString 's' reversed */
extern void dkt_str_reverse( DktString *s);
extern void dkt_str_free( DktString *s);
extern char* dkt_str_cstr( DktString *s);

extern DktStringBuilder dkt_strb_new(size_t char_capacity);
extern void dkt_strb_append( DktStringBuilder *, const DktString *s);
extern void dkt_strb_cappend( DktStringBuilder *, const char *s);
extern void dkt_strb_clear( DktStringBuilder *);
extern void dkt_strb_realloc( DktStringBuilder *, size_t new_capacity);
extern void dkt_strb_free( DktStringBuilder *);

extern char* dkt_strb_cmake( DktStringBuilder *, char *separator);
extern DktString dkt_strb_make( DktStringBuilder *, char *separator);

#if !DEVKIT_INTERFACING

extern DktIterable dkt_str_asiterable( DktString *);

#endif


/*
 * ########
 * # LIST #
 * ########
 */

/* An approach to variable length arrays in C. */

typedef struct {
	size_t length; 
	size_t capacity;
	size_t typesize;
	void *items;
} DktList;

#if !DEVKIT_INTERFACING
DktIterable dkt_list_asiterable( DktList *);
#endif

#define DKT_LIST_SIZE(T, capacity) (sizeof(DktList) + sizeof(T)*(capacity))

#ifdef DEVKIT_STRIP_PREFIXES

#define LIST_SIZE DKT_LIST_SIZE

#define list_new	dkt_list_new

#define list_contains	dkt_list_contains
#define list_get	devkit_get
#define list_add	dkt_list_add
#define list_nadd	dkt_list_nadd
#define	list_insert	dkt_list_insert
#define list_ninsert	dkt_list_ninsert
#define list_remove		dkt_list_remove
#define	list_nremove	dkt_list_nremove
#define list_concat		dkt_list_concat
#define list_sort	dkt_list_sort
#define list_sliceinto	dkt_list_sliceinto
#define list_copyto	dkt_list_copyto
#define list_expand	dkt_list_expand
#define list_trim	dkt_list_trim
#define list_free	dkt_list_free

#endif


/* Creates a new list */
extern DktList _dkt_list_new( const size_t typesize, const size_t capacity);
#define dkt_list_new( type, capacity) _dkt_list_new( sizeof(type), (capacity))

/* Deallocates the items from memory and sets all list values to 0 */
extern void dkt_list_free( DktList *);

/* Gives a reference to the item at 'index' in 'list' */
extern void* dkt_list_get( const DktList *, const size_t index);

/* Set item at 'index' of 'list' to 'value' */
extern void dkt_list_set( DktList *restrict l, size_t index, const void *restrict value);

/* Add 'nitems' items from 'values' to 'list' */
extern void dkt_list_nadd( DktList *restrict l, size_t nitems, void *const values);
#define dkt_list_add( l, var) dkt_list_nadd( (l), 1, (var))

/* Insert 'nitems' of 'values' in 'list' at 'index' */
extern void dkt_list_ninsert( DktList *l, size_t index, size_t nitems, void *values);
#define dkt_list_insert( l, index, var) dkt_list_ninsert( (l), (index), 1, (var))

/* Remove item at 'index' from 'list' and copy it to 'dest'.
 * If 'dest' is null, the value isn't copied */
extern void dkt_list_remove( void *dest, DktList *l, size_t index);

/* Remove 'nitems' items at 'indices' in 'list', copying them into 'dest' if not null */
extern void dkt_list_nremove( void *dest, DktList *l, const size_t nitems, const size_t *indices);

/* Checks is value is contained in list */
extern bool dkt_list_contains( const DktList *l, const void *const value);

/* Qsort adaptation for DktList. Sorts the list */
extern void dkt_list_sort( DktList *restrict l, DktComparator func);


/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
extern bool dkt_list_concat( DktList *restrict dest, const DktList *restrict src);

/* Copies a section of the items into a buffer */
extern void dkt_list_sliceinto( void *restrict dest, DktList *restrict l, const size_t start, const size_t end);

/* Copies list items to 'dest' buffer, overwriting its contents */
extern void dkt_list_copyto( void *restrict dest, DktList *l);

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
	size_t length;
	size_t typesize;
	void* items;
} DktArray;

#if !DEVKIT_INTERFACING
extern DktIterable dkt_arr_asiterable( DktArray *);
#endif

#define DEVKIT_ARR_SIZE(T, capacity) (sizeof(DktArray) + sizeof(T)*(capacity))

#ifdef DEVKIT_STRIP_PREFIXES

#define ARR_SIZE DEVKIT_ARR_SIZE

#define arr_new	dkt_arr_new
#define arr_of	dkt_arr_of

#define arr_get	dkt_arr_get
#define arr_copyto	dkt_arr_copyto
#define arr_sliceinto	dkt_arr_sliceinto
#define arr_set		dkt_arr_set
#define arr_sort	dkt_arr_sort
#define arr_concat	dkt_arr_concat
#define arr_free	dkt_arr_free

#endif


/* Allocates a new DktArray on the heap */
extern DktArray _dkt_arr_new( size_t typesize, size_t length);
#define dkt_arr_new( type, length) _dkt_arr_new( sizeof(type), (length))

/* Gets a reference to the item at 'index' in 'array' */
extern void* dkt_arr_get( DktArray *a, size_t index);

/* Sets 'array' item at 'index' to 'value' */
extern void dkt_arr_set( DktArray *restrict a, size_t index, void *restrict value);

extern void dkt_arr_of( DktArray *restrict a, const void *restrict values);

/* Copy 'array' items to buffer 'dest' */
extern void dkt_arr_copyto( void *restrict dest, DktArray *restrict a);

/* Copy 'array' items from 'start' to 'end' into buffer 'dest' */
extern void dkt_arr_sliceinto( void *restrict dest, DktArray *restrict a, const size_t start, const size_t end);

/* Concatenate 'array' and 'other', copying items into buffer 'dest' */
extern void dkt_arr_concat( void *restrict dest, DktArray *a, DktArray *other);

/* Qsort adaptation for DktArray */
extern void dkt_arr_sort( DktArray *a, DktComparator cmp);

/* Deallocates item buffer of 'array' if allocated on heap using the standard library,
 * sets all array values to 0 */
extern void dkt_arr_free( DktArray *a);

/*
 * ############
 * # POINTERS #
 * ############
 */

#if !DEVKIT_INTERFACING

#ifdef DEVKIT_STRIP_PREFIXES

#define asiterable	dkt_asiterable
#define linspace	dkt_linspace
#define flinspace	dkt_flinspace
#define range	dkt_range
#define lrange	dkt_lrange
#define contains	dkt_contains
#define unref	dkt_unref
#define ref		dkt_ref
#define free_all	dkt_free_all

#endif


extern inline DktIterable _dkt_asiterable( void* a, size_t length, size_t typesize);
#define dkt_asiterable( arr, length, type) _dkt_asiterable( (arr), (length), sizeof(type))	

extern void* _dkt_linspace( double start, double end, size_t steps, bool isfloat);
#define dkt_linspace( start, end, steps) _dkt_linspace( (start), (end), (steps), false)
#define dkt_flinspace( start, end, steps) _dkt_linspace( (start), (end), (steps), true)

extern void* _dkt_range( long start, long end, bool islong);
#define dkt_range( start, end) _dkt_range( (start), (end), false)
#define dkt_lrange( start, end) _dkt_range( (start), (end), true)

/* Checks if an array contains a certain value */
extern bool _dkt_contains(const void *const a, const size_t len, const size_t typesize, const void *value);
#define dkt_contains( a, len, var) _dkt_contains( (a), (len), sizeof(*(a)), (var))
/* Unreferences to pointer after casting */
#define dkt_unref( type, value) (*(type*)(value))

#define dkt_ref( type, ...) (type[]){__VA_ARGS__}

extern void _dkt_free_all( void **ptrs);
#define dkt_free_all(...) _dkt_free_all( (void*[]) {__VA_ARGS__, nullptr})

#endif


/* 
 * ###################################################################
 * # Loop pool implementation needed for nested 'enhanced for' loops #
 * ###################################################################
 */

#if !DEVKIT_INTERFACING

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
 */


/* Bypass for DktIterable in generic selection of _dkt_iterable. With this,
 * arrays can be converted to iterables and used in foreach loops.
 * It is recommended not to use stack arrays or, in general, stack allocations
 * as it may seg-fault */
extern inline DktIterable _dkt_dummy_asiterable(DktIterable *iter) {
	return *iter;
}

/* "Cast" to _dkt_iterable.
 * Works with Arrays, Lists, and other structures defined in devkit that have
 * a <...>_asiterable function.
 * Other structures can be compatible with 'foreach' if an 'asiterable'-like function
 * is defined for them in the section EXTRA ITERABLES at the start of this header file */

#define _dkt_iterable( structure) _Generic( (structure), \
		_DEVKIT_ITERABLES, \
		DktArray: dkt_arr_asiterable, \
		DktList: dkt_list_asiterable, \
		DktVector: dkt_vec_asiterable, \
		DktMatrix: dkt_mat_asiterable, \
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

/* Prefix stripping */

#ifdef DEVKIT_STRIP_PREFIXES

typedef DktArray Array;
typedef DktString String;
typedef DktStringBuilder StringBuilder;
typedef DktList List;

#endif

/* 
 * ###################
 * # IMPLEMENTATIONS #
 * ###################
 */


/* STRING IMPLEMENTATION */

//#define DEVKIT_STRING_IMPLEMENTATION
#ifdef DEVKIT_STRING_IMPLEMENTATION

#if !DEVKIT_INTERFACING
DktIterable dkt_str_asiterable( DktString *s) {
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


extern DktString dkt_str_slice( const DktString *restrict s, size_t start, size_t end) {
#ifdef DEVKIT_DEBUG
	assert( s->items);
	assert( end > start);
	assert( start >= 0 && end <= s->length);
#endif
	
	DktString substr = (DktString) {
		.items = malloc(end - start),
		.length = end - start
	};

	memcpy(substr.items, s->items + start, substr.length);
	return substr;
}

void dkt_str_cslice( char *restrict dest, const DktString *restrict s, size_t start, size_t end) {
#ifdef DEVKIT_DEBUG
	assert( dest);
	assert( s->items);
	assert( end > start);
	assert( start >= 0 && end <= s->length);
#endif

	size_t sublen = end - start;
	memset(dest, 0, sublen+1);
	strncpy( dest, s->items + start, sublen);
}

void dkt_str_reverse( DktString *s) {
	char buffer[s->length];
	memcpy( buffer, s->items, s->length);
	for (size_t idx = 0; idx < s->length; idx++) {
		s->items[idx] = buffer[s->length - idx - 1];
	}
}

extern void dkt_str_free( DktString *s) {
	free(s->items);
	s->length = 0;
}

extern char* dkt_str_cstr( DktString *s) {
	size_t len = s->length + 1;
	char *cstr = malloc(len);
	memset(cstr, 0, len);
	strncpy(cstr, s->items, s->length);
	return cstr;
}


extern DktStringBuilder dkt_strb_new(size_t byte_capacity) {
	DktString *items = malloc( byte_capacity);
	return (DktStringBuilder) {
		.count = 0,
		.size = 0,
		.capacity = byte_capacity,
		.items = items
	};
}

extern void dkt_strb_append( DktStringBuilder *sb, const DktString *s) {
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

extern void dkt_strb_cappend( DktStringBuilder *sb, const char *s) {
	DktString wrap = dkt_str_new(s);
	dkt_strb_append(sb, &wrap);
	dkt_str_free(&wrap);
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
	char buf[sb->size];
	memcpy(buf, sb->items, sb->size);

	// Reallocate items buffer and put data back
	sb->items = realloc(sb->items, new_capacity);

	memcpy(sb->items, buf, sb->size);
}

extern void dkt_strb_free( DktStringBuilder *sb) {
	free(sb->items);
	memset(sb, 0, sizeof(*sb));
}

extern char* dkt_strb_cmake( DktStringBuilder *sb, char *separator) {
	size_t build_size = sb->size + strlen(separator)*(sb->count-1) - sb->count*sizeof(DktString) + 1;
	char *build = malloc(build_size);
	memset(build, 0, build_size);

	char *cur = (char*)sb->items;
	for (int i = 0; i < sb->count; ++i) {
		DktString *src = (DktString*)cur;

		cur += sizeof(DktString);

		size_t amount = src->length;
		strncat( build, cur, amount);
		// Add separator between tokens
		if (i != sb->count - 1)
			strncat( build, separator, strlen(separator));

		cur += src->length;
	}
	return build;
}

extern DktString dkt_strb_make( DktStringBuilder *sb, char *separator) {
	char *raw_text = dkt_strb_cmake(sb, separator);
	DktString wrapped = dkt_str_new( raw_text);
	free(raw_text);
	return wrapped;
}

#endif


/* LIST IMPLEMENTATION */

//#define DEVKIT_LIST_IMPLEMENTATION
#ifdef DEVKIT_LIST_IMPLEMENTATION

#if !DEVKIT_INTERFACING
DktIterable dkt_list_asiterable( DktList *list) {
#ifdef DEVKIT_DEBUG
	assert( list != nullptr);
#endif
	return (DktIterable) { 
		.typesize=list->typesize, 
		.length=list->length, 
		.items=list->items
	};
}
#endif

DktList _dkt_list_new( size_t typesize, size_t capacity) {
	return (DktList) {
		.typesize = typesize,
		.length = 0,
		.capacity = capacity,
		.items = calloc(capacity,typesize),
	};
}


void dkt_list_free( DktList *this) {
#ifdef DEVKIT_DEBUG
	assert(this);
#endif
	free( this->items);
	memset(this, 0, sizeof(*this));
}


void* dkt_list_get( const DktList *this, const size_t index) {
	return this->items + index*this->typesize;
}

void dkt_list_set( DktList *restrict this, size_t index, const void *restrict value) {
#ifdef DEVKIT_DEBUG
	assert( this && value && index < this->length);
#endif

	memcpy( this->items + index*this->typesize, value, this->typesize);
}



void dkt_list_nadd( DktList *restrict this, size_t nitems, void *values) {
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


void dkt_list_ninsert( DktList *this, size_t index, size_t nitems, void *values) {
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


void dkt_list_remove( void *dest, DktList *this, size_t index) {
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

void dkt_list_nremove( void *dest, DktList *l, const size_t nitems, const size_t *indices) {
#ifdef DEVKIT_DEBUG
	assert( l && indices );
#endif

	size_t sorted[nitems];
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
}

bool dkt_list_contains( const DktList *l, const void *const value) {
	for (size_t idx = 0; idx < l->length; idx++) {
		if ( memcmp(l->items + idx*l->typesize, value, l->typesize) == 0)
			return true;
	}
	return false;
}

void dkt_list_sort( DktList *restrict l, DktComparator func) {
#ifdef DEVKIT_DEBUG
	assert( l && func);
#endif

	qsort( l->items, l->length, l->typesize, func);
}


bool dkt_list_concat( DktList *restrict l, const DktList *restrict other) {
#ifdef DEVKIT_DEBUG
	assert(l && other);
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

extern void dkt_list_sliceinto( void *restrict dest, DktList *l, const size_t start, const size_t end) {
	const size_t delta = end - start;

#ifdef DEVKIT_DEBUG
	assert(l);
	assert( delta >= 0 && delta < l->length);
#endif

	// Copy data to slice
	void *restrict src = l->items + start * l->typesize;
	memcpy( dest, src, delta*l->typesize);
}

void dkt_list_copyto( void *restrict dest, DktList *l) {
	memcpy( dest, l->items, l->length*l->typesize);
}


void dkt_list_expand( DktList *l, size_t new_capacity) {
#ifdef DEVKIT_DEBUG
	assert( l );
#endif
	size_t prev_size = l->capacity*l->typesize;

	void *new_items = calloc( new_capacity, l->typesize);
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
	
	void *trim = malloc( l->length * l->typesize);
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

//#define DEVKIT_ARRAY_IMPLEMENTATION
#ifdef DEVKIT_ARRAY_IMPLEMENTATION

#if !DEVKIT_INTERFACING
DktIterable dkt_arr_asiterable( DktArray *a) {
	assert( a != nullptr);
	return (DktIterable) { 
		.typesize=a->typesize, 
		.length=a->length, 
		.items=a->items
	};
}
#endif

DktArray _dkt_arr_new( size_t typesize, size_t length) {
	void *items = calloc(length, typesize);
	size_t len = length * typesize;
	memset(items, 0, len);
	return (Array) {
		.items = items,
		.length = len,
		.typesize = typesize
	};
}


void* dkt_arr_get( DktArray *a, size_t index) {
#ifdef DEVKIT_DEBUG
	assert(a);
	assert( index < a->length);
#endif
	return a->items + index*a->typesize;
}


void dkt_arr_copyto( void *dest, DktArray *a) {
#ifdef DEVKIT_DEBUG
	assert( dest && a );
#endif
	memcpy( dest, a->items, a->length*a->typesize);
}


void dkt_arr_set( DktArray *a, size_t index, void* value) {
#ifdef DEVKIT_DEBUG
	assert(a);
	assert(index < a->length);
#endif
	memcpy( a->items + index*a->typesize, value, a->typesize);
}

extern void dkt_arr_of( DktArray *restrict a, const void *restrict values) {
	for (size_t i = 0; i < a->length; ++i)
		memcpy(a->items + i*a->typesize, values + i * a->typesize, a->typesize);
}


void dkt_arr_sort( DktArray *a, DktComparator func) {
#ifdef DEVKIT_DEBUG
	assert( a != nullptr);
#endif
	qsort( a, a->length, a->typesize, func);
}



void dkt_arr_sliceinto( void *restrict dest, DktArray *restrict a, size_t start, size_t end) {
	size_t delta = end - start;

#ifdef DEVKIT_DEBUG
	assert( a );
	assert( delta >= 0 && delta < a->length);
#endif
	void *slice = dest;
	void *src = a->items + start*a->typesize;
	memcpy( slice, src, a->typesize*delta);
}


/* Concatenates 'array' and 'other' and copies the buffer into 'dest'.
 * NOTE: arrays must be of same type */
void dkt_arr_concat( void *restrict dest, DktArray *a, DktArray *other) {
#ifdef DEVKIT_DEBUG
	assert( a->typesize == other->typesize);
	assert( a && other );
#endif

	size_t newlen = a->length + other->length;

	void *concat = dest;
	memmove( concat, a->items, a->typesize*a->length);
	memmove( concat + a->typesize*a->length, other->items, a->typesize*other->length);
}


void dkt_arr_free( DktArray *a) {
	a->length = 0, a->typesize = 0;
	free(a->items);
}

#endif


/* POINTERS IMPLEMENTATION */

//#define DEVKIT_POINTERS_IMPLEMENTATION
#if defined(DEVKIT_POINTERS_IMPLEMENTATION) && !DEVKIT_INTERFACING

/* Returns true if 'array' contains 'value' */
extern bool _dkt_contains( 
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


/* Creates an iterable object associated with the 'array' of 'length' items of 'typesize' */
extern inline DktIterable _dkt_asiterable( void* a, size_t length, size_t typesize) {
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



#endif
