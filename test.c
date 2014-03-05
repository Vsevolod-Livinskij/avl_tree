/*
 * test.c
 *
 *  Created on: Feb 25, 2014
 *      Author: vsevolod
 */

#include "avl_tree_lib.h"

int main() {
	node* tree = avl_insert (NULL, 47);
	tree = avl_insert (tree, 44);
	tree = avl_insert (tree, 18);
	tree = avl_insert (tree, 65);
	tree = avl_insert (tree, 61);
	tree = avl_insert (tree, 98);
	tree = avl_insert (tree, 85);
	tree = avl_insert (tree, 5);
	tree = avl_insert (tree, 45);
	tree = avl_insert (tree, 17);
	tree = avl_insert (tree, 23);
	tree = avl_insert (tree, 7);
	tree = avl_insert (tree, 0);
	tree = avl_insert (tree, 150);
	tree = avl_insert (tree, 72);
	tree = avl_insert (tree, 66);
	tree = avl_insert (tree, 81);
	avl_dump (tree, 0);
	printf ("\n**************************\n");
	tree = avl_insert (tree, 81);
	avl_find (tree, 666);
	avl_find (tree, 47);
	avl_find (tree, 33);
	avl_delete (NULL);
	node* overload = avl_insert (NULL, 0);
	for (int i = 1; i < 10000; i++) {
		overload = avl_insert (overload, i);
	}
	//avl_dump (overload, 0);
	printf ("\n res: %d\n", avl_errno);
	avl_delete (tree);
	avl_delete (overload);
}




