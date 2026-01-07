#ifndef __DEVKIT_LIST_H
#define __DEVKIT_LIST_H

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

// Prefix stripping

#if __DEVKIT_USE_CUSTOM_ALLOCATOR

#define list_new( alloc, type, capacity) devkit_list_new( (alloc), sizeof(type), (capacity))
#define list_newptr( alloc, type, capacity) devkit_list_newptr( (alloc), sizeof(type), (capacity))
#define list_fromptr( alloc, length, ptr) devkit_list_from( (alloc), sizeof((ptr)[0]), (length), (ptr) )

#define list_remove devkit_list_remove
#define list_nremove devkit_list_nremove
#define list_slice devkit_list_slice
#define list_asiterable devkit_list_asiterable

#else

#define list_new( type, capacity) devkit_list_new( nullptr, sizeof(type), (capacity))
#define list_newptr( type, capacity) devkit_list_newptr( nullptr, sizeof(type), (capacity))
#define list_fromptr( length, ptr) devkit_list_from( nullptr, sizeof((ptr)[0]), (length), (ptr) )
#define list_fromarray( array) devkit_list_from( (array).typesize, (array).length, (array).items)
#define list_fromstr( str) devkit_list_from( 1, strlen(str), (str))

#define list_remove( list, index) devkit_list_remove( nullptr, (list), (index))
#define list_nremove( list, nitems, indices) devkit_list_nremove( nullptr,  (list), (nitems), (index))
#define list_slice( list, start, end) devkit_list_slice( nullptr,  (list), (start), (end))
#define list_asiterable( list) devkit_list_asiterable( nullptr, (list))

#endif

#define list_contains( list, var) devkit_contains( (list).items, (list).length, (list).typesize, &(var))
// Get reference of item in list
#define list_getref( list, index) ( (list).items + (index)*(list).typesize )
#define list_get( type, list, index) *(type*) list_getref( (list), (index) )
extern void* list_getitems( List *list);

extern void list_set( List *list, size_t index, void *const value);
#define list_add( list, var) list_nadd( (list), 1, (var))
extern void list_nadd( List *restrict list, size_t nitems, void *const values);
#define list_insert( list, index, var) list_ninsert( (list), (index), 1, (var))
extern void list_ninsert( List *list, size_t index, size_t nitems, void *values);
extern inline void list_sort( List *restrict list, Comparator func);
extern bool list_concat( List *restrict dest, const List *restrict src);

/* NOTE: 'expand', 'trim' and 'free' are disabled when using region allocations because it
 * causes unnecessary complications.
 * If your allocator supports this kind of task,
 * there is a setting for this (settings.h) */

// Raw declarations
extern List devkit_list_new( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t capacity);
extern List* devkit_list_newptr( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t capacity);
extern List devkit_list_from( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t nitems, void* items);
extern void* devkit_list_remove( DEVKIT_ALLOCATOR *alloc, List *list, size_t index);
extern void* devkit_list_nremove( DEVKIT_ALLOCATOR *alloc, List *list, const size_t nitems, const size_t indices[]);
extern List devkit_list_slice( DEVKIT_ALLOCATOR *alloc, List *restrict list, const size_t start, const size_t end);

extern void list_expand( List *list, size_t new_capacity);
extern void list_trim( List *restrict list);
extern void list_free( List *list);
extern void list_toarray( List *list, void* restrict dest);


/* IMPLEMENTATION START */

#ifdef DEVKIT_LIST_IMPLEMENTATION

/* List initializer */
extern List devkit_list_new( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t capacity) {
	return (List) { 
		.alloc=alloc,
		.typesize=typesize, 
		.capacity=capacity, 
		.length=0, 
		.items=DEVKIT_CALLOC( alloc, typesize, capacity)
	};
}

extern List* devkit_list_newptr( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t capacity) {
	List *list = DEVKIT_MALLOC( alloc, sizeof(List) + typesize*capacity);
	list->alloc = alloc;
	list->capacity = capacity;
	list->typesize = typesize;
	list->length = 0;
	list->items = list + sizeof(List);
	memset( list->items, 0, typesize*capacity);
	return list;
}


/* Returns a non-empty list with 'items' in it */
extern List devkit_list_from( DEVKIT_ALLOCATOR *alloc, size_t typesize, size_t nitems, void* items) {
	assert(items != nullptr);

	List list = { 
		.alloc=alloc, 
		.typesize=typesize, 
		.capacity=nitems, 
		.length=nitems, 
		.items=(alloc == nullptr)
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
void list_expand( List *list, size_t new_capacity) {
	assert( list != nullptr);

	size_t prev_size = list->capacity*TSIZE;
	char buf[ new_capacity*TSIZE];
	memcpy( buf, ITEMS, prev_size);
	DEVKIT_FREE( list->alloc, ITEMS, prev_size);

	void *new_items = DEVKIT_CALLOC( list->alloc, new_capacity, TSIZE);
	memcpy( new_items, buf, prev_size);
	ITEMS = new_items;
	list->capacity = new_capacity;
}


/* Returns a copy of the items */
void* list_copyitems( DEVKIT_ALLOCATOR *alloc, List *list) {
	void *copy = DEVKIT_MALLOC( alloc, list->length*TSIZE);
	memcpy( copy, list->items, list->length*TSIZE);
	return copy;
}


/* Set item at 'index' of 'list' to 'value', if index is in bounds.
 * 'value' is COPIED to the list, so it is not modified by the list */
void list_set( List *list, size_t index, void *const value) {
	assert( list != nullptr && value != nullptr);

	if (index < list->length && index >= 0) {
		memcpy( ITEMS + index*TSIZE, value, TSIZE);
		return;
	}
}


// Add 'nitems' ITEMS from 'values' to 'list'
void list_nadd( List *restrict list, size_t nitems, void *const values) {
	assert( list != nullptr && values != nullptr);

	size_t ptr = list->length; // Needed later
	list->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( list->length > list->capacity) list_expand( list, list->length);
	// Copy values in pointers
	memcpy( ITEMS + (list->length - nitems)*TSIZE, values, nitems*TSIZE);
}


// Insert 'nitems' of 'values' in 'list' at 'index'
void list_ninsert( List *list, size_t index, size_t nitems, void *values) {
	assert( list != nullptr && values != nullptr);

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
void* devkit_list_remove( DEVKIT_ALLOCATOR *alloc, List *list, size_t index) {
	assert( list != nullptr);

	void *removed = DEVKIT_MALLOC( alloc, TSIZE);
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
void* devkit_list_nremove( DEVKIT_ALLOCATOR *alloc, List *list, const size_t nitems, const size_t indices[]) {
	assert( list != nullptr && indices != nullptr);

	void* removed = DEVKIT_CALLOC( alloc, nitems, TSIZE);
	assert( removed != nullptr);

	size_t sorted[nitems];
	memcpy( sorted, indices, sizeof(size_t)*nitems);
	qsort( sorted, nitems, sizeof(size_t), i64cmp);

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
void list_trim( List *restrict list) {
	assert( list != nullptr);

	if (list->capacity == list->length) return;
	
	char buf[list->length];
	memcpy( buf, ITEMS, list->length*TSIZE);
	DEVKIT_FREE( list->alloc, ITEMS, list->capacity*TSIZE);

	void *trim = DEVKIT_MALLOC( list->alloc, list->length * TSIZE);
	assert( trim != nullptr), memcpy( trim, buf, list->length*TSIZE);

	ITEMS = trim;
	list->capacity = list->length;
}

/* Frees list memory used for items. After this, length and capacity
 * are set to 0 and the list should be unusable */
void list_free( List *list) {
	assert( list != nullptr);
	list->length = 0, list->capacity = 0;
	if (list->alloc == nullptr) free( ITEMS);
	else DEVKIT_FREE( list->alloc, ITEMS, list->capacity*TSIZE);
}


/* Sorts items of 'list' with comparator function 'func' */
inline void list_sort( List *restrict list, Comparator func) {
	assert( list != nullptr);
	assert( func != nullptr);

	qsort( ITEMS, list->length, list->typesize, func);
}


/* Copy list data to 'dest'. Make sure that 'dest' has enough
 * memory to store list items */
void list_toarray( List *list, void* restrict dest) {
	assert( list != nullptr);
	assert( dest != nullptr);

	memcpy( dest, ITEMS, list->length*TSIZE);
}


/* Add the items of 'src' to 'dest'. Lists must have same item type.
 * If concatenation is successful, returns true */
bool list_concat( List *restrict list, const List *restrict other) {
	assert(list != nullptr);
	assert(other != nullptr);

	// Exit if sizes are different
	if ( TSIZE != other->typesize) return false;
	// Index of concatenation
	size_t concat_pos = list->length * TSIZE;
	list->length += other->length;

	if ( list->length > list->capacity) list_expand(list, list->length);
	// Copy items
	memcpy( ITEMS + concat_pos, other->items, other->length*TSIZE);

	return true;
}


/* Returns a new list with a portion of 'list' items */
List devkit_list_slice( DEVKIT_ALLOCATOR *alloc, List *restrict list, const size_t start, const size_t end) {
	assert(list != nullptr);
	assert( end > start);

	size_t delta = end - start;
	List slice = devkit_list_new( alloc, TSIZE, delta);
	// Copy data to slice
	void *src = ITEMS + start * TSIZE;
	memcpy( slice.items, src, delta*TSIZE);
	slice.length = delta;

	return slice;
}

#endif

#undef ITEMS
#undef TSIZE

#endif
