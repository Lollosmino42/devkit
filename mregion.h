#ifndef _DEVKIT_MREGION_H
#define _DEVKIT_MREGION_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>


typedef struct {
	bool no_reset;
	size_t size;
	size_t cursor;
	void *data;
} MRegion;


extern MRegion mregion_new( size_t size, bool noreset); // Constructor

extern void* mregion_alloc( MRegion *mregion, size_t size);
extern void* mregion_calloc( MRegion *mregion, size_t nmemb, size_t size);
extern void mregion_reset( MRegion *mregion);
extern void mregion_destroy( MRegion *mregion);

/* WARNING: this function moves the cursor back to 'ptr' in 'mregion',
 * basically invalidating every other ptr allocated after 'ptr'.
 * Use cautiously to deallocate only what you want to. */
extern void mregion_free( MRegion *mregion, void* ptr, size_t size);




/* IMPLEMENTATION */

#define MREGION_IMPLEMENTATION
#ifdef MREGION_IMPLEMENTATION

MRegion mregion_new( size_t size, bool noreset) {
	void *data = malloc( size);
	return (MRegion) {
		.size = size,
		.cursor = 0,
		.data = data,
		.no_reset = noreset
	};
}


void* mregion_calloc( MRegion *mregion, size_t nmemb, size_t size) {
	size_t totalsize = nmemb*size;
	if ( totalsize > mregion->size - mregion->cursor) {
		if ( mregion->no_reset) {
			puts("MRegion has run out of memory and cannot reset!");
			exit(EXIT_FAILURE);
		}
		else mregion_reset( mregion);
	}
	// Allocation
	void *newptr = mregion->data + mregion->cursor;
	memset( newptr, 0, totalsize);
	mregion->cursor += totalsize;
	return newptr;
}


void* mregion_alloc( MRegion *mregion, size_t size) {
	if ( size > mregion->size - mregion->cursor) {
		if ( mregion->no_reset) {
			puts("MRegion has run out of memory and cannot reset!");
			exit(EXIT_FAILURE);
		}
		else mregion_reset( mregion);
	}

	void *newptr = mregion->data + mregion->cursor;
	mregion->cursor += size;
	return newptr;
}


void mregion_destroy( MRegion *mregion) {
	if (!mregion) return;

	free( mregion->data);
	mregion->cursor = 0, mregion->size = 0;
}


void mregion_free( MRegion *mregion, void* ptr, size_t size) {
	size_t delta = ptr - mregion->data;
	assert( delta >= 0 && "MRegion: Cannot deallocate at address outside of buffer!!!");
	mregion->cursor = delta;
}

void mregion_reset( MRegion *mregion) {
	assert( !mregion->no_reset && "MRegion: called 'reset' action on a buffer flagged as 'no_reset'!!!");
	mregion->cursor = 0;
}

#endif

#endif
