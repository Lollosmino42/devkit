#pragma once

#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/* Sort an array of anything in "ascending" order.
 * The values are compared with 'memcmp' */
#define sort( array, len) \
	__devkit_sort( array, len, sizeof(array[0]))

extern void __devkit_sort( void* array, size_t length, size_t typesize) {
	char temp_buf[typesize];

	for (size_t times = 0; times < length - 1; times++) {
		for ( size_t idx = 0; idx < length - 1; idx++) {
			void *current = array + idx;
			void *next = array + idx+1;

			if ( memcmp(current, next, typesize*length) > 0 ) {
				memcpy( temp_buf, current, typesize);
				memcpy( current, next, typesize);
				memcpy( next, temp_buf, typesize);
			}
		}
	}
}

