/*
 * avl_tree.c
 *
 *  Created on: Feb 25, 2014
 *      Author: vsevolod
 */

#include "avl_tree_lib.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define shft 6
#define POISON_VAL -13666

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
	node* tree;
};

////////////////////////////////////////////////////////////////////////////////
// Utility functions

static inline node* avl_create (data_t data) {
	node* ret = (node*) calloc (1, sizeof (struct node));
//printf ("\n1) %x\n", ret);
	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}

	ret -> data = data;
	ret -> height = 0;
	ret -> child [left] = NULL;
	ret -> child [right] = NULL;
	return ret;
}

static inline int avl_height (node* tree) {
	return tree ? tree -> height : 0;
}

static inline int avl_bal_fact (node* tree) {
	return avl_height (tree -> child [left]) -
		   avl_height (tree -> child [right]);
}

static inline void avl_rep_height (node* tree) {
	unsigned char max_height = (avl_height (tree -> child [left]) >
								avl_height (tree -> child [right])) ?
								avl_height (tree -> child [left]) :
								avl_height (tree -> child [right]);
	tree -> height = max_height + 1;
}

static node* avl_sml_left_rot (node* tree) {
	node* tmp = tree -> child [right];
	tree -> child [right] = tmp -> child [left];
	tmp -> child [left] = tree;
	avl_rep_height (tree);
	avl_rep_height (tmp);
	return tmp;
}

static node* avl_sml_right_rot (node* tree) {
	node* tmp = tree -> child [left];
	tree -> child [left] = tmp -> child [right];
	tmp -> child [right] = tree;
	avl_rep_height (tree);
	avl_rep_height (tmp);
	return tmp;
}

static node* avl_big_left_rot (node* tree) {
	node* chld_r = tree -> child [right];
	node* chld_l = tree -> child [right] -> child [left];
	tree -> child [right] = chld_l -> child [left];
	chld_r -> child [left]= chld_l -> child [right];
	chld_l -> child [left] = tree;
	chld_l -> child [right] = chld_r;
	avl_rep_height (tree);
	avl_rep_height (chld_r);
	avl_rep_height (chld_l);
	return chld_l;
}

static node* avl_big_right_rot (node* tree) {
	node* chld_r = tree -> child [left];
	node* chld_l = tree -> child [left] -> child [right];
	tree -> child [left] = chld_l -> child [right];
	chld_r -> child [right]= chld_l -> child [left];
	chld_l -> child [right] = tree;
	chld_l -> child [left] = chld_r;
	avl_rep_height (tree);
	avl_rep_height (chld_r);
	avl_rep_height (chld_l);
	return chld_l;
}

static node* avl_balance (node* tree) {
	if (avl_bal_fact (tree) == -2) {
		if (avl_bal_fact (tree -> child [right]) <= 0)
			return avl_sml_left_rot (tree);
		else
			return avl_big_left_rot (tree);
	} else if (avl_bal_fact (tree) == 2) {
		if (avl_bal_fact (tree -> child [left]) >= 0)
			return avl_sml_right_rot (tree);
		else
			return avl_big_right_rot (tree);
	}
	return tree;
}

static node* avl_get_min (node* tree) {
	if (!tree -> child [left])
		return tree;
	else
		return avl_get_min (tree -> child [left]);
}

static node* avl_cut_min (node* tree) {
	if (!tree -> child [left]) {
		return tree -> child [right];
	}
	else {
		tree -> child [left] = avl_cut_min (tree -> child [left]);
		return tree;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Recursive functions

static node* avl_insert (node* tree, data_t data) {
	node* ret = tree;
	if (!tree)
		ret = avl_create (data);
	else if (tree -> data == data) {
		return tree;
	} else if (tree -> data > data)
		tree -> child [left] = avl_insert (tree -> child [left], data);
	else
		tree -> child [right] = avl_insert (tree -> child [right], data);

	if (!ret) {
		errno = ENOMEM;
		return NULL;
	}

	avl_rep_height (ret);
	return avl_balance (ret);
}

static void avl_dump (node* tree, int shift) {
	for (int i = 0; i < shift; i++)
		printf (" ");

	if (!tree) {
		printf ("E\n");
		return;
	}

	printf ("%u : %d | %x\n", tree -> data, tree -> height, tree);
	//printf ("\nL: %10x\tN: %10x | %3d\tR: %10x\n",
	//		tree -> child [left], tree, tree -> data, tree -> child [right]);
	avl_dump (tree -> child [left], shift + shft);
	avl_dump (tree -> child [right], shift + shft);
}

static void avl_delete (node* tree) {
	if (!tree) {
		errno = EBADF;
		return;
	}
	avl_delete (tree -> child [left]);
	tree -> child [left] = NULL;
	avl_delete (tree -> child [right]);
	tree -> child [right] = NULL;
	free (tree);
	tree = NULL;
}

static int avl_find (node* tree, data_t key) {
	if (!tree)
		return -1;
	if (tree -> data == key)
		return 0;
	else if (tree -> data > key)
		return avl_find (tree -> child [left], key);
	else
		return avl_find (tree -> child [right], key);
}

static node* avl_iterator (node* tree, data_t (*foo) (data_t node_data, void* a), void* a) {
	if (!tree) {
		errno = EBADF;
		return NULL;
	}
	tree -> data = (*foo) (tree -> data, a);
	tree -> child [left] = avl_iterator (tree -> child [left], (*foo), a);
	tree -> child [right] = avl_iterator (tree -> child [right], (*foo), a);
	return tree;
}

static node* avl_remove (node* tree, data_t key) {
	if (!tree) {
		errno = EBADF;
		return NULL;
	}
	if (tree -> data == key) {
		if (!tree -> child [right]) {
			node* tmp = tree -> child [left];
			free (tree);
			return tmp;
		}
		node *tmp_left  = tree -> child [left],
			 *tmp_right = tree -> child [right],
			 *tmp_min = avl_get_min (tmp_right);

		tmp_min -> child [right] = avl_cut_min (tmp_right);
		tmp_min -> child [left] = tmp_left;
		free (tree);
		return tmp_min;

	} else if (tree -> data > key)
		tree -> child [left] = avl_remove (tree -> child [left], key);
	else
		tree -> child [right] = avl_remove (tree -> child [right], key);

	avl_rep_height (tree);
	return avl_balance (tree);
}

////////////////////////////////////////////////////////////////////////////////
// User available functions

head* avl_init_tree ()
{
	head* tmp = (head*) calloc (1, sizeof (head));
	if (!tmp) {
		errno = ENOMEM;
		return NULL;
	}

	return tmp;
}

int avl_insert_node (head* head, data_t key)
{
	if (!head) {
		errno = EBADF;
		return -1;
	}

	head -> tree = avl_insert (head -> tree, key);
	if (!head -> tree)
		return -1;
	else
		return 0;
}

int avl_dump_tree (head* head)
{
	if (!head) {
		errno = EBADF;
		return -1;
	}

	avl_dump (head -> tree, 0);
	return 0;
}

void avl_delete_tree (head** head)
{
	if (!head || !(*head)) {
		errno = EBADF;
		return;
	}

	if ((*head) -> tree)
		avl_delete ((*head) -> tree);
	free ((*head));
	*head = NULL;
}

int avl_find_node (head* head, data_t key) {
	if (!head) {
		errno = EBADF;
		return -1;
	}

	return avl_find (head -> tree, key);
}

int avl_iterator_tree (head* head, data_t (*foo) (data_t node_data, void* a), void* a)
{
	if (!head || !foo) {
		errno = EBADF;
		return -1;
	}

	avl_iterator (head -> tree, (*foo), a);
	return 0;
}

int avl_remove_node (head* head, data_t key)
{
	if (!head) {
		errno = EBADF;
		return -1;
	}

	head -> tree = avl_remove (head -> tree, key);
	if (!head -> tree)
		return -1;
	else
		return 0;
}
