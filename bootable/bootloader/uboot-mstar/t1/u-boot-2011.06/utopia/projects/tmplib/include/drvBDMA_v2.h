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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvBDMA_v2.h
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_BDMA_V2_H_
#define _DRV_BDMA_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MDrv_CMD_BDMA_MemCopy,
    MDrv_CMD_BDMA_Search,
    MDrv_CMD_BDMA_CRC32,
    MDrv_CMD_BDMA_Pattern_Fill,
    MDrv_CMD_BDMA_FlashCopy2Dram,
    MDrv_CMD_BDMA_MOBFSearch,
    MDrv_CMD_BDMA_GetStatus,
    MDrv_CMD_BDMA_GetInfo,
    MDrv_CMD_BDMA_GetLibVer,
    MDrv_CMD_BDMA_GetMinSize,
    MDrv_CMD_BDMA_SetDbgLevel,
    MDrv_CMD_BDMA_SetSPIOffsetForMCU,
    MDrv_CMD_BDMA_WaitFlashDone,
} eBdmaIoctlOpt;

//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef struct _BDMA_API_V1
{
}BDMA_API_V1;

typedef struct _BDMA_API_V2
{
}BDMA_API_V2;

// below are for utopia20 cmd and parameters structure
// for cmd MDrv_CMD_BDMA_MemCopy
typedef struct _BDMA_MEMCOPY_PARAM
{
    MS_PHY phy64SrcAddr;
    MS_PHY phy64DstAddr;
    MS_U32 u32Len;
}BDMA_MEMCOPY_PARAM, *PBDMA_MEMCOPY_PARAM;

// for cmd MDrv_CMD_BDMA_Search
typedef struct _BDMA_SEARCH_PARAM
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Len;
    MS_U32 u32Pattern;
    MS_U32 u32ExcluBit;
    BDMA_SrcDev eDev;
    MS_U32* pSearchAddr;
}BDMA_SEARCH_PARAM, *PBDMA_SEARCH_PARAM;

// for cmd MDrv_CMD_BDMA_CRC32
typedef struct _BDMA_CRC32_PARAM
{
    MS_PHY phy64Addr;
    MS_U32 u32Len;
    MS_U32 u32Poly;
    MS_U32 u32Seed;
    BDMA_SrcDev eDev;
    MS_BOOL bReflect;
    MS_U32 u32CRC32;
}BDMA_CRC32_PARAM, *PBDMA_CRC32_PARAM;

// for cmd MDrv_CMD_BDMA_Pattern_Fill
typedef struct _BDMA_PATTERN_FILL_PARAM
{
    MS_PHYADDR u32Addr;
    MS_U32 u32Len;
    MS_U32 u32Pattern;
    BDMA_DstDev eDev;
}BDMA_PATTERN_FILL_PARAM, *PBDMA_PATTERN_FILL_PARAM;

// for cmd MDrv_CMD_BDMA_FlashCopy2Dram
typedef struct _BDMA_FLASHCOPY2DRAM_PARAM
{
    MS_PHYADDR u32FlashAddr;
    MS_PHYADDR u32DramAddr;
    MS_U32 u32Len;
}BDMA_FLASHCOPY2DRAM_PARAM, *PBDMA_FLASHCOPY2DRAM_PARAM;

// for cmd MDrv_CMD_BDMA_MOBFSearch
#ifdef MOBF_ENABLE
typedef struct _BDMA_MOBFSEARCH_PARAM
{
    MS_PHY phy64Addr;
    MS_U32 u32Len;
    BDMA_MOBF_PS* pMobfPsCfg;
    BDMA_SrcDev eDev;
    MS_PHY phy64SearchAddr;
}BDMA_MOBFSEARCH_PARAM, *PBDMA_MOBFSEARCH_PARAM;
#endif

// for cmd MDrv_CMD_BDMA_GetStatus
typedef struct _BDMA_GETSTATUS_PARAM
{
    BDMA_Status *pStatus;
}BDMA_GETSTATUS_PARAM, *PBDMA_GETSTATUS_PARAM;


// for cmd MDrv_CMD_BDMA_GetInfo
typedef struct _BDMA_GETINFO_PARAM
{
    MS_U8       u8ChNum;
    MS_U16      u16ChipVer;
    MS_U32      u32IOMap;
    MS_U32      u32MIU1Base;
    MS_S32      s32Mutex;
    MS_BOOL     bInit;
    BDMA_DbgLv  eDbgLv;
    BDMA_HwInfo sHwCap;
}BDMA_GETINFO_PARAM, *PBDMA_GETINFO_PARAM;

// for cmd MDrv_CMD_BDMA_GetLibVer
typedef struct _BDMA_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
}BDMA_GETLIBVER_PARAM, *PBDMA_GETLIBVER_PARAM;

// for cmd MDrv_CMD_BDMA_GetMinSize
typedef struct _BDMA_GETMINSIZE_PARAM
{
    MS_U32 u32MinSize;
}BDMA_GETMINSIZE_PARAM, *PBDMA_GETMINSIZE_PARAM;

// for cmd MDrv_CMD_BDMA_SetDbgLevel
typedef struct _BDMA_SETDBGLEVEL_PARAM
{
    BDMA_DbgLv eLevel;
}BDMA_SETDBGLEVEL_PARAM, *PBDMA_SETDBGLEVEL_PARAM;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_BDMA_V2_H_

