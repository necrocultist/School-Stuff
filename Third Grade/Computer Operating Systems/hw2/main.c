#include <stdio.h>
#include "mymalloc.h"

int main() {
    int strategy;
    printf("Please enter the strategy type: 0 for BestFit, 1 for WorstFit, 2 for FirstFit, 3 for NextFit\n");
    scanf("%d", &strategy);

    // Declare processes with memory requirements
    size_t processSizes[] = {100 * sizeof(int), 250 * sizeof(char), 1000 * sizeof(int), 700};
    const char *processNames[] = {"P1", "P2", "P3", "P4"};
    void *processPointers[4] = {NULL};

    printf("Strategy Type: %d\n", strategy);

    // Allocate memory for each process using specified strats
    for (int i = 0; i < 4; ++i) {
        processPointers[i] = MyMalloc(processSizes[i], strategy);
        if (processPointers[i] != NULL) {
            printf("Allocation for %s succeeded. Address: %p\n", processNames[i], processPointers[i]);
        } else {
            printf("Allocation for %s failed.\n", processNames[i]);
        }
    }

    // Print the output of the DumpFreeList function
    printf("DumpFreeList output:\n");
    DumpFreeList();

    // Free memory for P1 and P3
    MyFree(processPointers[0]);
    MyFree(processPointers[2]);

    // Allocate memory for P5
    void *processP5 = MyMalloc(500 * sizeof(int), strategy);
    if (processP5 != NULL) {
        printf("Allocation for P5 succeeded. Address: %p\n", processP5);
    } else {
        printf("Allocation for P5 failed.\n");
    }

    // Print the output of the DumpFreeList function after deallocations and allocations
    printf("DumpFreeList output after deallocations and allocations:\n");
    DumpFreeList();

    return 0;
}
