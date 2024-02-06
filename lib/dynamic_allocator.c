/*
 * dynamic_allocator.c
 *
 *  Created on: Sep 21, 2023
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include <inc/queue.h>
#include "../inc/dynamic_allocator.h"
//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//=====================================================
// 1) GET BLOCK SIZE (including size of its meta data):
//=====================================================
uint32 get_block_size(void* va)
{
	struct BlockMetaData *curBlkMetaData = ((struct BlockMetaData *)va - 1) ;
	return curBlkMetaData->size ;
}
struct MemBlock_LIST memList;
//===========================
// 2) GET BLOCK STATUS:
//===========================
int8 is_free_block(void* va)
{
	struct BlockMetaData *curBlkMetaData = ((struct BlockMetaData *)va - 1) ;
	return curBlkMetaData->is_free ;
}

//===========================================
// 3) ALLOCATE BLOCK BASED ON GIVEN STRATEGY:
//===========================================
void *alloc_block(uint32 size, int ALLOC_STRATEGY)
{
	void *va = NULL;
	switch (ALLOC_STRATEGY)
	{
	case DA_FF:
		va = alloc_block_FF(size);
		break;
	case DA_NF:
		va = alloc_block_NF(size);
		break;
	case DA_BF:
		va = alloc_block_BF(size);
		break;
	case DA_WF:
		va = alloc_block_WF(size);
		break;
	default:
		cprintf("Invalid allocation strategy\n");
		break;
	}
	return va;
}

//===========================
// 4) PRINT BLOCKS LIST:
//===========================

void print_blocks_list(struct MemBlock_LIST list)
{
	cprintf("=========================================\n");
	struct BlockMetaData* blk ;
	cprintf("\nDynAlloc Blocks List:\n");
	LIST_FOREACH(blk, &list)
	{
		cprintf("(size: %d, isFree: %d)\n", blk->size, blk->is_free) ;
	}
	cprintf("=========================================\n");

}
//
////********************************************************************************//
////********************************************************************************//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
bool is_initialized = 0;
//==================================
// [1] INITIALIZE DYNAMIC ALLOCATOR:
//==================================
uint32 start ;
void initialize_dynamic_allocator(uint32 daStart, uint32 initSizeOfAllocatedSpace)
{
	//=========================================
	//DON'T CHANGE THESE LINES=================
	if (initSizeOfAllocatedSpace == 0)
		return ;
	is_initialized = 1;
	//=========================================
	//=========================================

	//TODO: [PROJECT'23.MS1 - #5] [3] DYNAMIC ALLOCATOR - initialize_dynamic_allocator()
	start =daStart;
	struct BlockMetaData* firstBlock=(struct BlockMetaData *) daStart;
	firstBlock->is_free=1;
	firstBlock->size=initSizeOfAllocatedSpace;

	LIST_INIT(&memList);
	LIST_INSERT_HEAD(&memList, firstBlock);
	//panic("initialize_dynamic_allocator is not implemented yet");
}


//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================

void *alloc_block_FF(uint32 size)
{

	cprintf("\nin alloc ff\n");
    char tr = 0;

    if (size == 0) {
        return NULL;
    }
    cprintf("\nbefore\n");
    if (!is_initialized)
    {

    uint32 required_size = size + sizeOfMetaData();
    uint32 da_start = (uint32)sbrk(required_size);
    //get new break since it's page aligned! thus, the size can be more than the required one
    uint32 da_break = (uint32)sbrk(0);
    initialize_dynamic_allocator(da_start, da_break - da_start);
    }
    cprintf("\nafter\n");

    struct BlockMetaData* current_block = (struct BlockMetaData*)start;
    cprintf("\nnext\n");
    while (current_block != NULL) {
    	//cprintf("\ncur %lu\nsize: %d\nsbrk%p",current_block,current_block->size,(void*)sbrk(0));
        if (current_block->is_free == 1 && current_block->size >= size + sizeOfMetaData()) {
        	//cprintf("\n#1\n");
            if (current_block->size == size+ sizeOfMetaData()) {
            	//cprintf("\n#2\n");
                current_block->is_free = 0;
               // cprintf("\n#3\n");
                tr = 1;

                return current_block+1 ;
            }
            else if (current_block->size > size+ sizeOfMetaData()) {
                tr = 1;
                //cprintf("\n#4\n");
                int new_size = current_block->size - size- sizeOfMetaData();
                //cprintf("\n#5\n");
                if (new_size <= sizeOfMetaData()) {
                	//cprintf("\n#6\n");
                    current_block->is_free = 0;

                    return current_block+1 ;
                }
                else {
                	cprintf("\ncurrent:%lu\n",current_block);
                	cprintf("\nsize:%lu\n",current_block->size);
                	cprintf("\nnew:%lu\n",current_block + current_block->size);
                	cprintf("\nsie:%lu\n",size);
                    current_block->size = size+ sizeOfMetaData() ;
                    current_block->is_free = 0;

                    cprintf("\nnew address:%lu\n",((char*)current_block + current_block->size));
                    cprintf("\nsize of :%d\n",sizeOfMetaData());
                    struct BlockMetaData* newBlock = (struct BlockMetaData*)((char*)current_block + current_block->size);
                    cprintf("\n#7\n");
                    newBlock->is_free = 1;
                    cprintf("\n#8\n");
                    newBlock->size = new_size ;
                    cprintf("\n#9\n");
                    cprintf("sbrk %p",(void*)sbrk(0));
                    cprintf("\nLIST_NEXT(current_block) %lu\n",LIST_NEXT(current_block));
                    newBlock->prev_next_info.le_next=0;
                    cprintf("\nLIST_NEXT(newBlock) %lu\n",LIST_NEXT(newBlock));
                    LIST_NEXT(newBlock) =LIST_NEXT(current_block);
                    cprintf("\n#10\n");
                    LIST_NEXT(current_block) = newBlock;
                    cprintf("\n#11\n");
                    LIST_PREV(newBlock) = current_block;
                    cprintf("\n#12\n");
                    if (LIST_NEXT(newBlock) != NULL) {
                    	cprintf("\n#13\n");
                    	LIST_PREV(LIST_NEXT(newBlock)) = newBlock;
                    }
                    cprintf("\n#8\n");

                    	return current_block+1 ;
               }
            }
        }

        current_block = LIST_NEXT(current_block);
    }
void* address =NULL ;
struct BlockMetaData* tail = (struct BlockMetaData*)start;
cprintf("\n#5\n");
    	    while (tail->prev_next_info.le_next != NULL) {
    	    	tail = LIST_NEXT(tail);
    	    }
    	    cprintf("\n#6\n");
    if (tr == 0) {
    	cprintf("\n#2\n");
    	address = sbrk(size + sizeOfMetaData());

        int np;
        if ((size + sizeOfMetaData()) % PAGE_SIZE == 0)
        		{
        			np = (size + sizeOfMetaData() )/ PAGE_SIZE;
        		}
        		else	//add 1 to the result in case the actual result/fraction came out as decimal
        		{
        			np = ((size + sizeOfMetaData() )/ PAGE_SIZE) + 1;
        		}
        if (address == (void*)-1) {

            return NULL;
        }
        else {
        	if(tail->is_free==1){
        		tail->size+=np*PAGE_SIZE;
        		return alloc_block_FF(size);
        	}
        	cprintf("\n#3\n");




            struct BlockMetaData* newB = (struct BlockMetaData*)(char*)address;

            newB->is_free=1;

            newB->size=np*PAGE_SIZE;

            LIST_NEXT(newB)=LIST_NEXT(tail);

            LIST_PREV(newB)=tail;

            LIST_NEXT(tail)=newB;

            if(LIST_NEXT(newB)!=NULL){

            	LIST_PREV(LIST_NEXT(newB))=newB;
            }

        	return alloc_block_FF(size);

        }
    }

    return address;
    /*cprintf("\n#1\n");
    struct BlockMetaData *currentBlock = (struct BlockMetaData*)start;
        struct BlockMetaData *prevBlock = NULL;
        cprintf("\n#2\n");
        while (currentBlock != NULL)
        {
            if (currentBlock->is_free && currentBlock->size >= size + sizeOfMetaData())
            {
                // Found a sufficiently large free block
                uint32 remainingSize = currentBlock->size - size - sizeOfMetaData();

                if (remainingSize >= 0)
                {
                    // Split the block into two: one for the newly allocated block and one for the residual free block
                	currentBlock->size = size + sizeOfMetaData();
                    struct BlockMetaData *newBlock = (struct BlockMetaData *)(currentBlock + size + sizeOfMetaData());
                    cprintf("\n#9\n");
                    cprintf("sbrk %p",sbrk(0));
                    cprintf("newB %lu",(currentBlock + size + sizeOfMetaData()));
                    newBlock->size = remainingSize;
                    cprintf("\n#10\n");
                    newBlock->is_free = 1;
                    cprintf("\n#11\n");

                    cprintf("\n#12\n");
                    currentBlock->is_free = 0;

                    // Update the linked list

                    LIST_NEXT(newBlock)=LIST_NEXT(currentBlock);

                    LIST_NEXT(currentBlock)=newBlock;
                    if(LIST_NEXT(newBlock)!=NULL){
                    	LIST_PREV(LIST_NEXT(newBlock))=newBlock;
                    }


                }
                else
                {
                    // Use the entire block for the newly allocated block
                    currentBlock->is_free = 0;
                }

                // Return a pointer to the beginning of the allocated space
                return (void *)((uint32 *)currentBlock + sizeOfMetaData());
            }

            prevBlock = currentBlock;
            currentBlock = LIST_NEXT(currentBlock);
        }
        cprintf("\n#3\n");
        // No sufficiently large free block found, use sbrk to create more space on the heap
        uint32 totalSize = size + sizeOfMetaData();
        void* ret = sbrk(totalSize);
        int np;
        		if (totalSize % PAGE_SIZE == 0)
        			np = (totalSize / PAGE_SIZE);
        		else
        			np = (totalSize / PAGE_SIZE) + 1;

        if (ret == (void*)-1) {

        	return NULL;
        	 }


         	if(prevBlock->is_free==1){
         		prevBlock->size+=totalSize;
        	return alloc_block_FF(size);
        }
         	cprintf("\n#4\n");
        struct BlockMetaData *newBlock = ret;
        cprintf("\n#5\n");
        newBlock->size = totalSize;
        newBlock->is_free = 0;
        LIST_NEXT(prevBlock)=newBlock;
        LIST_PREV(newBlock)=prevBlock;
        // Update the linked list
       // LIST_INSERT_TAIL(&memList,newBlock);

        // Return a pointer to the beginning of the allocated space
        return (void *)((uint8 *)newBlock + sizeOfMetaData());*/
}
//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
void *alloc_block_BF(uint32 size)
{
	//TODO: [PROJECT'23.MS1 - BONUS] [3] DYNAMIC ALLOCATOR - alloc_block_BF()

	char tr = 0;

	    if (size == 0) {
	        return NULL;
	    }

	    struct BlockMetaData* curBlock = NULL;
	    struct BlockMetaData* iter ;
	    LIST_FOREACH(iter, &(memList)) {

	        if (iter->is_free == 1 && iter->size >= size + sizeOfMetaData()) {
	            if (curBlock== NULL || iter->size < curBlock->size) {
	            	tr=1;
	            	curBlock = iter;
	            }
	        }
	    }

	    if (tr==1) {
	        if (curBlock->size == size) {
	        	curBlock->is_free = 0;
	        	curBlock->size = size;

	            return curBlock + 1;
	        } else if (curBlock->size > size) {

	            int new_size = curBlock->size - size;

	            if (new_size <= sizeOfMetaData()) {
	            	curBlock->is_free = 0;
	                return curBlock + 1;
	            } else {
	            	curBlock->size = size + sizeOfMetaData();
	            	curBlock->is_free = 0;

	                struct BlockMetaData* newBlock = (struct BlockMetaData*)((char*)curBlock + curBlock->size);
	                newBlock->is_free = 1;
	                newBlock->size = new_size - sizeOfMetaData();
	                if(curBlock->prev_next_info.le_next){
	                	LIST_INSERT_TAIL(&memList,newBlock);

	                }else{
	                	LIST_INSERT_AFTER(&(memList), curBlock, newBlock);
	                }


	                return curBlock + 1;
	           }
	        }
	    }

	    void* address;
	    if (tr == 0) {

	        void* new_block = sbrk(size + sizeOfMetaData());

	        if (new_block == (void*)-1) {
	            return NULL;
	        } else {
	        	struct BlockMetaData*	element = LIST_LAST(&memList);
	             address = alloc_block_BF(size);
	        }
	    }

	    return address;
}

//=========================================
// [6] ALLOCATE BLOCK BY WORST FIT:
//=========================================
void *alloc_block_WF(uint32 size)
{
	panic("alloc_block_WF is not implemented yet");
	return NULL;
}

//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
void *alloc_block_NF(uint32 size)
{
	panic("alloc_block_NF is not implemented yet");
	return NULL;
}

//===================================================
// [8] FREE BLOCK WITH COALESCING:
//===================================================
void free_block(void *va)
{

	//TODO: [PROJECT'23.MS1 - #7] [3] DYNAMIC ALLOCATOR - free_block()
	if(va == NULL){
		return;
	}else{
		cprintf("\n#1\n");
		struct BlockMetaData* blockToFree=(struct BlockMetaData *) (va-sizeOfMetaData());
		cprintf("\n#2\n");
		struct BlockMetaData* cur=blockToFree;
		cprintf("\n#3\n");
		blockToFree->is_free=1;
		cprintf("\n#4\n");
		struct BlockMetaData* prev =LIST_PREV(blockToFree);
		cprintf("\n#5\n");
		struct BlockMetaData* next =LIST_NEXT(blockToFree);
		cprintf("\n#6\n");
		if(prev != NULL &&prev->is_free==1 ){
			cprintf("\n#7\n");
			prev->size+=blockToFree->size;
			LIST_NEXT(prev)=next;
			//LIST_PREV(prev)=prev;
			cur=prev;
			if(next != NULL){
				LIST_PREV(next)=prev;
			}
			blockToFree->is_free=0;
			blockToFree->size=0;
			LIST_NEXT(blockToFree)=NULL;
			LIST_PREV(blockToFree)=NULL;
		}
		if (next != NULL && next->is_free) {
			cprintf("\n#8\n");
				cur->size += next->size;
				LIST_NEXT(cur) = LIST_NEXT(next);
			    if (LIST_NEXT(next) != NULL) {
			    	LIST_PREV(LIST_NEXT(next))= cur;
			}

			next->size = 0;
			next->is_free = 0;
			LIST_NEXT(next) = NULL;
			LIST_PREV(next) = NULL;
		    }

	}

	//panic("free_block is not implemented yet");
}

//=========================================
// [4] REALLOCATE BLOCK BY FIRST FIT:
//=========================================
void *realloc_block_FF(void* va, uint32 new_size)
{
	//TODO: [PROJECT'23.MS1 - #8] [3] DYNAMIC ALLOCATOR - realloc_block_FF()
	//panic("realloc_block_FF is not implemented yet");
	struct BlockMetaData* current_block=(struct BlockMetaData *) (va-sizeOfMetaData());
	struct BlockMetaData* next =LIST_NEXT(current_block);
	if(new_size == 0 && va != NULL){
		free_block(va);
		return NULL;
	}
	else if(va == NULL && new_size >0){
		return alloc_block_FF(new_size);
	}
	else if(va == NULL && new_size == 0){
		return NULL;

	}
	else{
		if(next->is_free && next->size>= new_size){
			next->is_free = 0;
			next->size = 0;
			current_block->size = (new_size + sizeOfMetaData());
			return (current_block + 1);
		}
			//clk

		else if(current_block->size > new_size && next->size == 0){
			//Evaluate 70%
			current_block->is_free=0;
			current_block->size =(new_size + sizeOfMetaData());
			return current_block + 1;
			 }
		else if(current_block->size > new_size && next->size != 0){
			/*struct BlockMetaData* block_to_free=(struct BlockMetaData *) (va - new_size - sizeOfMetaData());
			block_to_free->size = new_block->size - new_size - sizeOfMetaData();
			block_to_free->is_free = 1;
			LIST_NEXT(new_block) = block_to_free;
			LIST_NEXT(block_to_free) = next - sizeOfMetaData();
			LIST_PREV(next) = block_to_free;
			LIST_PREV(block_to_free) = new_block;
			next->size = new_block->size - new_size - sizeOfMetaData();*/
			//when size decresed the next pointer must be moved

			next->is_free = 1;
			next->size=current_block->size- new_size - sizeOfMetaData();
			current_block->size = new_size + sizeOfMetaData();
			next=(void*)(current_block + 1 + new_size);

			return current_block + 1;

		}
	}

	cprintf("end of the function without return \n");
	return NULL;
}
