#ifndef _DEVKIT_ITERABLE_H
#define _DEVKIT_ITERABLE_H

#include <stddef.h>

#include "../settings.h"

/* NOTE: this header should not be included on its own as it
 * is useless without _devkit_iterable structs */

/* Map: a function that takes a parameter and does something with it */
typedef void (*Map)(void*);


typedef struct devkit_iterable {
	void *items;
	size_t length;
	size_t typesize;
	size_t counter;
	DEVKIT_ALLOCATOR *alloc;
} Iterable;

/* Bypass for Iterable in generic selection of _devkit_iterable. With this,
 * pointers can be converted to iterables and used in foreach loops */
extern inline Iterable devkit_dummy_asiterable(DEVKIT_ALLOCATOR *alloc, Iterable *iter) {
	return *iter;
}


#include "array_struct.h"
#include "list_struct.h"
#include "math_struct.h"

/* Cast to _devkit_iterable.
 * Works with Arrays, Lists, and other structures defined in devkit that have
 * a <...>_asiterable function.
 * Other structures can be compatible with 'foreach' if an 'asiterable'-like function
 * is defined for them and if they are added in the 'settings.h' file in the macro
 * '__DEVKIT_EXTRA_ITERABLES' */
#define _devkit_iterable( structure) _Generic( (structure), \
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

DEVKIT_LOOP_POOL _DEVKIT_POOL = (DEVKIT_LOOP_POOL) {.loops = nullptr};


extern void _devkit_loop_pool_destroy() {
	free( _DEVKIT_POOL.loops);
}

extern void _devkit_loop_pool_init() {
	if (!_DEVKIT_POOL.loops) {
		_DEVKIT_POOL = (DEVKIT_LOOP_POOL) {
			.loops = calloc( 4, sizeof(Iterable*)),
			.length = 0,
			.capacity = 4
		};
		atexit( _devkit_loop_pool_destroy);
	}
}

#define _devkit_loop_current (_DEVKIT_POOL.loops[_DEVKIT_POOL.length - 1])


extern inline void __devkit_expand_loop_pool( size_t increment) {
	_DEVKIT_POOL.capacity += increment;
	_DEVKIT_POOL.loops = realloc( _DEVKIT_POOL.loops, _DEVKIT_POOL.capacity);
}

extern inline void _devkit_loop_new( Iterable *iter) {
	if ( _DEVKIT_POOL.length + 1 >= _DEVKIT_POOL.capacity)
		__devkit_expand_loop_pool( _DEVKIT_POOL.capacity);

	_DEVKIT_POOL.loops[_DEVKIT_POOL.length++] = iter;
}


#define _devkit_loop_close \
	if ( _DEVKIT_POOL.length != 0) _DEVKIT_POOL.length--;


#define foreach( type, var, iter, ...) \
	foreach_in( type, var, iter, 0, -1, __VA_ARGS__)
	  
#define foreach_in( type, var, iter, start, end, ...) do { \
	_devkit_loop_pool_init(); \
	_devkit_loop_new( (Iterable[]){ _devkit_iterable(iter)} ); \
	if (end > 0) _devkit_loop_current->length = end; \
	type var; \
	for (_devkit_loop_current->counter = (start>=0) ? start : 0; _devkit_loop_current->counter < _devkit_loop_current->length; _devkit_loop_current->counter++) { \
		var = ((type*) _devkit_loop_current->items)[_devkit_loop_current->counter]; \
		__VA_ARGS__; \
		memcpy( ((type*) _devkit_loop_current->items)+_devkit_loop_current->counter, &var, _devkit_loop_current->typesize); \
	} \
	_devkit_loop_close; \
} while (0)



#endif
