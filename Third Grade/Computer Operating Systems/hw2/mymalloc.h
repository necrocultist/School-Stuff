#ifndef MYMALLOC_H
#define MYMALLOC_H

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

#define PAGE_SIZE getpagesize()

// Define the structure for a memory block
struct node
{
    size_t size;
    int free;
    struct node *next;
};

// Function declarations
int InitMyMalloc(int HeapSize);
void *MyMalloc(size_t noOfBytes, int strategy);
void MyFree(void *ptr);
void DumpFreeList();

#endif /* MYMALLOC_H */
