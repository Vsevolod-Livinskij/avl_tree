/*
 * avl_tree.c
 *
 *  Created on: Feb 25, 2014
 *      Author: vsevolod
 */

#include "avl_tree_lib.h"

////////////////////////////////////////////////////////////////////////////////
// Utility functions

static inline node* avl_create (data_t data) {
	node* ret = (node*) calloc(1, sizeof (struct node));
	if (!ret) {
		avl_errno = no_mem;
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
	if (!tree) {
		avl_errno = null_ptr;
		return POISON_VAL;
	}

	return avl_height (tree -> child [left]) -
		   avl_height (tree -> child [right]);
}

static inline void avl_rep_height (node* tree) {
	if (!tree) {
		avl_errno = null_ptr;
		return;
	}
	unsigned char max_height = (avl_height (tree -> child [left]) >
								avl_height (tree -> child [right])) ?
								avl_height (tree -> child [left]) :
								avl_height (tree -> child [right]);
	tree -> height = max_height + 1;
}

static node* avl_sml_left_rot (node* tree) {
	if (!tree || !tree -> child [right]) {
		avl_errno = null_ptr;
		return NULL;
	}
	node* tmp = tree -> child [right];
	tree -> child [right] = tmp -> child [left];
	tmp -> child [left] = tree;
	avl_rep_height (tree);
	avl_rep_height (tmp);
	return tmp;
}

static node* avl_sml_right_rot (node* tree) {
	if (!tree || !tree -> child [left]) {
		avl_errno = null_ptr;
		return NULL;
	}
	node* tmp = tree -> child [left];
	tree -> child [left] = tmp -> child [right];
	tmp -> child [right] = tree;
	avl_rep_height (tree);
	avl_rep_height (tmp);
	return tmp;
}

static node* avl_big_left_rot (node* tree) {
	if (!tree || !tree -> child [right] ||
		!tree -> child [right] -> child [left]) {
		avl_errno = null_ptr;
		return NULL;
	}
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
	if (!tree || !tree -> child [left] ||
		!tree -> child [left] -> child [right]) {
		avl_errno = null_ptr;
		return NULL;
	}
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
	if (!tree) {
		avl_errno = null_ptr;
		return NULL;
	}

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
	if (!tree) {
		avl_errno = null_ptr;
		return NULL;
	}
	if (!tree -> child [left])
		return tree;
	else
		return avl_get_min (tree -> child [left]);
}

node* avl_cut_min (node* tree) {
	if (!tree) {
		avl_errno = null_ptr;
		return NULL;
	}

	if (!tree -> child [left]) {
		return tree -> child [right];
	}
	else {
		tree -> child [left] = avl_cut_min (tree -> child [left]);
		return tree;
	}
}

////////////////////////////////////////////////////////////////////////////////
// User available functions

node* avl_insert (node* tree, data_t data) {
	node* ret = tree;
	if (!tree)
		ret = avl_create (data);
	else if (tree -> data == data) {
		avl_errno = data_exst;
		return NULL;
	} else if (tree -> data > data)
		tree -> child [left] = avl_insert (tree -> child [left], data);
	else
		tree -> child [right] = avl_insert (tree -> child [right], data);

	avl_rep_height (ret);
	return avl_balance (ret);;
}

void avl_dump (node* tree, int shift) {
	for (int i = 0; i < shift; i++)
		printf (" ");

	if (!tree) {
		//printf ("E\n");
		return;
	}

	//printf ("%u :%d | %x\n", tree -> data, tree -> height, tree);
	printf ("\nL: %10x\tN: %10x | %3d\tR: %10x\n",
			tree -> child [left], tree, tree -> data, tree -> child [right]);
	avl_dump (tree -> child [left], shift + shft);
	avl_dump (tree -> child [right], shift + shft);
}

void avl_delete (node* tree) {
	if (!tree) {
		avl_errno = null_ptr;
		return;
	}
	avl_delete (tree -> child [left]);
	tree -> child [left] = NULL;
	avl_delete (tree -> child [right]);
	tree -> child [right] = NULL;
	free (tree);
	tree = NULL;
}

int avl_find (node* tree, data_t key) {
	if (!tree)
		return max_err;
	if (tree -> data == key)
		return success;
	else if (tree -> data > key)
		return avl_find (tree -> child [left], key);
	else
		return avl_find (tree -> child [right], key);
}

node* avl_iterator (node* tree, data_t (*foo) (data_t a, data_t node_data), data_t a) {
	if (!tree) {
		avl_errno = null_ptr;
		return NULL;
	}
	tree -> data = (*foo) (a, tree -> data);
	tree -> child [left] = avl_iterator (tree -> child [left], (*foo), a);
	tree -> child [right] = avl_iterator (tree -> child [right], (*foo), a);
	return tree;
}

node* avl_remove (node* tree, data_t key) {
	if (!tree) {
		avl_errno = null_ptr;
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
