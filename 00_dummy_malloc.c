#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void *dummy_malloc(size_t size) {
    void* prev_break_address = sbrk(0);
    if (sbrk(size) == (void*) -1) {
        return NULL;
    }

    return prev_break_address;
}

void test_small_allocation() {
    size_t size = 16;
    char *memory = (char *)dummy_malloc(size);
    if (memory) {
        printf("[Small Allocation] Allocated %zu bytes at %p\n", size, memory);
        strcpy(memory, "Hello, World!"); // Write to the memory
        printf("[Small Allocation] Content: %s\n", memory); // Print content
    } else {
        printf("[Small Allocation] Memory allocation failed\n");
    }
}

void test_large_allocation() {
    size_t size = 1024; // 1 KB
    int *memory = (int *)dummy_malloc(size);
    if (memory) {
        printf("[Large Allocation] Allocated %zu bytes at %p\n", size, memory);
        for (int i = 0; i < size / sizeof(int); i++) {
            memory[i] = i; // Fill with data
        }
        printf("[Large Allocation] First: %d, Last: %d\n", memory[0], memory[(size / sizeof(int)) - 1]);
    } else {
        printf("[Large Allocation] Memory allocation failed\n");
    }
}

void test_excessive_allocation() {
    size_t size = (size_t)-1; // Unrealistically large size
    void *memory = dummy_malloc(size);
    if (memory) {
        printf("[Excessive Allocation] Unexpectedly allocated %zu bytes at %p\n", size, memory);
    } else {
        printf("[Excessive Allocation] Failed as expected for size %zu\n", size);
    }
}

void test_multiple_small_allocations() {
    printf("[Multiple Allocations] Allocating multiple 32-byte blocks...\n");
    for (int i = 0; i < 5; i++) {
        void *memory = dummy_malloc(32); // 32 bytes
        if (memory) {
            printf("[Multiple Allocations] Allocated 32 bytes at %p\n", memory);
        } else {
            printf("[Multiple Allocations] Memory allocation failed at iteration %d\n", i);
            break;
        }
    }
}

int main() {
    printf("Testing memory allocation using dummay_malloc...\n");

    test_small_allocation();
    test_large_allocation();
    test_excessive_allocation();
    test_multiple_small_allocations();

    return 0;
}
