#ifndef _DEVKIT_LIST_H
#define _DEVKIT_LIST_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "settings.h"
#include "bits/comparator.h"
#include "bits/iterable.h"

/*
	LIST
*/

typedef struct devkit_list List;

/* Returns the size of a list for manual allocation */
#define DEVKIT_LIST_OF( type, capacity) (sizeof(List) + sizeof(type)*(capacity))


#if DEVKIT_STRIP_PREFIXES

#define LIST_OF DEVKIT_LIST_OF

#define list_new	devkit_list_new
#define list_fromptr	devkit_list_fromptr
#define list_fromarray	devkit_list_fromarray
#define list_fromstr	devkit_list_fromstr
#define list_contains	devkit_list_contains
#define list_getref	devkit_list_getref
#define list_get	devkit_list_get
#define list_add	devkit_list_add
#define list_nadd	devkit_list_nadd
#define	list_insert	devkit_list_insert
#define list_ninsert	devkit_list_ninsert
#define list_remove		devkit_list_remove
#define	list_nremove	devkit_list_nremove
#define list_concat		devkit_list_concat
#define list_sort	devkit_list_sort
#define list_slice	devkit_list_slice
#define list_copyitems	devkit_list_copyitems
#define list_expand	devkit_list_expand
#define list_trim	devkit_list_trim
#define list_free	devkit_list_free
#define list_toarray	devkit_list_toarray

#endif

#if DEVKIT_USE_CUSTOM_ALLOCATOR

#define devkit_list_new( alloc, type, capacity) _devkit_list_new( (alloc), sizeof(type), (capacity))
#define devkit_list_fromptr( alloc, length, ptr) _devkit_list_from( (alloc), sizeof((ptr)[0]), (length), (ptr) )
#define devkit_list_fromarray( alloc, array) _devkit_list_from( (alloc), (array).typesize, (array).length, (array).items)
#define devkit_list_fromstr( alloc, str) _devkit_list_from( (alloc), 1, strlen(str), (str))

#else

#define devkit_list_new( type, capacity) _devkit_list_new( nullptr, sizeof(type), (capacity))
#define devkit_list_fromptr( length, ptr) _devkit_list_from( nullptr, sizeof((ptr)[0]), (length), (ptr) )
#define devkit_list_fromarray( array) _devkit_list_from( nullptr, (array).typesize, (array).length, (array).items)
#define devkit_list_fromstr( str) _devkit_list_from( nullptr, 1, strlen(str), (str))

#endif

/* Raw declarations */
extern List _devkit_list_new( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t capacity);
extern List _devkit_list_from( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t nitems, void* items);


#define devkit_list_contains( list, var) devkit_contains( (list).items, (list).length, (list).typesize, &(var))
#define devkit_list_getref( list, index) ( (list).items + (index)*(list).typesize )
#define devkit_list_get( type, list, index) *(type*) list_getref( (list), (index) )
extern void devkit_list_set( List *list, size_t index, void *const value);

#define devkit_list_add( list, var) devkit_list_nadd( (list), 1, (var))
extern void devkit_list_nadd( List *restrict list, size_t nitems, void *const values);
#define devkit_list_insert( list, index, var) devkit_list_ninsert( (list), (index), 1, (var))
extern void devkit_list_ninsert( List *list, size_t index, size_t nitems, void *values);
extern void devkit_list_remove( void *dest, List *list, size_t index);
extern void devkit_list_nremove( void *dest, List *list, const size_t nitems, const size_t *indices);
extern inline void devkit_list_sort( List *restrict list, Comparator func);

extern bool devkit_list_concat( List *restrict dest, const List *restrict src);
extern void devkit_list_slice( List *restrict dest, List *list, const size_t start, const size_t end);
extern void devkit_list_copyitems( void *restrict dest, List *list);

extern void devkit_list_expand( List *list, size_t new_capacity);
extern void devkit_list_trim( List *restrict list);
extern void devkit_list_free( List *list);
extern void devkit_list_toarray( List *list, void* restrict dest);



/* IMPLEMENTATION START */

//#define DEVKIT_LIST_IMPLEMENTATION
#ifdef DEVKIT_LIST_IMPLEMENTATION

/* List initializer */
List _devkit_list_new( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t capacity) {
	return (List) { 
		.allocator=alloc,
		.typesize=typesize, 
		.capacity=capacity, 
		.length=0, 
		.items=DEVKIT_CALLOC( alloc, typesize, capacity)
	};
}

/* Returns a non-empty list with 'items' in it */
List _devkit_list_from( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t nitems, void* items) {
	assert(items);

	List list = { 
		.typesize=typesize, 
		.capacity=nitems, 
		.length=nitems, 
		.items=( !alloc)
			? calloc( nitems, typesize)
			: DEVKIT_CALLOC( alloc, nitems, typesize) 
	};
	// Copy values
	memcpy( list.items, items, nitems*typesize);

	return list;
}

// For ease of coding
#define TSIZE list->typesize 
#define ITEMS list->items


// Allocate more space for 'list' to increase its capacity to 'new_capacity'
void devkit_list_expand( List *list, size_t new_capacity) {
	assert( list );

	size_t prev_size = list->capacity*TSIZE;
	char buf[ prev_size];
	memcpy( buf, ITEMS, prev_size);
	DEVKIT_FREE( list->allocator, ITEMS, prev_size);

	void *new_items = DEVKIT_CALLOC( list->allocator, new_capacity, TSIZE);
	memcpy( new_items, buf, prev_size);
	ITEMS = new_items;
	list->capacity = new_capacity;
}


/* Copies list items to 'dest' buffer */
void devkit_list_copyitems( void *restrict dest, List *list) {
	memcpy( dest, list->items, list->length*TSIZE);
}


/* Set item at 'index' of 'list' to 'value', if index is in bounds.
 * 'value' is COPIED to the list, so it is not modified by the list */
void devkit_list_set( List *list, size_t index, void *const value) {
	assert( list && value );

	if (index < list->length && index >= 0) {
		memcpy( ITEMS + index*TSIZE, value, TSIZE);
		return;
	}
}


// Add 'nitems' ITEMS from 'values' to 'list'
void devkit_list_nadd( List *restrict list, size_t nitems, void *values) {
	assert( list && values );

	size_t ptr = list->length; // Needed later
	list->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( list->length > list->capacity) devkit_list_expand( list, list->length);
	// Copy values in pointers
	memcpy( ITEMS + ptr*TSIZE, values, nitems*TSIZE);
}


// Insert 'nitems' of 'values' in 'list' at 'index'
void devkit_list_ninsert( List *list, size_t index, size_t nitems, void *values) {
	assert( list && values );

	list->length += nitems;

	// Allocate more memory if needed
	if (list->length > list->capacity) devkit_list_expand( list, list->length);
	// Move following items forward, if there are any
	if (index < list->length) {
		memmove( ITEMS + (index+nitems)*TSIZE, ITEMS + index*TSIZE, TSIZE*(list->length-nitems - index) );
	}
	// Insert values at index
	memcpy( ITEMS + (index)*TSIZE, values, nitems*TSIZE);
}


/* Remove item at 'index' from 'list' and copy it to 'dest'.
 * If 'dest' is null, the value isn't copied */
void devkit_list_remove( void *dest, List *list, size_t index) {
	assert( list && index <= list->length);

	if (dest) memcpy( dest, ITEMS + index, TSIZE);

	// If the item isn't last, every following item must be shifted backwards.
	if (index != --list->length) {
		void *_dest = ITEMS + index*TSIZE;
		void *src = _dest + TSIZE; // ITEMS + (index+1)*TSIZE
		memmove( _dest, src, TSIZE * (list->length - 1 - index) );
	}
}


int _devkit_list_cmp(const void *a, const void*b) {
	return memcmp(a,b, sizeof(size_t));
}

/* Remove 'nitems' items at 'indices' in 'list' */
void devkit_list_nremove( void *dest, List *list, const size_t nitems, const size_t *indices) {
	assert( list && indices );

	size_t sorted[nitems];
	memcpy( sorted, indices, sizeof(size_t)*nitems);
	qsort( sorted, nitems, sizeof(size_t), _devkit_list_cmp);

	for (size_t item = 0; item < nitems; item++) {
		size_t index = indices[item] - item;
		if (dest) memcpy( dest + item*TSIZE, ITEMS + index*TSIZE, TSIZE);
		
		// If the item isn't last, every following item must be shifted backwards.
		if ( index != --list->length) {
			void *_dest = ITEMS + index*TSIZE, 
				 *src = ITEMS + (index+1)*TSIZE;
			memmove( _dest, src, TSIZE * (list->length - index));
		}
	}
}


/* Reduce list capacity to its length to free unneeded memory,
 * so use it only when you truly need it */
void devkit_list_trim( List *restrict list) {
	assert( list );

	if (list->capacity == list->length) return;
	
	char buf[list->length];
	memcpy( buf, ITEMS, list->length*TSIZE);
	DEVKIT_FREE( list->allocator, ITEMS, list->capacity*TSIZE);

	void *trim = DEVKIT_ALLOC( list->allocator, list->length * TSIZE);
	assert( trim != nullptr), memcpy( trim, buf, list->length*TSIZE);

	ITEMS = trim;
	list->capacity = list->length;
}

/* Frees list memory used for items. After this, length and capacity
 * are set to 0 and the list should be unusable */
void devkit_list_free( List *list) {
	assert( list );
	list->length = 0, list->capacity = 0;
	DEVKIT_FREE( list->allocator, list->items, list->capacity*list->typesize);
}


/* Sorts items of 'list' with comparator function 'func' */
inline void devkit_list_sort( List *restrict list, Comparator func) {
	assert( list && func);

	qsort( ITEMS, list->length, list->typesize, func);
}


/* Copy list data to 'dest'. Make sure that 'dest' has enough
 * memory to store list items */
void devkit_list_toarray( List *list, void* restrict dest) {
	assert( list && dest);

	memcpy( dest, ITEMS, list->length*TSIZE);
}


/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
bool devkit_list_concat( List *restrict list, const List *restrict other) {
	assert(list && other);

	// Exit if sizes are different
	if ( TSIZE != other->typesize) return false;
	// Index of concatenation
	size_t concat_pos = list->length * TSIZE;
	list->length += other->length;

	if ( list->length > list->capacity) devkit_list_expand(list, list->length);
	// Copy items
	memcpy( ITEMS + concat_pos, other->items, other->length*TSIZE);

	return true;
}


/* Returns a new list with a portion of 'list' items */
extern void devkit_list_slice( List *restrict dest, List *list, const size_t start, const size_t end) {
	size_t delta = end - start;

	assert(list);
	assert( delta >= 0 && dest->length >= delta);

	void *slice = dest->items;
	// Copy data to slice
	void *src = ITEMS + start * TSIZE;
	memcpy( slice, src, delta*TSIZE);
}

#endif

#undef ITEMS
#undef TSIZE

#endif
