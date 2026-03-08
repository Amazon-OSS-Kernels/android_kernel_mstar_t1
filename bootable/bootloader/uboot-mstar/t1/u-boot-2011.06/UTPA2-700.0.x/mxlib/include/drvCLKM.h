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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DRVCLKM_H_
#define _DRVCLKM_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvCLKM.h
/// @brief Byte CLK control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

// #define UART_TYPE_DEF
/// Version string.
#define CLKM_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    {'C','L','K','M'},                  /* IP__                                             */  \
    {'0','1'},                          /* 0.0 ~ Z.Z                                        */  \
    {'0','6'},                          /* 00 ~ 99                                          */  \
    {'0','0','3','1','0','0','5','0'},  /* CL#                                              */  \
    MSIF_OS



typedef enum _CLKM_DbgLvl
{
    E_CLKM_DBGLVL_NONE = 0,      /// no debug message
    E_CLKM_DBGLVL_WARNING,       /// show warning only
    E_CLKM_DBGLVL_ERROR,         /// show error only
    E_CLKM_DBGLVL_INFO,          /// show error & informaiton
    E_CLKM_DBGLVL_ALL,           /// show error, information & funciton name
}CLKM_DbgLvl;


typedef struct _CLKM_Info
{
    MS_BOOL     bInit;
    MS_S32      s32fd;
    MS_S32      s32Mutex;

}CLKM_Info;



typedef enum {

    E_MDRV_CMD_CLKM_Init,
    E_MDRV_CMD_CLKM_GET_HANDLE,
    E_MDRV_CMD_CLKM_CLK_GATE_DIABLE,
    E_MDRV_CMD_CLKM_SET_CLK_SOURCE,
    E_MDRV_CMD_CLKM_GET_SRAM_SD_INFO,


} eClkmIoctlOpt;

typedef enum _CLKM_Result
{
    E_CLKM_NOT_SUPPORT = -1
    ,E_CLKM_FAIL = 0
    ,E_CLKM_OK = 1
    ,E_CLKM_TIMEOUT
    ,E_CLKM_BUSY
}CLKM_Result;


typedef struct _CLKM_GetHandle_PARAM
{
    char *s8_Handle_Name;
    MS_S32 s32_Handle;

}CLKM_GetHandle_PARAM,*PCLKM_GetHandle_PARAM;


typedef struct _CLKM_CLK_GATE_DISABLE_PARAM
{

    MS_S32 s32_Handle;

}CLKM_CLK_GATE_DISABLE_PARAM,*PCLKM_CLK_GATE_DISABLE_PARAM;


typedef struct _CLKM_SET_CLK_SRC_PARAM
{
    MS_S32 s32_Handle;
    char *clk_src_name;

}CLKM_SET_CLK_SRC_PARAM,*PCLKM_SET_CLK_SRC_PARAM;


typedef struct _CLKM_GET_SRAM_SD_INFO_PARAM
{
    char *sram_sd_name;
    MS_U16 sram_sd_value;

}CLKM_GET_SRAM_SD_INFO_PARAM,*PCLKM_GET_SRAM_SD_INFO_PARAM;




MS_BOOL Drv_CLKM_Init(void);

void Drv_Clkm_Clk_Gate_Disable(MS_S32 s32Handle);

void Drv_Clkm_Set_Clk_Source(MS_S32 s32Handle,char *clk_src_name);

 MS_S32 Drv_Clkm_Get_Handle(char *name);

 MS_U16 Drv_Get_Sram_Sd_Info(char *name);

#ifdef __cplusplus
}
#endif
#endif // _DRVCLKM_H_
