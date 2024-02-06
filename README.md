# FOS

FOS is an educational operating system developed for the Ain Shams University Operating Systems Course CSW355. It is a forked and refactored version of the MIT Operating Systems Lab 6.828.

## Features

FOS includes several features and functionalities that have been added or modified for educational purposes. One of the significant contributions made to FOS is the implementation of the following components:

### 1. Kernel Heap

The kernel heap in FOS provides memory allocation and deallocation functionalities using the FIRST FIT algorithm. The following functions are available for managing memory in the kernel heap:

- `kmalloc`: Allocates the required space in the kernel heap.
- `kfree`: Frees the memory corresponding to a given address.
- `kheap_virtual_address`: Returns the virtual address corresponding to a given physical address, including the offset.
- `kheap_physical_address`: Returns the physical address corresponding to a given virtual address, including the offset.

### 2. User Heap

FOS also includes a user heap, which is responsible for memory allocation and deallocation for user-level programs. The following functionalities are implemented for the user heap:

- `malloc()`: Allocates the required space in the user heap using the FIRST FIT algorithm.
- `free()`: Frees the memory corresponding to a given address in the user heap.
- `Allocate_user_mem`: Marks the given range to indicate that it is reserved for the page allocator of the current environment (kernel-side function).
- `free_user_mem`: Unmarks the given range to indicate that it is not reserved for the page allocator of the current environment (kernel-side function).
