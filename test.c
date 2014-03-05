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
	/*
	node* overload = avl_insert (NULL, 0);
	for (int i = 1; i < 10000; i++) {
		overload = avl_insert (overload, i);
	}
	avl_dump (overload, 0);
	for (int i = 2; i < 998; i++) {
		overload = avl_remove (overload, i);
	}
	avl_dump (overload, 0);
	avl_delete (overload);
	*/
	node* arr_tree [10];
	for (int i = 0; i < 10; i++) {
		arr_tree [i] = avl_insert (NULL, 0);
	}
	for (unsigned long i = 0; i < 3960054; i++) {
		arr_tree [i % 9] = avl_insert (arr_tree [i % 9], i);
	}
	for (unsigned long i = 0; i < 3960; i++) {
		arr_tree [i % 9] = avl_remove (arr_tree [i % 9], i);
	}

	node* tree = avl_insert (NULL, 500);
	tree = avl_insert (tree, 25);
	tree = avl_insert (tree, 5);
	tree = avl_insert (tree, 89);
	tree = avl_insert (tree, 450);
	tree = avl_insert (tree, 250);
	tree = avl_insert (tree, 258);
	tree = avl_insert (tree, 275);
	tree = avl_insert (tree, 175);
	tree = avl_insert (tree, 123);
	tree = avl_insert (tree, 2);
	tree = avl_insert (tree, 498);
	tree = avl_insert (tree, 466);
	tree = avl_insert (tree, 108);
	tree = avl_insert (tree, 357);
	tree = avl_insert (tree, 281);
	for (int i = 0; i < 35; i++)
		tree = avl_remove (tree, i / 2);
	avl_dump (tree, 0);
	avl_find (tree, 800);
	avl_find (tree, 5);
	tree = avl_iterator (tree, (*foo), 2);
	avl_delete (tree);
}




