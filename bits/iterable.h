#ifndef __DEVKIT_ITERABLE_H
#define __DEVKIT_ITERABLE_H

#include <stddef.h>

#include "../settings.h"

/* NOTE: this header should not be included on its own as it
 * is useless without __devkit_iterable structs */

/* Map: a function that takes a parameter and does something with it */
typedef void (*Map)(void*);


typedef struct devkit_iterable {
	void *items;
	size_t length;
	size_t typesize;
	size_t counter;
	DEVKIT_ALLOCATOR *alloc;
} Iterable;

/* Bypass for Iterable in generic selection of __devkit_iterable. With this,
 * pointers can be converted to iterables and used in foreach loops */
extern inline Iterable devkit_dummy_asiterable(DEVKIT_ALLOCATOR *alloc, Iterable *iter) {
	return *iter;
}


#include "array_struct.h"
#include "list_struct.h"
#include "math_struct.h"

/* Cast to __devkit_iterable.
 * Works with Arrays, Lists, and other structures defined in devkit that have
 * a <...>_asiterable function.
 * Other structures can be compatible with 'foreach' if an 'asiterable'-like function
 * is defined for them and if they are added in the 'settings.h' file in the macro
 * '__DEVKIT_EXTRA_ITERABLES' */
#define __devkit_iterable( structure) _Generic( (structure), \
		DEVKIT_ITERABLES, \
		struct devkit_array: devkit_array_asiterable, \
		struct devkit_list: devkit_list_asiterable, \
		struct devkit_vector: devkit_vector_asiterable, \
		char*: devkit_str_asiterable, \
		struct devkit_matrix: devkit_matrix_asiterable, \
		Iterable: devkit_dummy_asiterable \
		) ( nullptr, (&(structure)))


typedef struct {
	Iterable **loops;
	size_t length;
	size_t capacity;
} DEVKIT_LOOP_POOL;

DEVKIT_LOOP_POOL __DEVKIT_POOL = (DEVKIT_LOOP_POOL) {.loops = nullptr};


extern void __devkit_loop_pool_destroy() {
	free( __DEVKIT_POOL.loops);
}

extern void __devkit_loop_pool_init() {
	if (!__DEVKIT_POOL.loops) {
		__DEVKIT_POOL = (DEVKIT_LOOP_POOL) {
			.loops = calloc( 4, sizeof(Iterable*)),
			.length = 0,
			.capacity = 4
		};
		atexit( __devkit_loop_pool_destroy);
	}
}

#define __devkit_loop_current (__DEVKIT_POOL.loops[__DEVKIT_POOL.length - 1])


extern inline void __devkit_expand_loop_pool( size_t increment) {
	__DEVKIT_POOL.capacity += increment;
	__DEVKIT_POOL.loops = realloc( __DEVKIT_POOL.loops, __DEVKIT_POOL.capacity);
}

extern inline void __devkit_loop_new( Iterable *iter) {
	if ( __DEVKIT_POOL.length + 1 >= __DEVKIT_POOL.capacity)
		__devkit_expand_loop_pool( __DEVKIT_POOL.capacity);

	__DEVKIT_POOL.loops[__DEVKIT_POOL.length++] = iter;
}


#define __devkit_loop_close \
	if ( __DEVKIT_POOL.length != 0) __DEVKIT_POOL.length--;


#define foreach( iter, type, var, ...) \
	foreach_in( iter, type, var, 0, -1, __VA_ARGS__)
	  
#define foreach_in( iter, type, var, start, end, ...) do { \
	__devkit_loop_pool_init(); \
	__devkit_loop_new( (Iterable[]){ __devkit_iterable(iter)} ); \
	if (end > 0) __devkit_loop_current->length = end; \
	type var; \
	for (__devkit_loop_current->counter = (start>=0) ? start : 0; __devkit_loop_current->counter < __devkit_loop_current->length; __devkit_loop_current->counter++) { \
		var = ((type*) __devkit_loop_current->items)[__devkit_loop_current->counter]; \
		__VA_ARGS__; \
		memcpy( ((type*) __devkit_loop_current->items)+__devkit_loop_current->counter, &var, __devkit_loop_current->typesize); \
	} \
	__devkit_loop_close; \
} while (0)



#endif
