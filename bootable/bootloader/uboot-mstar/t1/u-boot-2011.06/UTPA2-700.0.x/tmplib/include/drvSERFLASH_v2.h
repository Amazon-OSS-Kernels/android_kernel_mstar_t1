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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvBDMA_v2.h
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_SERFLASH_V2_H_
#define _DRV_SERFLASH_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    MDrv_CMD_SERFLASH_GetInfo,
    MDrv_CMD_SERFLASH_GetLibVer,
    MDrv_CMD_SERFLASH_GetStatus,
    MDrv_CMD_SERFLASH_SetDbgLevel,
    MDrv_CMD_SERFLASH_SetWPInfo,
    MDrv_CMD_SERFLASH_SetMcuCSCallBack,
    MDrv_CMD_SERFLASH_SetFlashWPCallBack,
    MDrv_CMD_SERFLASH_DetectType,
    MDrv_CMD_SERFLASH_DetectSize,
    MDrv_CMD_SERFLASH_Set2XRead,
    MDrv_CMD_SERFLASH_SetCKG,
    MDrv_CMD_SERFLASH_ClkDiv,
    MDrv_CMD_SERFLASH_SetMode,
    MDrv_CMD_SERFLASH_ReadUID,
    MDrv_CMD_SERFLASH_ChipSelect,
    MDrv_CMD_SERFLASH_Init,
    MDrv_CMD_SERFLASH_ReadID,
    MDrv_CMD_SERFLASH_Read,
    MDrv_CMD_SERFLASH_EraseChip,
    MDrv_CMD_SERFLASH_AddressToBlock,
    MDrv_CMD_SERFLASH_BlockToAddress,
    MDrv_CMD_SERFLASH_AddressErase,
    MDrv_CMD_SERFLASH_BlockErase,
    MDrv_CMD_SERFLASH_SectorErase,
    MDrv_CMD_SERFLASH_CheckWriteDone,
    MDrv_CMD_SERFLASH_Write,
    MDrv_CMD_SERFLASH_DMA,
    MDrv_CMD_SERFLASH_WriteProtect,
    MDrv_CMD_SERFLASH_WriteProtect_Enable_All_Range,
    MDrv_CMD_SERFLASH_WriteProtect_Disable_All_Range,
    MDrv_CMD_SERFLASH_WriteProtect_Disable_Range_Set,
    MDrv_CMD_SERFLASH_WriteProtect_Area,
    MDrv_CMD_SERFLASH_ReadStatusRegister,
    MDrv_CMD_SERFLASH_ReadStatusRegister2,
    MDrv_CMD_SERFLASH_WriteStatusRegister,
    MDrv_CMD_SERFLASH_CopyHnd,
    MDrv_CMD_SERFLASH_SetGPIO,
} eSerflashIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct _SERFLASH_READID
{
    MS_U8 *pu8FlashID;
    MS_U32 u32IDSize;
}SERFLASH_READID, *PSERFLASH_READID;

typedef struct _SERFLASH_READ
{
    MS_U32 u32FlashAddr;
    MS_U32 u32FlashSize;
    MS_U8 *user_buffer;
}SERFLASH_READ, *PSERFLASH_READ;

typedef struct _SERFLASH_ADDRTOBLK
{
    MS_U32 u32FlashAddr;
    MS_U32 *pu32BlockIndex;
}SERFLASH_ADDRTOBLK, *PSERFLASH_ADDRTOBLK;

typedef struct _SERFLASH_BLKTOADDR
{
    MS_U32 u32BlockIndex;
    MS_U32 *pu32FlashAddr;
}SERFLASH_BLKTOADDR, *PSERFLASH_BLKTOADDR;

typedef struct _SERFLASH_ADDRERASE
{
    MS_U32 u32StartAddr;
    MS_U32 u32EraseSize;
    MS_BOOL bWait;
}SERFLASH_ADDRERASE, *PSERFLASH_ADDRERASE;

typedef struct _SERFLASH_BLKERASE
{
    MS_U32 u32StartBlock;
    MS_U32 u32EndBlock;
    MS_BOOL bWait;
}SERFLASH_BLKERASE, *PSERFLASH_BLKERASE;

typedef struct _SERFLASH_SECERASE
{
    MS_U32 u32StartAddr;
    MS_U32 u32EndAddr;
}SERFLASH_SECERASE, *PSERFLASH_SECERASE;

typedef struct _SERFLASH_WRITE
{
    MS_U32 u32FlashAddr;
    MS_U32 u32FlashSize;
    MS_U8 *user_buffer;
}SERFLASH_WRITE, *PSERFLASH_WRITE;

typedef struct _SERFLASH_DMA
{
    MS_U32 u32FlashStart;
    MS_U32 u32DRAMStart;
    MS_U32 u32Size;
}SERFLASH_DMA, *PSERFLASH_DMA;

typedef struct _SERFLASH_WP
{
    MS_BOOL bEnable;
}SERFLASH_WP, *PSERFLASH_WP;

typedef struct _SERFLASH_WPDISABLERANGESET
{
    MS_U32 u32DisableLowerBound;
    MS_U32 u32DisableSize;
}SERFLASH_WPDISABLERANGESET, *PSERFLASH_WPDISABLERANGESET;

typedef struct _SERFLASH_WPAREA
{
    MS_BOOL bEnableAllArea;
    MS_U8 u8BlockProtectBits;
}SERFLASH_WPAREA, *PSERFLASH_WPAREA;

typedef struct _SERFLASH_COPYHND
{
    MS_PHYADDR u32FlashAddr;
    MS_PHYADDR u32DstAddr;
    MS_U32 u32Len;
    SPIDMA_Dev eDstDev;
    MS_U8 u8OpCfg;
}SERFLASH_COPYHND, *PSERFLASH_COPYHND;

typedef struct _SERFLASH_GETSTATUS
{
    SERFLASH_DrvStatus* pDrvStatus;
}SERFLASH_GETSTATUS, *PSERFLASH_GETSTATUS;

typedef struct _SERFLASH_GETLIBVER
{
    const MSIF_Version **ppVersion;
}SERFLASH_GETLIBVER, *PSERFLASH_GETLIBVER;

typedef struct _SERFLASH_SETDBGLEVEL
{
    MS_U8 u8DbgLevel;
}SERFLASH_SETDBGLEVEL, *PSERFLASH_SETDBGLEVEL;

typedef struct _SERFLASH_SETWPINFO
{
    MS_BOOL bWPInfo;
}SERFLASH_SETWPINFO, *PSERFLASH_SETWPINFO;

typedef struct _SERFLASH_SETMCUCSCB
{
    ms_Mcu_ChipSelect_CB ChipSel_cb;
}SERFLASH_SETMCUCSCB, *PSERFLASH_SETMCUCSCB;

typedef struct _SERFLASH_SETFLASHWPCB
{
    ms_Flash_SetHWWP_CB FlashWP_cb;
}SERFLASH_SETFLASHWPCB, *PSERFLASH_SETFLASHWPCB;

typedef struct _SERFLASH_DETECTSIZE
{
    MS_U32 *u32FlashSize;
}SERFLASH_DETECTSIZE, *PSERFLASH_DETECTSIZE;

typedef struct _SERFLASH_SET2XREAD
{
    MS_BOOL b2XMode;
}SERFLASH_SET2XREAD, *PSERFLASH_SET2XREAD;

typedef struct _SERFLASH_SETCKG
{
    SPI_DrvCKG eCKGspi;
}SERFLASH_SETCKG, *PSERFLASH_SETCKG;

typedef struct _SERFLASH_CLKDIV
{
    SPI_DrvClkDiv eClkDivspi;
}SERFLASH_CLKDIV, *PSERFLASH_CLKDIV;

typedef struct _SERFLASH_SETMODE
{
    MS_BOOL bXiuRiu;
}SERFLASH_SETMODE, *PSERFLASH_SETMODE;

typedef struct _SERFLASH_CHIPSELECT
{
    MS_U8 u8FlashIndex;
}SERFLASH_CHIPSELECT, *PSERFLASH_CHIPSELECT;

typedef struct _SERFLASH_READSTATUSREG
{
    MS_U8 *pu8StatusReg;
}SERFLASH_READSTATUSREG, *PSERFLASH_READSTATUSREG;

typedef struct _SERFLASH_READSTATUSREG2
{
    MS_U8 *pu8StatusReg;
}SERFLASH_READSTATUSREG2, *PSERFLASH_READSTATUSREG2;

typedef struct _SERFLASH_WRITESTATUSREG
{
    MS_U16 u16StatusReg;
}SERFLASH_WRITESTATUSREG, *PSERFLASH_WRITESTATUSREG;

typedef struct _SERFLASH_SETGPIO
{
    MS_BOOL bSwitch;
}SERFLASH_SETGPIO, *PSERFLASH_SETGPIO;

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

#endif // _DRV_SERFLASH_V2_H_

