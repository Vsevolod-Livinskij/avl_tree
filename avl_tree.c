/*
 * avl_tree.c
 *
 *  Created on: Feb 25, 2014
 *      Author: vsevolod
 */

#include "avl_tree_lib.h"

////////////////////////////////////////////////////////////////////////////////
// Utility functions

static inline node* avl_create (head* tree_head, data_t data)
{
	node* ret = (node*) calloc(1, sizeof (struct node));
	if (!ret) {
		tree_head -> head_errno = no_mem;
		return NULL;
	}
	ret -> data = data;
	ret -> height = 0;
	ret -> child [left] = NULL;
	ret -> child [right] = NULL;
	return ret;
}

static inline int avl_height (node* tree)
{
	return tree ? tree -> height : 0;
}

static inline int avl_bal_fact (head* tree_head, node* tree)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
		return POISON_VAL;
	}

	return avl_height (tree -> child [left]) -
		   avl_height (tree -> child [right]);
}

static inline void avl_rep_height (head* tree_head, node* tree)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
		return;
	}
	unsigned char max_height = (avl_height (tree -> child [left]) >
								avl_height (tree -> child [right])) ?
								avl_height (tree -> child [left]) :
								avl_height (tree -> child [right]);
	tree -> height = max_height + 1;
}

static node* avl_sml_left_rot (head* tree_head, node* tree)
{
	if (!tree || !tree -> child [right]) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}
	node* tmp = tree -> child [right];
	tree -> child [right] = tmp -> child [left];
	tmp -> child [left] = tree;
	avl_rep_height (tree_head, tree);
	avl_rep_height (tree_head, tmp);
	return tmp;
}

static node* avl_sml_right_rot (head* tree_head, node* tree)
{
	if (!tree || !tree -> child [left]) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}
	node* tmp = tree -> child [left];
	tree -> child [left] = tmp -> child [right];
	tmp -> child [right] = tree;
	avl_rep_height (tree_head, tree);
	avl_rep_height (tree_head, tmp);
	return tmp;
}

static node* avl_big_left_rot (head* tree_head, node* tree)
{
	if (!tree || !tree -> child [right] ||
		!tree -> child [right] -> child [left]) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}
	node* chld_r = tree -> child [right];
	node* chld_l = tree -> child [right] -> child [left];
	tree -> child [right] = chld_l -> child [left];
	chld_r -> child [left]= chld_l -> child [right];
	chld_l -> child [left] = tree;
	chld_l -> child [right] = chld_r;
	avl_rep_height (tree_head, tree);
	avl_rep_height (tree_head, chld_r);
	avl_rep_height (tree_head, chld_l);
	return chld_l;
}

static node* avl_big_right_rot (head* tree_head, node* tree)
{
	if (!tree || !tree -> child [left] ||
		!tree -> child [left] -> child [right]) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}
	node* chld_r = tree -> child [left];
	node* chld_l = tree -> child [left] -> child [right];
	tree -> child [left] = chld_l -> child [right];
	chld_r -> child [right]= chld_l -> child [left];
	chld_l -> child [right] = tree;
	chld_l -> child [left] = chld_r;
	avl_rep_height (tree_head, tree);
	avl_rep_height (tree_head, chld_r);
	avl_rep_height (tree_head, chld_l);
	return chld_l;
}

static node* avl_balance (head* tree_head, node* tree)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}

	if (avl_bal_fact (tree_head, tree) == -2) {
		if (avl_bal_fact (tree_head, tree -> child [right]) <= 0)
			return avl_sml_left_rot (tree_head, tree);
		else
			return avl_big_left_rot (tree_head, tree);
	} else if (avl_bal_fact (tree_head, tree) == 2) {
		if (avl_bal_fact (tree_head, tree -> child [left]) >= 0)
			return avl_sml_right_rot (tree_head, tree);
		else
			return avl_big_right_rot (tree_head, tree);
	}
	return tree;
}

static node* avl_get_min (head* tree_head, node* tree)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}
	if (!tree -> child [left])
		return tree;
	else
		return avl_get_min (tree_head, tree -> child [left]);
}

static node* avl_cut_min (head* tree_head, node* tree)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}

	if (!tree -> child [left]) {
		return tree -> child [right];
	}
	else {
		tree -> child [left] = avl_cut_min (tree_head, tree -> child [left]);
		return tree;
	}
}

////////////////////////////////////////////////////////////////////////////////
// Recursive functions

static node* avl_insert (head* tree_head, node* tree, data_t data)
{
	node* ret = tree;
	if (!tree)
		ret = avl_create (tree_head, data);
	else if (tree -> data == data) {
		tree_head -> head_errno = data_exst;
		return NULL;
	} else if (tree -> data > data)
		tree -> child [left] = avl_insert (tree_head,
										   tree -> child [left], data);
	else
		tree -> child [right] = avl_insert (tree_head,
											tree -> child [right], data);

	avl_rep_height (tree_head, ret);
	return avl_balance (tree_head, ret);
}

static void avl_dump (node* tree, int shift)
{
	for (int i = 0; i < shift; i++)
		printf (" ");

	if (!tree) {
		printf ("E\n");
		return;
	}

	printf ("%u :%d | %x\n", tree -> data, tree -> height, tree);
	//printf ("\nL: %10x\tN: %10x | %3d\tR: %10x\n",
	//		tree -> child [left], tree, tree -> data, tree -> child [right]);
	avl_dump (tree -> child [left], shift + shft);
	avl_dump (tree -> child [right], shift + shft);
}

static void avl_delete (head* tree_head, node* tree)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
		return;
	}
	avl_delete (tree_head, tree -> child [left]);
	tree -> child [left] = NULL;
	avl_delete (tree_head, tree -> child [right]);
	tree -> child [right] = NULL;
	free (tree);
	tree = NULL;
}

static int avl_find (node* tree, data_t key)
{
	if (!tree)
		return max_err;
	if (tree -> data == key)
		return success;
	else if (tree -> data > key)
		return avl_find (tree -> child [left], key);
	else
		return avl_find (tree -> child [right], key);
}

static node* avl_iterator (head* tree_head, node* tree,
		            data_t (*foo) (data_t a, data_t node_data), data_t a)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
		return NULL;
	}
	tree -> data = (*foo) (a, tree -> data);
	tree -> child [left] = avl_iterator (tree_head, tree -> child [left],
															  (*foo), a);
	tree -> child [right] = avl_iterator (tree_head, tree -> child [right],
																(*foo), a);
	return tree;
}

static node* avl_remove (head* tree_head, node* tree, data_t key)
{
	if (!tree) {
		tree_head -> head_errno = null_ptr;
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
			 *tmp_min = avl_get_min (tree_head, tmp_right);

		tmp_min -> child [right] = avl_cut_min (tree_head, tmp_right);
		tmp_min -> child [left] = tmp_left;
		free (tree);
		return tmp_min;

	} else if (tree -> data > key)
		tree -> child [left] = avl_remove (tree_head, tree -> child [left], key);
	else
		tree -> child [right] = avl_remove (tree_head, tree -> child [right], key);

	avl_rep_height (tree_head, tree);
	return avl_balance (tree_head, tree);
}

///////////////////////////////////////////////////////////////////////////////
// User available functions
head* avl_init_tree ()
{
	head* tmp = (head*) calloc (1, sizeof (head));
	if (!tmp)
		tmp -> head_errno = 0;
	return tmp;
}

int avl_insert_node (head* tree_head, data_t key)
{
	if (!tree_head)
		return null_ptr;
	tree_head -> tree = avl_insert (tree_head, tree_head -> tree, key);
	return tree_head -> head_errno;
}

int avl_dump_tree (head* tree_head)
{
	if (!tree_head)
		return null_ptr;
	avl_dump (tree_head -> tree, 0);
	return success;
}

int avl_delete_tree (head* tree_head)
{
	if (!tree_head)
		return null_ptr;
	if (tree_head -> tree)
		avl_delete (tree_head, tree_head -> tree);
	tree_head -> tree = NULL;
	int tmp = tree_head -> head_errno;
	free (tree_head);
	return tmp;
}

int avl_find_tree (head* tree_head, data_t key) {
	if (!tree_head)
		return null_ptr;

	return avl_find (tree_head -> tree, key);
}

int avl_iterator_tree (head* tree_head, data_t (*foo) (data_t a, data_t node_data),
		                                                                 data_t a)
{
	if (!tree_head)
		return null_ptr;
	tree_head -> tree = avl_iterator (tree_head, tree_head -> tree, (*foo), a);
	return tree_head -> head_errno;
}

int  avl_remove_tree (head* tree_head, data_t key)
{
	if (!tree_head)
		return null_ptr;
	tree_head -> tree = avl_remove (tree_head, tree_head -> tree, key);
	return tree_head -> head_errno;
}
