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

#ifndef _DRVSAR_PRIV_H_
#define _DRVSAR_PRIV_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvSAR.h"

#ifdef CONFIG_MSTAR_UTPA2K_SAR_ENABLE
typedef SAR_KpdResult (*IOCTL_SAR_Kpd_GetKeyCode)(MS_U8 *pu8Key, MS_U8 *pu8Repeat);
typedef SAR_KpdResult (*IOCTL_SAR_Kpd_Config)( SAR_KpdRegCfg* sarChInfo);

//SAR_Private
typedef struct _SAR_RESOURCE_PRIVATE
{
    MS_U32 SAR_Dummy;
}SAR_RESOURCE_PRIVATE;

typedef struct _SAR_RESOURCE_SHARED
{
    void* pSARResource;
}SAR_RESOURCE_SHARED;

typedef struct _SAR_INSTANT_PRIVATE
{
    IOCTL_SAR_Kpd_GetKeyCode fpSARKpdGetKeyCode;
    IOCTL_SAR_Kpd_Config  fpSARKpdConfig;
}SAR_INSTANT_PRIVATE;

void SARRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 SAROpen(void** pInstance, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 SARClose(void* pInstance);
MS_U32 SARIoctl(void* pInstance, MS_U32 u32Cmd, void *pArgs);
#endif

#ifdef __cplusplus
}
#endif
#endif // _DRVSAR_PRIV_H_
