#pragma once

#include <string.h>
#include <stdlib.h>

#include "settings.h"

#include "bits/iterable.h"

#define CHAR_SIZE 1

/* String alias for char* const */
typedef char* const String;


#if __DEVKIT_USE_CUSTOM_ALLOCATOR

#define strsub( alloc, string, start, end) __devkit_strsub( (alloc), (string), (start), (end))
#define strrev( alloc, string) __devkit_strrev( (alloc), (string))
#define str_asiterable( alloc, string) __devkit_str_asiterable( (alloc), (string))

#else

#define strsub( string, start, end) __devkit_strsub( nullptr, (string), (start), (end))
#define strrev( string) __devkit_strrev( nullptr, (string))
#define str_asiterable( string) __devkit_str_asiterable( nullptr, (string))

#endif

/* IMPLEMENTATION */

/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 * Does not prevent segmentation faults */
extern String __devkit_strsub( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end) {
	if ( end <= start ) return nullptr;

	size_t substr_len = end - start;
	String substring = calloc( substr_len, CHAR_SIZE);
	strncpy( substring, s + start, substr_len);
	return substring;
}

/* Returns String 's' reversed */
extern String __devkit_strrev( DEVKIT_ALLOCATOR *alloc, const String restrict s) {
	size_t slen = strlen(s);
	String reverse = calloc( slen, CHAR_SIZE);

	slen--;
	for (size_t idx = 0; idx <= slen; idx++) reverse[idx] = s[slen - idx];
	return reverse;
}


extern Iterable __devkit_str_asiterable( DEVKIT_ALLOCATOR *alloc, String *s) {
	return (Iterable) { alloc, CHAR_SIZE, strlen(*s), *s};
}


#undef CHAR_SIZE
