#ifndef __DEVKIT_SETTINGS_H
#define __DEVKIT_SETTINGS_H


/* 
 * #######################
 * # SETTINGS FOR DEVKIT #
 * #######################
 */

#define __DEVKIT_USE_CUSTOM_ALLOCATOR 1
#define __DEVKIT_CUSTOM_ALLOCATOR_HEADER "memregion.h"
#define __DEVKIT_CUSTOM_ALLOCATOR_TYPE MemRegion



#if __DEVKIT_USE_CUSTOM_ALLOCATOR

/* Custom allocator definition.
 * Set the macros accordingly */

// alloc stands for 'allocator'
#include __DEVKIT_CUSTOM_ALLOCATOR_HEADER

typedef __DEVKIT_CUSTOM_ALLOCATOR_TYPE DEVKIT_ALLOCATOR;

#define DEVKIT_NEW_ALLOCATOR( size)			new_mregion((size))
#define DEVKIT_MALLOC( alloc, size)			mregion_malloc((alloc), (size) )
#define DEVKIT_CALLOC( alloc, nmemb, size)	mregion_calloc((alloc), (nmemb), (size) )
#define DEVKIT_RESET( alloc)				mregion_reset((alloc))
#define DEVKIT_FREE( alloc, ptr, size)		mregion_free((alloc), (ptr), (size))
#define DEVKIT_DESTROY_ALLOCATOR( alloc)	mregion_destroy( (alloc))

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

#define DEVKIT_NEW_ALLOCATOR(_) nullptr
#define DEVKIT_MALLOC( _, size) malloc( size)
#define DEVKIT_CALLOC( _, nmemb, size) calloc( (nmemb), (size))
#define DEVKIT_RESET(_) 
#define DEVKIT_FREE(_, ptr, __) free
#define DEVKIT_DESTROY_ALLOCATOR(_) 

/* stdlib end */

#endif

#endif /* END settings.h */
