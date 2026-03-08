/**
* Copyright (c) 2006 – 2018 MStar Semiconductor, Inc.
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
//******************************************************************************
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
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
    asm __volatile__(
        "sync;"
        );
}

//-------------------------------------------------------------------------------------------------
/// Nop
/// @return None
//-------------------------------------------------------------------------------------------------
inline void MAsm_CPU_Nop(void)
{
    asm __volatile__ (
        "nop;"
        );
}

//-------------------------------------------------------------------------------------------------
/// SW Debug Breakpoint
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_SwDbgBp(void)
{
    asm __volatile__ (
        "SDBBP;"
        );
}

//-------------------------------------------------------------------------------------------------
/// Enter CPU power saving mode
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_PowerDown(void)
{
    asm __volatile__ (
        "wait;"
        "nop;"
        "nop;"
    );
}

//-------------------------------------------------------------------------------------------------
/// Set Status BEV
/// @param  bBEV \b IN: TRUE/FALSE: 1 bootstrap / 0 normal
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_StatusBEV(MS_BOOL bBEV)
{
    MS_U32 u32BEV = ((MS_U32)bBEV) << 22;

    asm __volatile__ (
        "mfc0   $8, $12;"
        "nop;"
        "move   $9, %0;"
        "or     $8, $8, $9;"
        "mtc0   $8, $12;"
        "nop; nop; nop;"
        :
        : "r"(u32BEV)
        : "$8", "$9"
        );
}

//-------------------------------------------------------------------------------------------------
/// Jump to the specified PC
/// @param  u32PC \b IN: PC
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_Jump(MS_U32 u32PC)
{
    typedef void ( *DirectJump ) (void);
    DirectJump pFunc = (DirectJump)u32PC;
    (*pFunc)();
}

//-------------------------------------------------------------------------------------------------
/// Get trail one
/// @param  u32Flags \b IN: 32-bit flag
/// @return trail one position
//-------------------------------------------------------------------------------------------------
MS_U8 MAsm_CPU_GetTrailOne(MS_U32 u32Flags)
{
    u32Flags = (~u32Flags) & (u32Flags-1);
    asm __volatile__
    (
        "clz %0, %0\n"
        : "=r" (u32Flags)
        : "0" (u32Flags)
    );
    return 32 - u32Flags;
}

//-------------------------------------------------------------------------------------------------
/// Disable system timer interrupt
/// @param  bEnable \b IN: TRUE: Enable timer interrupt, FALSE: Disable timer interrupt.
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_EnableTimerInterrupt(MS_BOOL bEnable)
{
    if(bEnable)
    {
        asm __volatile__ (
            "mfc0   $8, $12;"
            "nop;"
            "or     $8,$8,0x00008000;"      // IM7 = 1, timer interrupt enabled.
            "mtc0   $8,$12;"
            "nop; nop; nop;"
        );
    }
    else
    {
        asm __volatile__ (
            "mfc0   $8, $12;"
            "nop;"
            "and    $8,$8,0XFFFF7FFF;"      // IM7 = 0, timer interrupt disabled.
            "mtc0   $8,$12;"
            "nop; nop; nop;"
        );
    }
}

//-------------------------------------------------------------------------------------------------
/// Pause for specifc duration by CPU dummy instruction
/// @param  msec \b IN: miniseconds ( msec < (12800000/CPU_CLK_MHZ) )
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_DelayMs(MS_U32 msec)
{
    #define CPU_LOOP_MSEC(_msec)        ((_msec)*(MIPS_CLOCK_FREQ/1000/3)) // 3 cycles / loop

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
#if 0
    #define CPU_LOOP_USEC(_usec)        ((_usec)*(MIPS_CLOCK_FREQ/1000000/3)) // 3 cycles / loop

    volatile register MS_U32 loop = CPU_LOOP_USEC(usec);
    while(loop--);
#endif

    asm __volatile__ (
        "li     $8, %0\n"
        "mul    $9, $4, $8\n"
        "1:\n"
        "addiu  $9, $9, -1\n"
        "bnez   $9, 1b\n"
        :
        :"I" (MIPS_CLOCK_FREQ/1000000/3)
        :"$8", "$9"
    );
}

//-------------------------------------------------------------------------------------------------
/// Set EBASE
/// @param  u32addr \b IN: MIPS Code Start Address
/// @return None
//-------------------------------------------------------------------------------------------------
void MAsm_CPU_SetEBASE(MS_U32 u32addr)
{
    asm __volatile__ (
        // Set interrupt mode to compaitible mode.
        // 1. Need not to fill IntCtlVs, the default value is already 0.
        // 2. Set CauseIv = 0. Use general exception vector. page 66, 117.
        "mfc0    $26, $13;"         //mfc0    k0, cause
        "nop; nop;"                 //mfc0_delay
        "li     $27, ~(1<<23);"     //li      k1, ~(1<<23)
        "and    $26, $26, $27;"     //and     k0, k0, k1
        "mtc0   $26, $13;"          //mtc0    k0, cause
        "nop; nop;"                 //mfc0_delay

        // 3. Set StatusBEV  = 1 and adjust EBASE coprocessor
        "mfc0   $26, $12;"          //mfc0    k0, status
        "nop; nop;"                 //mfc0_delay
        "li     $27, (1<<22);"      //li      k1, (1<<22)
        "or     $26, $26, $27;"     //or      k0, k0, k1
        "mtc0   $26, $12;"          //mtc0    k0, status
        "nop; nop;"                 //mfc0_delay

        "mfc0   $26, $15, 1;"       //mfc0    k0, $15, 1
        "nop; nop;"                 //mfc0_delay
        "move   $27, %0;"           //move    k1, u32addr
        "or     $26, $26, $27;"     //or      k0, k0, k1
        "mtc0   $26, $15, 1;"       //mtc0    k0, $15, 1
        "nop; nop;"                 //mfc0_delay

        // 4. Set StatusBEV & StatusEXL to be zero. page 66,
        "mfc0   $26, $12;"          //mfc0    k0, status
        "nop; nop;"                 //mfc0_delay
        "li     $27, ~((1<<22)|(1<<1));" //li      k1, ~((1<<22)|(1<<1))
        "and    $26, $26, $27;"     //and     k0, k0, k1
        "mtc0   $26, $12;"          //mtc0    k0, status
        "nop; nop;"                 //mfc0_delay
        // Now the Refill exception vector is u32addr and
        // general exception vector is u32addr.
        :
        : "r"(u32addr)
        : "$12", "$13", "$15", "$26", "$27"
        );
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

            asm __volatile__ (
            "lui     $8, ((%0*1000)>>16);"
            "ori     $8, $8, ((%0*1000)&0xFFFF);"
            "mtc0    $0,$9;"
            "nop; nop;  nop;"
            "mtc0    $8,$11;"
            "nop; nop;  nop;"
            :
        :"I" (MIPS_CLOCK_FREQ/1000000/2)
        :"$8", "$9"
            );

}

