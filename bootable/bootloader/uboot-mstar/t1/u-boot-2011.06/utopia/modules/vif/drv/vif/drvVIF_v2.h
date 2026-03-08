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


#ifndef _DRVVIF_V2_H_
#define _DRVVIF_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "drvVIF.h"

//****************************************************************************
// Public attributes.
//****************************************************************************

#ifndef BOOL
#define BOOL MS_BOOL
#endif
#ifndef BYTE
#define BYTE MS_U8
#endif
#ifndef WORD
#define WORD MS_U16
#endif
#ifndef DWORD
#define DWORD MS_U32
#endif
#ifndef U8
#define U8 MS_U8
#endif
#ifndef U16
#define U16 MS_U16
#endif
#ifndef U32
#define U32 MS_U32
#endif
#ifndef VIRT
#define VIRT MS_VIRT
#endif

typedef enum 
{
    Drv_CMD_VIF_Version,
    Drv_CMD_VIF_SetClock,
    Drv_CMD_VIF_Init,
    Drv_CMD_VIF_Exit,
    Drv_CMD_VIF_Reset,
    Drv_CMD_VIF_Handler,
    Drv_CMD_VIF_SetSoundSystem,
    Drv_CMD_VIF_SetIfFreq,
    Drv_CMD_VIF_Read_CR_FOE,
    Drv_CMD_VIF_Read_CR_LOCK_STATUS,
    Drv_CMD_VIF_BypassDBBAudioFilter,
    Drv_CMD_VIF_SetFreqBand,

    Drv_CMD_VIF_GetInputLevelIndicator,
    Drv_CMD_VIF_SetParameter,
    Drv_CMD_VIF_ShiftClk,
    Drv_CMD_VIF_SetPowerState,
    Drv_CMD_VIF_WriteByte,
    Drv_CMD_VIF_ReadByte,
    
} eVifIoctlOpt;

typedef struct DLL_PACKED _VIF_INIT
{
    VIFInitialIn *pVIF_InitData;
    DWORD u32InitDataLen;
}eVIF_INIT, *PVIF_INIT;

typedef struct DLL_PACKED _VIF_SETREGVALUE
{
    U32 u32Addr;
    U8 u8Value;
}VIF_SETREGVALUE, *PVIF_SETREGVALUE;

typedef struct DLL_PACKED _VIF_SET_PARA
{
    BOOL bRet;
    VIF_PARA_GROUP ParaGroup;
    void *pVIF_Para;
    DWORD u32DataLen;
}VIF_SETPARA, *PVIF_SETPARA;

typedef struct DLL_PACKED _VIF_COPYTOUSER
{
    BOOL bEnable;
    BYTE u8Value;
    WORD u16Value;
    DWORD u32Value;
    EN_POWER_MODE u16PowerState;
}VIF_COPYTOUSER, *PVIF_COPYTOUSER;


#ifdef __cplusplus
}
#endif

#endif //_DRVVIF_V2_H_

