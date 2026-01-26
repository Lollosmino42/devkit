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
 * Struct definitions
 */

typedef struct {
	const char *file, *function;
	const int line;
} DevkitLocation;

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


#define DEVKIT_LOCATION_PTR( _file, _function, _line) ((DevkitLocation[]) { (DevkitLocation) {\
	.file = (_file), \
	.function = (_function), \
	.line = (_line) \
}})

#define DEVKIT_DEBUGGER_NULLPTR_ERROR "Devkit debugger: pointer is null!"
#define DEVKIT_DEBUGGER_ALLOC_FAIL(size) "Devkit Debugger: allocation failed (%lu bytes)!", (size)

#define DEVKIT_DEBUGGER_PRINT(location, ...) printf("Devkit_Debugger: "); \
	printf( __VA_ARGS__); printf(" -> %s\n", (location)->function)

#define DEVKIT_DEBUGGER_PRINTINFO(location, ...) printf("Devkit_Debugger: "); \
	printf( __VA_ARGS__); printf("\n => File: %s, Function: %s, Line: %d\n", (location)->file, (location)->function, (location)->line)


/* Function declarations */

extern void devkit_debug_setup_register( size_t capacity);
extern void devkit_debug_register_ptr( DevkitLocation *, void *pointer, size_t size);
extern void devkit_debug_update_tag();

extern bool devkit_debug_pointer_eq( DevkitPointer *this, DevkitPointer *other);

extern void* devkit_debug_allocate( DevkitLocation *, size_t size);
extern void* devkit_debug_callocate( DevkitLocation *, size_t nmemb, size_t size);
extern void devkit_debug_free( DevkitLocation *, void *pointer);


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

extern void devkit_debug_register_ptr( DevkitLocation *data, void *pointer, size_t size) {
	if (DEVKIT_REGISTER.length >= DEVKIT_REGISTER.capacity - 1) {
		DEVKIT_DEBUGGER_PRINTINFO(data, "register is full!");
		return;
	}
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
			return;
		}
	}
	DEVKIT_REGISTER.available = DEVKIT_REGISTER.length;
}


extern bool devkit_debug_pointer_eq( DevkitPointer *this, DevkitPointer *other) {
	return (this->pointer == other->pointer &&
			this->free_calls == other->free_calls)
		? true : false;
}


extern void* devkit_debug_allocate( DevkitLocation *data, size_t size) {
	void *allocation = malloc( size);
	if (!allocation) {
		DEVKIT_DEBUGGER_PRINTINFO(data, DEVKIT_DEBUGGER_ALLOC_FAIL(size));
		exit(EXIT_FAILURE);
	}
	DEVKIT_DEBUGGER_PRINT(data, "%lu bytes allocated", size);
	devkit_debug_register_ptr(data, allocation, size);

	return allocation;
}

extern void* devkit_debug_callocate( DevkitLocation *data, size_t nmemb, size_t size) {
	void *allocation = calloc( nmemb, size);
	if (!allocation) {
		DEVKIT_DEBUGGER_PRINTINFO(data, DEVKIT_DEBUGGER_ALLOC_FAIL(size));
		exit(1);
	}
	DEVKIT_DEBUGGER_PRINT(data, "cluster of %lu × %lu bytes allocated", nmemb, size);
	devkit_debug_register_ptr(data, allocation, nmemb*size);

	return allocation;
}

extern void devkit_debug_free( DevkitLocation *data, void *pointer) {
	if (!pointer) {
		DEVKIT_DEBUGGER_PRINTINFO(data, DEVKIT_DEBUGGER_NULLPTR_ERROR);
	}

	size_t slot;
	for (slot = 0; slot < DEVKIT_REGISTER.length; ++slot) {
		if ( devkit_debug_pointer_eq( &DEVKIT_REGISTER.items[slot], &DEVKIT_POINTER_NULL) ) {
			break;
		}
	}
	// Print info about pointer if in register, otherwise print a warning
	if ( slot != DEVKIT_REGISTER.length) {
		DevkitPointer ptr_data = DEVKIT_REGISTER.items[slot];
		DEVKIT_DEBUGGER_PRINT(data, "freeing pointer %p of size %lu", ptr_data.pointer, ptr_data.size);
		++ptr_data.free_calls;
	}
	else {
		DEVKIT_DEBUGGER_PRINTINFO(data, "freeing pointer that is not in register (stack pointer?)");
	}
	free(pointer);
}


#define malloc(size) \
	devkit_debug_allocate( DEVKIT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (size))
#define calloc(nmemb, size) \
	devkit_debug_callocate( DEVKIT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (nmemb), (size))
#define free(ptr) devkit_debug_free( DEVKIT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (ptr))

#endif
