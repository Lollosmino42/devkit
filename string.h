#ifndef _DEVKIT_STRING_H
#define _DEVKIT_STRING_H

#include <string.h>
#include <stdlib.h>

#include "settings.h"
#include "bits/iterable.h"


/* String alias for char* const.
 * It's a modifiable string*/
typedef char* const String;


#if DEVKIT_USE_CUSTOM_ALLOCATOR

#define devkit_string_slice _devkit_string_slice
#define devkit_string_reverse _devkit_string_reverse

#else

#define devkit_string_slice( string, start, end) _devkit_string_slice( nullptr, (string), (start), (end))
#define devkit_string_reverse( string) _devkit_string_reverse( nullptr, (string))

#endif

#if DEVKIT_STRIP_PREFIXES

#define string_slice devkit_string_slice
#define string_reverse devkit_string_reverse

#endif

/* Declarations */

extern String _devkit_string_slice( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end);
extern String _devkit_string_reverse( DEVKIT_ALLOCATOR *alloc, const String restrict s);

/* IMPLEMENTATION */

//#define DEVKIT_STRING_IMPLEMENTATION
#ifdef DEVKIT_STRING_IMPLEMENTATION

/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 * Does not prevent segmentation faults */
String _devkit_string_slice( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end) {
	if ( end <= start ) return nullptr;

	size_t substr_len = end - start;
	String substring = calloc( substr_len, 1);
	strncpy( substring, s + start, substr_len);
	return substring;
}

/* Returns String 's' reversed */
String _devkit_string_reverse( DEVKIT_ALLOCATOR *alloc, const String restrict s) {
	size_t slen = strlen(s);
	String reverse = calloc( slen, 1);

	slen--;
	for (size_t idx = 0; idx <= slen; idx++) reverse[idx] = s[slen - idx];
	return reverse;
}

#endif

#endif
