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

////////////////////////////////////////////////////////////////////////////////
//  [Doxygen]
/// file apiGOP_SC.c
/// This file includes MStar application interface for Graphic output path
/// @brief API for Graphics output path (GOP)
//
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif

#include "MsTypes.h"
#include "MsCommon.h"
#include "MsVersion.h"
#include "apiGOP_SC.h"
#include "drv_gop_sc.h"
#include "drvMMIO.h"

static MS_BOOL gfSkipInit = FALSE;   // Flag for: If the system needs to show BootLogo,Skip reset flow to avoid garbage.

MS_BOOL MApi_GOP_SC_Init(void)
{
    MS_VIRT vGOPSCRiuBaseAddr;
    MS_U32  u32NonPMBankSize;

    // get MMIO base
    if(MDrv_MMIO_GetBASE( &vGOPSCRiuBaseAddr, &u32NonPMBankSize, MS_MODULE_PM ) != TRUE)
    {
        printf("GOP_SC GetBase failed\n");
        return FALSE;
    }

    MDrv_GOP_SC_Init_riu_base(vGOPSCRiuBaseAddr);

    if(!gfSkipInit)
    {
        MDrv_GOP_SC_Init();
    }

    return TRUE;
}

void MApi_GOP_SC_SkipInit(MS_BOOL bEnable)
{
    gfSkipInit = bEnable;
    return;
}

MS_BOOL MApi_GOP_SC_SetCfg(MAPI_GOP_SC_INFO *pGopSC_Info)
{
    DRV_GOP_SC_InCfg stGopSC_InCfg;
    DRV_GOP_SC_OutCfg stGopSC_OutCfg;
    memset(&stGopSC_InCfg,0,sizeof(DRV_GOP_SC_InCfg));
    memset(&stGopSC_OutCfg,0,sizeof(DRV_GOP_SC_OutCfg));
    memcpy(&stGopSC_InCfg,&pGopSC_Info->stInputCfg,sizeof(MAPI_GOP_SC_InCfg));

    switch(pGopSC_Info->u8TVSys)
    {
        case EN_GOP_SC_NTSC:
        case EN_GOP_SC_PAL_M:
            stGopSC_OutCfg.u16VttOut = 525;
            stGopSC_OutCfg.u16HttOut = 858;
            stGopSC_OutCfg.u16VdeOut = 480;
            stGopSC_OutCfg.u16HdeOut = 720;
            break;
        case EN_GOP_SC_PAL:
            stGopSC_OutCfg.u16VttOut = 625;
            stGopSC_OutCfg.u16HttOut = 864;
            stGopSC_OutCfg.u16VdeOut = 576;
            stGopSC_OutCfg.u16HdeOut = 720;
            break;
        default:    //Default = PAL
            stGopSC_OutCfg.u16VttOut = 625;
            stGopSC_OutCfg.u16HttOut = 864;
            stGopSC_OutCfg.u16VdeOut = 576;
            stGopSC_OutCfg.u16HdeOut = 720;
            break;
    }

    stGopSC_OutCfg.bInterlace = TRUE;   //U4 only support i out
    MDrv_GOP_SC_SetParams(&stGopSC_InCfg,&stGopSC_OutCfg,(MS_U8)pGopSC_Info->u8TVSys);

    MDrv_GOP_SC_SetDst((DRV_GOP_SC_Dst)pGopSC_Info->stDst);
    MDrv_GOP_SC_SetCfg();

    if(pGopSC_Info->stDst == EN_GOP_SC_DST_FBL)
    {
        MDrv_GOP_SC_SetLock();
    }

    MDrv_GOP_SC_MuxSel(pGopSC_Info->enSrcSel);

    return TRUE;
}

MS_BOOL MApi_GOP_SC_MuxSel(EN_GOP_SC_MUX_SEL u8Sel)
{
    MDrv_GOP_SC_MuxSel(u8Sel);
    return TRUE;
}

MS_BOOL MApi_GOP_SC_SetFPLL_Enable(MS_BOOL bEnable)
{
    MDrv_GOP_SC_SetFPLL_Enable(bEnable);
    return TRUE;
}

