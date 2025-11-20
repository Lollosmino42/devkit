#pragma once

#include <stdlib.h>
#include <string.h>

#include "sort.h"

#include <stdio.h>

extern bool __devkit_contains( 
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


/* Contains function for any array */
#define contains( array, len, var) \
	__devkit_contains( array, len, sizeof(array[0]), &(var))

/* Contains function macro designed for list */
#define list_contains( list, var) \
	__devkit_contains( (list).items, (list).length, (list).typesize, &var)


/* Instead of reallocating the ptr directly, it allocates
 * a larger chunk to copy the data into, then deallocates the old chunk */
extern void* safe_realloc( void* ptr, size_t oldmem, size_t newmem) {
	void* copy = malloc( newmem);
	if (copy) {
		memcpy( copy, ptr, oldmem);
		free(ptr);
		return copy;
	}
	else {
		return nullptr;
	}
}


/*
	-----------
	LIST STRUCT
	-----------
*/

/* A more comprehensible approach to arrays in C. */
typedef struct {
	size_t typesize;
	size_t capacity;
	size_t length;
	void **items;
} List;


#define PTR_SIZE ( sizeof(void*) )


/* Initializer macro */
#define new_list( type, capacity) \
	__devkit_new_list( sizeof(type), capacity)

extern List __devkit_new_list( size_t typesize, size_t capacity) {
	return (List) { typesize, capacity, 0, calloc( typesize, capacity)};
}


/* Get item at 'index' of 'type' in 'list' */
#define list_get( type, list, index) *(type*) (list).items[index]


// For ease of coding
#define TSIZE list->typesize 
#define ITEMS list->items


/* Allocate more space for 'list' to increase its capacity to 'new_capacity' */
extern void list_expand( List *list, size_t new_capacity) {
	void* new_items = calloc( new_capacity, PTR_SIZE);
	memcpy( new_items, ITEMS, list->length*PTR_SIZE);
	free( ITEMS);
	ITEMS = new_items;
	list->capacity = new_capacity;
}


/* Set item at 'index' of 'list' to 'value', if index is in bounds */
extern void list_set( List *list, size_t index, void *const value) {
	if (index < list->length && index >= 0) {
		memcpy( ITEMS[index*TSIZE], value, TSIZE);
		return;
	}
}


/* Append one item to 'list' */
#define list_append( list, var) list_nappend( list, 1, &var)

/* Append 'nitems' ITEMS from 'values' to 'list' */
extern void list_nappend( List *restrict list, size_t nitems, void *const values) {
	size_t ptr = list->length; // Needed later
	list->length += nitems;

	// Allocate more memory if length increases beyond capacity
	if ( list->length > list->capacity) list_expand( list, list->length);
	// Copy values in pointers
	for (size_t val = 0; val < nitems; val++, ptr++ ) {
		ITEMS[ptr] = malloc( TSIZE);
		memcpy( ITEMS[ptr], values + val*TSIZE, TSIZE);
	}
}


/* Insert 'value' in 'list' at 'index' */
#define list_insert( list, index, var) \
	list_ninsert( list, index, 1, &var)

/* Insert 'nitems' of 'values' in 'list' at 'index' */
extern void list_ninsert( List *list, size_t index, size_t nitems, void *values) {
	list->length += nitems;

	// Allocate more memory if needed
	if (list->length > list->capacity) list_expand( list, list->length);
	// Move following items forward, if there are any
	if (index < list->length) memmove( &ITEMS[index+nitems], &ITEMS[index], PTR_SIZE*nitems);

	// Insert values at index
	for (size_t offset = 0; offset < nitems; offset++) {
		ITEMS[index + offset] = malloc( TSIZE);
		memcpy( ITEMS[index + offset], values + offset*TSIZE, TSIZE);
	}
}


/* Remove item at 'index' from 'list' */
extern void* list_remove( List *list, size_t index) {
	void *removed = malloc( PTR_SIZE);
	removed = ITEMS[index];

	// If the item isn't last, every following item must be shifted backwards.
	if (index != --list->length) {
		void **dest = &ITEMS[index];
		void **src = &ITEMS[index+1];
		memmove( dest, src, PTR_SIZE * (list->length - 1 - index) );
	}
	// Deallocation of removed item
	free(ITEMS[list->length + 1]);

	return removed;
}


/* Remove 'nitems' items at 'indices' in 'list' */
extern void* list_nremove( List *list, const size_t nitems, const size_t indices[]) {
	void* removed = calloc( nitems, TSIZE);

	for (size_t item = 0; item < nitems; item++) {
		size_t index = indices[item] - item;
		memcpy( removed + item*TSIZE, ITEMS[index], TSIZE);
		
		// If the item isn't last, every following item must be shifted backwards.
		if ( index != --list->length) {
			void **dest = &ITEMS[index], 
				 **src = &ITEMS[index+1];
			memmove( dest, src, PTR_SIZE * (list->length - index - 1));
		}
		// Deallocate removed item
		free( ITEMS[list->length + 1]);
	}

	return removed;
}


/* Reduce list capacity to its length. This make the list deallocate unneeded memory,
 * so use it only when you truly need it*/
extern void list_trim( List *restrict list) {
	if (list->capacity == list->length) return;
	
	ITEMS = safe_realloc( ITEMS, list->capacity * PTR_SIZE, list->length * PTR_SIZE);
	list->capacity = list->length;
}


/* Deallocates all items inside list to avoid memory leaks */
extern void list_free( List *list) {
	// Free every item pointer
	for (size_t item = 0; item < list->length; item++) {
		free( ITEMS[item]);
	}
	// Free array pointer
	free( ITEMS);
}



#undef PTR_SIZE
#undef ITEMS
#undef TSIZE
