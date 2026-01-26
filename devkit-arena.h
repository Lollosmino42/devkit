#ifndef _DEVKIT_ARENA_H
#define _DEVKIT_ARENA_H

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
	bool noreset;
	size_t size;
	size_t cursor;
	void *data;
} DevkitArena;

#ifdef DEVKIT_STRIP_PREFIXES
typedef DevkitArena Arena;
#endif


extern DevkitArena devkit_arena_new( size_t size, bool noreset); // Constructor

/* Reserve 'size' bytes of memory to a new pointer */
extern void* devkit_arena_alloc( DevkitArena *arena, size_t size);
/* Reserve a cluster of 'nmemb'*'size' bytes of memory to a new pointer */
extern void* devkit_arena_calloc( DevkitArena *arena, size_t nmemb, size_t size);
/* Reset arena cursor to zero */
extern void devkit_arena_reset( DevkitArena *arena);
extern void devkit_arena_destroy( DevkitArena *arena);

/* WARNING: this function moves the cursor back to 'ptr' in 'arena',
 * basically invalidating every other ptr allocated after 'ptr'.
 * Use cautiously to deallocate only what you want to. */
extern void devkit_arena_free( DevkitArena *arena, void* ptr, size_t size);




/* IMPLEMENTATION */

#define DEVKIT_ARENA_IMPLEMENTATION
#ifdef DEVKIT_ARENA_IMPLEMENTATION

DevkitArena devkit_arena_new( size_t size, bool noreset) {
	void *data = malloc( size);
	return (DevkitArena) {
		.size = size,
		.cursor = 0,
		.data = data,
		.noreset = noreset
	};
}


void* devkit_arena_calloc( DevkitArena *arena, size_t nmemb, size_t size) {
	size_t totalsize = nmemb*size;
	if ( totalsize > arena->size - arena->cursor) {
		if ( arena->noreset) {
			puts("DevkitArena has run out of memory and cannot reset!");
			exit(EXIT_FAILURE);
		}
		else devkit_arena_reset( arena);
	}
	// Allocation
	void *newptr = arena->data + arena->cursor;
	memset( newptr, 0, totalsize);
	arena->cursor += totalsize;
	return newptr;
}


void* devkit_arena_alloc( DevkitArena *arena, size_t size) {
	if ( size > arena->size - arena->cursor) {
		if ( arena->noreset) {
			puts("DevkitArena has run out of memory and cannot reset!");
			exit(EXIT_FAILURE);
		}
		else devkit_arena_reset( arena);
	}

	void *newptr = arena->data + arena->cursor;
	arena->cursor += size;
	return newptr;
}


void devkit_arena_destroy( DevkitArena *arena) {
	if (!arena) return;

	free( arena->data);
	arena->cursor = 0, arena->size = 0;
}


void devkit_arena_free( DevkitArena *arena, void* ptr, size_t size) {
	size_t delta = ptr - arena->data;
	assert( delta >= 0 && "DevkitArena: Cannot deallocate at address outside of buffer!!!");
	arena->cursor = delta;
}

void devkit_arena_reset( DevkitArena *arena) {
	assert( !arena->noreset && "DevkitArena: called 'reset' action on a buffer flagged as 'noreset'!!!");
	arena->cursor = 0;
}

#endif

#endif
