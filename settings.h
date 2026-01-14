#ifndef _DEVKIT_SETTINGS_H
#define _DEVKIT_SETTINGS_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ <= 201710L
#define nullptr_t void*
#define nullptr NULL
#include <stdbool.h>
#endif

/* 
 * #######################
 * # SETTINGS FOR DEVKIT #
 * #######################
 */

/*
 * ENV settings of devkit 
 */

#define DEVKIT_STRIP_PREFIXES 1
// Use a custom memory allocator
#define DEVKIT_USE_CUSTOM_ALLOCATOR 1
	// If using one, allocator header file and struct name
	#define _DEVKIT_CUSTOM_ALLOCATOR_HEADER "mregion.h"
	#define _DEVKIT_CUSTOM_ALLOCATOR_TYPE MRegion
	// --------------------------------------------------
	#if DEVKIT_USE_CUSTOM_ALLOCATOR
	#include _DEVKIT_CUSTOM_ALLOCATOR_HEADER
	typedef _DEVKIT_CUSTOM_ALLOCATOR_TYPE DEVKIT_ALLOCATOR;

/*
 * Macros for custom allocator
 * Define macros accordingly
 */

// alloc stands for 'allocator'
#define DEVKIT_ALLOC( alloc, size)			mregion_alloc	((alloc), (size) )
#define DEVKIT_CALLOC( alloc, nmemb, size)	mregion_calloc	((alloc), (nmemb), (size) )
#define DEVKIT_RESET( alloc)				mregion_reset	((alloc))
#define DEVKIT_FREE( alloc, ptr, size)		mregion_free	((alloc), (ptr), (size))

// Enable support for custom iterables
#define DEVKIT_EXTRA_ITERABLES 0
// and add them below

#endif
#if DEVKIT_EXTRA_ITERABLES

/*
 * ###################
 * # EXTRA ITERABLES #
 * ###################
 */

/* Iterable definition
	typedef struct devkit_iterable {
		void *items;
		size_t length;
		size_t typesize;
		size_t counter; <- ignore this (nothing changes if you touch it, so do not)
		DEVKIT_ALLOCATOR *alloc;
	} Iterable;
*/

/* 
 * To make a structure iterable, enable DEVKIT_EXTRA_ITERABLES,
 * then make a function as such:
 *
 * Iterable <func_name>( DEVKIT_ALLOCATOR *<alloc>, <struct_t> <struct>)
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
	// ADD YOUR ITERABLES HERE

#endif

/* 
 * ################
 * # SETTINGS END #
 * ################
 */

// Anything below should not be touched


#ifdef _DEVKIT_ITERABLES
#define DEVKIT_ITERABLES _DEVKIT_ITERABLES, default: nullptr
#else
#define DEVKIT_ITERABLES default: nullptr
#endif


#if !DEVKIT_USE_CUSTOM_ALLOCATOR

#undef DEVKIT_ALLOC
#undef DEVKIT_CALLOC
#undef DEVKIT_RESET
#undef DEVKIT_FREE

#include <stdlib.h>
#include <stddef.h>

/* Standard library allocation */

typedef nullptr_t DEVKIT_ALLOCATOR;

#define DEVKIT_ALLOC( _, size) malloc( size)
#define DEVKIT_CALLOC( _, nmemb, size) calloc( (nmemb), (size))
#define DEVKIT_RESET(_) 
#define DEVKIT_FREE(_, ptr, __) free

/* stdlib end */

#endif

#endif /* END settings.h */
