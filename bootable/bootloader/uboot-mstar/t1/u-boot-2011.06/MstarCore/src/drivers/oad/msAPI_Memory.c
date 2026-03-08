/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of
* the GNU General Public License as published by the Free Software Foundation;
* either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program;
* if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,
* MA 02111-1307, USA.
*/
////////////////////////////////////////////////////////////////////////////////
//
/// @file msAPI_Memory.h
/// This file includes MStar application interface for memory access.
/// @brief API for memory buffer pool management
/// @author MStar Semiconductor, Inc.
//
////////////////////////////////////////////////////////////////////////////////

#define MSAPI_MEMORY_C

/******************************************************************************/
/*                           Header Files                                     */
/******************************************************************************/

#include <common.h>
#include <command.h>
#include <config.h>
#include <MsTypes.h>
#include <linux/string.h>
#include "msAPI_Memory.h"
#include <MsIRQ.h>
#include <MsOS.h>

#ifndef MSOS_TYPE_LINUX

/********************************************************************************/
/*                               Functions                                      */
/********************************************************************************/

/******************************************************************************/
/// API for Init memory buffer pool at system boot-up
/******************************************************************************/
void msAPI_Memory_Init(void)
{
    return;
}

/******************************************************************************/
/// API for Allocate memory
/// @param u16Number_of_bytes \b IN: number of bytes to be allocated
/// @param enBufID \b IN: buffer ID
/// @return void *:
///- NULL: FAIL
///- XDATA_DRAM_START_ADDR ~ (XDATA_DRAM_START_ADDR+MEMALLOC_POOL_SIZE): SUCCESS
/******************************************************************************/
void * msAPI_Memory_Allocate(MS_U16 u16Number_of_bytes,EN_BUFFER_ID enBufID)
{
    return (void*)MsOS_AllocateMemory((MS_U32)u16Number_of_bytes, (MS_U32)enBufID);
}

/******************************************************************************/
/// API for Free allocated memory
/// @param *pFree \b IN: pointer to be released
/// @param enBufID \b IN: buffer ID to be released
/******************************************************************************/
MS_U8 msAPI_Memory_Free(void *pFree, EN_BUFFER_ID enBufID)
{
     MS_BOOL bResult;
    bResult = MsOS_FreeMemory(pFree, (MS_U32)enBufID);

    if (bResult == TRUE)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void *msAPI_Memory_ReAllocate(void *pAlloc, MS_U16 newsize, EN_BUFFER_ID enBufID)
{
    return MsOS_ReallocateMemory(pAlloc, (MS_S32)newsize, (MS_S32)enBufID);
}

#else // MSOS_TYPE_LINUX
#include <stdlib.h>

void msAPI_Memory_Init(void)
{

}
inline void * msAPI_Memory_Allocate(MS_U16 u16Number_of_bytes,EN_BUFFER_ID enBufID)
{
    UNUSED(enBufID);
    return malloc(u16Number_of_bytes);
}

void *msAPI_Memory_ReAllocate(void *pAlloc, MS_U16 newsize, EN_BUFFER_ID enBufID)
{
    UNUSED(enBufID);
    if(pAlloc!=NULL)
    {
        return realloc(pAlloc, newsize);
    }
    else
    {
        return NULL;
    }
}

inline MS_U8 msAPI_Memory_Free(void *pFree, EN_BUFFER_ID enBufID)
{
    UNUSED(enBufID);
    if(pFree!=NULL)
    {
        free(pFree);
    }
    return 1;
}

#endif //MSOS_TYPE_LINUX


//void msAPI_Aeon_Disable(void)
//{
//}

#undef MSAPI_MEMORY_C

