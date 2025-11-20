

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "devkit.h"


void printList( List *list) {
	for ( size_t idx = 0; idx < list->length; idx++) {
		printf("%d\n", list_get(int, *list, idx));
	}
}

int main() {
	unsigned REMOVE_AMOUNT = 2;

	List list = new_list( int, 15);
	int var = 4;
	int vars[] = {var,var,var,var,var};

	list_nappend( &list, 5, vars);

	int other = 3;
	list_insert( &list, 3, other);

	int *removed = list_nremove( &list, 2, (size_t[]) {3,5});
	
	printf( "List length: %lu\n", list.length );
	printf( "List capacity: %lu\n", list.capacity );
	printList( &list);
}
