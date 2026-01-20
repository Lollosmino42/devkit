#ifndef DEVKIT_DEBUGGER_H
#define DEVKIT_DEBUGGER_H

/*
 * Debugger (memory allocations for now)
 * Will have more features in the future (i guess)
 *
 *			:D
 */
#include <stdint.h>
#include <stddef.h>

/*
 * Not using typedefs to avoid name clashing after header definition
 *
 * Struct definitions
 */

typedef struct {
	uintptr_t address;
	size_t tag, size, free_calls;
} DEVKIT_POINTER;


struct devkit_register {
	DEVKIT_POINTER *items;
	size_t length, capacity, available_tag;
} DEVKIT_REGISTER;


/* Function declarations */

extern void* devkit_debug_allocate( size_t size);
extern void* devkit_debug_callocate( size_t nmemb, size_t size);
extern void devkit_debug_free( void *pointer);


#endif
