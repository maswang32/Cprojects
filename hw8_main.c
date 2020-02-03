/* Mason Wang, masonwang */
/* CS152, Winter 2019 */
/* hw8 */




#include<stdio.h>
#include<stdlib.h>
#include "memory.h"
#include "llist.h"
#include "bst.h"
#include "my_alloc.h"

extern bst* avail_mem;



/* print__mem_list
 * purpose: made by me to print memory lists while testing
 * inputs: list - the list
 * outputs: screen
 *
 */
void print_list(llist *list)
{
    llist_node* temp = list->head;
    //before end of list
    while(temp != NULL)
    {
        memory_print(temp->item);
        temp = temp->next;
    }
}



/* test_allocate_memory_page_and_split
 * purpose: tests allocate mem page and split
 * inputs:
 * outputs: screen
 *
 */
void test_allocate_memory_page_and_split()
{
    printf("\n\nNow testing allocate memory page and Split\n"
           "First, the memory page is this: \n");
    memory* page = allocate_memory_page();
    memory_print(page);
    printf("\n\nNow, we split 128 bytes off of it.");
    void* begin_addr = split_memory(page, 128);
    printf("\nMaking sure the locations are right:");
    printf("\nResulting memory structure:\n");
    memory_print(page);
    printf("\nSize of page, given by the first 8 bytes of the page:\n");
    int bytesize = *((int*)page->addr);
    printf("%d bytes", bytesize);
    printf("\nSizeof split chunk, by preceding 8 bytes of the split chunk:\n");
    printf("%d bytes", *((int*)((char*)begin_addr - 8)));
    printf("\n\nDifference between start of page and start of new chunk:\n");
    printf("%ld bytes, expected 4096-128 =3968",
	 (char*)begin_addr - (char*)page->addr);
}


/* test_traversals
 * purpose: tests traversals
 * inputs: bst b
 * outputs: screen
 *
 */
void test_traversals(bst* b)
{
    printf("Creating list and filling it inorder:\n");
    llist* testlist = create_llist();
    inorder_traversal_insert_llist(b->root, testlist);
    printf("\nResult:\n");
    print_list(testlist);

    printf("\n\nCreating list and filling it reverse order:\n");
    llist* revtestlist = create_llist();
    reverse_order_traversal_insert_llist(b->root, revtestlist);
    printf("\nResult:\n");
    print_list(revtestlist);
}
/* test_iterate
 * purpose: tests iterate funcs
 * inputs: bst b
 * outputs: screen
 *
 */
void test_iterate(bst *b)
{

    printf("\nFirst result of iterate:\n");
    memory* result = (memory*)bst_iterate(b);
    memory_print(result);

    printf("\nSecond result of iterate:\n");
    result = (memory*)bst_iterate(NULL);
    memory_print(result);

    printf("\nThird result of iterate:\n");
    result = (memory*)bst_iterate(NULL);
    memory_print(result);

    printf("\nFourth result of iterate:\n");
    result = (memory*)bst_iterate(NULL);
    memory_print(result);

    printf("\nFifth result of iterate:\n");
    result = (memory*)bst_iterate(NULL);
    memory_print(result);

    printf("\nSixth result of iterate: (should be NULL)\n");
    result = (memory*)bst_iterate(NULL);
    memory_print(result);

    printf("\n\n\nTests for Successors Begin: node_item_or_successor"
           "and bst_item_or_successor\n");

    printf("\nFirst Test-successor of size 9. Expected to return 4, 10.\n");
    memory* test1 = memory_new((void*)12, 9);
    result = (memory*)bst_item_or_successor(b, test1);
    memory_print(result);



    printf("\nSecond Test-successor of size 200. Expected to return 2, 350.\n");
    memory* test2 = memory_new((void*)12, 200);
    result = (memory*)bst_item_or_successor(b, test2);
    memory_print(result);

    printf("\nThird Test-successor of size 550. Expected to return 3, 550.\n");
    memory* test3 = memory_new((void*)12, 550);
    result = (memory*)bst_item_or_successor(b, test3);
    memory_print(result);


    printf("\nFourth Test-successor of size 600. Expected to return NULL.\n");
    memory* test4 = memory_new((void*)12, 600);
    result = (memory*)bst_item_or_successor(b, test4);
    //if the result is null the test passed
    if(result==NULL)
    {
        printf("Success!\n");
    }
}

/* test3
 * purpose: tests exer 3
 * inputs: none
 * outputs: screen
 */

void test3()
{
    printf("Initializing avail_mem using init_alloc\nResult:\n");
    init_alloc();
    bst_inorder_traversal(avail_mem, memory_print);
    printf("\nExpected an empty tree. We have already tested the"
           " successor functions. Now, test my_malloc.\n\n");
    printf("Since we have an empty tree, expect to create a new page.\n"
           "my_malloc(10) Results in this tree:\n");
    void* result = my_malloc(10);
    print_memory();
    printf("\nExpected leftover size is 4088-16-8 - 4064\n");
    printf("\nAlso, we check the size copied right before the usable memory:"
	" %d bytes",
           *((int*)((char*)result - 8)));


    printf("\n\n\n\nNow testing the case where we split off the end of the"
		" memory"
           "my_malloc(128) results in this tree:\n");
    result = my_malloc(128);
    print_memory();
    printf("\nExpected leftover size is 4064-128-8 - 3928\n");
    printf("\nAlso, we check the size copied right before the usable memory:"
	" %d bytes",
           *((int*)((char*)result - 8)));



    printf("\n\n\n\nNow testing the case where we don't split off the end of"
		" the memory\n"
           "my_malloc(2000) returns this address: ");
    result = my_malloc(2000);
    printf("%p", result);
    printf("\n8 bytes before the result we get %p, which we should verify was"
           " in the tree.", (void*)((char*)result - 8));
    printf("\nThe new tree (which should have that address removed):\n");
    print_memory();

    printf("\n\nWe also verify that all the addresses end in 0 or 8, to check"
           " our rounding...\n\n");

    printf("\n\n\nNow testing my_free, with a usable chunk of 128.\n");
    void* myfreetest = malloc(136*sizeof(char));
    *((int*)myfreetest) = 128;
    myfreetest = (void*)((char*)myfreetest + 8);
    printf("Usable memory starts: %p\n", myfreetest);
    my_free(myfreetest);
    printf("\nResult of my_free:\n");
    print_memory();

}


/* testmerge
 * purpose: tests merge
 * inputs: none
 * outputs: screen
 */


void testmerge()
{
    printf("\nTesting merge. Testing case where merging occurs.\n"
           "first = (0, 128), second = (136, 400). Expected size "
           "536. \n\nResult:\n");
    memory* first = memory_new((void*)0, 128);
    memory* second = memory_new((void*)136, 400);
    memory* result = merge_memory(first, second);
    memory_print(result);


    printf("\nTesting merge. Testing case where merging doesn't occur.\n"
           "first = (0, 128), second = (144, 400). Expected NULL.");
    second = memory_new((void*)144, 400);
    result = merge_memory(first, second);
    //test passed if result is null
    if(result==NULL)
    {
        printf("\nSuccess!\n");
    }
}


/* testcompact
 * purpose: tests compact
 * inputs: none
 * outputs: screen
 */

void testcompact()
{
    printf("\nTesting merge with two things.\n");
    printf("\nInitializing memory. Chunking off and"
           "putting back 128 bytes.\n");

    init_alloc();
    void* to_free = my_malloc(128);
    my_free(to_free);
    printf("Before compaction:\n");
    print_memory();
    printf("\n\nAfter compaction:\n");
    compact_memory();
    print_memory();
    printf("\nExpected Result: size 4088.\n");


    printf("\n\n\nTesting merge with many chunks.\n"
           "\nChunking off 128 and 256 bytes\n"
           "this time.");
    init_alloc();
    to_free = my_malloc(128);
    void* to_free2 = my_malloc(256);
    my_free(to_free2);
    my_free(to_free);
    printf("Before compaction:\n");
    print_memory();
    printf("\n\nAfter compaction:\n");
    compact_memory();
    print_memory();
    printf("\nExpected Result: size 4088.\n");
}

int main()
{
    printf("\n\n\n\n\n----------TESTING BEGINS-----------\n\n\n\n\n");
    printf("\n\n\n----Now Testing allocate page and split");
    test_allocate_memory_page_and_split();


    printf("\n\n---Making a test tree---\nLooks like:\n");
    bst* testtree = bst_new(memory_size_cmp);
    memory* m1 = memory_new((void*)1, 500);
    memory* m2 = memory_new((void*)2, 350);
    memory* m3 = memory_new((void*)3, 550);
    memory* m4 = memory_new((void*)4, 10);
    memory* m5 = memory_new((void*)5, 64);
    bst_insert(testtree, m1);
    bst_insert(testtree, m2);
    bst_insert(testtree, m3);
    bst_insert(testtree, m4);
    bst_insert(testtree, m5);
    bst_inorder_traversal(testtree, memory_print);

    printf("\n\n\n----Now Testing Traversal to List (assuming this is what is"
          " meant by having tests for helpers)----\n\n\n");
    test_traversals(testtree);

    printf("\n\n\n----Now Testing Iterate, helpers, and Successor----\n\n\n");
    test_iterate(testtree);


    printf("\n\n\n----Now Testing Exercise 3----\n\n\n");
    test3();

    printf("\n\n\n----Now Testing Merge----\n\n\n");
    testmerge();

    printf("\n\n\n----Now Testing Compaction---\n\n\n");
    testcompact();








    return 0;
}

