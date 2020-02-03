/* Mason Wang, masonwang */
/* CS152, Winter 2019 */
/* hw8 */

#include<stdio.h>
#include<stdlib.h>
#include "bst.h"
#include "llist.h"


/* malloc a new node and assign the data
 * pointer to its data field
 */
node* node_new(void* data){
    node* ret;
    ret = (node*)(malloc(sizeof(node)));
    ret->data = data;
    ret->left = NULL;
    ret->right = NULL;
    return ret;
}

/* create a new bst, initialize its root to be NULL
 */
bst* bst_new(int (*cmp)(const void* x, const void* y)){
    bst* ret;
    ret = (bst*)malloc(sizeof(bst));
    ret->root = NULL;
    ret->cmp = cmp;
    return ret;
}

/* Insert a node to to a subtree with a root node as parameter
 * Insertion is in sorted order.
 * Return the new root of the modified subtree.
 */
//if they're equal, I decided to put it toward the right.
node* node_insert(node* root, void* data,
int (*cmp)(const void* x, const void* y)){
    //if root is null, point to new root
    if(root == NULL)
    {
        return node_new(data);
    }
    node* temp = root;
    node* toplace = node_new(data);

    //while we haven't found the end
    while(temp != NULL)
    {
        //if the data is left of the temp
        if(cmp(data, temp->data) == -1)
        {
            //if left is available
            if(temp->left == NULL)
            {
                temp->left = toplace;
                return root;
            }
            temp = temp->left;
        }
        //if data is left of the temp
        else
        {
            //if right is available, place
            if(temp->right == NULL)
            {
                temp->right = toplace;
                return root;
            }
            temp = temp->right;
        }
    }
    return root;
}

/* Insert a new node to the bst
 */
void bst_insert(bst* b, void* data){
    node* newroot = node_insert(b->root, data, b->cmp);
    b->root = newroot;
}


/* finds the rightmost node of a tree
 */
node* find_rightmost(node* root, int (*cmp)(const void* x, const void* y)){
    //return NULL if empty tree
    if(root == NULL)
    {
        return NULL;
    }
    //if both children empty, return root
    if((root->right == NULL)&&(root->left == NULL))
    {
        return root;
    }
    //if right empty but not left, look in left
    if(root->right == NULL)
    {
        return find_rightmost(root->left, cmp);
    }
    //if right not empty, recurse on right
    else
    {
        return find_rightmost(root->right, cmp);
    }
}

/* delete a node from a subtree with a given root node
 * use the comparison function to search the node and delete
 * it when a matching node is found. This function only
 * deletes the first occurrence of the node, i.e, if multiple
 * nodes contain the data we are looking for, only the first node
 * we found is deleted.
 * Return the new root node after deletion.
 */
node* node_delete(node* root, void* data,
    int (*cmp)(const void* x, const void* y)){

    //case 1: null root
    if(root == NULL)
    {
        return NULL;
    }




    node* l = root->left;
    node* r = root->right;

    //case 2 -- root has no children
    if((l == NULL)&&(r == NULL))
    {
        //root is to be removed
        if(cmp(root->data, data)==0)
        {
            free(root);
            return NULL;
        }
        //root is not to be removed
        else
        {
            return root;
        }
    }

    //case 3: root is the node to remove and has children
    if(cmp(root->data, data) == 0)
    {
        //if root has only right and we want to remove it
        if(l == NULL)
        {
            free(root);
            return r;
        }
        //if root has only left and we want to remove it
        else if(r == NULL)
        {
            free(root);
            return l;
        }
        //if both children exist, find the rightmost of the
        //left subtree, copy it over to root, and delete it.
        node* newrootdata = find_rightmost(l, cmp)->data;
        node_delete(root, newrootdata, cmp);
        root->data = newrootdata;
        return root;
    }


    //case 4-root is not the node to remove, right is null but not left
    if(r == NULL)
    {
        root->left = node_delete(l, data, cmp);
        return root;
    }

    //case 5-root is not the node to remove, left is null but not right
    if(l == NULL)
    {
        root->right = node_delete(r, data, cmp);
        return root;
    }

    //case 6 - left and right are non-null, root is not the node to remove
    //looking at root, if the node to delete should be on the left
    if(cmp(data, root->data) == -1)
    {
        //if the left node itself is to be deleted
        if(cmp(data, l) == 0)
        {
            //if the left subtree (to be deleted) has no children
            if((l->left == NULL)&&(l->right == NULL))
            {
                root->left = NULL;
                free(l);
                return root;
            }
            //if the left subtree (to be deleted) has a left child only
            if((l->left != NULL)&&(l->right == NULL))
            {
                root->left = l->left;
                free(l);
                return root;
            }
            //if the left subtree (to be deleted) has a right child only
            if((l->left == NULL)&&(l->right != NULL))
            {
                root->left = l->right;
                free(l);
                return root;
            }
            //if the left subtree (to be deleted) has both children,
            //the case is the same as if the left node is not to
            //be deleted (see below)
        }
        //if the left node itself is not to be deleted
        root->left = node_delete(l, data, cmp);
        return root;
    }

    //looking at root, if the node to delete should be on the right
    else if(cmp(data, root->data) == 1)
    {
        //if the right node itself is to be deleted
        if(cmp(data, r) == 0)
        {
            //if the right subtree (to be deleted) has no children
            if((r->left == NULL)&&(r->right == NULL))
            {
                root->right = NULL;
                free(r);
                return root;
            }
            //if the right subtree (to be deleted) has a left child only
            if((r->left != NULL)&&(r->right == NULL))
            {
                root->right = r->left;
                free(r);
                return root;
            }
            //if the right subtree (to be deleted) has a right child only
            if((r->left == NULL)&&(r->right != NULL))
            {
                root->right = r->right;
                free(r);
                return root;
            }
            //if the right subtree (to be deleted) has both children,
          // then it's the same case
            //as if the right node itself is not to be deleted.
        }
        //if the right node itself is not to be deleted
        root->right = node_delete(r, data, cmp);
        return root;
    }
    printf("\noops\n");
    return root;
}
/* delete a node containing data from the bst
 */
void bst_delete(bst* b, void* data){
    b->root = node_delete(b->root, data, b->cmp);
}

/* Search for a node containing data in a subtree with
 * a given root node. Use recursion to search that node.
 * Return the first occurrence of node.
 */
void* node_search(node* root, void* data,
    int (*cmp)(const void* x, const void* y)){

    //not found case (base case)
    if(root == NULL)
    {
        fprintf(stderr, "error node_search: not found");
        return NULL;
    }
    //should we look at the left?
    if(cmp(data, root->data) == -1)
    {
        return node_search(root->left, data, cmp);
    }
    //...or the right?
    else if(cmp(data, root->data) == 1)
    {
        return node_search(root->right, data, cmp);
    }
    //if it's found, it's the node
    else
    {
        return root->data;
    }
}

/* Search a node with data in a bst.
 */
void* bst_search(bst* b, void* data){
    return node_search(b->root, data, b->cmp);
}

/* traverse a subtree with root in ascending order.
 * Apply func to the data of each node.
 */
void inorder_traversal(node* root, void(*func)(void* data)){

    //does nothing to empty tree/base case
    if(root == NULL)
    {
        return;
    }


    inorder_traversal(root->left, func);
    func(root->data);
    inorder_traversal(root->right, func);
}

/* traverse a bst in ascending order.
 * Apply func to the data of each node.
 */
void bst_inorder_traversal(bst* b, void(*func)(void* data)){
    inorder_traversal(b->root, func);
}







//STUFF FOR HW8 STARTS

// a helper function to create linked list from bst by in order traversal
void inorder_traversal_insert_llist(node* root, llist* l){
    //does nothing to empty tree/base case
    if(root == NULL)
    {
        return;
    }

    inorder_traversal_insert_llist(root->left, l);
    insert_tail(l, root->data);
    inorder_traversal_insert_llist(root->right, l);
}

// a helper function to create linked list from bst by backwards order traversal
void reverse_order_traversal_insert_llist(node* root, llist* l){
    //does nothing to empty tree/base case
    if(root == NULL)
    {
        return;
    }

    reverse_order_traversal_insert_llist(root->left, l);
    insert_head(l, root->data);
    reverse_order_traversal_insert_llist(root->right, l);


}

/* an iterator to iterate through the bst in ascending order
 */
void* bst_iterate(bst* b){

    //static initialization of the list to
    // an empty list.
    static llist *tree_list = NULL;
    if(tree_list == NULL)
    {
        tree_list = create_llist();
    }

    //if bst given is NOT null
    if(b)
    {
        inorder_traversal_insert_llist(b->root, tree_list);
    }
    //if the list is over, or if the list is empty;
    //return NULL.
    if((list_is_empty(tree_list))||(tree_list->head == NULL))
    {
        return NULL;
    }
	// get out the item for the current node
	void *item = tree_list->head->item;
	// advance for the next call
	tree_list->head = tree_list->head->next;
	return item;
}


/* an iterator to iterate a node in ascending order
 */
void* node_iterate(node* root){

    //static initialization of the list to
    // an empty list.
    static llist *node_list = NULL;
    //if list null, make empty
    if(node_list == NULL)
    {
        node_list = create_llist();
    }

    //if node given is NOT null
    if(root)
    {
        inorder_traversal_insert_llist(root, node_list);
    }
    //if the list is over, or if the list is empty;
    //return NULL.
    if((list_is_empty(node_list))||(node_list->head == NULL))
    {
        return NULL;
    }
	// get out the item for the current node
	void *item = node_list->head->item;
	// advance for the next call
	node_list->head = node_list->head->next;
	return item;
}



/* node_free
 * frees a node and a bunch of subnodes of it.
 * doesn't free the data field, just the
 * node architectures
 */

void node_free(node* root)
{
    //doesn't free anything if null
    if(root == NULL)
    {
        return;
    }

    node_free(root->left);
    node_free(root->right);
    free(root);
}

/* free the bst with
 */
void bst_free(bst* b){
    node_free(b->root);
}





/*****************************
 *    HOMEWORK 7
 *****************************/
/*
 * item_or_successor
 *
 * find an item that is equal or, if there isn't one that is
 * equal, find the one that is next larger to the requested
 * item.
 *
 * The purpose of this function is that, given a size of a
 * chunk of memory desired, it attempts to find an available
 * chunk of memory the same size. If there is no chunk of
 * memory the same size, it finds the chunk of memory that is
 * closest to that size but larger.
 *
 * It returns a pointer to the memory struct. The compare
 * function in the bst is the one that performs the comparisons.
 */
void* node_item_or_successor(node *n, void *item,
        int (*cmp)(const void* x, const void* y))
{

    void* result = node_iterate(n);

    //while result is not null, or nothing has been found.
    while(1)
    {
        if(result == NULL)
        {
            return NULL;
        }
        //if result is less than item, then
        // a comparison will return -1.
        //thus, if result is greater than or
        //equal to item, we have found our match,
        //and will not get -1.
        if(cmp(result, item)>-1)
        {
            return result;
        }
        result = node_iterate(NULL);
    }
}
//bst version, see above desc
void* bst_item_or_successor(bst *b, void *item)
{
    void* suc_result = node_item_or_successor(b->root, item, b->cmp);
    return suc_result;
}
