/* Mason Wang, masonwang */
/* CS152, Winter 2019 */
/* hw8 */


#ifndef MY_ALLOC_H
#define MY_ALLOC_H
#include <stdio.h>
#include <stdlib.h>


/* print_available_memory
 *
 * Print out all available memory in ascending order by size.
 */
void print_memory();

/* init_alloc
 *
 * Initializes the data structures of the allocator.
 * This must be called prior to any my_malloc and my_free calls.
 */
void init_alloc();

/* my_malloc
 *
 * function that finds a piece of available memory that is at least
 * num_bytes size. A pointer to the beginning of the usable piece of
 * that chunk is returned.
 */
void *my_malloc(int num_bytes);


/* my_free
 *
 * Function that returns the memory chunk whose usable piece starts
 * at that address back to the set of available memory so that it can
 * be reused in a subsequent free call
 */
void my_free(void *address);

/* compact_memory
 *
 * Search through all available memory and attempt to merge memory
 * that is stored next to each other.
 * This uses global variable avail_mem, so it does not need any
 * input parameters. Look at the bst functions and memory functions.
 */
void compact_memory();

#endif
