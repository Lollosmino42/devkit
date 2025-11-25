#pragma once

#include <stdlib.h>
#include <string.h>

#include "comparators.h"


/* Returns 'true' if 'array' contains 'value' */
extern bool __devkit_contains( 
					const void *const array, 
					const size_t len, 
					const size_t typesize, 
					const void *value
				) {
	for ( size_t idx = 0; idx < len; idx++) {
		if ( memcmp( array + idx*typesize, value, typesize) == 0)
			return true;
	}
	return false;
}


/* Contains function for any array */
#define contains( array, len, var) \
	__devkit_contains( array, len, sizeof(array[0]), &(var))

/* Contains function macro designed for list */
#define list_contains( list, var) \
	__devkit_contains( (list).items, (list).length, (list).typesize, &(var))


/*
	-----------
	VECTOR STRUCT
	-----------
*/

/* A more comprehensible approach to arrays in C. */
typedef struct {
	const size_t typesize;
	size_t capacity;
	size_t length;
	void *items;
} List;


/* Initializer macro */
#define new_list( type, capacity) \
	__devkit_new_list( sizeof(type), capacity)

extern List __devkit_new_list( size_t typesize, size_t capacity) {
	return (List) { typesize, capacity, 0, calloc( typesize, capacity)};
}


/* Get item at 'index' of 'type' in 'list' */
#define list_get( type, list, index) *(type*) ((list).items + index*(list).typesize)


// For ease of coding
#define TSIZE list->typesize 
#define ITEMS list->items


/* Allocate more space for 'list' to increase its capacity to 'new_capacity' */
extern void list_expand( List *list, size_t new_capacity) {
	void* new_items = calloc( new_capacity, TSIZE);
	memcpy( new_items, ITEMS, list->length*TSIZE);
	free( ITEMS);
	ITEMS = new_items;
	list->capacity = new_capacity;
}


/* Set item at 'index' of 'list' to 'value', if index is in bounds.
 * 'value' is COPIED to the list, so it is not modified by the list */
extern void vector_set( List *list, size_t index, void *const value) {
	if (index < list->length && index >= 0) {
		memcpy( ITEMS + index*TSIZE, value, TSIZE);
		return;
	}
}


/* Append one item to 'list' */
#define list_add( list, var) list_nadd( list, 1, var)

/* Add 'nitems' ITEMS from 'values' to 'list' */
extern void list_nadd( List *restrict list, size_t nitems, void *const values) {
	size_t ptr = list->length; // Needed later
	list->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( list->length > list->capacity) list_expand( list, list->length);
	// Copy values in pointers
	memcpy( ITEMS + (list->length - nitems)*TSIZE, values, nitems*TSIZE);
}


/* Insert 'value' in 'list' at 'index' */
#define list_insert( list, index, var) \
	list_ninsert( list, index, 1, var)

/* Insert 'nitems' of 'values' in 'list' at 'index' */
extern void list_ninsert( List *list, size_t index, size_t nitems, void *values) {
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
	void* removed = calloc( nitems, TSIZE);

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
	if (list->capacity == list->length) return;
	
	void *trim = calloc( list->length, TSIZE);
	memcpy( trim, ITEMS, list->length*TSIZE);
	free( ITEMS);
	ITEMS = trim;

	list->capacity = list->length;
}

/* Frees list memory used for items. After this, length and capacity
 * are set to 0 */
extern inline void list_free( List *list) {
	free( ITEMS);
	list->length = list->capacity = 0;
}


/* Sorts items of 'list' with comparator function 'func' */
extern inline void list_sort( List *restrict list, Comparator func) {
	qsort( ITEMS, list->length, list->typesize, func);
}


/* Copy list data to 'dest'. Make sure that 'dest' has enough
 * memory to store list items */
extern void list_toarray( List *list, void* restrict dest) {
	memcpy( dest, ITEMS, list->length*TSIZE);
}


/* Create new list from existing array */
#define list_fromarray( array, length) \
	__devkit_vector_fromarray( (array), sizeof((array)[0]), (length) )

extern List __devkit_list_fromarray( void* restrict array, size_t typesize, size_t length) {
	List list = { typesize, length, length, calloc( length, typesize) };
	memcpy( list.items, array, typesize*length);
	return list;
}


/* Add the items of 'other' to 'list'. Both vectors must have same item type.
 * If concatenation is successful, returns true */
extern bool list_concat( List *restrict list, const List *restrict other) {
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

#include <assert.h>

/* Returns a new list with a portion of 'list' items */
extern List list_slice( List *restrict list, const size_t start, const size_t end) {
	assert( end > start);

	size_t delta = end - start + 1;
	List slice = __devkit_new_list( TSIZE, delta);
	// Copy data to slice
	void *src = ITEMS + start * TSIZE;
	memcpy( slice.items, src, delta*TSIZE);
	slice.length = delta;

	return slice;
}


#undef ITEMS
#undef TSIZE
