#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#define align4(x) (((((x) - 1) >> 2) <<2) + 4)

typedef struct t_block {
    int size;
    int free;
    struct t_block* next;
    char data[1];
} *t_block;

#define BLOCK_META_SIZE sizeof(struct t_block) - 1

t_block base = NULL;

t_block find_sufficient_block(size_t required_space, t_block* last_block) {
    t_block block = base;
    while (block && !(block->free && block->size >= required_space)) {
        *last_block = block;
        block = block->next;
    }

    return block;
}

t_block extend_heap(t_block last_block, size_t size) {
    t_block block = sbrk(0);
    if(sbrk(BLOCK_META_SIZE + size) == (void*)-1) {
        // sbrk fails!
        return NULL;
    }

    // allocate block right away
    block->free = 0;
    block->size = size;
    block->next = NULL;
    if (last_block) {
        last_block->next = block;
    }

    return block;
}

void split_block(t_block block, size_t chunck_size) {
    t_block new_block;
    new_block = block->data + chunck_size;
    new_block->free = 1;
    new_block->size = block->size - chunck_size - BLOCK_META_SIZE;

    new_block->next = block->next;
    block->next = new_block;

    block->size = chunck_size;
}


void *ff_malloc(size_t size) {
    size = align4(size);

    if (base == NULL) {
        t_block block = extend_heap(NULL, size);
        if (!block) {
            return NULL;
        }

        base = block;
        return block->data;
    }
    
    // base != null
    t_block last_block;
    t_block sufficient_block = find_sufficient_block(size, &last_block);
    // found a sufficent free block to allocate
    if (sufficient_block != NULL) {
        if (sufficient_block->size >= size + BLOCK_META_SIZE + 4) {
            split_block(sufficient_block, size);
        }
        sufficient_block->free = 0;
        return sufficient_block;
    } else {
        // Else we extend heap
        t_block block = extend_heap(last_block, size);
        if (!block) {
            return NULL;
        }

        return block->data;
    }

}

void test_small_allocation() {
    size_t size = 16;
    char *memory = (char *)ff_malloc(size);
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
    int *memory = (int *)ff_malloc(size);
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
    void *memory = ff_malloc(size);
    if (memory) {
        printf("[Excessive Allocation] Unexpectedly allocated %zu bytes at %p\n", size, memory);
    } else {
        printf("[Excessive Allocation] Failed as expected for size %zu\n", size);
    }
}

void test_multiple_small_allocations() {
    printf("[Multiple Allocations] Allocating multiple 32-byte blocks...\n");
    for (int i = 0; i < 5; i++) {
        void *memory = ff_malloc(32); // 32 bytes
        if (memory) {
            printf("[Multiple Allocations] Allocated 32 bytes at %p\n", memory);
        } else {
            printf("[Multiple Allocations] Memory allocation failed at iteration %d\n", i);
            break;
        }
    }
}

int main() {
    printf("Testing memory allocation using ff_malloc...\n");

    test_small_allocation();
    test_large_allocation();
    test_excessive_allocation();
    test_multiple_small_allocations();

    return 0;
}

