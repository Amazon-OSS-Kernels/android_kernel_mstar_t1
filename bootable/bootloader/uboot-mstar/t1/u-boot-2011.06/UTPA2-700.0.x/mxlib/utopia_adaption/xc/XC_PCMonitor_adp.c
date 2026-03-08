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
/// file    XC_PCMONITOR_adp.c
/// @brief  Scaler API layer Interface.
////////////////////////////////////////////////////////////////////////////////

#ifndef _XC_PCMONITOR_ADP_C_
#define _XC_PCMONITOR_ADP_C_

//------------------------------------------------------------------------------
//  Include Files
//------------------------------------------------------------------------------
// Common Definition
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/string.h>
#include <linux/slab.h>
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_adp.h"
#include "MsOS.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_PCMonitor.h"
#include "apiXC_PCMonitor_v2.h"
#include "apiXC_v2.h"
#include "XC_PCMonitor_adp.h"
#include "XC_adp.h"

//------------------------------------------------------------------------------
//  Driver Compiler Options
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Defines
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Structurs
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Variables
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Variables
//------------------------------------------------------------------------------

//Function parameter
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_INIT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_RESTART[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_SETTIMINGCOUNT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GETCURRENTSTATE[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GETSYNCSTATUS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_HFREQX10[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_HFREQX1K[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_VFREQX10[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_VFREQX1K[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_VTOTAL[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_SYNCLOSS[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_INVALIDTIMINGDETECT[5];
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_SETTIMINGCOUNTEX[5];

//Function parameter's pointer
UADP_STRUCT_POINTER_TABLE spt_XC_PCMONITOR_MS_WINDOW_TYPE[5];

//------------------------------------------------------------------------------
//  Debug Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Local Functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//  Global Functions
//------------------------------------------------------------------------------
MS_U32 XC_PCMONITOR_adp_Init(void)
{

//set table
    MS_U32 temp=0;

    //Function parameter
    //pstXC_PCMONITOR_INIT                  pXC_PCMONITOR_INIT                 = (pstXC_PCMONITOR_INIT)temp;
    //pstXC_PCMONITOR_RESTART               pXC_PCMONITOR_RESTART              = (pstXC_PCMONITOR_RESTART)temp;
    //pstXC_PCMONITOR_SETTIMINGCOUNT        pXC_PCMONITOR_SETTIMINGCOUNT       = (pstXC_PCMONITOR_SETTIMINGCOUNT)temp;
    //pstXC_PCMONITOR                       pXC_PCMONITOR                      = (pstXC_PCMONITOR)temp;
    //pstXC_PCMONITOR_GETCURRENTSTATE       pXC_PCMONITOR_GETCURRENTSTATE      = (pstXC_PCMONITOR_GETCURRENTSTATE)temp;
    //pstXC_PCMONITOR_GETSYNCSTATUS         pXC_PCMONITOR_GETSYNCSTATUS        = (pstXC_PCMONITOR_GETSYNCSTATUS)temp;
    //pstXC_PCMONITOR_GET_HFREQX10          pXC_PCMONITOR_GET_HFREQX10         = (pstXC_PCMONITOR_GET_HFREQX10)temp;
    //pstXC_PCMONITOR_GET_HFREQX1K          pXC_PCMONITOR_GET_HFREQX1K         = (pstXC_PCMONITOR_GET_HFREQX1K)temp;
    //pstXC_PCMONITOR_GET_VFREQX10          pXC_PCMONITOR_GET_VFREQX10         = (pstXC_PCMONITOR_GET_VFREQX10)temp;
    //pstXC_PCMONITOR_GET_VFREQX1K          pXC_PCMONITOR_GET_VFREQX1K         = (pstXC_PCMONITOR_GET_VFREQX1K)temp;
    //pstXC_PCMONITOR_GET_VTOTAL            pXC_PCMONITOR_GET_VTOTAL           = (pstXC_PCMONITOR_GET_VTOTAL)temp;
    pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO  pXC_PCMONITOR_GET_DVI_HDMI_DE_INFO = (pstXC_PCMONITOR_GET_DVI_HDMI_DE_INFO)temp;
    //pstXC_PCMONITOR_SYNCLOSS              pXC_PCMONITOR_SYNCLOSS             = (pstXC_PCMONITOR_SYNCLOSS)temp;
    //pstXC_PCMONITOR_INVALIDTIMINGDETECT   pXC_PCMONITOR_INVALIDTIMINGDETECT  = (pstXC_PCMONITOR_INVALIDTIMINGDETECT)temp;
    //pstXC_PCMONITOR_SETTIMINGCOUNTEX      pXC_PCMONITOR_SETTIMINGCOUNTEX     = (pstXC_PCMONITOR_SETTIMINGCOUNTEX)temp;


	//Function parameter's pointer
    //MS_WINDOW_TYPE*        pXC_PCMOTITOR_msWin     = (MS_WINDOW_TYPE*)temp;

    //Function parameter's pointer
    UADPBypassSetSPT(&spt_XC_PCMONITOR_MS_WINDOW_TYPE[0],UADP_SPT_SELF_SIZE,0, sizeof(MS_WINDOW_TYPE));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_MS_WINDOW_TYPE[1],UADP_SPT_END , 0, 0);

    //Function parameter
    UADPBypassSetSPT(&spt_XC_PCMONITOR_INIT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_INIT));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_INIT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_RESTART[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_RESTART));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_RESTART[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_SETTIMINGCOUNT));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR));
    UADPBypassSetSPT(&spt_XC_PCMONITOR[1],UADP_SPT_END , 0, 0);

	UADPBypassSetSPT(&spt_XC_PCMONITOR_GETCURRENTSTATE[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GETCURRENTSTATE));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GETCURRENTSTATE[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GETSYNCSTATUS[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GETSYNCSTATUS));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GETSYNCSTATUS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX10[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_HFREQX10));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX10[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX1K[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_HFREQX1K));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_HFREQX1K[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX10[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_VFREQX10));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX10[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX1K[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_VFREQX1K));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VFREQX1K[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VTOTAL[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_VTOTAL));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_VTOTAL[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_GET_DVI_HDMI_DE_INFO));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[1],UADP_SPT_POINTER_TO_NEXT,
        ((unsigned long)(&(pXC_PCMONITOR_GET_DVI_HDMI_DE_INFO->msWin)) - (unsigned long)pXC_PCMONITOR_GET_DVI_HDMI_DE_INFO), (unsigned long)spt_XC_PCMONITOR_MS_WINDOW_TYPE);
    UADPBypassSetSPT(&spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO[2],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_SYNCLOSS[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_SYNCLOSS));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_SYNCLOSS[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_INVALIDTIMINGDETECT[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_INVALIDTIMINGDETECT));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_INVALIDTIMINGDETECT[1],UADP_SPT_END , 0, 0);

    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNTEX[0],UADP_SPT_SELF_SIZE,0, sizeof(stXC_PCMONITOR_SETTIMINGCOUNTEX));
    UADPBypassSetSPT(&spt_XC_PCMONITOR_SETTIMINGCOUNTEX[1],UADP_SPT_END , 0, 0);


    return 0;

}

MS_U32 XC_PCMONITOR_adp_Ioctl(void* pInstanceTmp, MS_U32 u32Cmd, void* const pArgs)
{
    MS_U32 u32Ret;
    char buffer_arg[2];

    switch(u32Cmd)
    {

        case E_XC_PCMONITOR_CMD_INIT :
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_INIT, spt_XC_PCMONITOR_INIT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_RESTART:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_RESTART, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_SETTIMINGCOUNT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_SETTIMINGCOUNT, NULL,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR, spt_XC_PCMONITOR,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GETCURRENTSTATE:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GETCURRENTSTATE, spt_XC_PCMONITOR_GETCURRENTSTATE,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GETSYNCSTATUS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GETSYNCSTATUS,spt_XC_PCMONITOR_GETSYNCSTATUS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_HFREQX10:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_HFREQX10,spt_XC_PCMONITOR_GET_HFREQX10,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_HFREQX1K:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_HFREQX1K,spt_XC_PCMONITOR_GET_HFREQX1K,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_VFREQX10:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_VFREQX10,spt_XC_PCMONITOR_GET_VFREQX10,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_VFREQX1K:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_VFREQX1K,spt_XC_PCMONITOR_GET_VFREQX1K,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_GET_VTOTAL:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_VTOTAL,spt_XC_PCMONITOR_GET_VTOTAL,buffer_arg,sizeof(buffer_arg));
            break;
        }

		case E_XC_PCMONITOR_CMD_GET_DVI_HDMI_DE_INFO:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO,spt_XC_PCMONITOR_GET_DVI_HDMI_DE_INFO,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_SYNCLOSS:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_SYNCLOSS,spt_XC_PCMONITOR_SYNCLOSS,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_INVALIDTIMINGDETECT:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_INVALIDTIMINGDETECT,spt_XC_PCMONITOR_INVALIDTIMINGDETECT,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_SETTIMINGCOUNTEX:
        {
            u32Ret = UADPBypassIoctl(pInstanceTmp,u32Cmd,pArgs,spt_XC_PCMONITOR_SETTIMINGCOUNTEX,spt_XC_PCMONITOR_SETTIMINGCOUNTEX,buffer_arg,sizeof(buffer_arg));
            break;
        }

        case E_XC_PCMONITOR_CMD_NONE:
        case E_XC_PCMONITOR_CMD_MAX:
        default:
        {
            printf("Command %ld is not existed\n",u32Cmd);
            u32Ret = UTOPIA_STATUS_ERR_INVALID_HANDLE;
            break;
        }

    }

	return u32Ret;
   // return UtopiaIoctl(pModuleDDI->pInstant,u32Cmd,arg);
}

#endif


