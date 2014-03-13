/*
 * test.c
 *
 *  Created on: Feb 25, 2014
 *      Author: vsevolod
 */

#include "avl_tree_lib.h"

data_t foo (data_t a, data_t node_data) {
	return a * node_data;
}

int main() {

	head* tree = avl_init_tree();


	avl_insert_node (tree, 50);
	avl_insert_node (tree, 45);
	avl_insert_node (tree, 0);
	avl_insert_node (tree, 77);
	avl_insert_node (tree, 100);
	avl_insert_node (tree, 25);
	avl_insert_node (tree, 49);
	avl_insert_node (tree, 33);
	avl_insert_node (tree, 78);
	avl_insert_node (tree, 59);
	avl_insert_node (tree, 27);
	avl_insert_node (tree, 17);

	tree -> tree -> child [right] -> height = 5;
	avl_remove_tree (tree, 77);

	avl_dump_tree (tree);


	avl_find_tree (tree, 50);
	avl_find_tree (tree, 25);
	avl_find_tree (tree, 0);

	avl_iterator_tree (tree, (*foo), 2);

	avl_dump_tree (tree);

	avl_remove_tree (tree, 50);
	avl_remove_tree (tree, 25);
	avl_remove_tree (tree, 0);


	avl_dump_tree (tree);

	avl_delete_tree (tree);


	head* arr_tree [10];
	for (int i = 0; i < 10; ++i)
		arr_tree [i] = avl_init_tree();

	for (unsigned long i = 0; i < 78945612; ++i)
		avl_insert_node (arr_tree [i % 9], i);

	for (unsigned long i = 0; i < 16785; ++i)
		avl_find_tree (arr_tree [i % 9], (i + 2547) / 5);

	avl_iterator_tree (arr_tree [5], (*foo), 3);

	for (unsigned long i = 0; i < 94587; ++i)
		avl_remove_tree (arr_tree [i % 9], (i + 752) / 5);

	for (int i = 0; i < 10; ++i)
			avl_delete_tree (arr_tree [i]);

	avl_insert_node (NULL, 9);
	avl_dump_tree (NULL);
	avl_find_tree (NULL, 50);
	avl_iterator_tree (NULL, (*foo), 2);
	avl_remove_tree (NULL, 0);
	avl_delete_tree (NULL);
	avl_delete_tree (tree);
	avl_dump_tree (tree);
}




