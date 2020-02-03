/* Mason Wang, masonwang */
/* CS152, Winter 2019 */
/* hw8 */


#include <stdio.h>
#include <stdlib.h>
#include "bst.h"
#include "memory.h"
#include "llist.h"

/* Global variables
 * By declaring these outside of a function:
 *   Scope: Every function inside of this file may use them.
 *   		No functions in other files may use them.
 *   Lifetime: They exist for the entire program - they are created
 *		when the program begins and exist until the program
 *		ends.
 */

/* This is the tree that holds the available memory. */
bst *avail_mem = NULL;

/* This includes all of the functions for the memory allocator.
 * The last two functions (my_malloc and my_free) are public
 * functions with prototypes in the header function. The rest of the
 * functions are helper functions only called by my_malloc and
 * my_free. You must implement these helper functions. If you wish,
 * you may add helper functions we did not define.
 */

/* compact_memory
 *
 * Search through all available memory and attempt to merge memory
 * that is stored next to each other.
 * This uses global variable avail_mem, so it does not need any
 * input parameters. Look at the bst functions and memory functions.
 */
void compact_memory()
{
    bst* by_addr = bst_new(memory_addr_cmp);
    void* res = bst_iterate(avail_mem);

    //fills the second bst which is ordered by address
    while(res != NULL)
    {
        bst_insert(by_addr, res);
        res = bst_iterate(NULL);
    }


    //makes that into a list, but the memory addr should be
    //from greatest to smallest
    //to account for the case with multiple consecutive
    //merges
    llist* sort_addr_list = create_llist();
    reverse_order_traversal_insert_llist(by_addr->root, sort_addr_list);

    llist_node* first_node = sort_addr_list->head;
    //while the first node still has a next node
    while(first_node->next != NULL)
    {
        //since the list starts from the last
        //memory addresses, the node is the second mem,
        //and its next is the first address.
        memory* secondmem = (memory*)(first_node->item);
        memory* firstmem = (memory*)(first_node->next->item);

        memory* result = merge_memory(firstmem, secondmem);
        //if we CAN merge, then remove both and readd the first
        if(result != NULL)
        {
            bst_delete(avail_mem, secondmem);
            bst_delete(avail_mem, firstmem);
            bst_insert(avail_mem, result);
        }
        first_node = first_node->next;
    }
}





/* print_available_memory
 *
 * Print out all available memory in ascending order by size.
 */
void print_memory()
{
    bst_inorder_traversal(avail_mem, memory_print);
}
/* init_alloc
 *
 * Initializes the data structures. This initializes avail_mem so
 * that, instead of being a NULL pointer, it points to a valid bst
 * struct whose root pointer is NULL.
 */
void init_alloc()
{
    avail_mem = bst_new(memory_size_cmp);
}


/* my_malloc
 *
 * function that finds a piece of available memory that is at least
 * num_bytes size. A pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(int num_bytes)
{
    int actual_num_bytes = num_bytes + (8-(num_bytes)%8)%8;

    memory* s_item = memory_new(NULL, actual_num_bytes);
    memory* search_result = (memory*)bst_item_or_successor(avail_mem, s_item);
    //if result is NULL, need more memory
    if(search_result == NULL)
    {
        memory* to_ins = allocate_memory_page();
        void* usable = split_memory(to_ins, actual_num_bytes);
        bst_insert(avail_mem, to_ins);
        return usable;
    }
    //case where we can split the big chunk
    if(search_result->size >= 2*(actual_num_bytes))
    {
        void* usable_addr = split_memory(search_result, actual_num_bytes);
        return usable_addr;
    }
    char* usable = (char*)search_result->addr + 8;
    bst_delete(avail_mem, search_result);
    return usable;
}


/* my_free
 *
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent malloc call
 */
void my_free(void *address)
{
    int* sizeloc = ((int*)((char*)address - 8));
    memory* to_ins = memory_new((void*)sizeloc, *sizeloc);
    bst_insert(avail_mem, to_ins);
}
