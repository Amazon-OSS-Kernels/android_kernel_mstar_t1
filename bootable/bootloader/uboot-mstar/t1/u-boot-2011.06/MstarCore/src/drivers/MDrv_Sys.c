/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#include <common.h>
#include <MsTypes.h>
#include <MsUboot.h>
#include <drvMMIO.h>
#include <ShareType.h>
#include <mhal_sys.h>
#if defined(CONFIG_MSTAR_ROM_BOOT_WITH_EMMC_FLASH)
#include "./mmc/inc/common/eMMC.h"
#endif
extern void __MDrv_WriteByte( U32 u32Reg, U8 u8Value );
extern void __MDrv_Write4Byte( U32 u32Reg, U32 u32Value );

unsigned long RIU_MAP=0;

#define MDrv_Write4Byte( u32Reg, u32Val )                                               \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) && !((u32Reg) & 0x01) )                         \
    {                                                                                   \
        RIU[u32Reg] = (U16)(u32Val);                                                    \
        RIU[(u32Reg) + 2] = (U16)((u32Val) >> 16);                                      \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_Write4Byte( u32Reg, u32Val );                                            \
    }                                                                                   \
    MST_MACRO_END

#define MDrv_WriteRegBit( u32Reg, bEnable, u8Mask )                                     \
    MST_MACRO_START                                                                     \
    U32 u32Reg8 = ((u32Reg) * 2) - ((u32Reg) & 1);                                      \
    RIU8[u32Reg8] = (bEnable) ? (RIU8[u32Reg8] |  (u8Mask)) :                           \
                                (RIU8[u32Reg8] & ~(u8Mask));                            \
    MST_MACRO_END

#define _MDrv_WriteByte( u32Reg, u8Val )                                                 \
    MST_MACRO_START                                                                     \
    if ( __builtin_constant_p( u32Reg ) )                                               \
    {                                                                                   \
        RIU8[((u32Reg) * 2) - ((u32Reg) & 1)] = u8Val;                                  \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        __MDrv_WriteByte( u32Reg, u8Val );                                              \
    }                                                                                   \
    MST_MACRO_END

void MDrv_Sys_WholeChipReset( void )
{

    Mhal_Sys_WholeChipReset();
    while(1);
}

