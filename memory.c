/* Mason Wang, masonwang */
/* CS152, Winter 2019 */
/* hw8 */



#include<stdio.h>
#include<stdlib.h>
#include "memory.h"

/* memory_new
 * create a new memory struct, initialze its address and size
 */
memory* memory_new(void* addr, unsigned int size){
    memory* m = (memory*)malloc(sizeof(memory));
    m->addr = addr;
    m->size = size;
    return m;
}

/* free the dynamically allocated memory struct
 */
void memory_free(void* p){
    memory* m = (memory*)p;
    free(m);
}

/* compare two memory variables x and y by address
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_addr_cmp(const void* x, const void* y){
    memory* mem_x = (memory*) x;
    memory* mem_y = (memory*) y;

    //if x before y
    if(mem_x->addr < mem_y->addr)
    {
        return -1;
    }
    //if y before x
    else if(mem_x->addr > mem_y->addr)
    {
        return 1;
    }
    return 0;
}

/* compare two memory variables x and y by size
 * if x is less than y, return -1
 * if x is greater than y, return 1
 * if they are equal, return 0
 */
int memory_size_cmp(const void* x, const void* y){
    memory* mem_x = (memory*) x;
    memory* mem_y = (memory*) y;
    //if x before y
    if(mem_x->size < mem_y->size)
    {
        return -1;
    }
    //if y before x
    else if(mem_x->size > mem_y->size)
    {
        return 1;
    }
    return 0;
}

/* print the memory address and size
 */
void memory_print(void* data){
    if (data == NULL) return;
    memory* m = (memory*)data;
    printf("address: %p, size: %u\n", m->addr, m->size);
}


/* allocate_memory_page
 *
 * Call malloc to request a page of data - 4096 bytes. Create
 * a memory struct and initialize it to store the resulting
 * large chunk of data that was allocated. Return a pointer
 * to the memory struct.
 */
memory *allocate_memory_page()
{
    void* addr;
    addr = malloc(4096);
    int* sizeloc = (int*)addr;
    *sizeloc = 4088;
    return memory_new(addr, 4088);
}


/* split_memory
 *
 * Given a memory struct and a desired size of memory,
 * perform the operations necessary to remove the desired
 * size of memory from the end of the chunk and record
 * the new information for the smaller chunk. Return a
 * pointer to the beginning of the chunk you are handing out.
 */
void *split_memory(memory* data, unsigned int size_desired)
{
    //if there is not enough space
    //printf("\n%d\n", data->size);

    if(data->size < size_desired + 8)
    {
        fprintf(stderr, "error split_memory: size_desired too large\n");
        return NULL;
    }

    //calculate the updated size of the data that was cut off in the memory
    //struct.
    int newsize = data->size - size_desired - 8;

    //change the size of the data that was cut off in the memory struct
    data->size = newsize;

    //change the size of the data that was cut off in the first chunk's 8 bytes
    int* sizeloc = (int*)data->addr;
    *sizeloc = newsize;

    //creates a new char to find the return value
    char* newloc = (char*)data->addr;

    //places newloc where the new chunk's size should be. Then,
    //sets the size of the newly chunked off memory
    newloc = newloc + 8 + newsize;
    int* newchunksize = (int*)newloc;
    *newchunksize = size_desired;

    //places new loc 8 more bytes forward, where
    // the actual memory starts.
    newloc += 8;
    return (void*)newloc;
}


/* merge_memory
 *
 * Given two memory structs, check to see if the two can be
 * merged. They can be merged if the two are next to each other
 * in memory with no break in between. If they can be merged,
 * return a memory struct pointer to a struct containing the information
 * for a single memory chunk containing the old two chunks.
 * If they cannot be merged (there is space between them), then
 * return NULL;
 *
 * Make sure that you free any memory structs that you need to.
 */
memory *merge_memory(memory *first, memory *second)
{
    char* first_end = (char*)first->addr + first->size + 8;
    //if the mems are consecutive
    if((void*)first_end == second->addr)
    {
        first->size = second->size + first->size + 8;
        memory_free(second);
        return first;
    }
    return NULL;
}
