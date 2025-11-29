#ifndef __DEVKIT_AL_TRANSLATION_H
#define __DEVKIT_AL_TRANSLATION_H

#include "array.h"
#include "list.h"

extern Array list_to_array( List *list) {
	Array array = { list->typesize, list->length, calloc(list->length, list->typesize) };
	memcpy( array.items, list->items, list->typesize*list->length);
	return array;
}


extern List array_to_list( Array *array) {
	List list = { array->typesize, array->length, array->length, calloc(array->length, array->typesize) };
	memcpy( list.items, array->items, array->typesize*array->length);
	return list;
}

#endif
