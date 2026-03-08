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

#ifndef _DRVCPU_PRIV_H_
#define _DRVCPU_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvCPU.h
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
#include "drvCPU.h"

typedef void (*IOCTL_CPU_DISABLE)(void);
typedef void (*IOCTL_CPU_ENABLE)(MS_U32 u32_ADR);
typedef void (*IOCTL_CPU_SETHOST)(MS_BOOL bHost);
typedef void (*IOCTL_CPU_INITFRONT)(void);
typedef void (*IOCTL_CPU_INITEND)(MS_U32 u32_ADR);
typedef const COPRO_DrvInfo* (*IOCTL_CPU_GETINFO)(void);
typedef COPRO_Result (*IOCTL_CPU_GETLIBVER)(const MSIF_Version **ppVersion);
typedef COPRO_Result (*IOCTL_CPU_GETSTATUS)(COPRO_DrvStatus *pStatus);
typedef void (*IOCTL_CPU_SETDBGLEVEL)(MS_U8 u8Level);
typedef MS_U32 (*IOCTL_CPU_GETBASE)(void);
typedef MS_U32 (*IOCTL_CPU_QUERYCLOCK)(void);

//#################################
typedef struct _CPU_RESOURCE_PRIVATE
{
    MS_U32 Dummy;
}CPU_RESOURCE_PRIVATE;
//#################################

typedef struct _CPU_INSTANT_PRIVATE
{   
    IOCTL_CPU_DISABLE       fpCPUDisable;
    IOCTL_CPU_ENABLE        fpCPUEnable;
    IOCTL_CPU_SETHOST       fpCPUSetHost;
    IOCTL_CPU_INITFRONT     fpCPUInitFront;
    IOCTL_CPU_INITEND       fpCPUInitEnd;
    IOCTL_CPU_GETINFO       fpCPUGetInfo;
    IOCTL_CPU_GETLIBVER     fpCPUGetLibVer;
    IOCTL_CPU_GETSTATUS     fpCPUGetStatus;
    IOCTL_CPU_SETDBGLEVEL   fpCPUSetDbgLevel;
    IOCTL_CPU_GETBASE       fpCPUGetBase;
    IOCTL_CPU_QUERYCLOCK    fpCPUQueryClock;

}CPU_INSTANT_PRIVATE;

void CPURegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 CPUOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 CPUClose(void* pInstance);
MS_U32 CPUIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

void _MDrv_CPU_InitFront(void);
void _MDrv_CPU_InitEnd(MS_U32 u32_ADR);
void _MDrv_CPU_Disable(void);
void _MDrv_CPU_Enable(MS_U32 u32_ADR);
void _MDrv_CPU_SetHost(MS_BOOL bHost);
const COPRO_DrvInfo* _MDrv_CPU_GetInfo(void);
COPRO_Result _MDrv_CPU_GetLibVer(const MSIF_Version **ppVersion);
COPRO_Result _MDrv_CPU_GetStatus(COPRO_DrvStatus *pStatus);
void _MDrv_CPU_SetDbgLevel(MS_U8 u8Level);
MS_U32 _MDrv_CPU_GetBase(void);
MS_U32 _MDrv_CPU_QueryClock(void);


#ifdef __cplusplus
}
#endif
#endif // _DRVCPU_PRIV_H_
