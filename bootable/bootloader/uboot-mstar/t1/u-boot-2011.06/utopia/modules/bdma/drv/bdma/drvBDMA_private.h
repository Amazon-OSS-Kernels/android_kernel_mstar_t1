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

#ifndef _DRVBDMA_PRIV_H_
#define _DRVBDMA_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvBDMA.h"

typedef MS_U32 (*IOCTL_BDMA_MEMCPY)(MS_PHY, MS_PHY, MS_U32);
typedef MS_U32 (*IOCTL_BDMA_SEARCH)(MS_PHY, MS_U32, MS_U32, MS_U32, BDMA_SrcDev);
typedef MS_U32 (*IOCTL_BDMA_CRC32)(MS_PHY, MS_U32, MS_U32, MS_U32, BDMA_SrcDev, MS_BOOL);
typedef MS_U32 (*IOCTL_BDMA_PATTERN_FILL)(MS_PHY, MS_U32, MS_U32, BDMA_DstDev);
typedef MS_U32 (*IOCTL_BDMA_FLASHCOPY2DRAM)(MS_PHY, MS_PHY, MS_U32);
#ifdef MOBF_ENABLE
typedef MS_U32 (*IOCTL_BDMA_MOBFSEARCH)(MS_PHY, MS_U32, BDMA_MOBF_PS*, BDMA_SrcDev);
#endif
typedef void (*IOCTL_BDMA_GETSTATUS)(BDMA_Status*);
typedef BDMA_Info* (*IOCTL_BDMA_GETINFO)(void);
typedef BDMA_Result (*IOCTL_BDMA_GETLIBVER)(const MSIF_Version **);
typedef MS_U32 (*IOCTL_BDMA_GETMINSIZE)(void);
typedef BDMA_Result (*IOCTL_BDMA_SETDBGLEVEL)(BDMA_DbgLv);
typedef void (*IOCTL_BDMA_SETSPIOFFSETFORMCU)(void);
typedef BDMA_Result (*IOCTL_BDMA_WAITFLASHDONE)(void);
typedef BDMA_Result (*IOCTL_BDMA_INIT)(MS_PHY);
typedef BDMA_Result (*IOCTL_BDMA_COPYHND)(MS_PHY, MS_PHY, MS_U32, BDMA_CpyType, MS_U8);

typedef struct _BDMA_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}BDMA_RESOURCE_PRIVATE;

typedef struct _BDMA_INSTANT_PRIVATE
{
    IOCTL_BDMA_MEMCPY           fpBDMAMemCpy;
    IOCTL_BDMA_SEARCH           fpBDMASearch;
    IOCTL_BDMA_CRC32            fpBDMACRC32;
    IOCTL_BDMA_PATTERN_FILL     fpBDMAPatternFill;
    IOCTL_BDMA_FLASHCOPY2DRAM   fpBDMAFlashCopy2Dram;
#ifdef MOBF_ENABLE
    IOCTL_BDMA_MOBFSEARCH       fpBDMAMOBFSearch;
#endif
    IOCTL_BDMA_GETSTATUS        fpBDMAGetStatus;
    IOCTL_BDMA_GETINFO              fpBDMAGetInfo;
    IOCTL_BDMA_GETLIBVER            fpBDMAGetLibVer;
    IOCTL_BDMA_GETMINSIZE           fpBDMAGetMinSize;
    IOCTL_BDMA_SETDBGLEVEL          fpBDMASetDbgLevel;
    IOCTL_BDMA_SETSPIOFFSETFORMCU   fpBDMASetSPIOffsetForMCU;
    IOCTL_BDMA_WAITFLASHDONE        fpBDMAWaitFlashDone;
    IOCTL_BDMA_INIT                 fpBDMAInit;
    IOCTL_BDMA_COPYHND              fpBDMACopyHnd;
}BDMA_INSTANT_PRIVATE;

void BDMARegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 BDMAOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 BDMAClose(void* pInstance);
MS_U32 BDMAIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
MS_U32 BDMAStr(MS_U32 u32PowerState, void* pModule);

BDMA_Result _MDrv_BDMA_MemCopy(MS_PHYADDR u32SrcAddr, MS_PHYADDR u32DstAddr, MS_U32 u32Len);
MS_U32 _MDrv_BDMA_Search(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, MS_U32 u32ExcluBit, BDMA_SrcDev eDev);
MS_U32 _MDrv_BDMA_CRC32(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Poly, MS_U32 u32Seed, BDMA_SrcDev eDev, MS_BOOL bReflect);
BDMA_Result _MDrv_BDMA_PatternFill(MS_PHYADDR u32Addr, MS_U32 u32Len, MS_U32 u32Pattern, BDMA_DstDev eDev);
BDMA_Result _MDrv_BDMA_FlashCopy2Dram(MS_PHYADDR u32FlashAddr, MS_PHYADDR u32DramAddr, MS_U32 u32Len);
#ifdef MOBF_ENABLE
MS_U32 _MDrv_BDMA_MOBFSearch(MS_PHYADDR u32Addr, MS_U32 u32Len, BDMA_MOBF_PS* pMobfPsCfg, BDMA_SrcDev eDev);
#endif
void _MDrv_BDMA_GetStatus(BDMA_Status *pStatus);
void _MDrv_BDMA_SetSPIOffsetForMCU(void);
BDMA_Result _MDrv_BDMA_WaitFlashDone(void);
BDMA_Result _MDrv_BDMA_Init(MS_PHY phy64Miu1Base);
BDMA_Result _MDrv_BDMA_CopyHnd (MS_PHY phy64SrcAddr, MS_PHY phy64DstAddr, MS_U32 u32Len, BDMA_CpyType eCpyType, MS_U8 u8OpCfg);

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
