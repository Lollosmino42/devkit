#ifndef _DEVKIT_H
#define _DEVKIT_H

/*
 * Includes all headers.
 * Don't forget to define the implementations!
 */

#ifdef DEVKIT_IMPLEMENTATION

#define DEVKIT_LIST_IMPLEMENTATION
#define DEVKIT_MATH_IMPLEMENTATION
#define DEVKIT_ARRAY_IMPLEMENTATION
#define DEVKIT_POINTERS_IMPLEMENTATION
#define DEVKIT_STRING_IMPLEMENTATION
#define DEVKIT_MREGION_IMPLEMENTATION

#endif

#include "fdict.h"

#include "string.h"
#include "list.h"
#include "array.h"
#include "pointers.h"

#include "math.h"

#endif
