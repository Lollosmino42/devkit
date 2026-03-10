#ifndef _DEVKIT_DEBUGGER_H
#define _DEVKIT_DEBUGGER_H

#if defined(__STDC__) && __STDC_VERSION__ < 202311L
#define nullptr NULL
#define constexpr const
#include <stdbool.h>
#endif

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
	int line;
} DevkitLocation;

typedef struct {
	void *pointer;
	size_t size;
	DevkitLocation location;
} DevkitPointer;


typedef struct {
	DevkitPointer *items;
	size_t size, capacity, available;
} DevkitRegister;

DevkitRegister DEVKIT_REGISTER;
bool DEVKIT_REGISTER_SET = false;

constexpr DevkitPointer DEVKIT_POINTER_NULL = {0};


#define DEVKIT_LOCATION_PTR( _file, _function, _line) ((DevkitLocation) {\
	.file = (_file), \
	.function = (_function), \
	.line = (_line) \
})

#define DEVKIT_DEBUGGER_NULLPTR_WARNING "Devkit debugger: pointer is null!"
#define DEVKIT_DEBUGGER_ALLOC_FAIL(size) "Devkit Debugger: allocation failed (%lu bytes)!", (size)

#define DEVKIT_DEBUGGER_PRINT(location, ...) printf("Devkit Debugger: "); \
	printf( __VA_ARGS__); \
	printf(" -> %s\n", (location)->function)

#define DEVKIT_DEBUGGER_PRINTINFO(location, ...) printf("Devkit Debugger: "); \
	printf( __VA_ARGS__); \
	printf("\n => File: %s, Function: %s, Line: %d\n", (location)->file, \
													(location)->function, \
													(location)->line)

#define DEVKIT_DEBUGGER_WARN(location, ...) printf("Devkit Debugger WARNING: "); \
	printf( __VA_ARGS__); \
	printf("\n => File: %s, Function: %s, Line: %d\n", (location)->file, \
													(location)->function, \
													(location)->line)


/* Function declarations */

extern void devkit_debug_setup_register( size_t capacity);
extern void devkit_debug_register_ptr( DevkitLocation *, void *pointer, size_t size);
extern void devkit_debug_update_available();
extern void devkit_debug_close_register();

extern bool devkit_debug_pointer_isnull( DevkitPointer *this);

extern void* devkit_debug_allocate( DevkitLocation, size_t size);
extern void* devkit_debug_callocate( DevkitLocation, size_t nmemb, size_t size);
extern void devkit_debug_free( DevkitLocation, void *pointer);


/* IMPLEMENTATION */

extern void devkit_debug_close_register() {
	if (!DEVKIT_REGISTER_SET) return;
	puts("-----------------");
	puts(" On exit:");
	puts("-----------------");

	bool problems;
	for (size_t idx = 0; idx < DEVKIT_REGISTER.capacity; idx++) {
		DevkitPointer *ptr_data = &DEVKIT_REGISTER.items[idx];
		if ( devkit_debug_pointer_isnull(ptr_data))
			continue;

		if (ptr_data->pointer) {
			DEVKIT_DEBUGGER_WARN(&ptr_data->location,
					"pointer %p of size %lu was not freed before exit",
					ptr_data->pointer, ptr_data->size);
			if (!problems) problems = true;
		}
	}
	if (!problems)
		puts(" Everything should be fine");
	free(DEVKIT_REGISTER.items);
}

extern void devkit_debug_setup_register( size_t capacity) {
	if (DEVKIT_REGISTER_SET)
		return;
	DevkitPointer *items = calloc( capacity, sizeof(DevkitPointer));
	DEVKIT_REGISTER = (DevkitRegister) {
		.items = items,
		.capacity = capacity,
		.size = 0,
		.available = 0
	};
	DEVKIT_REGISTER_SET = true;
	atexit( devkit_debug_close_register);
}

extern void devkit_debug_register_ptr( DevkitLocation *loc, void *pointer, size_t size) {
	if (!DEVKIT_REGISTER_SET) {
		DEVKIT_DEBUGGER_WARN(loc, "Register is not set up!");
		return;
	}
	if (DEVKIT_REGISTER.size >= DEVKIT_REGISTER.capacity - 1) {
		DEVKIT_DEBUGGER_PRINTINFO(loc, "register is full!");
		return;
	}
	DEVKIT_REGISTER.items[DEVKIT_REGISTER.available] = (DevkitPointer) {
		.pointer = pointer,
		.size = size,
		.location = *loc
	};
	++DEVKIT_REGISTER.size;
	devkit_debug_update_available();
}


extern void devkit_debug_update_available() {
	for (uint32_t probe = 0; probe < UINT32_MAX; ++probe) {
		if (devkit_debug_pointer_isnull( &DEVKIT_REGISTER.items[probe])) {
			DEVKIT_REGISTER.available = probe;
			return;
		}
	}
	DEVKIT_REGISTER.available = DEVKIT_REGISTER.size;
}


extern bool devkit_debug_pointer_isnull( DevkitPointer *this) {
	return this->pointer == nullptr && this->size == 0;
}


extern void* devkit_debug_allocate( DevkitLocation loc, size_t size) {
	void *allocation = malloc( size);
	if (!allocation) {
		DEVKIT_DEBUGGER_WARN(&loc, DEVKIT_DEBUGGER_ALLOC_FAIL(size));
		exit(EXIT_FAILURE);
	}
	DEVKIT_DEBUGGER_PRINT(&loc, "%lu bytes allocated at %p", size, allocation);
	devkit_debug_register_ptr(&loc, allocation, size);

	return allocation;
}

extern void* devkit_debug_callocate( DevkitLocation loc, size_t nmemb, size_t size) {
	void *allocation = calloc( nmemb, size);
	if (!allocation) {
		DEVKIT_DEBUGGER_PRINTINFO(&loc, DEVKIT_DEBUGGER_ALLOC_FAIL(size));
		exit(1);
	}
	DEVKIT_DEBUGGER_PRINT(&loc, "cluster of %lu × %lu bytes allocated at %p",
			nmemb, size, allocation);
	devkit_debug_register_ptr(&loc, allocation, nmemb*size);

	return allocation;
}

extern void devkit_debug_free( DevkitLocation loc, void *pointer) {
	if (!DEVKIT_REGISTER_SET) return;
	if (!pointer) {
		DEVKIT_DEBUGGER_WARN(&loc, DEVKIT_DEBUGGER_NULLPTR_WARNING);
	}

	// Find pointer in register
	size_t slot = 0;
	while (slot < DEVKIT_REGISTER.size && 
			(DEVKIT_REGISTER.items[slot].pointer != pointer ||
			devkit_debug_pointer_isnull( &DEVKIT_REGISTER.items[slot]) )) {
		++slot;
	}
	// Print info about pointer if in register, otherwise print a warning
	if ( slot != DEVKIT_REGISTER.size) {
		DevkitPointer ptr_data = DEVKIT_REGISTER.items[slot];
		DEVKIT_DEBUGGER_PRINT(&loc, "freeing pointer %p of size %lu", 
			ptr_data.pointer, ptr_data.size);
		// Remove freed pointer from register
		DEVKIT_REGISTER.items[slot] = DEVKIT_POINTER_NULL;
		devkit_debug_update_available();
	}
	// Print warning
	else {
		DEVKIT_DEBUGGER_PRINTINFO(&loc, 
				"freeing pointer that is not in register (stack pointer?)");
	}
	free(pointer);
}


#define malloc(size) \
	devkit_debug_allocate( DEVKIT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (size))
#define calloc(nmemb, size) \
	devkit_debug_callocate( DEVKIT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (nmemb), (size))
#define free(ptr) \
	devkit_debug_free( DEVKIT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (ptr))

#endif
