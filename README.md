# Malloc Implementation 

Exploring dynamic memory allocation on Linux by attempting to rewrite the `malloc` function in C.

## Overview of `malloc`
In a typical Linux system, malloc interacts with the kernel to manage memory using system calls like `sbrk` or `mmap`. The heap is managed through these system calls to allocate large blocks of memory.

## System Calls Used by `malloc`
- **sbrk**: Adjusts the program's data segment, effectively growing or shrinking the heap.
- **mmap**: Maps files or devices into memory but can also allocate large chunks of anonymous memory.

