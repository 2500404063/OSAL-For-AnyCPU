/*
 * osal_mem.c
 *
 *  Created on: 2022.9.29
 *      Author: Felix
 */

#include "osal_mem.h"

static osal_uint8 g_heap[HEAP_SIZE];

void osal_mem_init(void)
{
	//Make up the header for the entire heap.
	mem_header* p = (mem_header*)g_heap;
	*p = HEAP_SIZE - OSAL_MEM_HEADER_SIZE;
}

void* osal_mem_alloc(mem_size size)
{
	if (size <= 0 || size > HEAP_SIZE - OSAL_MEM_HEADER_SIZE)
		return 0x0;

	//Align
	if (size % OSAL_MEM_ALIGN > 0) {
		size = OSAL_MEM_ALIGN * ((mem_size)(size / OSAL_MEM_ALIGN) + 1);
	}

	mem_header* block = (mem_header*)g_heap;
	//If block jumps out of the extend of heap, means no more space to allocate.
	while (block < (mem_header*)(g_heap + HEAP_SIZE))
	{
		//Find one block which is not allocated, otherwise jump to the next block.
		if (*block & OSAL_MEM_MASK_STATE)
		{
			block = (mem_header*)((osal_uint8*)block + (*block & OSAL_MEM_MASK_SIZE) + OSAL_MEM_HEADER_SIZE);
		}
		else
		{
			//Compute current block's size.
			mem_size cur_block_size = *block & OSAL_MEM_MASK_SIZE;
			//Whether current block has enough space to allocate.
			if (cur_block_size >= size)
			{
				//If has enough space, the block will be divided if the left space is equal or bigger than header's size.
				mem_size left = cur_block_size - size;
				if (left > OSAL_MEM_HEADER_SIZE)
				{
					mem_header* temp = (mem_header*)((osal_uint8*)block + size + OSAL_MEM_HEADER_SIZE);
					left = left - OSAL_MEM_HEADER_SIZE;
					*temp = left;
					*block = OSAL_MEM_MASK_STATE | size;
				}
				else
				{
					*block |= OSAL_MEM_MASK_STATE;
				}
				return (osal_uint8*)block + OSAL_MEM_HEADER_SIZE;
			}
			else
			{
				//Jump to next block
				block = (mem_header*)((osal_uint8*)block + (*block & OSAL_MEM_MASK_SIZE) + OSAL_MEM_HEADER_SIZE);
			}
		}
	}
	return 0x0;
}

void osal_mem_free(void* addr)
{
	mem_header* block = (mem_header*)((osal_uint8*)addr - OSAL_MEM_HEADER_SIZE);
	*(block) &= OSAL_MEM_MASK_SIZE;
}

void* osal_mem_realloc(void* addr, mem_size size)
{
	//Align
	if (size % OSAL_MEM_ALIGN > 0) {
		size = OSAL_MEM_ALIGN * ((mem_size)(size / OSAL_MEM_ALIGN) + 1);
	}

	//Because reallocating should keep linear sapce, it could only be two blocks: current block, and next_block.
	mem_header* block = (mem_header*)((osal_uint8*)addr - OSAL_MEM_HEADER_SIZE);
	mem_size block_size = *block & OSAL_MEM_MASK_SIZE;

	//3 Types of reallocating:
	//1. larger
	//2. smaller
	//3. unchanged
	if (size > block_size)
	{
		mem_header* next_block = (mem_header*)((osal_uint8*)block + block_size + OSAL_MEM_HEADER_SIZE);
		mem_size next_block_size = *next_block & OSAL_MEM_MASK_SIZE;

		if ((*next_block & OSAL_MEM_MASK_STATE) || next_block_size < size - block_size)
		{
			//If the next block has already allocated, or the next space cannot content the new size, should newly allocate.
			//Prior to newly allocate, the old space should be freed.
			osal_mem_free(addr);
			return osal_mem_alloc(size);
		}
		else
		{
			//Whether the left space is superfluous.
			mem_size left = next_block_size - (size - block_size);
			if (left > OSAL_MEM_HEADER_SIZE)
			{
				mem_header* tailBlcok = (mem_header*)((osal_uint8*)block + size + OSAL_MEM_HEADER_SIZE);
				*tailBlcok = left - OSAL_MEM_HEADER_SIZE;
				*block = OSAL_MEM_MASK_STATE | size;
			}
			else
			{
				*block = OSAL_MEM_MASK_STATE | (size + left);
			}
			return (osal_uint8*)block + OSAL_MEM_HEADER_SIZE;
		}
	}
	else if (size < block_size)
	{
		if (block_size - size > OSAL_MEM_HEADER_SIZE)
		{
			mem_header* next_block = (mem_header*)((osal_uint8*)block + size + OSAL_MEM_HEADER_SIZE);
			*next_block = block_size - size - OSAL_MEM_HEADER_SIZE;
			return addr;
		}
		else
		{
			return addr;
		}
	}
	else
	{
		return addr;
	}
}

void osal_mem_gc(void) {
	mem_header* cur_block = (mem_header*)g_heap;
	mem_size cur_block_size = *cur_block & OSAL_MEM_MASK_SIZE;
	mem_header* next_block = (mem_header*)((osal_uint8*)cur_block + OSAL_MEM_HEADER_SIZE + cur_block_size);
	mem_size next_block_size = *next_block & OSAL_MEM_MASK_SIZE;
	while (next_block < (mem_header*)(g_heap + HEAP_SIZE))
	{
		if ((*cur_block & OSAL_MEM_MASK_STATE) == 0 && (*next_block & OSAL_MEM_MASK_STATE) == 0) {
			*cur_block = cur_block_size + next_block_size + OSAL_MEM_HEADER_SIZE;
			cur_block_size = cur_block_size + next_block_size + OSAL_MEM_HEADER_SIZE;
		}
		else {
			cur_block = next_block;
			cur_block_size = next_block_size;
		}
		next_block = (mem_header*)((osal_uint8*)cur_block + OSAL_MEM_HEADER_SIZE + cur_block_size);
		next_block_size = *next_block & OSAL_MEM_MASK_SIZE;
	}
}