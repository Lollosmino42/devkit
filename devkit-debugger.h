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
#include <stdio.h>
#include <assert.h>

/*
 * Not using typedefs to avoid name clashing after header definition
 *
 * Struct definitions
 */
#ifndef DEVKIT_DEBUGGER_CAPACITY
#define DEVKIT_DEBUGGER_CAPACITY 300
#endif

typedef struct {
	void *pointer;
	uint32_t free_calls;
	size_t size;
} DevkitPointer;


typedef struct {
	DevkitPointer *items;
	size_t length, capacity, available;
} DevkitRegister;

DevkitRegister DEVKIT_REGISTER;
DevkitPointer DEVKIT_POINTER_NULL = {
	.free_calls = 0,
	.size = 0,
	.pointer = 0
};

#define DEVKIT_DEBUGGER_ALLOC_FAIL( size) "Devkit Debugger: allocation failed (%lu bytes)!", (size)
#define DEVKIT_DEBUGGER_PRINT(...) printf("Devkit_Debugger: "); \
	printf( __VA_ARGS__)
#define DEVKIT_DEBUGGER_PRINTERR(...) DEVKIT_DEBUGGER_PRINT(__VA_ARGS__); \
	printf("=> File: %s, Function: %s, Line: %d\n", __FILE__, __FUNCTION__, __LINE__)


/* Function declarations */

extern void devkit_debug_setup_register( size_t capacity);
extern void devkit_debug_register_ptr( void *pointer, size_t size);
extern void devkit_debug_update_tag();
extern void devkit_debug_destroy_register();

extern bool devkit_debug_pointer_eq( DevkitPointer *this, DevkitPointer *other);

extern void* devkit_debug_allocate( size_t size);
extern void* devkit_debug_callocate( size_t nmemb, size_t size);
extern void devkit_debug_free( void *pointer);


/* IMPLEMENTATION */


extern void devkit_debug_setup_register( size_t capacity) {
	DevkitPointer *items = calloc( capacity, sizeof(DevkitPointer));
	for (size_t idx = 0; idx < capacity; ++idx) {
		items[idx] = DEVKIT_POINTER_NULL;
	}
	DEVKIT_REGISTER = (DevkitRegister) {
		.items = items,
		.capacity = capacity,
		.length = 0,
		.available = 0
	};
}

extern void devkit_debug_register_ptr( void *pointer, size_t size) {
	DEVKIT_REGISTER.items[DEVKIT_REGISTER.available] = (DevkitPointer) {
		.pointer = pointer,
		.size = size,
		.free_calls = 0,
	};
	++DEVKIT_REGISTER.length;
	devkit_debug_update_tag();
}


extern void devkit_debug_update_tag() {
	for (uint32_t probe = 0; probe < UINT32_MAX; ++probe) {
		if (devkit_debug_pointer_eq( &DEVKIT_REGISTER.items[probe], &DEVKIT_POINTER_NULL )) {
			DEVKIT_REGISTER.available = probe;
			break;
		}
	}
}


extern bool devkit_debug_pointer_eq( DevkitPointer *this, DevkitPointer *other) {
	return (this->pointer == other->pointer &&
			this->free_calls == other->free_calls)
		? true : false;
}


extern void* devkit_debug_allocate( size_t size) {
	void *allocation = malloc( size);
	if (!allocation) {
		DEVKIT_DEBUGGER_PRINTERR( DEVKIT_DEBUGGER_ALLOC_FAIL(size));
		exit(1);
	}
	DEVKIT_DEBUGGER_PRINT("%lu bytes allocated", size);
	devkit_debug_register_ptr( allocation, size);

	return allocation;
}

extern void* devkit_debug_callocate( size_t nmemb, size_t size) {
	void *allocation = calloc( nmemb, size);
	if (!allocation) {
		DEVKIT_DEBUGGER_PRINTERR( DEVKIT_DEBUGGER_ALLOC_FAIL(size));
		exit(1);
	}
	DEVKIT_DEBUGGER_PRINT("cluster of %lu × %lu bytes allocated", nmemb, size);
	devkit_debug_register_ptr( allocation, nmemb*size);

	return allocation;
}

extern void devkit_debug_free( void *pointer) {
	size_t slot;
	for (slot = 0; slot < DEVKIT_REGISTER.length; ++slot) {
		if ( devkit_debug_pointer_eq( &DEVKIT_REGISTER.items[slot], &DEVKIT_POINTER_NULL) ) {
			break;
		}
	}
	// Print info about pointer if in register, otherwise print a warning
	if ( slot != DEVKIT_REGISTER.length) {
		DevkitPointer ptr_data = DEVKIT_REGISTER.items[slot];
		DEVKIT_DEBUGGER_PRINT("freeing pointer %p of size %lu", ptr_data.pointer, ptr_data.size);
		++ptr_data.free_calls;
	}
	else {
		DEVKIT_DEBUGGER_PRINTERR("freeing pointer that is not in register!");
	}
	free(pointer);
}



#endif
