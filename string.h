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


#if DEVKIT_STRIP_PREFIXES

#define string_slice devkit_string_slice
#define string_reverse devkit_string_reverse
#define string_new devkit_string_new

#endif


/* Declarations */

extern String* devkit_string_new( const char *text);
extern String devkit_string_new_stack( const char *text);
extern char* devkit_string_slice( const String *restrict s, size_t start, size_t end);
extern void devkit_string_reverse( String *s);

/* IMPLEMENTATION */

#define DEVKIT_STRING_IMPLEMENTATION
#ifdef DEVKIT_STRING_IMPLEMENTATION


String* devkit_string_new( const char *text) {
	size_t length = strlen(text);
	String *this = malloc( sizeof(*this) + length);
	this->length = length;
	this->heap_default = true;
	this->items = (char*)(this + 1);
	memcpy( this->items, text, length);
	return this;
}

String devkit_string_new_stack( const char *text) {
	size_t length = strlen(text);
	char *items = malloc( length);
	return (String) {
		.length = length,
		.items = items,
		.heap_default = false
	};
}


/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 */
char* devkit_string_slice( const String *restrict s, size_t start, size_t end) {
	assert( end > start);

	size_t substr_len = end - start;
	char *temp = malloc(substr_len + 1);
	strncpy( temp, s->items + start, substr_len);
	temp[substr_len] = '\0';
	return temp;
}

/* Returns String 's' reversed */
void devkit_string_reverse( String *s) {
	char buffer[s->length];
	memcpy( buffer, s->items, s->length);
	for (size_t idx = 0; idx < s->length; idx++) {
		s->items[idx] = buffer[s->length - idx - 1];
	}
}


#endif

#endif
