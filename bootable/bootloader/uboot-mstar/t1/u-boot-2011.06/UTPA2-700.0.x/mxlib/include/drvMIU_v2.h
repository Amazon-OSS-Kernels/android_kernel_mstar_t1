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
/// @file   drvMIU_v2.h
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MIU_V2_H_
#define _DRV_MIU_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
typedef enum _eMIUIoctlCmd
{
    MDrv_CMD_MIU_Init,
    MDrv_CMD_MIU_Mask_Req_OPM_R,
    MDrv_CMD_MIU_Mask_Req_DNRB_R,
    MDrv_CMD_MIU_Mask_Req_DNRB_W,
    MDrv_CMD_MIU_Mask_Req_DNRB_RW,
    MDrv_CMD_MIU_Mask_Req_SC_RW,
    MDrv_CMD_MIU_Mask_Req_MVOP_R,
    MDrv_CMD_MIU_Mask_Req_MVD_R,
    MDrv_CMD_MIU_Mask_Req_MVD_W,
    MDrv_CMD_MIU_Mask_Req_MVD_RW,
    MDrv_CMD_MIU_Mask_Req_AUDIO_RW,
    MDrv_CMD_MIU_Mask_Req,
    MDrv_CMD_MIU_UnMask_Req,
    MDrv_CMD_MIU_SetSsc,
    MDrv_CMD_MIU_SetSscValue,
    MDrv_CMD_MIU_Protect,
    MDrv_CMD_MIU_SelMIU,
    MDrv_CMD_MIU_GetProtectInfo,
    MDrv_CMD_MIU_SetGroupPriority,
    MDrv_CMD_MIU_SetHighPriority,
    MDrv_CMD_MIU_PrintProtectInfo,
    MDrv_CMD_MIU_Dram_Size,
    MDrv_CMD_MIU_ProtectAlign,

}eMIUIoctlCmd;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////
// below are for utopia20 cmd and parameters structure
// for below cmds
// MDrv_CMD_MIU_Mask_Req_OPM_R,
// MDrv_CMD_MIU_Mask_Req_DNRB_R,
// MDrv_CMD_MIU_Mask_Req_DNRB_W,
// MDrv_CMD_MIU_Mask_Req_DNRB_RW,
// MDrv_CMD_MIU_Mask_Req_SC_RW,
// MDrv_CMD_MIU_Mask_Req_MVOP_R,
// MDrv_CMD_MIU_Mask_Req_MVD_R,
// MDrv_CMD_MIU_Mask_Req_MVD_W,
// MDrv_CMD_MIU_Mask_Req_MVD_RW,
// MDrv_CMD_MIU_Mask_Req_AUDIO_RW,

typedef struct _MIU_RET
{
    MS_BOOL bRet;
}MIU_RET, *PMIU_RET;

typedef struct _MIU_MASK
{
    MS_U8 u8Mask;
    MS_U8 u8Miu;
}MIU_MIU_MASK, *PMIU_MIU_MASK;

// for below cmds MDrv_CMD_MIU_Mask_Req, MDrv_CMD_MIU_UnMask_Req
typedef struct _MIU_MIUREQ_MASK
{
    MS_U8 u8Miu;
    eMIUClientID eClientID;
}MIU_MIUREQ_MASK, *PMIU_MIUREQ_MASK;

// for below cmds MDrv_CMD_MIU_SetSsc
typedef struct _MIU_SET_SSC
{
    MS_U16 u16Fmodulation;
    MS_U16 u16FDeviation;
    MS_BOOL bEnable;
    MS_BOOL bRet;
}MIU_SET_SSC, *PMIU_SET_SSC;

// for below cmds MDrv_CMD_MIU_SetSscValue
typedef struct _MIU_SET_SSCVALUE
{
    MS_U8 u8MiuDev;
    MS_U16 u16Fmodulation;
    MS_U16 u16FDeviation;
    MS_BOOL bEnable;
    MS_BOOL bRet;
}MIU_SET_SSCVALUE, *PMIU_SET_SSCVALUE;

// for below cmds MDrv_CMD_MIU_Protect

typedef MS_U8 MIU_PROYECT_ID[16];

#ifdef BIFROST_32BIT_MODE
typedef struct DLL_PACKED _MIU_PROTECT
{
    MS_U8 u8Blockx;
    MS_U8 *pu8ProtectId;
    MS_U32 phy64Start;
    MS_U32 phy64End;
    MS_BOOL bSetFlag;
    MS_BOOL bRet;
}MIU_PROTECT, *PMIU_PROTECT;
#else
typedef struct DLL_PACKED _MIU_PROTECT
{
    MS_U8 u8Blockx;
    MS_U8 *pu8ProtectId;
    MS_PHY phy64Start;
    MS_PHY phy64End;
    MS_BOOL bSetFlag;
    MS_BOOL bRet;
}MIU_PROTECT, *PMIU_PROTECT;
#endif

// for below cmds MDrv_CMD_MIU_SelMIU
typedef struct _MIU_SELMIU
{
    eMIUClientID eClientID;
    eMIU_SelType eType;
    MS_BOOL bRet;
}MIU_SELMIU, *PMIU_SELMIU;

// for below cmds MDrv_CMD_MIU_GetProtectInfo
typedef struct DLL_PACKED _MIU_GETPROTECTINFO
{
    MS_U8 u8MiuDev;
    MIU_PortectInfo *pInfo;
    MS_BOOL bRet;
}MIU_GETPROTECTINFO, *PMIU_GETPROTECTINFO;

// for below cmds MDrv_CMD_MIU_SetGroupPriority
typedef struct _MIU_SETGROUPPRIORITY
{
    MS_U8 u8MiuDev;
    MIU_GroupPriority sPriority;
    MS_BOOL bRet;
}MIU_SETGROUPPRIORITY, *PMIU_SETGROUPPRIORITY;


// for below cmds MDrv_CMD_MIU_SetHighPriority
typedef struct _MIU_SETHIGHPRIORITY
{
    MS_U8 u8MiuDev;
    eMIUClientID eClientID;
    MS_BOOL bMask;
    MS_BOOL bRet;
}MIU_SETHIGHPRIORITY, *PMIU_SETHIGHPRIORITY;

// for below cmds MDrv_CMD_MIU_Dram_Size
typedef struct _MIU_DRAM_SIZE
{
    MS_U8 u8MiuDev;
    MS_U8 DramSize;
    MS_BOOL bRet;
}MIU_DRAM_SIZE, *PMIU_DRAM_SIZE;

// for below cmds MDrv_CMD_MIU_ProtectAlign
typedef struct DLL_PACKED _MIU_PROTECT_ALIGN
{
    MS_U32 *u32PageShift;
    MS_BOOL bRet;
}MIU_PROTECT_ALIGN, *PMIU_PROTECT_ALIGN;

#ifdef __cplusplus
}
#endif

#endif // _DRV_MIU_V2_H_

