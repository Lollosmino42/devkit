#ifndef __DEVKIT_STRING_H
#define __DEVKIT_STRING_H

#include <string.h>
#include <stdlib.h>

#include "settings.h"

#include "bits/iterable.h"

#define CHAR_SIZE 1

/* String alias for char* const */
typedef char* const String;


#if __DEVKIT_USE_CUSTOM_ALLOCATOR

#define strsub devkit_strsub
#define strrev devkit_strrev
#define str_asiterable devkit_str_asiterable

#else

#define strsub( string, start, end) devkit_strsub( nullptr, (string), (start), (end))
#define strrev( string) devkit_strrev( nullptr, (string))
#define str_asiterable( string) devkit_str_asiterable( nullptr, (string))

#endif

/* IMPLEMENTATION */

/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 * Does not prevent segmentation faults */
extern String devkit_strsub( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end) {
	if ( end <= start ) return nullptr;

	size_t substr_len = end - start;
	String substring = calloc( substr_len, CHAR_SIZE);
	strncpy( substring, s + start, substr_len);
	return substring;
}

/* Returns String 's' reversed */
extern String devkit_strrev( DEVKIT_ALLOCATOR *alloc, const String restrict s) {
	size_t slen = strlen(s);
	String reverse = calloc( slen, CHAR_SIZE);

	slen--;
	for (size_t idx = 0; idx <= slen; idx++) reverse[idx] = s[slen - idx];
	return reverse;
}


extern Iterable devkit_str_asiterable( DEVKIT_ALLOCATOR *alloc, String *s) {
	return (Iterable) { 
		.alloc=alloc, 
		.typesize=CHAR_SIZE, 
		.length=strlen(*s), 
		.items=*s
	};
}


#undef CHAR_SIZE

#endif
