#ifndef __DEVKIT_COMPARATOR_H
#define __DEVKIT_COMPARATOR_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef __compar_fn_t Comparator;

extern int i32cmp(const void *a, const void *b) {
	return memcmp(a,b,sizeof(int32_t));
}

extern int i64cmp(const void *a, const void *b) {
	return memcmp(a,b,sizeof(int64_t));
}

extern int i16cmp(const void *a, const void *b) {
	return memcmp(a,b,sizeof(int16_t));
}

extern int i8cmp(const void *a, const void *b) {
	return memcmp(a,b,sizeof(int8_t));
}

extern int fcmp(const void *a, const void *b) {
	return memcmp(a,b,sizeof(float));
}

extern int dcmp(const void *a, const void *b) {
	return memcmp(a,b,sizeof(double));
}

extern int ldcmp(const void *a, const void *b) {
	return memcmp(a,b,sizeof(long double));
}

/* Compares addresses of pointers. Can be useful to sort pointers by address */
extern int ptrcmp(const void *a, const void *b) {
	return memcmp(&a,&b,sizeof(uintptr_t));
}


#endif
