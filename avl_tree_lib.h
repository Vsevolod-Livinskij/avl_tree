/*
 * avl_tree_lib.h
 *
 *  Created on: Feb 25, 2014
 *      Author: vsevolod
 */

#ifndef AVL_TREE_LIB_H_
#define AVL_TREE_LIB_H_

#include <stdlib.h>
#include <stdio.h>

#define shft 6
#define POISON_VAL -13666

typedef unsigned int data_t;
typedef struct node node;

int avl_errno;

enum errors {
	success = 0,
	null_ptr = -1,
	no_mem = -2,
	data_exst = -3,
	max_err
};

enum childs {
	left = 0,
	right = 1,
	max_chld
};

struct node {
	data_t data;
	unsigned char height;
	node* child [max_chld];
};

node* avl_insert (node* tree, data_t data);
void avl_dump (node* tree, int shift);
void avl_delete (node* tree);
int avl_find (node* tree, data_t key);
node* avl_iterator (node* tree, data_t (*foo) (data_t a, data_t node_data), data_t a);
node* avl_remove (node* tree, data_t key);
#endif /* AVL_TREE_LIB_H_ */
