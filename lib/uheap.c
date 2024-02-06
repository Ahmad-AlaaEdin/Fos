#include <inc/lib.h>

//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

int FirstTimeFlag = 1;
void InitializeUHeap()
{
	if(FirstTimeFlag)
	{
#if UHP_USE_BUDDY
		initialize_buddy();
		cprintf("BUDDY SYSTEM IS INITIALIZED\n");
#endif
		FirstTimeFlag = 0;
	}

}
struct allocated{
	uint32 va;
	uint32 size;
}array[7000];
int c = 0;
//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//=============================================
// [1] CHANGE THE BREAK LIMIT OF THE USER HEAP:
//=============================================
/*2023*/
void* sbrk(int increment)
{
	return (void*) sys_sbrk(increment);
}

//=================================
// [2] ALLOCATE SPACE IN USER HEAP:
//=================================

/*unsigned int numOfPages[(USER_HEAP_MAX - USER_HEAP_START) / PAGE_SIZE] = {0};*/

unsigned int NumOfNeededPages; // Number of pages needed for the requested memory allocation

int NextAllocIndex = 0;


void* malloc(uint32 size)
{

	uint32 print=(USER_HEAP_MAX-sys_get_hard_limit())/PAGE_SIZE;
	cprintf("\n%d\n",print);
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	if (size == 0) return NULL ;
	//==============================================================
	//TODO: [PROJECT'23.MS2 - #09] [2] USER HEAP - malloc() [User Side]
	// Write your code here, remove the panic and write your code


	if(size <= DYN_ALLOC_MAX_BLOCK_SIZE){
cprintf("\n^^^^^^^^^^^^^^^^^^^^^\n");
		return (void*)alloc_block_FF(size);

	}


	int np;
		if (size % PAGE_SIZE == 0)
				np = (size / PAGE_SIZE);
		else
				np = (size / PAGE_SIZE) + 1;
	int check = 0;
	uint32 ptr;
	int flag=0;
	uint32 start=sys_get_hard_limit()+PAGE_SIZE;
	cprintf("\n%d\n",start);
	int ret;

	for(uint32 i=start;i< USER_HEAP_MAX;i+=PAGE_SIZE){

		ret=sys_get_perm(i);
		if (!(ret&PERM_AVAILABLE)||ret==-1) {

		if (check == 0)
		ptr=i;
		check++;
		} else
			check = 0;

		if (check == np) {
			cprintf("\nfonud\n");
		flag=1;
		break;
		}

	}
	if(flag){
		cprintf("\nCounter : %ld\n",c);
		cprintf("\naddress : %lu\n",ptr);
		sys_allocate_user_mem(ptr,size);
		cprintf("\n``11``\n");
		array[c].va=ptr;
		cprintf("\n``22``\n");
		array[c].size=np*PAGE_SIZE;
		cprintf("\n``33``\n");
		c++;
		return (void*)(ptr);
	}

	cprintf("\nnot fonud\n");
	return NULL;

}

//=================================
// [3] FREE SPACE FROM USER HEAP:
//=================================
void free(void* virtual_address)
{
	cprintf("\nva %p\n",virtual_address);
	if(virtual_address<(void*)sys_get_hard_limit()){
		cprintf("\nfree 1\n");
		free_block(virtual_address);
	}else{
		cprintf("\nfree 2\n");
		for(int i=0;i<c;i++){
			if((void*)array[i].va==virtual_address){
				cprintf("\nuint32: %lu\n",array[i].va);
				sys_free_user_mem(array[i].va,array[i].size);

				break;
			}
		}
	}
	//TODO: [PROJECT'23.MS2 - #11] [2] USER HEAP - free() [User Side]
	// Write your code here, remove the panic and write your code
	//panic("free() is not implemented yet...!!");
}


//=================================
// [4] ALLOCATE SHARED VARIABLE:
//=================================
void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	if (size == 0) return NULL ;
	//==============================================================
	panic("smalloc() is not implemented yet...!!");
	return NULL;
}

//========================================
// [5] SHARE ON ALLOCATED SHARED VARIABLE:
//========================================
void* sget(int32 ownerEnvID, char *sharedVarName)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	//==============================================================
	// Write your code here, remove the panic and write your code
	panic("sget() is not implemented yet...!!");
	return NULL;
}


//==================================================================================//
//============================== BONUS FUNCTIONS ===================================//
//==================================================================================//

//=================================
// REALLOC USER SPACE:
//=================================
//	Attempts to resize the allocated space at "virtual_address" to "new_size" bytes,
//	possibly moving it in the heap.
//	If successful, returns the new virtual_address, in which case the old virtual_address must no longer be accessed.
//	On failure, returns a null pointer, and the old virtual_address remains valid.

//	A call with virtual_address = null is equivalent to malloc().
//	A call with new_size = zero is equivalent to free().

//  Hint: you may need to use the sys_move_user_mem(...)
//		which switches to the kernel mode, calls move_user_mem(...)
//		in "kern/mem/chunk_operations.c", then switch back to the user mode here
//	the move_user_mem() function is empty, make sure to implement it.
void *realloc(void *virtual_address, uint32 new_size)
{
	//==============================================================
	//DON'T CHANGE THIS CODE========================================
	InitializeUHeap();
	//==============================================================

	// Write your code here, remove the panic and write your code
	panic("realloc() is not implemented yet...!!");
	return NULL;

}


//=================================
// FREE SHARED VARIABLE:
//=================================
//	This function frees the shared variable at the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from main memory then switch back to the user again.
//
//	use sys_freeSharedObject(...); which switches to the kernel mode,
//	calls freeSharedObject(...) in "shared_memory_manager.c", then switch back to the user mode here
//	the freeSharedObject() function is empty, make sure to implement it.

void sfree(void* virtual_address)
{
	// Write your code here, remove the panic and write your code
	panic("sfree() is not implemented yet...!!");
}


//==================================================================================//
//========================== MODIFICATION FUNCTIONS ================================//
//==================================================================================//

void expand(uint32 newSize)
{
	panic("Not Implemented");

}
void shrink(uint32 newSize)
{
	panic("Not Implemented");

}
void freeHeap(void* virtual_address)
{
	panic("Not Implemented");

}
