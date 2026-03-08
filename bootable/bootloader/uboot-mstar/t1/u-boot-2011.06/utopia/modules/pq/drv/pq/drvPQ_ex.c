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
//==============================================================================
#ifdef STELLAR

#define _DRV_PQ_EX_C


#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#endif

#ifdef MSOS_TYPE_LINUX
#include <pthread.h>
#endif

#include "MsCommon.h"
//#include "MsVersion.h"
#include "MsOS.h"
#include "MsTypes.h"
#include "utopia.h"
#include "utopia_dapi.h"

#include "hwreg_utility2.h"
#include "color_reg.h"

#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "apiXC_v2.h"
#include "apiXC_Ace.h"

#include "drvPQ_Define.h"
#include "drvPQ_Declare.h"
#include "drvPQ.h"
#include "drvPQ_cus.h"
#include "drvPQ_Datatypes.h"
#include "mhal_pq.h"
#include "mhal_pq_cus.h"
#include "drvPQ_Bin.h"

#include "QualityMode.h"
#include "PQ_private.h"
#include "apiPQ_v2.h"
#include "mhal_pq_adaptive.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#ifndef UNUSED //to avoid compile warnings...
#define UNUSED(var) (void)((var) = (var))
#endif

#define PQ_EX_DBG(x)               (x)

//-------------------------------------------------------------------------------------------------
//  Local Variable
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Function
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PQ_EX_Ctrl_U2(void* pInstance,MS_U32 u32Cmd,void *pbuf,MS_U32 u32BufSize)
{
    if ((pbuf == NULL)  || (u32BufSize == 0))
    {
        printf(" %s %d: invalid input parameters of Cmd(%u). return  \n", __FUNCTION__,  __LINE__,u32Cmd);
        return FALSE;
    }
    switch(u32Cmd)
    {
        case E_PQ_CMD_EX_SET_ICC_REGION:
            PQ_EX_DBG(printf("\33[1;32m [PQ_EX] %s %d: E_PQ_CMD_EX_SET_ICC_REGION 	\33[m \n", __FUNCTION__,  __LINE__));
            Hal_PQ_set_sram_icc_crd_table(pInstance,SC_FILTER_SRAM_ICC_CRD, pbuf);
            break;
        case E_PQ_CMD_EX_SET_IHC_REGION:
            PQ_EX_DBG(printf("\33[1;32m [PQ_EX] %s %d: E_PQ_CMD_EX_SET_IHC_REGION 	\33[m \n", __FUNCTION__,  __LINE__));
            Hal_PQ_set_sram_ihc_crd_table(pInstance,SC_FILTER_SRAM_IHC_CRD, pbuf);
            break;
        case E_PQ_CMD_EX_DUMP_CUS_TABLE:
            PQ_EX_DBG(printf("\33[1;32m [PQ_EX] %s %d: E_PQ_CMD_EX_DUMP_CUS_TABLE 	\33[m \n", __FUNCTION__,  __LINE__));
            MDrv_PQ_DumpTable_(MAIN, pInstance, (EN_IP_Info* )pbuf);
            break;
        case E_PQ_CMD_EX_NUM:
        default:
            printf(" %s %d: invalid cmd id = %x  \n", __FUNCTION__,  __LINE__, u32Cmd);
            break;
    }

    return TRUE;
}


MS_BOOL MDrv_PQ_EX_Ctrl(MS_U32 u32Cmd,void *pbuf,MS_U32 u32BufSize)
{
    if (pu32PQInst == NULL)
    {
        printf("[%s,%5d]No instance existed, please get an instance by calling MDrv_PQ_Init() first\n",__FUNCTION__,__LINE__);
        return FALSE;
    }

    stPQ_CMD_EX_CTRL PQArgs;
    PQArgs.u32Cmd = u32Cmd;
    PQArgs.pbuf = pbuf;
    PQArgs.u32BufSize = u32BufSize;
    PQArgs.bReturn = FALSE;

    if(UtopiaIoctl(pu32PQInst, E_PQ_CMD_EX_CTRL, (void*)&PQArgs) != UTOPIA_STATUS_SUCCESS)
    {
        printf("PQ engine GET_CONFIG Ioctl fail\n");
        return E_PQ_RET_FAIL;
    }
    else
    {
        return PQArgs.bReturn;
    }
}


#endif

//--------------------------------------------------------------
