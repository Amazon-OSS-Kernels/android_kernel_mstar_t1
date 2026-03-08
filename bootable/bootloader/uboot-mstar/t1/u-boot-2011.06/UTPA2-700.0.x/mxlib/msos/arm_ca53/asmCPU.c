/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    asmCPU.c
/// @brief  MIPS Inline Assembly Wrapper
/// @note   Compile only with mips32 or mips32r2, but not mips16 or mips16e
///////////////////////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "asmCPU.h"
#include "halCHIP.h"

//-------------------------------------------------------------------------------------------------
// Local Defines
//------------------------------------------------------------------------------------------------
#ifdef CONFIG_MBOOT
#define RIUBASE   0x1F000000
//#define PERIBASE  0x16000000
#else
#define RIUBASE   0xFD000000
//#define PERIBASE  0xFC000000
#endif

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Local Function Prototypes
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Flush EC's write FIFO
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_Sync(void)
{
#if defined (__aarch64__)
    asm __volatile__ (
        "dsb sy;"
        );
#else
    asm __volatile__ (
        "DSB"
        );
#endif
}

//-------------------------------------------------------------------------------------------------
/// Nop
/// @return None
//-------------------------------------------------------------------------------------------------
inline void MAsm_CPU_Nop(void)
{
#if defined (__aarch64__)
    asm __volatile__ (
        "nop;"
        );
#else
    asm __volatile__ (
        "nop;"
        );
#endif
}

//-------------------------------------------------------------------------------------------------
/// SW Debug Breakpoint
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_SwDbgBp(void)
{
    // @FIXME: Richard: which trap should I issue?
#if defined (__aarch64__)
    asm __volatile__ (
        "nop;"
        );
#else
    asm __volatile__ (
        "BKPT 0x1234;"
        );
#endif
}

//-------------------------------------------------------------------------------------------------
/// Enter CPU power saving mode
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_PowerDown(void)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Set Status BEV
/// @param  bBEV \b IN: TRUE/FALSE: 1 bootstrap / 0 normal
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_StatusBEV(MS_BOOL bBEV)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Jump to the specified PC
/// @param  u32PC \b IN: PC
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_Jump(MS_U32 u32PC)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//-------------------------------------------------------------------------------------------------
/// Get trail one
/// @param  u32Flags \b IN: 32-bit flag
/// @return trail one position
//-------------------------------------------------------------------------------------------------
MS_U8 MAsm_CPU_GetTrailOne(MS_U32 u32Flags)
{
    MS_U32 i;
    for (i = 0; i< 32; i++)
    {
        if (u32Flags & (1<< i))
        {
            return i;
        }
    }
    return 32;
}

//-------------------------------------------------------------------------------------------------
/// Disable system timer interrupt
/// @param  bEnable \b IN: TRUE: Enable timer interrupt, FALSE: Disable timer interrupt.
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_EnableTimerInterrupt(MS_BOOL bEnable)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

/*To do: you should use MsOS_DelayTask instead of this API*/
void MAsm_CPU_DelayMs(MS_U32 msec)
{
    #define CPU_LOOP_MSEC(_msec)        ((_msec) * ((ARM_CLOCK_FREQ / 2 / 1000) / 3)) // 3 cycles / loop

    register MS_U32 loop = CPU_LOOP_MSEC(msec);
    while(loop--);
}

//-------------------------------------------------------------------------------------------------
/// Pause for specifc duration by CPU dummy instruction
/// @param  usec \b IN: microseconds ( usec < (12800000/CPU_CLK_MHZ) )
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_DelayUs(MS_U32 usec)
{
#if defined (__aarch64__)
    asm __volatile__ (
        "nop;"
    );
#else
    asm __volatile__ (
        "mov  r0, %0\n"
        "wait: subs    r0, r0, #1\n"
        "bne     wait\n"
        :
        :"r" (usec)
        :"memory"
    );
#endif
}

//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32addr \b IN: MIPS Code Start Address
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_SetEBASE(MS_U32 u32addr)
{
    //printf("[%s][%d] %s is not supported\n", __FUNCTION__, __LINE__, __FUNCTION__);
}

//
// System time
//


//-------------------------------------------------------------------------------------------------
/// Get current system time in timer ticks
/// @return system time in timer ticks
//-------------------------------------------------------------------------------------------------
MS_U32 gsystem_time_ms=0;
MS_U32 MAsm_GetSystemTime (void)
{
    return gsystem_time_ms;
}

void MAsm_CPU_TimerInit(void)
{
#if defined (__aarch64__)
#else
    asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));
    asm volatile("mcr p15, 0, %0, c14, c2, 0" : : "r" (ARM_CLOCK_FREQ / 2 / 1000));
#endif
}

void MAsm_CPU_TimerStart(void)
{
#if defined (__aarch64__)
#else
    asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (1));
#endif
}

void MAsm_CPU_TimerClean(void)
{
#if defined (__aarch64__)
#else
    asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" (0));
#endif
}

