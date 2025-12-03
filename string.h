<<<<<<< HEAD
#ifndef __DEVKIT_STRING_H
#define __DEVKIT_STRING_H
=======
#pragma once
>>>>>>> f011996 (rebuild main branch for repo)

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
<<<<<<< HEAD
=======
#define str_asiterable devkit_str_asiterable
>>>>>>> f011996 (rebuild main branch for repo)

#else

#define strsub( string, start, end) devkit_strsub( nullptr, (string), (start), (end))
#define strrev( string) devkit_strrev( nullptr, (string))
<<<<<<< HEAD
=======
#define str_asiterable( string) devkit_str_asiterable( nullptr, (string))
>>>>>>> f011996 (rebuild main branch for repo)

#endif

/* IMPLEMENTATION */

/* Substring of String 's'.
 * Returns 'nullptr' if end <= start, start is 
 * out of bounds. 
 * Does not prevent segmentation faults */
<<<<<<< HEAD
String devkit_strsub( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end) {
=======
extern String devkit_strsub( DEVKIT_ALLOCATOR *alloc, const String restrict s, size_t start, size_t end) {
>>>>>>> f011996 (rebuild main branch for repo)
	if ( end <= start ) return nullptr;

	size_t substr_len = end - start;
	String substring = calloc( substr_len, CHAR_SIZE);
	strncpy( substring, s + start, substr_len);
	return substring;
}

/* Returns String 's' reversed */
<<<<<<< HEAD
String devkit_strrev( DEVKIT_ALLOCATOR *alloc, const String restrict s) {
=======
extern String devkit_strrev( DEVKIT_ALLOCATOR *alloc, const String restrict s) {
>>>>>>> f011996 (rebuild main branch for repo)
	size_t slen = strlen(s);
	String reverse = calloc( slen, CHAR_SIZE);

	slen--;
	for (size_t idx = 0; idx <= slen; idx++) reverse[idx] = s[slen - idx];
	return reverse;
}


<<<<<<< HEAD
Iterable devkit_str_asiterable( DEVKIT_ALLOCATOR *alloc, String *s) {
	return (Iterable) { 
		.alloc=alloc, 
		.typesize=CHAR_SIZE, 
		.length=strlen(*s), 
		.items=*s
	};
=======
extern Iterable devkit_str_asiterable( DEVKIT_ALLOCATOR *alloc, String *s) {
	return (Iterable) { alloc, CHAR_SIZE, strlen(*s), *s};
>>>>>>> f011996 (rebuild main branch for repo)
}


#undef CHAR_SIZE
<<<<<<< HEAD

#endif
=======
>>>>>>> f011996 (rebuild main branch for repo)
