#ifndef __DEVKIT_STRING_H
#define __DEVKIT_STRING_H

#include <string.h>
#include <stdlib.h>

#include "settings.h"
#include "bits/iterable.h"


/* String alias for char* const */
typedef char* const String;


#if __DEVKIT_USE_CUSTOM_ALLOCATOR

#define strsub devkit_strsub
#define strrev devkit_strrev

#else

#define strsub( string, start, end) devkit_strsub( nullptr, (string), (start), (end))
#define strrev( string) devkit_strrev( nullptr, (string))

#endif

/* Declarations */

extern String devkit_strsub( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end);
extern String devkit_strrev( DEVKIT_ALLOCATOR *alloc, const String restrict s);

extern Iterable devkit_str_asiterable( DEVKIT_ALLOCATOR *alloc, String *s);

/* IMPLEMENTATION */

//#define DEVKIT_STRING_IMPLEMENTATION
#ifdef DEVKIT_STRING_IMPLEMENTATION

/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 * Does not prevent segmentation faults */
String devkit_strsub( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end) {
	if ( end <= start ) return nullptr;

	size_t substr_len = end - start;
	String substring = calloc( substr_len, 1);
	strncpy( substring, s + start, substr_len);
	return substring;
}

/* Returns String 's' reversed */
String devkit_strrev( DEVKIT_ALLOCATOR *alloc, const String restrict s) {
	size_t slen = strlen(s);
	String reverse = calloc( slen, 1);

	slen--;
	for (size_t idx = 0; idx <= slen; idx++) reverse[idx] = s[slen - idx];
	return reverse;
}


Iterable devkit_str_asiterable( DEVKIT_ALLOCATOR *alloc, String *s) {
	return (Iterable) { 
		.alloc=alloc, 
		.typesize=1, 
		.length=strlen(*s), 
		.items=*s
	};
}

#endif

#endif
