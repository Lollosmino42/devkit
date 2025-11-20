#pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>


#define CHAR_SIZE 1

/* How it should always have been... 
 * Note that Strings are supposed to be
 * immutable: 'const char *'.
 * And remember that 'String' still
 * represents a pointer type and always will,
 * not a String object of any kind.
 * It is just an aesthetic choice */
typedef char* const String;


/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 * Does not prevent segmentation faults */
extern String strsub( String s, size_t start, size_t end) {
	if ( end <= start ) return nullptr;

	size_t substr_len = end - start;
	String substring = calloc( substr_len, CHAR_SIZE);
	strncpy( substring, s + start, substr_len);
	return substring;
}

/* Returns String 's' reversed */
extern String strrev( String restrict s) {
	size_t slen = strlen(s);
	String reverse = calloc( slen, CHAR_SIZE);

	slen--;
	for (size_t idx = 0; idx <= slen; idx++) reverse[idx] = s[slen - idx];
	return reverse;
}


#undef CHAR_SIZE
