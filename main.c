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

void main() {
    int* x = dummy_malloc(4);
    *x = 99;

    printf("Hello C!\n");
    printf("Heap address: %p\n", (void*) malloc(1));
    printf("X: %d\n", *x);
}
