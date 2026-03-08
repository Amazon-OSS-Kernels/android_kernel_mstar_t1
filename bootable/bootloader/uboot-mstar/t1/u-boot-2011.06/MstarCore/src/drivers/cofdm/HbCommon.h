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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   HB_Common.h
/// @brief  MStar OS Wrapper
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __HB_COMMON_H__
#define __HB_COMMON_H__

// #ifdef MS_DEBUG
#if 0

extern void MAsm_CPU_SwDbgBp(void);

#define HB_ASSERT(_bool_)                                                                                       \
{                                                                                                               \
    if ( ! ( _bool_ ) )                                                                                         \
    {                                                                                                           \
        printf("[%s][%d] HB_ASSERT FAIL\n", __FUNCTION__, __LINE__);                                            \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
}

#define HB_printf(...)                                                                                          \
({                                                                                                              \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] printf in ISR\n", __FUNCTION__, __LINE__);                                             \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    printf(__VA_ARGS__);                                                                                        \
})

#define HB_AllocateMemory(u32Size, s32PoolId)                                                                   \
({                                                                                                              \
    void* temp = NULL;                                                                                          \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] allocate in ISR\n", __FUNCTION__, __LINE__);                                           \
        HB_ASSERT(0);                                                                                           \
    }                                                                                                           \
    temp = MsOS_AllocateMemory ((u32Size), (s32PoolId));                                                        \
    printf("[%s][%d] allocate 0x%08x 0x%08x %d\n", __FUNCTION__, __LINE__, (s32PoolId), (int)temp, (u32Size));  \
    (temp);                                                                                                     \
})


#define HB_FreeMemory(pAddress, s32PoolId)                                                                      \
({                                                                                                              \
    MS_BOOL ret;                                                                                                \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] free in ISR\n", __FUNCTION__, __LINE__);                                               \
        HB_ASSERT(0);                                                                                           \
    }                                                                                                           \
    ret = MsOS_FreeMemory ((pAddress), (s32PoolId));                                                            \
    printf("[%s][%d] free 0x%08x 0x%08x\n", __FUNCTION__, __LINE__, (s32PoolId), (int)(pAddress));              \
    (ret);                                                                                                      \
})

#define HB_ObtainMutex(id, time)                                                                                \
({                                                                                                              \
    MS_BOOL ret;                                                                                                \
    if ((id & 0xFFFF0000)!= 0x76540000)                                                                         \
    {                                                                                                           \
        printf("[%s][%d] invalid mutex id 0x%08x\n", __FUNCTION__, __LINE__, id);                               \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] obtain mutex in ISR\n", __FUNCTION__, __LINE__);                                       \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    ret = MsOS_ObtainMutex(id, time);                                                                           \
    (ret);                                                                                                      \
})

#define HB_ReleaseMutex(id)                                                                                     \
({                                                                                                              \
    MS_BOOL ret;                                                                                                \
    if ((id & 0xFFFF0000)!= 0x76540000)                                                                         \
    {                                                                                                           \
        printf("[%s][%d] invalid mutex id 0x%08x\n", __FUNCTION__, __LINE__, id);                               \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    if (MsOS_In_Interrupt())                                                                                    \
    {                                                                                                           \
        printf("[%s][%d] obtain mutex in ISR\n", __FUNCTION__, __LINE__);                                       \
        MAsm_CPU_SwDbgBp();                                                                                     \
    }                                                                                                           \
    ret = MsOS_ReleaseMutex(id);                                                                                \
    (ret);                                                                                                      \
})

#else // #ifdef MS_DEBUG

#define HB_ASSERT                       MS_ASSERT
#define HB_printf                       printf
#if 0
#ifdef MS_DEBUG
#define HB_printf(fmt, args...)         printf("[EPGDB][%06d]  " fmt, __LINE__, ## args)
#else
#define HB_printf(fmt, args...)
#endif
#endif
#define HB_AllocateMemory               MsOS_AllocateMemory
#define HB_FreeMemory                   MsOS_FreeMemory
#define HB_ObtainMutex                  MsOS_ObtainMutex
#define HB_ReleaseMutex                 MsOS_ReleaseMutex

#endif // #ifdef MS_DEBUG

#endif // __MEMORY_DEF_H__
