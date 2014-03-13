/*
* avl_tree_lib.h
*
* Created on: Feb 25, 2014
* Author: vsevolod
*/

#ifndef AVL_TREE_LIB_H_
#define AVL_TREE_LIB_H_

typedef unsigned int data_t;
typedef struct node node;
typedef struct head head;

struct node;
struct head;

head* avl_init_tree ();
int avl_insert_node (head* head, data_t key);
int avl_dump_tree (head* head);
void avl_delete_tree (head** head);
int avl_find_node (head* head, data_t key);
int avl_iterator_tree (head* head, data_t (*foo) (data_t node_data, void* a), void* a);
int avl_remove_node (head* tree_head, data_t key);
#endif /* AVL_TREE_LIB_H_ */
