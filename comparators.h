#pragma once

#include <stdlib.h>
#include <string.h>

typedef __compar_fn_t Comparator;

#define byte unsigned char

int bytecmp(const byte *a, const byte *b) {
	return *a - *b;
}

int intcmp( const int *a, const int *b) {
	return *a - *b;
}

int shortcmp( const short *a, const short *b) {
	return *a - *b;
}

long longcmp( const long *a, const long *b) {
	return *a - *b;
}

double doublecmp( const double *a, const double *b) {
	return *a - *b;
}

long double ldoublecmp( const long double *a, const long double *b) {
	return *a - *b;
}

float floatcmp( const float *a, const float *b) {
	return *a - *b;
}


/* Unsigned versions */

#define u unsigned

long uintcmp( const u *a, const u *b) {
	return *a - *b;
}

long long ulongcmp( const u long *a, const u long *b) {
	return *a - *b;
}


#undef byte
#undef u
