#ifndef _DEVKIT_DEBUGGER_H
#define _DEVKIT_DEBUGGER_H

/*
 * Debugger (memory allocations for now)
 * Will have more features in the future (i guess)
 *
 *			:D
 */
#include <stdint.h>
#include <stdlib.h>

/*
 * Not using typedefs to avoid name clashing after header definition
 *
 * Struct definitions
 */
#ifndef DEVKIT_DEBUGGER_CAPACITY
#define DEVKIT_DEBUGGER_CAPACITY 300
#endif

typedef struct {
	uintptr_t address;
	size_t tag, size, free_calls;
} DevkitPointer;


typedef struct {
	DevkitPointer *items;
	size_t length, capacity, available_tag;
} DevkitRegister;


/* Function declarations */

extern DevkitRegister devkit_debug_new_register( size_t capacity);

extern void* devkit_debug_allocate( size_t size);
extern void* devkit_debug_callocate( size_t nmemb, size_t size);
extern void devkit_debug_free( void *pointer);


#endif
