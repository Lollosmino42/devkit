#ifndef __DEVKIT_SETTINGS_H
#define __DEVKIT_SETTINGS_H


/* 
 * #######################
 * # SETTINGS FOR DEVKIT #
 * #######################
 */

/*
 * ENV settings of devkit 
 */
#define __DEVKIT_BACKWARDS_COMPATABILITY 0
#define __DEVKIT_USE_CUSTOM_ALLOCATOR 0
#define __DEVKIT_CUSTOM_ALLOCATOR_HEADER "mregion.h"
#define __DEVKIT_CUSTOM_ALLOCATOR_TYPE MRegion
#define __DEVKIT_EXTRA_ITERABLES 0


#if __DEVKIT_EXTRA_ITERABLES

/*
 * ###################
 * # EXTRA ITERABLES #
 * ###################
 */


/* 
 * To make a struct iterable, enable __DEVKIT_EXTRA_ITERABLES,
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
#define __DEVKIT_ITERABLES \
	// ADD YOUR ITERABLES HERE

#endif



#ifdef __DEVKIT_ITERABLES
#define DEVKIT_ITERABLES __DEVKIT_ITERABLES, default: nullptr
#else
#define DEVKIT_ITERABLES default: nullptr
#endif
#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#include __DEVKIT_CUSTOM_ALLOCATOR_HEADER
typedef __DEVKIT_CUSTOM_ALLOCATOR_TYPE DEVKIT_ALLOCATOR;


/* Custom allocator definition.
 * Set the macros accordingly */

// alloc stands for 'allocator'

#define DEVKIT_MALLOC( alloc, size)			mregion_malloc	((alloc), (size) )
#define DEVKIT_CALLOC( alloc, nmemb, size)	mregion_calloc	((alloc), (nmemb), (size) )
#define DEVKIT_RESET( alloc)				mregion_reset	((alloc))
#define DEVKIT_FREE( alloc, ptr, size)		mregion_free	((alloc), (ptr), (size))

/* Custom allocator end */

/* 
 * ################
 * # SETTINGS END #
 * ################
 */


#else

#include <stdlib.h>
#include <stddef.h>

/* Standard library allocation */

typedef nullptr_t DEVKIT_ALLOCATOR;

#define DEVKIT_MALLOC( _, size) malloc( size)
#define DEVKIT_CALLOC( _, nmemb, size) calloc( (nmemb), (size))
#define DEVKIT_RESET(_) 
#define DEVKIT_FREE(_, ptr, __) free

/* stdlib end */

#endif

#endif /* END settings.h */
