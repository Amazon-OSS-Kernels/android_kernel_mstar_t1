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
/// @file   drvCPU_v2.h
/// @brief  MIU Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_CPU_V2_H_
#define _DRV_CPU_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure

typedef enum {
    MDrv_CMD_CPU_InitFront,
    MDrv_CMD_CPU_InitEnd,
    MDrv_CMD_CPU_Disable,
    MDrv_CMD_CPU_Enable,
    MDrv_CMD_CPU_SetHost,
    MDrv_CMD_CPU_GetInfo,
    MDrv_CMD_CPU_GetLibVer,
    MDrv_CMD_CPU_GetStatus,
    MDrv_CMD_CPU_SetDbgLevel,
    MDrv_CMD_CPU_GetBase,
    MDrv_CMD_CPU_QueryClock,
} eCPUIoctlOpt;

typedef enum
{
	CPU_MODULE_V1,
	CPU_MODULE_V2,
} eCPUModuleVersion;

//////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------
typedef struct _CPU_API_V1
{
}CPU_API_V1;

typedef struct _CPU_API_V2
{
}CPU_API_V2;

// below are for utopia20 cmd and parameters structure
// for cmd MDrv_COPRO_Init_End
typedef struct _CPU_INITEND_PARAM
{
    MS_U32 u32_ADR;
}CPU_INITEND_PARAM, *PCPU_INITEND_PARAM;

// for cmd MDrv_COPRO_Enable
typedef struct _CPU_ENABLE_PARAM
{
    MS_U32 u32_ADR;
}CPU_ENABLE_PARAM, *PCPU_ENABLE_PARAM;

// for cmd MDrv_SetBEON_Host
typedef struct _CPU_SETHOST_PARAM
{
    MS_BOOL bHost;
}CPU_SETHOST_PARAM, *PCPU_SETHOST_PARAM;

// for cmd MDrv_COPRO_GetLibVer
typedef struct _CPU_GETLIBVER_PARAM
{
    const MSIF_Version **ppVersion;
}CPU_GETLIBVER_PARAM, *PCPU_GETLIBVER_PARAM;

// for cmd MDrv_COPRO_GetStatus
typedef struct _CPU_GETSTATUS_PARAM
{
    COPRO_DrvStatus *pStatus;
}CPU_GETSTATUS_PARAM, *PCPU_GETSTATUS_PARAM;

// for cmd MDrv_COPRO_SetDbgLevel
typedef struct _CPU_SETDBGLEVEL_PARAM
{
    MS_U8 u8Level;
}CPU_SETDBGLEVEL_PARAM, *PCPU_SETDBGLEVEL_PARAM;

// for cmd MDrv_COPRO_GetInfo
typedef struct _CPU_GETINFO_PARAM
{
    MS_BOOL     bInit;
    MS_U32      u32Addr;
}CPU_GETINFO_PARAM, *PCPU_GETINFO_PARAM;

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

#endif // _DRV_CPU_V2_H_

