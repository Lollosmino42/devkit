#ifndef _DEVKIT_STRING_H
#define _DEVKIT_STRING_H

#include <string.h>
#include <stdlib.h>

#include "settings.h"
#include "bits/iterable.h"


/* String alias for char* const.
 * It's a modifiable string*/
#include "bits/string_struct.h"
typedef struct devkit_string String;


#if DEVKIT_USE_CUSTOM_ALLOCATOR

#define devkit_string_slice _devkit_string_slice
#define devkit_string_reverse _devkit_string_reverse
#define devkit_string_new _devkit_string_new

#else

#define devkit_string_slice( string, start, end) _devkit_string_slice( nullptr, (string), (start), (end))
#define devkit_string_reverse( string) _devkit_string_reverse( nullptr, (string))
#define devkit_string_new( text) _devkit_string_new( nullptr, (text))

#endif

#if DEVKIT_STRIP_PREFIXES

#define string_slice devkit_string_slice
#define string_reverse devkit_string_reverse
#define string_new devkit_string_new

#endif

/* Declarations */

extern String _devkit_string_new( DEVKIT_ALLOCATOR *alloc, char *text);
extern String _devkit_string_slice( DEVKIT_ALLOCATOR *alloc, const String *restrict s, size_t start, size_t end);
extern void _devkit_string_reverse( String *s);

/* IMPLEMENTATION */

//#define DEVKIT_STRING_IMPLEMENTATION
#ifdef DEVKIT_STRING_IMPLEMENTATION


extern String _devkit_string_new( DEVKIT_ALLOCATOR *alloc, char *text) {
	size_t length = strlen(text);
	char *items = DEVKIT_ALLOC( alloc, length);
	memcpy( items, text, length);
	return (String) {
		.length = length,
		.items = items,
		.allocator = alloc
	};
}

/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 */
String _devkit_string_slice( DEVKIT_ALLOCATOR *alloc, const String *s, size_t start, size_t end) {
	assert( end > start);

	size_t substr_len = end - start;
	char *items = DEVKIT_ALLOC( alloc, substr_len);
	strncpy( items, s->items + start, substr_len);
	return (String) {
		.items = items,
		.length = substr_len,
		.allocator = alloc
	};
}

/* Returns String 's' reversed */
void _devkit_string_reverse( String *s) {
	char buffer[s->length];
	memcpy( buffer, s->items, s->length);
	for (size_t idx = 0; idx < s->length; idx++) {
		s->items[idx] = buffer[s->length - idx - 1];
	}
}


#endif

#endif
