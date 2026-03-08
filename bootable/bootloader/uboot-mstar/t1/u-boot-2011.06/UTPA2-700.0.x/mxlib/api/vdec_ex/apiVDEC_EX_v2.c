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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    apiVDEC_EX_v2.c
/// @brief  VDEC EXTENSION API FOR DUAL STREAMS
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "utopia_dapi.h"
#include "vdec_ex_private.h"
//#include "apiVDEC_EX_v2.h"
#include "utopia.h"

#ifndef ANDROID
#define VPRINTF printf
#else
#include <sys/mman.h>
#include <cutils/ashmem.h>
#include <cutils/log.h>

#define VPRINTF ALOGD
#endif

//-------------------------------------------------------------------------------------------------
//  Global Variable
//-------------------------------------------------------------------------------------------------
void* pModuleVDEC_EX_V2 = NULL;
char  ResourceGroup[E_VDEC_EX_POOL_ID_MAX][20] = {{"VDEC_EX_R0"},};

//-------------------------------------------------------------------------------------------------
//  API Function
//-------------------------------------------------------------------------------------------------
void VDEC_EXRegisterToUtopia(FUtopiaOpen ModuleType)
{
    MS_U32 i = 0, j = 0;
    void* psResource = NULL;

    // 1. deal with module
    UtopiaModuleCreate(MODULE_VDEC_EX, 0, &pModuleVDEC_EX_V2);
    UtopiaModuleRegister(pModuleVDEC_EX_V2);
    UtopiaModuleSetupFunctionPtr(pModuleVDEC_EX_V2,
                                (FUtopiaOpen)VDEC_EX_V2_Open,
                                (FUtopiaClose)VDEC_EX_V2_Close,
                                (FUtopiaIOctl)VDEC_EX_V2_IOctl);

    // 2. deal with resource
    for(i=E_VDEC_EX_POOL_ID_INTERNAL;i<E_VDEC_EX_POOL_ID_MAX;i++) // for each resource group in different memory pool
    {
        UtopiaModuleAddResourceStart(pModuleVDEC_EX_V2,i);
        for(j=0;j<1;j++) // for each resource group in same memory pool
        {
            UtopiaResourceCreate(ResourceGroup[i],sizeof(VDEC_EX_RESOURCE_PRIVATE),&psResource);
            UtopiaResourceRegister(pModuleVDEC_EX_V2,psResource,i);
        }
        UtopiaModuleAddResourceEnd(pModuleVDEC_EX_V2,i);
    }
}

MS_U32 VDEC_EX_V2_Open(void** ppInstance, const void* const pAttribute)
{
    void* pResource = NULL;
    VDEC_EX_RESOURCE_PRIVATE* pVDEC_EX_ResourcePrivate = NULL;
    VDEC_EX_INSTANCE_PRIVATE* psVDEC_EX_InstPri = NULL;
    //MS_U8 i = 0;

    // Check there is available decoder to be use
    if(UtopiaResourceObtain(pModuleVDEC_EX_V2, E_VDEC_EX_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }


    UtopiaResourceGetPrivate(pResource,(void*)(&pVDEC_EX_ResourcePrivate));

#if 0
    for(i=0;i<SUPPORT_DECODER_NUM_MAX;i++)
    {
        if(pVDEC_EX_ResourcePrivate->u8DecoderInUse[i] == 0)
        {
            pVDEC_EX_ResourcePrivate->u8DecoderInUse[i] = 1;
            break;
        }
    }

    if(i == SUPPORT_DECODER_NUM_MAX)
    {
        VPRINTF("No decoder can be used,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }
#endif
    UtopiaResourceRelease(pResource);


    //Create instance
    UtopiaInstanceCreate(sizeof(VDEC_EX_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psVDEC_EX_InstPri);

    //Recode decoder index
    //psVDEC_EX_InstPri->Index_of_decoder = i;

    //Link Function pointer
    psVDEC_EX_InstPri->fpVDEC_EX_GetFreeStream  = MApi_VDEC_EX_V2_GetFreeStream;
    psVDEC_EX_InstPri->fpVDEC_EX_Init           = MApi_VDEC_EX_V2_Init;
    psVDEC_EX_InstPri->fpVDEC_EX_SetControl     = MApi_VDEC_EX_V2_SetControl;
    psVDEC_EX_InstPri->fpVDEC_EX_GetControl     = MApi_VDEC_EX_V2_GetControl;
    psVDEC_EX_InstPri->fpVDEC_EX_PreSetControl  = MApi_VDEC_EX_V2_PreSetControl;
    psVDEC_EX_InstPri->fpVDEC_EX_PostSetControl = MApi_VDEC_EX_V2_PostSetControl;
    return UTOPIA_STATUS_SUCCESS;
}


MS_U32 VDEC_EX_V2_IOctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
    VDEC_EX_INSTANCE_PRIVATE* psVDEC_EX_InstPri = NULL;
    if(pArgs == NULL)
    {
        VPRINTF("[Error]%s,pArgs is NULL\n",__FUNCTION__);
        return UTOPIA_STATUS_FAIL;
    }
    UtopiaInstanceGetPrivate(pInstance, (void*)&psVDEC_EX_InstPri);
    E_VDEC_EX_V2_IOCTL_CMD IOcmd = (E_VDEC_EX_V2_IOCTL_CMD)u32Cmd;
    MS_U32 ret = UTOPIA_STATUS_SUCCESS;
    VDEC_EX_V2_IO_Param* pLocal_Args = (VDEC_EX_V2_IO_Param*)pArgs;
    VDEC_EX_Result* pRet = (VDEC_EX_Result*)(pLocal_Args->pRet);
    *pRet = E_VDEC_EX_FAIL;
    switch(IOcmd)
    {
        case E_VDEC_EX_V2_CMD_GET_FREE_STREAM_ID:
            *pRet = psVDEC_EX_InstPri->fpVDEC_EX_GetFreeStream(pLocal_Args->param[0],
                                                               *((MS_U32*)(pLocal_Args->param[1])),
                                                               *((VDEC_EX_Stream*)(pLocal_Args->param[2])),
                                                               *((VDEC_EX_CodecType*)(pLocal_Args->param[3])));
            break;
        case E_VDEC_EX_V2_CMD_INIT:
            *pRet = psVDEC_EX_InstPri->fpVDEC_EX_Init((VDEC_StreamId*)pLocal_Args->StreamID,(VDEC_EX_InitParam*)(pLocal_Args->param[0]));
            break;
        case E_VDEC_EX_V2_CMD_SET_CONTROL:
            *pRet = psVDEC_EX_InstPri->fpVDEC_EX_SetControl((VDEC_StreamId*)pLocal_Args->StreamID,
                                                            pLocal_Args->eUserCmd,
                                                            pLocal_Args->param,
                                                            FALSE);
            break;
        case E_VDEC_EX_V2_CMD_GET_CONTROL:
            *pRet = psVDEC_EX_InstPri->fpVDEC_EX_GetControl((VDEC_StreamId*)pLocal_Args->StreamID,
                                                            pLocal_Args->eUserCmd,
                                                            pLocal_Args->param);
            break;
        case E_VDEC_EX_V2_CMD_PRE_SET_CONTROL:
            *pRet = psVDEC_EX_InstPri->fpVDEC_EX_PreSetControl((VDEC_StreamId*)pLocal_Args->StreamID,
                                                               pLocal_Args->eUserCmd,
                                                               pLocal_Args->param);
            break;
        case E_VDEC_EX_V2_CMD_POST_SET_CONTROL:
            *pRet = psVDEC_EX_InstPri->fpVDEC_EX_PostSetControl((VDEC_StreamId*)pLocal_Args->StreamID,
                                                                pLocal_Args->eUserCmd,
                                                                pLocal_Args->param);
            break;
        default:
            ret = UTOPIA_STATUS_NOT_SUPPORTED;
            printf("Error Command = %d\n",u32Cmd);
            break;
    }

    return ret;
}

MS_U32 VDEC_EX_V2_Close(void* pInstance)
{
    void* pResource = NULL;
    VDEC_EX_RESOURCE_PRIVATE* pVDEC_EX_ResourcePrivate = NULL;
    VDEC_EX_INSTANCE_PRIVATE* psVDEC_EX_InstPri = NULL;

    // Get instance private
    UtopiaInstanceGetPrivate(pInstance, (void**)&psVDEC_EX_InstPri);

    // Get resource private
    if(UtopiaResourceObtain(pModuleVDEC_EX_V2, E_VDEC_EX_POOL_ID_INTERNAL, &pResource) != UTOPIA_STATUS_SUCCESS)
    {
        VPRINTF("UtopiaResourceObtain fail,%s,%d\n",__FUNCTION__,__LINE__);
        return UTOPIA_STATUS_ERR_RESOURCE;
    }

    UtopiaResourceGetPrivate(pResource,(void*)(&pVDEC_EX_ResourcePrivate));

    // Set using decoder index to 0
    //pVDEC_EX_ResourcePrivate->u8DecoderInUse[psVDEC_EX_InstPri->Index_of_decoder] = 0;

    UtopiaResourceRelease(pResource);

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

#endif

