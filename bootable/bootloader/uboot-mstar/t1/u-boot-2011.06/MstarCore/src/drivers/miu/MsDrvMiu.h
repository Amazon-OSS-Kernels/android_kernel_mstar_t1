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

#ifndef __DRVMIU__
#define __DRVMIU__

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <configs/uboot_board_config.h>
#include <ShareType.h>
#include <drvMIU.h>

typedef enum
{
    E_MIU_ID_0=0x00,
    E_MIU_ID_1,
    E_MIU_ID_2,
    E_MIU_ID_MAX
}EN_MIUID;

#define CONFIG_SYS_MIU2_INTERVAL 0xC0000000

#define	MIU_MAX_GP_NUM		        (16)         //Max MIU Group number
#define MIU_MAX_GP_PRIORITY_NUM     (16)         //Max MIU Group Priority number

typedef struct
{
    MIU_ID miu_id;
    MS_U8 u8Group;
    char *c_Mmap_Id;
    MS_U32 u32Start;
    MS_U32 u32End;
    MS_U8 clientID[16];
} stMIU_PortectInfo;


///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Global Functions
///
///////////////////////////////////////////////////////////////////////////////////////////////////
U8 MDrv_MIU_SelectMIU_UBoot(U8 bMIU1, U16 *au16SelMiu, U8 u8GpNum);
U8 MDrv_MIU_SetGroupPriority_UBoot(U8 bMIU1, U8 *au8GpPriority, U8 u8GpNum);
MS_BOOL MsDrv_MIU_Protect(MS_U8  u8Blockx, MS_U8  *pu8ProtectId, MS_U64 u64Start, MS_U64 u64End, MS_BOOL bSetFlag);
void MsDrv_kernelProtect(MS_U8  u8Blockx,MS_U64 u64Lx1Start, MS_U64 u64Lx1End);
void MsDrv_PMProtect(MS_U8  u8Blockx,MS_U64 u64Lx1Start, MS_U64 u64Lx1End);
void MsDrv_NuttxProtect(MS_U8  u8Blockx,MS_U64 u64NuttxStart, MS_U64 u64NuttxEnd);
void MsDrv_MiuInit(void);
MIU_DDR_SIZE dram_size_to_enum(MS_U32 u32DramSize);
char* MsDrv_GetMIUSize(MS_U8 MiuID);
MS_U32 BA2PA(MS_U64 u64usAddr);
MS_U64 PA2BA(MS_U32 u32PhyAddr);

//-------------------------------------------------------------------------------------------------
/// get MIU_ID by input uPhyAddrs
/// @INPUT:  uPhyAddrs                 \b OUT: MIU_ID
//-------------------------------------------------------------------------------------------------
EN_MIUID MsApi_Miu_GetID(unsigned int uPhyAddrs);


#endif	//__DRVMIU__

