#ifndef __DEVKIT_LIST_H
#define __DEVKIT_LIST_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "comparators.h"
#include "iterable.h"



/*
	-----------
	LIST STRUCT
	-----------
*/

/* An approach to variable length arrays in C. */
typedef struct {
	const size_t typesize;
	size_t capacity;
	size_t length;
	void *items;
} List;


#define new_list( type, capacity) \
	(List) { sizeof(type), capacity, 0, calloc( capacity, sizeof(type)) }

extern List list_of( size_t typesize, size_t nitems, void* items);

#define list_fromarray( array, length) \
	list_of( sizeof((array)[0]), (length), (array) )
// Get reference of item in list
#define list_getref( list, index) ( (list).items + (index)*(list).typesize )

#define list_get( type, list, index) \
	*(type*) list_getref( (list), (index) )

#define list_contains( list, var) \
	__devkit_contains( (list).items, (list).length, (list).typesize, &(var))

extern void list_expand( List *list, size_t new_capacity);

extern void list_set( List *list, size_t index, void *const value);

#define list_add( list, var) list_nadd( (list), 1, (var))

extern void list_nadd( List *restrict list, size_t nitems, void *const values);

#define list_insert( list, index, var) list_ninsert( (list), (index), 1, (var))

extern void list_ninsert( List *list, size_t index, size_t nitems, void *values);

extern void* list_remove( List *list, size_t index);

extern void* list_nremove( List *list, const size_t nitems, const size_t indices[]);

extern void list_trim( List *restrict list);

extern inline void list_free( List *list);

extern inline void list_sort( List *restrict list, Comparator func);

extern void list_toarray( List *list, void* restrict dest);

extern bool list_concat( List *restrict list, const List *restrict other);

extern List list_slice( List *restrict list, const size_t start, const size_t end);

extern inline Iterable list_asiterable( List *list);




/* IMPLEMENTATION START */

/* List initializer */
extern List __devkit_new_list( size_t typesize, size_t capacity) {
	return (List) { typesize, capacity, 0, calloc( typesize, capacity)};
}


/* Returns a non-empty list with 'items' in it */
extern List list_of( size_t typesize, size_t nitems, void* items) {
	assert(items != nullptr);

	List list = { typesize, nitems, nitems, calloc( nitems, typesize) };
	// Copy values
	memcpy( list.items, items, nitems*typesize);

	return list;
}

// For ease of coding
#define TSIZE list->typesize 
#define ITEMS list->items


// Allocate more space for 'list' to increase its capacity to 'new_capacity'
extern void list_expand( List *list, size_t new_capacity) {
	assert(list != nullptr);

	void* new_items = calloc( new_capacity, TSIZE);
	assert( new_items != nullptr);

	memcpy( new_items, ITEMS, list->length*TSIZE);
	free( ITEMS);
	ITEMS = new_items;
	list->capacity = new_capacity;
}


/* Set item at 'index' of 'list' to 'value', if index is in bounds.
 * 'value' is COPIED to the list, so it is not modified by the list */
extern void list_set( List *list, size_t index, void *const value) {
	assert( list != nullptr);
	assert( value != nullptr);

	if (index < list->length && index >= 0) {
		memcpy( ITEMS + index*TSIZE, value, TSIZE);
		return;
	}
}


// Add 'nitems' ITEMS from 'values' to 'list'
extern void list_nadd( List *restrict list, size_t nitems, void *const values) {
	assert( list != nullptr);
	assert( values != nullptr);

	size_t ptr = list->length; // Needed later
	list->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( list->length > list->capacity) list_expand( list, list->length);
	// Copy values in pointers
	memcpy( ITEMS + (list->length - nitems)*TSIZE, values, nitems*TSIZE);
}


// Insert 'nitems' of 'values' in 'list' at 'index'
extern void list_ninsert( List *list, size_t index, size_t nitems, void *values) {
	assert( list != nullptr);
	assert( values != nullptr);

	list->length += nitems;

	// Allocate more memory if needed
	if (list->length > list->capacity) list_expand( list, list->length);
	// Move following items forward, if there are any
	if (index < list->length) {
		memmove( ITEMS + (index+nitems)*TSIZE, ITEMS + index*TSIZE, TSIZE*(list->length-nitems - index) );
	}
	// Insert values at index
	memcpy( ITEMS + (index)*TSIZE, values, nitems*TSIZE);
}


/* Remove item at 'index' from 'list' */
extern void* list_remove( List *list, size_t index) {
	assert( list != nullptr);

	void *removed = malloc(TSIZE);
	memcpy( removed, ITEMS + index, TSIZE);

	// If the item isn't last, every following item must be shifted backwards.
	if (index != --list->length) {
		void *dest = ITEMS + index*TSIZE;
		void *src = dest + TSIZE; // ITEMS + (index+1)*TSIZE
		memmove( dest, src, TSIZE * (list->length - 1 - index) );
	}

	return removed;
}


/* Remove 'nitems' items at 'indices' in 'list' */
extern void* list_nremove( List *list, const size_t nitems, const size_t indices[]) {
	assert( list != nullptr);
	assert( indices != nullptr);

	void* removed = calloc( nitems, TSIZE);
	assert( removed != nullptr);

	size_t sorted[nitems];
	memcpy( sorted, indices, sizeof(size_t)*nitems);
	qsort( sorted, nitems, sizeof(size_t), (Comparator) ulongcmp);

	for (size_t item = 0; item < nitems; item++) {
		size_t index = indices[item] - item;
		memcpy( removed + item*TSIZE, ITEMS + index*TSIZE, TSIZE);
		
		// If the item isn't last, every following item must be shifted backwards.
		if ( index != --list->length) {
			void *dest = ITEMS + index*TSIZE, 
				 *src = ITEMS + (index+1)*TSIZE;
			memmove( dest, src, TSIZE * (list->length - index));
		}
	}

	return removed;
}


/* Reduce list capacity to its length to free unneeded memory,
 * so use it only when you truly need it */
extern void list_trim( List *restrict list) {
	assert( list != nullptr);

	if (list->capacity == list->length) return;
	
	void *trim = calloc( list->length, TSIZE);
	assert( trim != nullptr);

	memcpy( trim, ITEMS, list->length*TSIZE);
	free( ITEMS);
	ITEMS = trim;

	list->capacity = list->length;
}

/* Frees list memory used for items. After this, length and capacity
 * are set to 0 and the list should be unusable */
extern inline void list_free( List *list) {
	assert( list != nullptr);

	free( ITEMS);
	list->length = 0, list->capacity = 0;
}


/* Sorts items of 'list' with comparator function 'func' */
extern inline void list_sort( List *restrict list, Comparator func) {
	assert( list != nullptr);
	assert( func != nullptr);

	qsort( ITEMS, list->length, list->typesize, func);
}


/* Copy list data to 'dest'. Make sure that 'dest' has enough
 * memory to store list items */
extern void list_toarray( List *list, void* restrict dest) {
	assert( list != nullptr);
	assert( dest != nullptr);

	memcpy( dest, ITEMS, list->length*TSIZE);
}


/* Add the items of 'other' to 'list'. Both vectors must have same item type.
 * If concatenation is successful, returns true */
extern bool list_concat( List *restrict list, const List *restrict other) {
	assert(list != nullptr);
	assert(other != nullptr);

	// Exit if sizes are different
	if (TSIZE != other->typesize) return false;
	// Index of concatenation
	size_t concat_pos = list->length * TSIZE;
	
	list->length += other->length;
	if (list->length > list->capacity) 
		list_expand(list, list->length);
	// Copy items
	memcpy( ITEMS + concat_pos, other->items, other->length*TSIZE);

	return true;
}


/* Returns a new list with a portion of 'list' items */
extern List list_slice( List *restrict list, const size_t start, const size_t end) {
	assert(list != nullptr);
	assert( end > start);

	size_t delta = end - start;
	List slice = __devkit_new_list( TSIZE, delta);
	// Copy data to slice
	void *src = ITEMS + start * TSIZE;
	memcpy( slice.items, src, delta*TSIZE);
	slice.length = delta;

	return slice;
}


/* Returns a new iterable associated to the list.
 * NOTE: this iterable has direct REFERENCES to the list data! Be careful!*/
extern inline Iterable list_asiterable( List *list) {
	assert( list != nullptr);
	return (Iterable) { TSIZE, list->length, ITEMS};
}


#undef ITEMS
#undef TSIZE

#endif
