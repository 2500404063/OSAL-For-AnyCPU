/*
 * osal_mem.h
 *
 *  Created on: 2022.9.29
 *      Author: Felix
 */

#ifndef CODE_OSAL_MEM_H_
#define CODE_OSAL_MEM_H_

#include "osal_hal.h"

//Header Config
#define HEAP_SIZE				8192                 /*Heap Size, bytes*/
#define OSAL_MEM_ALIGN			4
#define OSAL_MEM_MASK_STATE		0x80000000
#define OSAL_MEM_MASK_SIZE		0x7FFFFFFF
typedef osal_uint32				mem_header;

//Definitions
#define OSAL_MEM_HEADER_SIZE    sizeof(mem_header)
typedef mem_header				mem_size;

void osal_mem_init(void);
void* osal_mem_alloc(mem_size size);
void* osal_mem_realloc(void* addr, mem_size size);
void osal_mem_free(void* addr);
void osal_mem_gc(void);

#endif /* CODE_OSAL_MEM_H_ */
