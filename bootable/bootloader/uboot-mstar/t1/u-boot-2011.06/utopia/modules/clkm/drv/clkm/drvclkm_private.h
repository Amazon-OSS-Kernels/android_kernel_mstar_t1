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

#ifndef _DRVCLKM_PRIV_H_
#define _DRVCLKM_PRIV_H_

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
#include "drvCLKM.h"

typedef MS_S32 (*IOCTL_CLKM_GET_HANDLE)(char *);
typedef MS_BOOL (*IOCTL_CLKM_SET_CLK_SOURCE)(MS_S32,char *);
typedef void (*IOCTL_CLKM_CLK_GATE_DISABLE)(MS_S32);
typedef MS_U16 (*IOCTL_CLKM_GET_SRAM_SD_Info)(char *name);


typedef struct _CLKM_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}CLKM_RESOURCE_PRIVATE;

typedef struct _CLKM_INSTANT_PRIVATE
{
    IOCTL_CLKM_GET_HANDLE           fpCLKMGetHandle;
    IOCTL_CLKM_SET_CLK_SOURCE         fpCLKMSetClkSource;
    IOCTL_CLKM_CLK_GATE_DISABLE fpCLKMClkGateDisable;
    IOCTL_CLKM_GET_SRAM_SD_Info fpCLKMGetSramSdInfo;
  
}CLKM_INSTANT_PRIVATE;




void CLKMRegisterToUtopia(FUtopiaOpen ModuleType);

MS_U32 CLKMOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);

MS_U32 CLKMClose(void* pInstance);

MS_U32 CLKMIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);
 
MS_S32 _MDrv_SET_CLK_SEL(MS_S32 handle,char *clk_src_name);

MS_S16 _Drv_Get_Src_Name_Offset(clock_table *table , char *src_name);

#ifdef __cplusplus
}
#endif
#endif // _DRVCLKM_PRIV_H_
