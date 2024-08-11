#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include "mymalloc.h"
#include <bits/mman-linux.h>
#define PAGE_SIZE getpagesize()

void *heapStart = NULL;
struct node *memorylist = NULL;

// Function to initialize the memory list
int InitMyMalloc(int HeapSize)
{
    // If the memorylist is already initialized or the HeapSize is not positive, return -1
    if (memorylist != NULL || HeapSize <= 0)
    {
        return -1;
    }

    // Adjust HeapSize to the nearest multiple of PAGE_SIZE

    HeapSize = (HeapSize + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;

    // Request memory for the heap using mmap
    memorylist = (struct node *)mmap(NULL, HeapSize, PROT_READ | PROT_WRITE, MAP_PRIVATE | -1, -1, 0);

    if (memorylist == MAP_FAILED)
    {
        return -1; // Memory allocation failed
    }

    // Initialize the memorylist node
    memorylist->size = HeapSize - sizeof(struct node);
    memorylist->free = 1;
    memorylist->next = NULL;

    return 0; // Success
}

// Function to divide a block into two parts
void divideBlock(struct node *suitableBlock, size_t requestedSize)
{
    // Create a new block in the remaining space after allocation
    struct node *extraBlock = (struct node *)((char *)suitableBlock + requestedSize + sizeof(struct node));

    // Update the size, next pointer and free status of the extraBlock
    extraBlock->size = (suitableBlock->size) - requestedSize - sizeof(struct node);
    extraBlock->next = suitableBlock->next;
    extraBlock->free = 1;

    // Update the size, next pointer and free status of the suitableBlock
    suitableBlock->size = requestedSize;
    suitableBlock->next = extraBlock;
    suitableBlock->free = 0;
}

// Function to allocate memory
void *MyMalloc(size_t noOfBytes, int strategy)
{
    static struct node *lastNode = NULL; // For NextFit
    struct node *current, *bestFit = NULL, *worstFit = NULL;
    void *result;

    // Check if the memorylist is initialized
    if (!(memorylist->size))
    {
        printf("Memory not initialized. Call InitMyMalloc()\n");
        return NULL;
    }

    current = memorylist;

    // Choose the allocation strat
    switch (strategy)
    {
    case 0: // BestFit
        // Find the smallest free block that fits
        while (current != NULL)
        {
            if (current->free && current->size >= noOfBytes &&
                (bestFit == NULL || current->size < bestFit->size))
            {
                bestFit = current;
            }
            current = current->next;
        }
        current = bestFit;
        break;
    case 1: // WorstFit
        // Find the largest free block that fits
        while (current != NULL)
        {
            if (current->free && current->size >= noOfBytes &&
                (worstFit == NULL || current->size > worstFit->size))
            {
                worstFit = current;
            }
            current = current->next;
        }
        current = worstFit;
        break;
    case 2: // FirstFit
        // Find the first free block that fits
        while ((((current->size) < noOfBytes) || ((current->free) == 0)) && (current->next != NULL))
        {
            current = current->next;
        }
        break;
    case 3: // NextFit
        // Find the next free block that fits
        if (lastNode == NULL)
        {
            lastNode = memorylist;
        }
        current = lastNode;
        do
        {
            if (current->free && current->size >= noOfBytes)
            {
                break;
            }
            current = current->next;
            if (current == NULL)
            {
                current = memorylist;
            }
        } while (current != lastNode);
        lastNode = current;
        break;
    default:
        printf("Invalid strategy\n");
        return NULL;
    }

    // If no suitable block is found, return NULL
    if (current == NULL)
    {
        printf("Cannot fit the node\n");
        return NULL;
    }

    // If the block size is exactly equal to the requested size, allocate it
    if ((current->size) == noOfBytes)
    {
        current->free = 0;
        result = (void *)(++current);
        printf("Exact fitting node allocated\n");
        return result;
    }

    // If the block size is greater than the requested size, divide the block
    else if ((current->size) > (noOfBytes + sizeof(struct node)))
    {
        divideBlock(current, noOfBytes);
        result = (void *)(++current);
        printf("Node allocated with a divideBlock\n");
        return result;
    }
    // If no suitable block is found, return NULL
    else
    {
        result = NULL;
        printf("Cannot fit the node\n");
        return result;
    }
}

// Function to merge free memory blocks
void combine()
{
    // Initialize current and previous pointers
    struct node *current = NULL;
    current = memorylist;

    // Traverse the memory list
    while ((current->next) != NULL)
    {
        // If current and the next are both free

        if ((current->free) && (current->next->free))
        {
            // Merge current with next and update the size
            current->next = current->next->next;
            current->size += (current->next->size) + sizeof(struct node);
        }
        // Move to the next node
        current = current->next;
    }
}

// Function to free a memory block
void MyFree(void *ptr)
{
    // If the pointer is NULL, do nothing and return
    if (ptr == NULL)
    {
        return;
    }

    // Check if the pointer is within the heap memory range
    if (((char *)heapStart <= (char *)ptr) && ((char *)ptr <= ((char *)heapStart + 20000)))
    {
        struct node *currentNode = (struct node *)ptr;
        --currentNode;         // Move back to the node header
        currentNode->free = 1; // Mark the node as free
        combine();         // Coalesce free memory spaces
    }
    else
    {
        printf("Invalid pointer\n");
    }
}

// Function to display the free list
void DumpFreeList()
{   
    struct node *current = memorylist;  // Start from the beginning
    printf("Addr\tSize\tStatus\n"); // Print the header
    while (current != NULL) // Traverse the list
    {
        // Print the address, size and status of the node
        printf("%p\t%zu\t%s\n", (void *)current, current->size, (current->free ? "Empty" : "Full"));
        current = current->next;    // Move to the next node
    }
}