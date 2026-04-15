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
} DktLocation;

typedef struct {
	void *pointer;
	size_t size;
	DktLocation location;
} DktPointer;


typedef struct {
	DktPointer *items;
	size_t size, capacity, available;
} DktRegister;

DktRegister DKT_REGISTER;
bool DKT_REGISTER_SET = false;

constexpr DktPointer DKT_POINTER_NULL = {0};

#define DKT_DEBUGGER "[Devkit Debugger]"
#define DKT_DEBUGGER_WARNING "[Devkit Debugger WARNING]"

#define DKT_LOCATION_PTR( _file, _function, _line) ((DktLocation) {\
	.file = (_file), \
	.function = (_function), \
	.line = (_line) \
})

#define DKT_DEBUGGER_NULLPTR_WARNING \
	DKT_DEBUGGER": pointer is null!"

#define DKT_DEBUGGER_ALLOC_FAIL(size) \
	DKT_DEBUGGER": allocation failed (%lu bytes)!", (size)

#define DKT_DEBUGGER_PRINT(location, ...) printf(DKT_DEBUGGER": "); \
	printf( __VA_ARGS__); \
	printf(" -> %s\n", (location)->function)

#define DKT_DEBUGGER_PRINTINFO(location, ...) printf(DKT_DEBUGGER": "); \
	printf( __VA_ARGS__); \
	printf("\n => File: %s, Function: %s, Line: %d\n", (location)->file, \
													(location)->function, \
													(location)->line)

#define DKT_DEBUGGER_WARN(location, ...) printf(DKT_DEBUGGER_WARNING": "); \
	printf( __VA_ARGS__); \
	printf("\n => File: %s, Function: %s, Line: %d\n", (location)->file, \
													(location)->function, \
													(location)->line)


/* Function declarations */

extern void dkt_debug_setup_register( size_t capacity);
extern void dkt_debug_register_ptr( DktLocation *, void *pointer, size_t size);
extern void dkt_debug_update_available();
extern void dkt_debug_close_register();

extern bool dkt_debug_pointer_isnull( DktPointer *this);

extern void* dkt_debug_allocate( DktLocation, size_t size);
extern void* dkt_debug_callocate( DktLocation, size_t nmemb, size_t size);
extern void dkt_debug_free( DktLocation, void *pointer);


/* IMPLEMENTATION */

//#define DEVKIT_DEBUGGER_IMPLEMENTATION
#ifdef DEVKIT_DEBUGGER_IMPLEMENTATION

extern void dkt_debug_close_register() {
	if (!DKT_REGISTER_SET) return;
	puts("-----------------");
	puts(" On exit:");
	puts("-----------------");

	bool problems;
	for (size_t idx = 0; idx < DKT_REGISTER.capacity; idx++) {
		DktPointer *ptr_data = &DKT_REGISTER.items[idx];
		if ( dkt_debug_pointer_isnull(ptr_data))
			continue;

		if (ptr_data->pointer) {
			DKT_DEBUGGER_WARN(&ptr_data->location,
					"pointer %p of size %lu was not freed before exit",
					ptr_data->pointer, ptr_data->size);
			if (!problems) problems = true;
		}
	}
	if (!problems)
		puts(" Everything should be fine");
	free(DKT_REGISTER.items);
}

extern void dkt_debug_setup_register( size_t capacity) {
	if (DKT_REGISTER_SET)
		return;
	DktPointer *items = calloc( capacity, sizeof(DktPointer));
	DKT_REGISTER = (DktRegister) {
		.items = items,
		.capacity = capacity,
		.size = 0,
		.available = 0
	};
	DKT_REGISTER_SET = true;
	atexit( dkt_debug_close_register);
}

extern void dkt_debug_register_ptr( DktLocation *loc, void *pointer, size_t size) {
	if (!DKT_REGISTER_SET) {
		DKT_DEBUGGER_WARN(loc, "Register is not set up!");
		return;
	}
	if (DKT_REGISTER.size >= DKT_REGISTER.capacity) {
		DKT_DEBUGGER_PRINTINFO(loc, "register is full!");
		return;
	}
	DKT_REGISTER.items[DKT_REGISTER.available] = (DktPointer) {
		.pointer = pointer,
		.size = size,
		.location = *loc
	};
	++DKT_REGISTER.size;
	dkt_debug_update_available();
}


extern void dkt_debug_update_available() {
	uint32_t probe;

	for (probe = 0; probe < DKT_REGISTER.capacity; ++probe) {
		if (dkt_debug_pointer_isnull( &DKT_REGISTER.items[probe])) {
			break;
		}
	}
	DKT_REGISTER.available = probe;
}


extern bool dkt_debug_pointer_isnull( DktPointer *this) {
	return this->pointer == nullptr && this->size == 0;
}


extern void* dkt_debug_allocate( DktLocation loc, size_t size) {
	void *allocation = malloc( size);
	if (!allocation) {
		DKT_DEBUGGER_WARN(&loc, DKT_DEBUGGER_ALLOC_FAIL(size));
		exit(EXIT_FAILURE);
	}
	DKT_DEBUGGER_PRINT(&loc, "%lu bytes allocated at %p", size, allocation);
	dkt_debug_register_ptr(&loc, allocation, size);

	return allocation;
}

extern void* dkt_debug_callocate( DktLocation loc, size_t nmemb, size_t size) {
	void *allocation = calloc( nmemb, size);
	if (!allocation) {
		DKT_DEBUGGER_PRINTINFO(&loc, DKT_DEBUGGER_ALLOC_FAIL(size));
		exit(1);
	}
	DKT_DEBUGGER_PRINT(&loc, "cluster of %lu × %lu bytes allocated at %p",
			nmemb, size, allocation);
	dkt_debug_register_ptr(&loc, allocation, nmemb*size);

	return allocation;
}

extern void dkt_debug_free( DktLocation loc, void *pointer) {
	if (!DKT_REGISTER_SET) return;
	if (!pointer) {
		DKT_DEBUGGER_WARN(&loc, DKT_DEBUGGER_NULLPTR_WARNING);
	}

	// Find pointer in register
	size_t slot = 0;
	while (slot < DKT_REGISTER.capacity && 
			(DKT_REGISTER.items[slot].pointer != pointer ||
			dkt_debug_pointer_isnull( &DKT_REGISTER.items[slot]) )) {
		++slot;
	}
	// Print info about pointer if in register, otherwise print a warning
	if ( slot != DKT_REGISTER.capacity) {
		DktPointer ptr_data = DKT_REGISTER.items[slot];
		DKT_DEBUGGER_PRINT(&loc, "freeing pointer %p of size %lu", 
			ptr_data.pointer, ptr_data.size);
		// Remove freed pointer from register
		DKT_REGISTER.items[slot] = DKT_POINTER_NULL;
		dkt_debug_update_available();
	}
	// Print warning
	else {
		DKT_DEBUGGER_PRINTINFO(&loc, 
				"freeing pointer that is not in register (stack pointer?)");
	}
	free(pointer);
}

#endif

/* Debugger macros.
 * Defined at end of file to avoid overlap with stdlib functions in implementation */
#define malloc(size) \
	dkt_debug_allocate( DKT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (size))
#define calloc(nmemb, size) \
	dkt_debug_callocate( DKT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (nmemb), (size))
#define free(ptr) \
	dkt_debug_free( DKT_LOCATION_PTR( __FILE__, __FUNCTION__, __LINE__), (ptr))

#endif
