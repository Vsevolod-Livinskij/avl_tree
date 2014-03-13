/*
 * test.c
 *
 *  Created on: Feb 25, 2014
 *      Author: vsevolod
 */

#include "avl_tree_lib.h"
#include <stdlib.h>
#include <stdio.h>

data_t foo (data_t node_data, void* a) {
	return node_data * node_data;
}

int main() {

	head* arr [3];

	for (int i = 0; i < 3; i++) {
		arr [i] = avl_init_tree ();
	}

	int tmp = 0;
	for (int i = 0; i < 5000; i++) {
		avl_insert_node (arr [i % 3], (tmp + i) % (i + 19));
		tmp += i;
	}

	for (int i = 0; i < 3; i++) {
		//avl_dump_tree (arr [i]);
	}

	avl_insert_node (arr [0], 0);
	for (int i = 0; i < 100; i++) {
		avl_find_node (arr [i % 3], i % 9);
	}

	for (int i = 0; i < 1000; i++) {
		avl_remove_node (arr [i % 3], i);
	}

	for (int i = 0; i < 3; i++) {
		avl_delete_tree (& arr [i]);
	}

	head* for_iter = avl_init_tree ();
	for (int i = 0; i < 20; i++)
		avl_insert_node (for_iter, i);
	avl_dump_tree (for_iter);
	avl_iterator_tree (for_iter, (*foo), NULL);
	avl_dump_tree (for_iter);

	avl_iterator_tree (NULL, (*foo), NULL);
	avl_iterator_tree (for_iter, (NULL), NULL);

	avl_delete_tree (&for_iter);

	avl_insert_node (NULL, 19);
	avl_dump_tree (NULL);
	avl_find_node (NULL, 19);
	avl_remove_node (NULL, 19);
	avl_delete_tree (NULL);

	head* tree = avl_init_tree ();
	avl_insert_node (tree, 19);
	avl_dump_tree (tree);
	avl_find_node (tree, 19);
	avl_remove_node (tree, 19);
	avl_delete_tree (&tree);
	avl_dump_tree (tree);
	avl_delete_tree (&tree);

/*
	head* tree = avl_init_tree ();
	int tmp = 0;
	for (int i = 0; i < 25; i++) {
		avl_insert_node (tree, (tmp + i) % (i + 19));
		printf ("\ni %d | %d\n", i, (tmp + i) % (i + 19));
		avl_dump_tree (tree);
		tmp += i;
		i++;
	}
	avl_dump_tree (tree);
	avl_delete_tree (&tree);
*/
}




