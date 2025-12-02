#ifndef __DEVKIT_LISTDEFS_H
#define __DEVKIT_LISTDEFS_H

#include "../settings.h"

#if __DEVKIT_USE_CUSTOM_ALLOCATOR
#define asiterable( alloc, array, length, typesize) devkit_asiterable( (alloc), (array), (length), typesize)	
#define linspace(alloc, start, end, steps) devkit_linspace( (alloc), (start), (end), (steps))
#define generator( alloc, base, length, typesize, map) devkit_generator( (alloc), (base), (length), (typesize), (map))
#define range( alloc, start, end) devkit_range( (alloc), (start), (end))

#else
#define asiterable( array, length, typesize) devkit_asiterable( nullptr, (array), (length), typesize)	
#define linspace( start, end, steps) devkit_linspace( nullptr, (start), (end), (steps))
#define generator( base, length, typesize, map) devkit_range( nullptr, (base), (length), (typesize), (map))
#define range( start, end) devkit_generator( nullptr, (start), (end))

#endif

#endif
