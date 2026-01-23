#ifndef _DEVKIT_LIST_H
#define _DEVKIT_LIST_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "bits/comparator.h"
#include "bits/iterable.h"

/*
	LIST
*/

typedef struct devkit_list List;

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
extern List* _devkit_list_new( const size_t typesize, const size_t capacity);
#define devkit_list_new( type, capacity) _devkit_list_new( sizeof(type), (capacity))

/* Creates a new list whose struct is on the stack (not the items) */
extern List _devkit_list_new_stack( const size_t typesize, const size_t capacity);
#define devkit_list_new_stack( type, capacity) _devkit_list_new_stack( sizeof(type), (capacity))


/* Deallocates the items from memory if those items where allocated using stdlib,
 * sets all list values to 0 */
extern void devkit_list_freeitems( List *list);

/* Gives a reference to the item at 'index' in 'list' */
extern void* devkit_list_itemat( const List *list, const size_t index);

/* Set item at 'index' of 'list' to 'value', if index is in bounds.
 * 'value' is COPIED to the list */
extern void devkit_list_set( List *restrict list, size_t index, const void *restrict value);

/* Add 'nitems' items from 'values' to 'list' */
extern void devkit_list_nadd( List *restrict list, size_t nitems, void *const values);
#define devkit_list_add( list, var) devkit_list_nadd( (list), 1, (var))

/* Insert 'nitems' of 'values' in 'list' at 'index' */
extern void devkit_list_ninsert( List *list, size_t index, size_t nitems, void *values);
#define devkit_list_insert( list, index, var) devkit_list_ninsert( (list), (index), 1, (var))

/* Remove item at 'index' from 'list' and copy it to 'dest'.
 * If 'dest' is null, the value isn't copied */
extern void devkit_list_remove( void *dest, List *list, size_t index);

/* Remove 'nitems' items at 'indices' in 'list', copying them into 'dest' if not null */
extern void devkit_list_nremove( void *dest, List *list, const size_t nitems, const size_t *indices);

/* Checks is value is contained in list */
extern bool devkit_list_contains( const List *list, const void *const value);

/* Qsort adaptation for List. Sorts the list */
extern void devkit_list_sort( List *restrict list, Comparator func);


/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
extern bool devkit_list_concat( List *restrict dest, const List *restrict src);

/* Copies a section of the items into a buffer */
extern void devkit_list_sliceinto( void *restrict dest, List *restrict list, const size_t start, const size_t end);

/* Copies list items to 'dest' buffer, ovverriding its content */
extern void devkit_list_copyto( void *restrict dest, List *list);


/* Allocate more space for 'list' to increase its capacity to 'new_capacity' */
extern void devkit_list_expand( List *list, size_t new_capacity);

/* Reduce list capacity to its length to free unneeded memory */
extern void devkit_list_trim( List *list);



/* IMPLEMENTATION START */

//#define DEVKIT_LIST_IMPLEMENTATION
#ifdef DEVKIT_LIST_IMPLEMENTATION


List* _devkit_list_new( size_t typesize, size_t capacity) {
	List *this = malloc( sizeof(*this) + typesize * capacity);
	this->items = this + 1;
	this->typesize = typesize;
	this->capacity = capacity;
	this->length = 0;
	return this;
}

List _devkit_list_new_stack( size_t typesize, size_t capacity) {
	return (List) {
		.typesize = typesize,
		.length = 0,
		.capacity = capacity,
		.items = calloc(capacity,typesize),
	};
}


void devkit_list_freeitems( List *list) {
#ifdef DEVKIT_DEBUG
	assert(list);
#endif
	list->length = 0, list->capacity = 0, 
	list->typesize = 0;
	free( list->items);
}


void* devkit_list_itemat( const List *list, const size_t index) {
	return list->items + index*list->typesize;
}

void devkit_list_set( List *restrict list, size_t index, const void *restrict value) {
#ifdef DEVKIT_DEBUG
	assert( list && value && index < list->length);
#endif

	memcpy( list->items + index*list->typesize, value, list->typesize);
}



void devkit_list_nadd( List *restrict list, size_t nitems, void *values) {
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


void devkit_list_ninsert( List *list, size_t index, size_t nitems, void *values) {
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


void devkit_list_remove( void *dest, List *list, size_t index) {
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

void devkit_list_nremove( void *dest, List *list, const size_t nitems, const size_t *indices) {
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

bool devkit_list_contains( const List *list, const void *const value) {
	for (size_t idx = 0; idx < list->length; idx++) {
		if ( memcmp(list->items + idx*list->typesize, value, list->typesize) == 0)
			return true;
	}
	return false;
}

void devkit_list_sort( List *restrict list, Comparator func) {
#ifdef DEVKIT_DEBUG
	assert( list && func);
#endif

	qsort( list->items, list->length, list->typesize, func);
}


bool devkit_list_concat( List *restrict list, const List *restrict other) {
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

extern void devkit_list_sliceinto( void *restrict dest, List *list, const size_t start, const size_t end) {
	const size_t delta = end - start;

#ifdef DEVKIT_DEBUG
	assert(list);
	assert( delta >= 0 && delta < list->length);
#endif

	// Copy data to slice
	void *restrict src = list->items + start * list->typesize;
	memcpy( dest, src, delta*list->typesize);
}

void devkit_list_copyto( void *restrict dest, List *list) {
	memcpy( dest, list->items, list->length*list->typesize);
}


void devkit_list_expand( List *list, size_t new_capacity) {
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

void devkit_list_trim( List *list) {
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


#endif
