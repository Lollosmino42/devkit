#ifndef _DEVKIT_ARENA_H
#define _DEVKIT_ARENA_H

#if defined(__STDC__) && __STDC_VERSION__ < 202311L
#define nullptr NULL
#include <stdbool.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


/* 
 * ################
 * # DEKVIT ARENA #
 * ################
 */

/* Struct definition */

typedef struct {
	void *data;
	size_t size;
	size_t cursor;
	bool noreset;
} DktArena;

#ifdef DEVKIT_STRIP_PREFIXES
typedef DktArena Arena;

#define arena_new    	dkt_arena_new
#define arena_alloc   	dkt_arena_alloc
#define arena_reset   	dkt_arena_reset
#define arena_destroy 	dkt_arena_destroy
#define arena_free    	dkt_arena_free
#endif


extern DktArena dkt_arena_new( size_t size, bool noreset); // Constructor

/* Reserve 'size' bytes of memory to a new pointer */
extern void* dkt_arena_alloc( DktArena *arena, size_t size);
/* Reset arena cursor to zero */
extern void dkt_arena_reset( DktArena *arena);
extern void dkt_arena_destroy( DktArena *arena);

/* 
 * NOTE: this function moves the cursor back to 'ptr' in 'arena',
 * basically invalidating every other ptr allocated after 'ptr'.
 * Use cautiously to deallocate only what you want to. 
 */
extern void dkt_arena_free( DktArena *arena, void* ptr, size_t size);




/* IMPLEMENTATION */

//#define DEVKIT_ARENA_IMPLEMENTATION
#ifdef DEVKIT_ARENA_IMPLEMENTATION

DktArena dkt_arena_new( size_t size, bool noreset) {
	void *data = malloc( size);
	return (DktArena) {
		.size = size,
		.cursor = 0,
		.data = data,
		.noreset = noreset
	};
}


void* dkt_arena_alloc( DktArena *arena, size_t size) {
	if ( size > arena->size - arena->cursor) {
		if ( arena->noreset) {
			puts("DktArena has run out of memory and cannot reset!");
			return nullptr;
		}
		else dkt_arena_reset( arena);
	}

	void *newptr = arena->data + arena->cursor;
	memset( newptr, 0, size);
	arena->cursor += size;
	return newptr;
}


void dkt_arena_destroy( DktArena *arena) {
	if (!arena) return;

	free( arena->data);
	arena->cursor = 0, arena->size = 0;
}


void dkt_arena_free( DktArena *arena, void* ptr, size_t size) {
	size_t delta = ptr - arena->data;
	assert( delta >= 0 && "DktArena: Can not free address outside of buffer!!!");
	arena->cursor = delta;
}

void dkt_arena_reset( DktArena *arena) {
	assert( !arena->noreset && "DktArena: called 'reset' action on a buffer flagged as 'noreset'!!!");
	arena->cursor = 0;
}

#endif

#endif
