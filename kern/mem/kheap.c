#include "kheap.h"
#include <inc/memlayout.h>
#include <inc/dynamic_allocator.h>
#include "memory_manager.h"
#include <inc/queue.h>

struct allocInfo {
	uint32 start;
	uint32 end;
	int used;
};
#define Mega 1024*1024
struct allocInfo allocArr[Mega];
int initialize_kheap_dynamic_allocator(uint32 daStart,
		uint32 initSizeToAllocate, uint32 daLimit) {
	//TODO: [PROJECT'23.MS2 - #01] [1] KERNEL HEAP - initialize_kheap_dynamic_allocator()
	//Initialize the dynamic allocator of kernel heap with the given start address, size & limit
	//All pages in the given range should be allocated
	//Remember: call the initialize_dynamic_allocator(..) to complete the initialization
	//Return:
	//	On success: 0
	//	Otherwise (if no memory OR initial size exceed the given limit): E_NO_MEM

	start = daStart;
	sBrk = daStart + initSizeToAllocate;
	oldBrk = daStart + initSizeToAllocate;
	hard_limit = daLimit;
	struct FrameInfo* frame;
	int ret;
	for (uint32 i = start; i < sBrk; i += PAGE_SIZE) {
		ret = allocate_frame(&frame);
		if (ret == E_NO_MEM) {
			return E_NO_MEM;
		}



		ret = map_frame(ptr_page_directory, frame, i, PERM_WRITEABLE);
		if (ret == E_NO_MEM) {
			return E_NO_MEM;
		}
		frame->va=i;

	}
	initialize_dynamic_allocator(daStart, initSizeToAllocate);
	//Comment the following line(s) before start coding...

	//panic("not implemented yet");
	return 0;
}

void* sbrk(int increment) {
	//TODO: [PROJECT'23.MS2 - #02] [1] KERNEL HEAP - sbrk()
	/* increment > 0: move the segment break of the kernel to increase the size of its heap,
	 * 				you should allocate pages and map them into the kernel virtual address space as necessary,
	 * 				and returns the address of the previous break (i.e. the beginning of newly mapped memory).
	 * increment = 0: just return the current position of the segment break
	 * increment < 0: move the segment break of the kernel to decrease the size of its heap,
	 * 				you should deallocate pages that no longer contain part of the heap as necessary.
	 * 				and returns the address of the new break (i.e. the end of the current heap space).
	 *
	 * NOTES:
	 * 	1) You should only have to allocate or deallocate pages if the segment break crosses a page boundary
	 * 	2) New segment break should be aligned on page-boundary to avoid "No Man's Land" problem
	 * 	3) Allocating additional pages for a kernel dynamic allocator will fail if the free frames are exhausted
	 * 		or the break exceed the limit of the dynamic allocator. If sbrk fails, kernel should panic(...)
	 */

	//TODO: [PROJECT'23.MS2 - #02] [1] KERNEL HEAP - sbrk()
		/* increment > 0: move the segment break of the kernel to increase the size of its heap,
		 * 				you should allocate pages and map them into the kernel virtual address space as necessary,
		 * 				and returns the address of the previous break (i.e. the beginning of newly mapped memory).
		 * increment = 0: just return the current position of the segment break
		 * increment < 0: move the segment break of the kernel to decrease the size of its heap,
		 * 				you should deallocate pages that no longer contain part of the heap as necessary.
		 * 				and returns the address of the new break (i.e. the end of the current heap space).
		 *
		 * NOTES:
		 * 	1) You should only have to allocate or deallocate pages if the segment break crosses a page boundary
		 * 	2) New segment break should be aligned on page-boundary to avoid "No Man's Land" problem
		 * 	3) Allocating additional pages for a kernel dynamic allocator will fail if the free frames are exhausted
		 * 		or the break exceed the limit of the dynamic allocator. If sbrk fails, kernel should panic(...)
		 */
		cprintf("\nhkheap\n");
		if (increment == 0)
			return (void*) sBrk;

		int np;
		if (increment % PAGE_SIZE == 0)
			np = (increment / PAGE_SIZE);
		else
			np = (increment / PAGE_SIZE) + 1;

		if (sBrk + (np * PAGE_SIZE) < hard_limit) {

			struct FrameInfo* frame;
			int ret;
			for (uint32 i = sBrk; i < sBrk + (np * PAGE_SIZE); i += PAGE_SIZE) {
				ret = allocate_frame(&frame);
				if (ret == E_NO_MEM) {
					panic("no mem");
				}
				ret = map_frame(ptr_page_directory, frame, i, PERM_WRITEABLE);
				if (ret == E_NO_MEM) {
					free_frame(frame);
					panic("no mem");
				}
			}
			sBrk += (np * PAGE_SIZE);

			return (void*) (sBrk - (np * PAGE_SIZE));
		} else {
			panic("no mem");
		}

		//MS2: COMMENT THIS LINE BEFORE START CODING====
		return (void*) -1;
		//panic("not implemented yet");
}

void* kmalloc(unsigned int size) {

	int index;
	struct FrameInfo* frame = NULL;
	uint32 * ptr_page_table = NULL;
	int np;
	if (size % PAGE_SIZE == 0)
		np = (size / PAGE_SIZE);
	else
		np = (size / PAGE_SIZE) + 1;

	//TODO: [PROJECT'23.MS2 - #03] [1] KERNEL HEAP - kmalloc()
	//refer to the project presentation and documentation for details
	// use "isKHeapPlacementStrategyFIRSTFIT() ..." functions to check the current strategy
	if (size <= DYN_ALLOC_MAX_BLOCK_SIZE) {
		if (isKHeapPlacementStrategyFIRSTFIT())
			return alloc_block_FF(size);
		if (isKHeapPlacementStrategyBESTFIT())
			return alloc_block_BF(size);
	} else {

		for (int i = 0; i < Mega; i++) {
			if (allocArr[i].used == 0) {
				index = i;
				break;
			}
		}
		int check = 0;
		for (uint32 j = hard_limit + PAGE_SIZE; j < KERNEL_HEAP_MAX; j +=
		PAGE_SIZE) {
			frame = get_frame_info(ptr_page_directory, j, &ptr_page_table);
			if (frame == NULL) {
				if (check == 0)
					allocArr[index].start = j;
				allocArr[index].end = j;
				check++;
			} else
				check = 0;

			if (check == np) {
				allocArr[index].used = 1;
				break;
			}
		}
		if (allocArr[index].used) {
			for (uint32 j = allocArr[index].start; j <= allocArr[index].end;
					j += PAGE_SIZE) {
				allocate_frame(&frame);
				frame->va=j;
				map_frame(ptr_page_directory, frame, j,
				PERM_PRESENT | PERM_WRITEABLE);

			}
			return (void*) allocArr[index].start;
		}
	}

	//change this "return" according to your answer
	//kpanic_into_prompt("kmalloc() is not implemented yet...!!");
	return NULL;
}

void kfree(void* virtual_address)

{

	struct MemBlock;

	if (virtual_address >= (void*) KERNEL_HEAP_START
			&& virtual_address < (void*) hard_limit) {

		free_block(virtual_address);
	} else if (virtual_address
			>= (void*) (hard_limit + PAGE_SIZE)&& virtual_address < (void*)KERNEL_HEAP_MAX) {

		int index, check = 0;
		for (int i = 0; i < Mega; i++) {
			if ((uint32) virtual_address == allocArr[i].start) {
				check = 1;
				index = i;
			}
		}

		if (check) {
			for (uint32 i = allocArr[index].start; i <= allocArr[index].end;
					i += PAGE_SIZE) {
				struct FrameInfo* frame = NULL;
				uint32 *ptr=NULL;
				frame = get_frame_info(ptr_page_directory,i,&ptr);
				frame->va=0;
				unmap_frame(ptr_page_directory, i);
				uint32*ptrPT;
				get_page_table(ptr_page_directory, i, &ptrPT);
				if (ptrPT != NULL)
					ptrPT[PTX(i)] = 0;
			}
		}
	} else {
		cprintf("\nKfreepanci\n");
		panic("wrong adrees");
	}
	//TODO: [PROJECT'23.MS2 - #04] [1] KERNEL HEAP - kfree()
	//refer to the project presentation and documentation for details
	// Write your code here, remove the panic and write your code
	//panic("kfree() is not implemented yet...!!");

}

unsigned int kheap_virtual_address(unsigned int physical_address) {
	//TODO: [PROJECT'23.MS2 - #05] [1] KERNEL HEAP - kheap_virtual_address()
	//refer to the project presentation and documentation for details
	// Write your code here, remove the panic and write your code
	//panic("kheap_virtual_address() is not implemented yet...!!");

	//EFFICIENT IMPLEMENTATION ~O(1) IS REQUIRED ==================

	//change this "return" according to your answer


	struct FrameInfo* frame = to_frame_info(physical_address);

	 uint32 offset = physical_address%PAGE_SIZE;
	 unsigned int frame_off = frame->va + (offset );
	 if (frame_off == (offset >> 20)||frame->va==0) {
	 return 0;
	 }
	 return frame_off;

}

unsigned int kheap_physical_address(unsigned int virtual_address) {
//TODO: [PROJECT'23.MS2 - #06] [1] KERNEL HEAP - kheap_physical_address()
	//refer to the project presentation and documentation for details
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");

	uint32 ptx = PTX(virtual_address);
	uint32 offset = virtual_address & 0x00000FFF;

	uint32* ptr_page_table = NULL;
	get_page_table(ptr_page_directory, virtual_address, &ptr_page_table);
	uint32 off1=(((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset)%PAGE_SIZE;
	if (ptr_page_table != NULL){
		struct FrameInfo* frame = to_frame_info(((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset);
		unsigned int off2 = (((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset) << 20;
		return ((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset;
	}
	return 0;
}

void kfreeall() {
	panic("Not implemented!");

}

void kshrink(uint32 newSize) {
	panic("Not implemented!");
}

void kexpand(uint32 newSize) {
	panic("Not implemented!");
}

//=================================================================================//
//============================== BONUS FUNCTION ===================================//
//=================================================================================//
// krealloc():

//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to kmalloc().
//	A call with new_size = zero is equivalent to kfree().

void *krealloc(void *virtual_address, uint32 new_size) {
	//TODO: [PROJECT'23.MS2 - BONUS#1] [1] KERNEL HEAP - krealloc()
	// Write your code here, remove the panic and write your code
	return NULL;
	panic("krealloc() is not implemented yet...!!");
}
