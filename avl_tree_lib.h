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
typedef struct head head;

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

struct head {
	int head_errno;
	node* tree;
};

head* avl_init_tree ();
int avl_insert_node (head* tree_head, data_t key);
int avl_dump_tree (head* tree_head);
int avl_delete_tree (head* tree_head);
int avl_find_tree (head* tree_head, data_t key);
int avl_iterator_tree (head* tree_head, data_t (*foo) (data_t a, data_t node_data),
		                                                                 data_t a);
int  avl_remove_tree (head* tree_head, data_t key);
#endif /* AVL_TREE_LIB_H_ */
