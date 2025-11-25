#pragma once

#include "array.h"
#include "list.h"

extern Array list_to_array( List *list) {
	Array array = { list->typesize, list->length, calloc(list->length, list->typesize) };
	memcpy( array.items, list->items, list->typesize*list->length);
	return array;
}


extern List array_to_list( Array *array) {
	List list = { array->typesize, array->size, array->size, calloc(array->size, array->typesize) };
	memcpy( list.items, array->items, array->typesize*array->size);
	return list;
}
