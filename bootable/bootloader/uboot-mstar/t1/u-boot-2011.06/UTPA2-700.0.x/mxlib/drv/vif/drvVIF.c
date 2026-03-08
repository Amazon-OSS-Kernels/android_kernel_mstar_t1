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
#ifndef _DRVVIF_C_
#define _DRVVIF_C_

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif
//#include "MsVersion.h"
#include "MsCommon.h"
//#include "halVIF.h"
//#include "halVIF_Customer.h"
//#include "regVIF.h"
#include "drvVIF.h"
#include "drvVIF_v2.h"
#include "../../utopia_core/utopia.h"

//-------------------------------------------------------------------------------------------------
//  Global variables
//-------------------------------------------------------------------------------------------------

void* ppVIFInstant = NULL;
static MS_U32 u32VIFopen = 0;

//-------------------------------------------------------------------------------------------------
//  Global VIF functions
//-------------------------------------------------------------------------------------------------

void DRV_VIF_WriteByte(U32 u32Reg, U8 u8Val )
{
    VIF_SETREGVALUE SetRegInfo = {0};

    SetRegInfo.u32Addr = u32Reg;
    SetRegInfo.u8Value = u8Val;

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_WriteByte, &SetRegInfo);
    else
        printf("\r\n ======== DRV_VIF_WriteByte/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

U8 DRV_VIF_ReadByte(U32 u32Reg )
{
    VIF_SETREGVALUE SetRegInfo = {0};

    SetRegInfo.u32Addr = u32Reg;
    SetRegInfo.u8Value = 0;

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_ReadByte, &SetRegInfo);
        return SetRegInfo.u8Value;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_ReadByte/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return 0;
    }
}

void DRV_VIF_Version(void)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Version, NULL);
    else
        printf("\r\n ======== DRV_VIF_Version/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

U8 DRV_VIF_Read_CR_FOE(void)
{
    VIF_COPYTOUSER CopyToUser = {0};

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Read_CR_FOE, &CopyToUser);
        return CopyToUser.u8Value;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_Read_CR_FOE/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return 0;
    }
}

U8 DRV_VIF_Read_CR_LOCK_STATUS(void)
{
    VIF_COPYTOUSER CopyToUser = {0};

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Read_CR_LOCK_STATUS, &CopyToUser);
        return CopyToUser.u8Value;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_Read_CR_LOCK_STATUS/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return 0;
    }
}

void DRV_VIF_SetClock(BOOL bEnable)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetClock, &bEnable);
    else
        printf("\r\n ======== DRV_VIF_SetClock/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen)
{
    eVIF_INIT _eVIF_Init = {0};

    _eVIF_Init.pVIF_InitData = pVIF_InitData;
    _eVIF_Init.u32InitDataLen = u32InitDataLen;

    //printf("\r\n ========= DRV_VIF_Init =========== \n");

    void* pAttribte = NULL;
    if(u32VIFopen == 0)  // First time open
    {
        //if(UtopiaOpen(MODULE_VIF |KERNEL_MODE, &ppVIFInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
        if(UtopiaOpen(MODULE_VIF, &ppVIFInstant, 0, pAttribte) == UTOPIA_STATUS_SUCCESS)
        {
            u32VIFopen = 1;

            UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Init, &_eVIF_Init);

            //printf("\r\n ======== DRV_VIF_Init/VIF Open Successful %x =========", (WORD)u32VIFopen);
        }
        else
            printf("\r\n ======== DRV_VIF_Init/VIF Open Fail %x =========", (WORD)u32VIFopen);
    }
    else
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Init, &_eVIF_Init);
}

void DRV_VIF_Exit(void)
{
    //printf("\r\n ========== DRV_VIF_Exit ========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Exit, NULL);
    else
        printf("\r\n ======== DRV_VIF_Exit/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_Reset(void)
{
    //printf("\r\n ========== DRV_VIF_Reset ========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Reset, NULL);
    else
        printf("\r\n ======== DRV_VIF_Reset/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_Handler(BOOL bAutoScan)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_Handler, &bAutoScan);
    else
        printf("\r\n ======== DRV_VIF_Handler/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem)
{
    //printf("\r\n ========= DRV_VIF_SetSoundSystem ========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetSoundSystem, &ucSoundSystem);
    else
        printf("\r\n ======== DRV_VIF_SetSoundSystem/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_SetIfFreq(IfFrequencyType u16IfFreq)
{
    //printf("\r\n ========= DRV_VIF_SetIfFreq =========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetIfFreq, &u16IfFreq);
    else
        printf("\r\n ======== DRV_VIF_SetIfFreq/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_BypassDBBAudioFilter(BOOL bEnable)
{
    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_BypassDBBAudioFilter, &bEnable);
    else
        printf("\r\n ======== DRV_VIF_BypassDBBAudioFilter/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

void DRV_VIF_SetFreqBand(FrequencyBand u8FreqBand)
{
    //printf("\r\n ========= DRV_VIF_SetFreqBand =========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetFreqBand, &u8FreqBand);
    else
        printf("\r\n ======== DRV_VIF_SetFreqBand/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

BOOL DRV_VIF_GetInputLevelIndicator(void)
{
    VIF_COPYTOUSER CopyToUser = {0};

    //printf("\r\n ========= DRV_VIF_GetInputLevelIndicator =========== \n");

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_GetInputLevelIndicator, &CopyToUser);
        return CopyToUser.bEnable;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_GetInputLevelIndicator/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return FALSE;
    }
}

BOOL DRV_VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen)
{
    //printf("\r\n =========== DRV_VIF_SetParameter =========== \n");

    VIF_SETPARA SetParaInfo = {0};

    SetParaInfo.ParaGroup = paraGroup;
    SetParaInfo.pVIF_Para = pVIF_Para;
    SetParaInfo.u32DataLen = u32DataLen;
    SetParaInfo.bRet = 0;

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetParameter, &SetParaInfo);
        return SetParaInfo.bRet;
    }
    else
    {
        printf("\r\n ======== DRV_VIF_SetParameter/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return FALSE;
    }
}

void DRV_VIF_ShiftClk(BYTE VifShiftClk)
{
     //printf("\r\n =========== DRV_VIF_ShiftClk =========== \n");

    if(u32VIFopen == 1)
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_ShiftClk, &VifShiftClk);
    else
        printf("\r\n ======== DRV_VIF_ShiftClk/VIF Open Fail %x =========", (WORD)u32VIFopen);
}

MS_U32 MDrv_VIF_SetPowerState(EN_POWER_MODE u16PowerState)
{
    VIF_COPYTOUSER CopyToUser = {0};
    MS_U32 u32Ret = 0;

    CopyToUser.u16PowerState = u16PowerState;
    CopyToUser.u32Value = 0;

    if(u32VIFopen == 1)
    {
        UtopiaIoctl(ppVIFInstant, Drv_CMD_VIF_SetPowerState, &CopyToUser);

        u32Ret = CopyToUser.u32Value;

        if(u32Ret == 0)
            return UTOPIA_STATUS_SUCCESS;
        else
            return UTOPIA_STATUS_FAIL;
    }
    else
    {
        printf("\r\n ======== MDrv_VIF_SetPowerState/VIF Open Fail %x =========", (WORD)u32VIFopen);
        return UTOPIA_STATUS_FAIL;
    }
}

#endif //_DRVVIF_C_

