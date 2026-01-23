#ifndef _DEVKIT_SETTINGS_H
#define _DEVKIT_SETTINGS_H


/* 
 * #################################
 * # SETTINGS FOR DEVKIT ITERABLES #
 * #################################
 */


// Enable support for custom iterables
#define DEVKIT_EXTRA_ITERABLES 0
// and add them below

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
	} DevkitIterable;
*/

/* 
 * To make a structure iterable, enable DEVKIT_EXTRA_ITERABLES,
 * then make a function as such:
 *
 * DevkitIterable <func_name>( <struct_t> *<struct>)
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
	/* ADD YOUR ITERABLES HERE */

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

#endif /* END settings.h */
