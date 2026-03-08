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

#ifndef _DRV_SAR_V2_H_
#define _DRV_SAR_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure
#ifdef CONFIG_MSTAR_UTPA2K_SAR_ENABLE
typedef enum {
    MDrv_CMD_Kpd_GetKeyCode,
    MDrv_CMD_SAR_Config,
} eSARIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct _SAR_PRIVATE_PARAM_Kpd_GetKeyCode
{
   MS_U8 u8Key;
   MS_U8 u8Repeat;
   MS_U32 u32Retn;
} SAR_PRIVATE_PARAM_Kpd_GetKeyCode, *PSAR_PRIVATE_PARAM_Kpd_GetKeyCode ;

typedef struct  _SAR_PRIVATE_PARAM_KeyConfig
{
   SAR_KpdRegCfg *pKeyRegCfg;
} SAR_PRIVATE_PARAM_KeyConfig, *PSAR_PRIVATE_PARAM_KeyConfig;

typedef union _SAR_PRIVATE_PARAM
{
    SAR_PRIVATE_PARAM_Kpd_GetKeyCode  privateKpd_GetKeyCode;
    SAR_PRIVATE_PARAM_KeyConfig  privateKpd_Config;
}SAR_PRIVATE_PARAM;
#endif

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//For Utopia 2K
SAR_Result MDrv_SAR_GetKeyCode_U2K(MS_U8 *pu8Key, MS_U8 *pu8Repeat);
SAR_KpdResult MDrv_SAR_Kpd_GetKeyCode_U2K(MS_U8 *pu8Key, MS_U8 *pu8Repeat);
SAR_KpdResult MDrv_SAR_Kpd_SetChInfo_U2K(SAR_KpdRegCfg *sarChInfo);

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_SAR_V2_H_

