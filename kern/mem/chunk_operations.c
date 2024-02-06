/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"
#include <inc/queue.h>
#include <kern/tests/utilities.h>

//extern void inctst();

/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va on the given page_directory
//	If the page table at any destination page in the range is not exist, it should create it
//	If ANY of the destination pages exists, deny the entire process and return -1. Otherwise, cut-paste the number of pages and return 0
//	ALL 12 permission bits of the destination should be TYPICAL to those of the source
//	The given addresses may be not aligned on 4 KB
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	panic("cut_paste_pages() is not implemented yet...!!");

	return 0 ;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va on the given page_directory
//	Ranges DO NOT overlapped.
//	If ANY of the destination pages exists with READ ONLY permission, deny the entire process and return -1.
//	If the page table at any destination page in the range is not exist, it should create it
//	If ANY of the destination pages doesn't exist, create it with the following permissions then copy.
//	Otherwise, just copy!
//		1. WRITABLE permission
//		2. USER/SUPERVISOR permission must be SAME as the one of the source
//	The given range(s) may be not aligned on 4 KB
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	panic("copy_paste_chunk() is not implemented yet...!!");
	return 0;
}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va on the given page_directory
//	Ranges DO NOT overlapped.
//	It should set the permissions of the second range by the given perms
//	If ANY of the destination pages exists, deny the entire process and return -1. Otherwise, share the required range and return 0
//	If the page table at any destination page in the range is not exist, it should create it
//	The given range(s) may be not aligned on 4 KB
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	panic("share_chunk() is not implemented yet...!!");
	return 0;
}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate the given virtual range [<va>, <va> + <size>) in the given address space  <page_directory> with the given permissions <perms>.
//	If ANY of the destination pages exists, deny the entire process and return -1. Otherwise, allocate the required range and return 0
//	If the page table at any destination page in the range is not exist, it should create it
//	Allocation should be aligned on page boundary. However, the given range may be not aligned.
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	panic("allocate_chunk() is not implemented yet...!!");
	return 0;
}

//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	panic("calculate_allocated_space() is not implemented yet...!!");
}


//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
//This function should calculate the required number of pages for allocating and mapping the given range [start va, start va + size) (either for the pages themselves or for the page tables required for mapping)
//	Pages and/or page tables that are already exist in the range SHOULD NOT be counted.
//	The given range(s) may be not aligned on 4 KB
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	panic("calculate_required_frames() is not implemented yet...!!");
	return 0;
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	/*=============================================================================*/
	//TODO: [PROJECT'23.MS2 - #10] [2] USER HEAP - allocate_user_mem() [Kernel Side]
	/*REMOVE THESE LINES BEFORE START CODING */
	 /*size=ROUNDUP(size,PAGE_SIZE);
		      uint32 max= virtual_address+size;
		      for(int i=virtual_address;i<max;i+=PAGE_SIZE)
		     {
		    	 pf_add_empty_env_page(e,i,1);
		     }*/
	cprintf("alloc_user");
	int np;
	if (size % PAGE_SIZE == 0)
			np = (size / PAGE_SIZE);
	else
			np = (size / PAGE_SIZE) + 1;

	uint32 *pt=NULL;
	int ret;
	for(uint32 i =0;i<np;i++){

		ret =get_page_table(e->env_page_directory,virtual_address,&pt);
		if(ret==TABLE_NOT_EXIST)
			create_page_table(e->env_page_directory,virtual_address);
		pt_set_page_permissions(e->env_page_directory,virtual_address,(PERM_AVAILABLE),0);
		uint32 r=pt_get_page_permissions(e->env_page_directory,virtual_address);
		//cprintf("avliale  %lu    status: %lu ",virtual_address,r&PERM_AVAILABLE);
		virtual_address+=PAGE_SIZE;
	}
	cprintf("end_alloc_user");

		return;
	/*=============================================================================*/

	// Write your code here, remove the panic and write your code
	//panic("allocate_user_mem() is not implemented yet...!!");
}
unsigned int converter(struct Env* e,unsigned int virtual_address) {
//TODO: [PROJECT'23.MS2 - #06] [1] KERNEL HEAP - kheap_physical_address()
	//refer to the project presentation and documentation for details
	// Write your code here, remove the panic and write your code
	//panic("kheap_physical_address() is not implemented yet...!!");

	uint32 ptx = PTX(virtual_address);
	uint32 offset = virtual_address & 0x00000FFF;

	uint32* ptr_page_table = NULL;
	get_page_table(e->env_page_directory, virtual_address, &ptr_page_table);
	uint32 off1=(((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset)%PAGE_SIZE;
	if (ptr_page_table != NULL){
		struct FrameInfo* frame = to_frame_info(((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset);
		unsigned int off2 = (((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset) << 20;
		return ((ptr_page_table[ptx] >> 12) * PAGE_SIZE) + offset;
	}
	return 0;
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	/*==========================================================================*/
	//TODO: [PROJECT'23.MS2 - #12] [2] USER HEAP - free_user_mem() [Kernel Side]
	/*REMOVE THESE LINES BEFORE START CODING */
	//inctst();

	uint32 end =virtual_address+size;
	struct FrameInfo* frame ;
	uint32* ptp=NULL;
	struct Env* env=e;
	uint32* pg;
	uint32 *dir =e->env_page_directory;
	for(uint32 i=virtual_address;i<end;i+=PAGE_SIZE ){
		get_page_table(dir,i,&ptp);
		frame = get_frame_info(dir,i,&pg);
		if(isPageReplacmentAlgorithmLRU(PG_REP_LRU_LISTS_APPROX)){
		struct WorkingSetElement*t;
		LIST_FOREACH(t,&(e->ActiveList)){
			if(t->virtual_address==i){
				LIST_REMOVE(&(e->ActiveList),t);
				kfree((void*) t->virtual_address);
				break;
			}
		}
		}/*else if(isPageReplacmentAlgorithmFIFO()){
			if(frame != NULL){
				if(frame->element != NULL){
					if(env->isFull){

					}
				}
			}
		}*/
		//kfree((void*)frame->element);
		if(frame->element!= NULL){
					struct WorkingSetElement* temp = frame->element;
					LIST_REMOVE(&(e->page_WS_list),frame->element);
					cprintf("\npresent2 %p\n",temp);
					cprintf("\nframe ref1 %d\n",frame->references);


					cprintf("\nframe ref2 %d\n",frame->references);
					kfree(temp);
					frame->element=NULL;

				}
		pt_set_page_permissions(dir,i,0,PERM_WRITEABLE|PERM_PRESENT|PERM_AVAILABLE);
		pf_remove_env_page(env,i);
		unmap_frame(dir,i);

	}
	return;
	/*==========================================================================*/

	// Write your code here, remove the panic and write your code
	//panic("free_user_mem() is not implemented yet...!!");

	//TODO: [PROJECT'23.MS2 - BONUS#2] [2] USER HEAP - free_user_mem() IN O(1): removing page from WS List instead of searching the entire list

}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

