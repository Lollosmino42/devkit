#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "iterable.h"

#define CHAR_SIZE 1

/* How it should always have been... 
 * Remember that 'String' still
 * represents a pointer type and always will,
 * not a String object of any kind.
 * If you want an immutable String, just type
 * 'const String'!*/
typedef char* const String;


/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 * Does not prevent segmentation faults */
extern String substring( const String restrict s, size_t start, size_t end) {
	if ( end <= start ) return nullptr;

	size_t substr_len = end - start;
	String substring = calloc( substr_len, CHAR_SIZE);
	strncpy( substring, s + start, substr_len);
	return substring;
}

/* Returns String 's' reversed */
extern String str_reverse( const String restrict s) {
	size_t slen = strlen(s);
	String reverse = calloc( slen, CHAR_SIZE);

	slen--;
	for (size_t idx = 0; idx <= slen; idx++) reverse[idx] = s[slen - idx];
	return reverse;
}


/* Sorts the characters of the string lexicographically */
extern void str_lexsort( String restrict s) {
}


#undef CHAR_SIZE
