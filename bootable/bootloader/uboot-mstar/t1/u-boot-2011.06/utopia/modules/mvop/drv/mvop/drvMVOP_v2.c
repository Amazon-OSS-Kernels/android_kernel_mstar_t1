/**
* Copyright (c) 2006 â€“ 2018 MStar Semiconductor, Inc.
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

#include "MsTypes.h"
#ifndef MSOS_TYPE_LINUX_KERNEL
#include <stdio.h>
#include <string.h>
#endif
#include "MsDevice.h"
#include "utopia.h"
#include "utopia_dapi.h"
//#include "drvXC_IOPort.h" //apple ??????
#include "drvMVOP.h"
#include "MVOP_private.h"
#include "drvMVOP_v2.h"


// ========== Global definition ==========
void* pModuleMVOP = NULL;

void MVOPRegisterToUtopia(FUtopiaOpen ModuleType)
{
    // 1. deal with module
    UtopiaModuleCreate(MODULE_MVOP, 0, &pModuleMVOP); //MODULE_MVOP???? where
    UtopiaModuleRegister(pModuleMVOP);
    // register func for module, after register here, then ap call UtopiaOpen/UtopiaIoctl/UtopiaClose can call to these registered standard func
    UtopiaModuleSetupFunctionPtr(pModuleMVOP, (FUtopiaOpen)MVOPOpen, (FUtopiaClose)MVOPClose, (FUtopiaIOctl)MVOPIoctl);

#if defined(MSOS_TYPE_LINUX_KERNEL)
        UtopiaModuleSetupSTRFunctionPtr(pModuleMVOP,(FUtopiaSTR)MVOPStr);
        UtopiaModuleSetSTRPrivate(pModuleMVOP, sizeof(MVOP_INSTANCE_PRIVATE));
#endif

    // 2. deal with resource  , XCºÝ¥i¯à»Ý­n¨â­Óresource¡A¨ä¤¤¤@­Ó¬Oreadonly¡A¥t¥~¤@­Ó¬OXC¾ã­Ó¥]¦íªºresource
    void* psResource = NULL;

    // 3. resource can allocate private for internal use
    UtopiaModuleAddResourceStart(pModuleMVOP, E_MVOP_POOL_ID_INTERNAL);
    UtopiaResourceCreate("MVOP", sizeof(MVOP_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleMVOP, psResource, E_MVOP_POOL_ID_INTERNAL);
	UtopiaModuleAddResourceEnd(pModuleMVOP, E_MVOP_POOL_ID_INTERNAL);

    // shall we init first?
}

MS_U32 MVOPStr(MS_U32 u32PowerState, void* pModule)
{
    MVOP_INSTANCE_PRIVATE *psMVOPInstPri = NULL;

    UtopiaModuleGetSTRPrivate(pModule, (void**)&psMVOPInstPri);

    if(psMVOPInstPri == NULL || psMVOPInstPri->fpMVOP_SetPowerState_Kernel == NULL)
    {
        return UTOPIA_STATUS_FAIL;
    }

    if (u32PowerState == E_POWER_SUSPEND)
    {
        void* pInstance = NULL;
        stMVOP_SET_POWER_STATE tmpMVOPArgs;
        tmpMVOPArgs.u16PowerState = E_POWER_SUSPEND;
        tmpMVOPArgs.u32Ret = FALSE;
        pstMVOP_SET_POWER_STATE  pMVOPArgs = &tmpMVOPArgs;
        pMVOPArgs->u32Ret = psMVOPInstPri->fpMVOP_SetPowerState_Kernel(pInstance, pMVOPArgs->u16PowerState);

        return pMVOPArgs->u32Ret;
    }
    else if (u32PowerState == E_POWER_RESUME)
    {
        void* pInstance = NULL;
        stMVOP_SET_POWER_STATE tmpMVOPArgs;
        tmpMVOPArgs.u16PowerState = E_POWER_RESUME;
        tmpMVOPArgs.u32Ret = FALSE;
        pstMVOP_SET_POWER_STATE  pMVOPArgs = &tmpMVOPArgs;
        pMVOPArgs->u32Ret = psMVOPInstPri->fpMVOP_SetPowerState_Kernel(pInstance, pMVOPArgs->u16PowerState);

        return pMVOPArgs->u32Ret;
    }

    return UTOPIA_STATUS_FAIL;
}


MS_U32 MVOPOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    MVOP_INSTANCE_PRIVATE *psMVOPInstPri = NULL;
    UtopiaInstanceCreate(sizeof(MVOP_INSTANCE_PRIVATE), ppInstance);
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psMVOPInstPri);

    //Main MVOP
    psMVOPInstPri->fpMVOP_Init = MDrv_MVOP_Init_V2;
    psMVOPInstPri->fpMVOP_Exit = MDrv_MVOP_Exit_V2;
    psMVOPInstPri->fpMVOP_Enable = MDrv_MVOP_Enable_V2;
    psMVOPInstPri->fpMVOP_SetInputCfg = MDrv_MVOP_SetInputCfg_V2;
    psMVOPInstPri->fpMVOP_SetOutputCfg = MDrv_MVOP_SetOutputCfg_V2;

    psMVOPInstPri->fpMVOP_EnableBlackBG = MDrv_MVOP_EnableBlackBG_V2;
    psMVOPInstPri->fpMVOP_EnableUVShift = MDrv_MVOP_EnableUVShift_V2;
    psMVOPInstPri->fpMVOP_SetMonoMode = MDrv_MVOP_SetMonoMode_V2;
    psMVOPInstPri->fpMVOP_GetHSize = MDrv_MVOP_GetHSize_V2;
    psMVOPInstPri->fpMVOP_GetVSize = MDrv_MVOP_GetVSize_V2;
    psMVOPInstPri->fpMVOP_GetHStart = MDrv_MVOP_GetHStart_V2;
    psMVOPInstPri->fpMVOP_GetVStart = MDrv_MVOP_GetVStart_V2;
    psMVOPInstPri->fpMVOP_GetIsInterlace = MDrv_MVOP_GetIsInterlace_V2;
    psMVOPInstPri->fpMVOP_GetIsHDuplicate = MDrv_MVOP_GetIsHDuplicate_V2;
    psMVOPInstPri->fpMVOP_GetIsEnable = MDrv_MVOP_GetIsEnable_V2;
    psMVOPInstPri->fpMVOP_GetOutputTiming = MDrv_MVOP_GetOutputTiming_V2;
    psMVOPInstPri->fpMVOP_GetLibVer = MDrv_MVOP_GetLibVer_V2;
    psMVOPInstPri->fpMVOP_CheckCapability = MDrv_MVOP_CheckCapability_V2;
    psMVOPInstPri->fpMVOP_GetMaxHOffset = MDrv_MVOP_GetMaxHOffset_V2;
    psMVOPInstPri->fpMVOP_GetMaxVOffset = MDrv_MVOP_GetMaxVOffset_V2;

    psMVOPInstPri->fpMVOP_SetDbgLevel = MDrv_MVOP_SetDbgLevel_V2;
    psMVOPInstPri->fpMVOP_GetInfo = MDrv_MVOP_GetInfo_V2;
    psMVOPInstPri->fpMVOP_GetStatus = MDrv_MVOP_GetStatus_V2;
    psMVOPInstPri->fpMVOP_SetClk = MDrv_MVOP_SetClk_V2;
    psMVOPInstPri->fpMVOP_SetPattern = MDrv_MVOP_SetPattern_V2;
    psMVOPInstPri->fpMVOP_SetTileFormat = MDrv_MVOP_SetTileFormat_V2;
    psMVOPInstPri->fpMVOP_GetDstInfo = MDrv_MVOP_GetDstInfo_V2;
    psMVOPInstPri->fpMVOP_SetFixVtt = MDrv_MVOP_SetFixVtt_V2;
    psMVOPInstPri->fpMVOP_SetMMIOMapBase = MDrv_MVOP_SetMMIOMapBase_V2;
    psMVOPInstPri->fpMVOP_MiuSwitch = MDrv_MVOP_MiuSwitch_V2;
    psMVOPInstPri->fpMVOP_SetBaseAdd = MDrv_MVOP_SetBaseAdd_V2;
    psMVOPInstPri->fpMVOP_SelOPField = MDrv_MVOP_SEL_OP_FIELD_V2;
    psMVOPInstPri->fpMVOP_SetRegSizeFromMVD = MDrv_MVOP_SetRegSizeFromMVD_V2;
    psMVOPInstPri->fpMVOP_SetStartPos = MDrv_MVOP_SetStartPos_V2;
    psMVOPInstPri->fpMVOP_SetImageWidthHight = MDrv_MVOP_SetImageWidthHight_V2;
    psMVOPInstPri->fpMVOP_SetVOPMirrorMode = MDrv_MVOP_SetVOPMirrorMode_V2;
    psMVOPInstPri->fpMVOP_InvOPVS = MDrv_MVOP_INV_OP_VS_V2;
    psMVOPInstPri->fpMVOP_ForceTop = MDrv_MVOP_FORCE_TOP_V2;
    psMVOPInstPri->fpMVOP_EnableFreerunMode = MDrv_MVOP_EnableFreerunMode_V2;
    psMVOPInstPri->fpMVOP_GetBaseAdd = MDrv_MVOP_GetBaseAdd_V2;

    //Sub MVOP
    psMVOPInstPri->fpMVOP_SubInit = MDrv_MVOP_SubInit_V2;
    psMVOPInstPri->fpMVOP_SubExit = MDrv_MVOP_SubExit_V2;
    psMVOPInstPri->fpMVOP_SubEnable = MDrv_MVOP_SubEnable_V2;
    psMVOPInstPri->fpMVOP_SubSetInputCfg = MDrv_MVOP_SubSetInputCfg_V2;
    psMVOPInstPri->fpMVOP_SubSetOutputCfg = MDrv_MVOP_SubSetOutputCfg_V2;

    psMVOPInstPri->fpMVOP_SubEnableBlackBG = MDrv_MVOP_EnableBlackBG_V2;
    psMVOPInstPri->fpMVOP_SubEnableUVShift = MDrv_MVOP_SubEnableUVShift_V2;
    psMVOPInstPri->fpMVOP_SubSetMonoMode = MDrv_MVOP_SubSetMonoMode_V2;
    psMVOPInstPri->fpMVOP_SubGetStatus = MDrv_MVOP_SubGetStatus_V2;
    psMVOPInstPri->fpMVOP_SubGetHSize = MDrv_MVOP_SubGetHSize_V2;
    psMVOPInstPri->fpMVOP_SubGetVSize = MDrv_MVOP_SubGetVSize_V2;
    psMVOPInstPri->fpMVOP_SubGetHStart = MDrv_MVOP_SubGetHStart_V2;
    psMVOPInstPri->fpMVOP_SubGetVStart = MDrv_MVOP_SubGetVStart_V2;
    psMVOPInstPri->fpMVOP_SubGetIsInterlace = MDrv_MVOP_SubGetIsInterlace_V2;
    psMVOPInstPri->fpMVOP_SubGetIsHDuplicate = MDrv_MVOP_SubGetIsHDuplicate_V2;
    psMVOPInstPri->fpMVOP_SubGetIsEnable = MDrv_MVOP_SubGetIsEnable_V2;
    psMVOPInstPri->fpMVOP_SubGetOutputTiming = MDrv_MVOP_SubGetOutputTiming_V2;
    psMVOPInstPri->fpMVOP_SubCheckCapability = MDrv_MVOP_SubCheckCapability_V2;
    psMVOPInstPri->fpMVOP_SubGetMaxHOffset = MDrv_MVOP_SubGetMaxHOffset_V2;
    psMVOPInstPri->fpMVOP_SubGetMaxVOffset = MDrv_MVOP_SubGetMaxVOffset_V2;
    psMVOPInstPri->fpMVOP_SubSetClk = MDrv_MVOP_SubSetClk_V2;
    psMVOPInstPri->fpMVOP_SubSetPattern = MDrv_MVOP_SubSetPattern_V2;
    psMVOPInstPri->fpMVOP_SubSetTileFormat = MDrv_MVOP_SubSetTileFormat_V2;
    psMVOPInstPri->fpMVOP_SubGetDstInfo = MDrv_MVOP_SubGetDstInfo_V2;
    psMVOPInstPri->fpMVOP_SubSetFixVtt = MDrv_MVOP_SubSetFixVtt_V2;
    psMVOPInstPri->fpMVOP_SubSetMMIOMapBase = MDrv_MVOP_SubSetMMIOMapBase_V2;
    psMVOPInstPri->fpMVOP_SubMiuSwitch = MDrv_MVOP_SubMiuSwitch_V2;
    psMVOPInstPri->fpMVOP_SubSetBaseAdd = MDrv_MVOP_SubSetBaseAdd_V2;
    psMVOPInstPri->fpMVOP_SubGetBaseAdd = MDrv_MVOP_SubGetBaseAdd_V2;
    psMVOPInstPri->fpMVOP_SubSetVOPMirrorMode = MDrv_MVOP_SubSetVOPMirrorMode_V2;
    psMVOPInstPri->fpMVOP_SubEnableFreerunMode = MDrv_MVOP_SubEnableFreerunMode_V2;


    //Third MVOP
    psMVOPInstPri->fpMVOP_ExInit = MDrv_MVOP_EX_Init_V2;
    psMVOPInstPri->fpMVOP_ExExit = MDrv_MVOP_EX_Exit_V2;
    psMVOPInstPri->fpMVOP_ExEnable = MDrv_MVOP_EX_Enable_V2;
    psMVOPInstPri->fpMVOP_ExInputCfg = MDrv_MVOP_EX_SetInputCfg_V2;
    psMVOPInstPri->fpMVOP_ExOutputCfg = MDrv_MVOP_EX_SetOutputCfg_V2;

    psMVOPInstPri->fpMVOP_ExSetClk = MDrv_MVOP_EX_SetClk_V2;
    psMVOPInstPri->fpMVOP_ExSetPattern = MDrv_MVOP_EX_SetPattern_V2;
    psMVOPInstPri->fpMVOP_ExSetTileFormat = MDrv_MVOP_EX_SetTileFormat_V2;
    psMVOPInstPri->fpMVOP_ExEnableUVShift = MDrv_MVOP_EX_EnableUVShift_V2;
    psMVOPInstPri->fpMVOP_ExEnableBlackBG = MDrv_MVOP_EX_EnableBlackBG_V2;
    psMVOPInstPri->fpMVOP_ExSetMonoMode = MDrv_MVOP_EX_SetMonoMode_V2;
    psMVOPInstPri->fpMVOP_ExSetFixVtt = MDrv_MVOP_EX_SetFixVtt_V2;
    psMVOPInstPri->fpMVOP_ExMiuSwitch = MDrv_MVOP_EX_MiuSwitch_V2;
    psMVOPInstPri->fpMVOP_ExSetVOPMirrorMode = MDrv_MVOP_EX_SetVOPMirrorMode_V2;
    psMVOPInstPri->fpMVOP_ExEnableFreerunMode = MDrv_MVOP_EX_EnableFreerunMode_V2;

    psMVOPInstPri->fpMVOP_ExGetOutputTiming = MDrv_MVOP_EX_GetOutputTiming_V2;
    psMVOPInstPri->fpMVOP_ExGetIsEnable = MDrv_MVOP_EX_GetIsEnable_V2;
    psMVOPInstPri->fpMVOP_ExGetHSize = MDrv_MVOP_EX_GetHSize_V2;
    psMVOPInstPri->fpMVOP_ExGetVSize = MDrv_MVOP_EX_GetVSize_V2;
    psMVOPInstPri->fpMVOP_ExGetHStart = MDrv_MVOP_EX_GetHStart_V2;
    psMVOPInstPri->fpMVOP_ExGetVStart = MDrv_MVOP_EX_GetVStart_V2;
    psMVOPInstPri->fpMVOP_ExGetIsInterlace = MDrv_MVOP_EX_GetIsInterlace_V2;
    psMVOPInstPri->fpMVOP_ExGetIsHDuplicate = MDrv_MVOP_EX_GetIsHDuplicate_V2;
    psMVOPInstPri->fpMVOP_ExGetStatus = MDrv_MVOP_EX_GetStatus_V2;
    psMVOPInstPri->fpMVOP_ExCheckCapability = MDrv_MVOP_EX_CheckCapability_V2;
    psMVOPInstPri->fpMVOP_ExGetDstInfo = MDrv_MVOP_EX_GetDstInfo_V2;

    psMVOPInstPri->fpMVOP_SendBlueScreen = MDrv_MVOP_SendBlueScreen_V2;
    psMVOPInstPri->fpMVOP_SetCommand = MDrv_MVOP_SetCommand_V2;
    psMVOPInstPri->fpMVOP_GetCommand = MDrv_MVOP_GetCommand_V2;
    psMVOPInstPri->fpMVOP_SetFrequency = MDrv_MVOP_SetFrequency_V2;

    psMVOPInstPri->fpMVOP_EnableInterrupt = MDrv_MVOP_EnableInterrupt_V2;
    psMVOPInstPri->fpMVOP_GetIntStatus = MDrv_MVOP_GetIntStatus_V2;
    psMVOPInstPri->fpMVOP_SubEnableInterrupt = MDrv_MVOP_SubEnableInterrupt_V2;
    psMVOPInstPri->fpMVOP_SubGetIntStatus = MDrv_MVOP_SubGetIntStatus_V2;
    psMVOPInstPri->fpMVOP_ExEnableInterrupt = MDrv_MVOP_EX_EnableInterrupt_V2;
    psMVOPInstPri->fpMVOP_ExGetIntStatus = MDrv_MVOP_EX_GetIntStatus_V2;

    psMVOPInstPri->fpMVOP_SetPowerState = MDrv_MVOP_SetPowerState_V2;
    psMVOPInstPri->fpMVOP_SetPowerState_Kernel = MDrv_MVOP_SetPowerState_Kernel_V2;
#if defined(MSOS_TYPE_LINUX_KERNEL)
    MVOP_INSTANCE_PRIVATE* psMVOPInstSTRPri = NULL;
    UtopiaModuleGetSTRPrivate(pModuleMVOP, (void**)&psMVOPInstSTRPri);

    psMVOPInstSTRPri->fpMVOP_SetPowerState_Kernel = MDrv_MVOP_SetPowerState_Kernel_V2;
#endif

	return UTOPIA_STATUS_SUCCESS;
}


MS_U32 MVOPIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs)
{
	void* pModule = NULL;
	void* pResource = NULL;
    MVOP_INSTANCE_PRIVATE* psMVOPInstPri = NULL;

    UtopiaInstanceGetModule(pInstance, &pModule);

    if(UtopiaInstanceGetPrivate(pInstance, (void*)&psMVOPInstPri)!=UTOPIA_STATUS_SUCCESS)
    {
        printf("[%s,%5d] Instance Get Private failed\n",__FUNCTION__,__LINE__);
    }

    //printf("[%s,%5d] u32Cmd type = %ld\n",__FUNCTION__,__LINE__,u32Cmd);
    MS_U32 u32Return = UTOPIA_STATUS_FAIL;
    switch(u32Cmd)
    {
        case E_MVOP_CMD_INIT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                psMVOPInstPri->fpMVOP_Init(pInstance);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EXIT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                psMVOPInstPri->fpMVOP_Exit(pInstance);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_ENABLE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                pstMVOP_ENABLE pMVOPArgs = (pstMVOP_ENABLE) pArgs;
                psMVOPInstPri->fpMVOP_Enable(pInstance, pMVOPArgs->bEnable);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_INPUTCFG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                pstMVOP_SET_INPUTCFG  pMVOPArgs = (pstMVOP_SET_INPUTCFG) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_SetInputCfg(pInstance, pMVOPArgs->in, pMVOPArgs->pCfg);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_OTPUTCFG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                pstMVOP_SET_OTPUTCFG pMVOPArgs = (pstMVOP_SET_OTPUTCFG) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_SetOutputCfg(pInstance, pMVOPArgs->pstVideoStatus, pMVOPArgs->bEnHDup);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_ENABLE_BLACK_BG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                psMVOPInstPri->fpMVOP_EnableBlackBG(pInstance);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_ENABLE_UV_SHIFT:

            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                pstMVOP_ENABLE_UV_SHIFT pMVOPArgs = (pstMVOP_ENABLE_UV_SHIFT) pArgs;
                psMVOPInstPri->fpMVOP_EnableUVShift(pInstance, pMVOPArgs->bEnable);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_MONO_MODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                pstMVOP_SET_MONO_MODE pMVOPArgs = (pstMVOP_SET_MONO_MODE) pArgs;
                psMVOPInstPri->fpMVOP_SetMonoMode(pInstance, pMVOPArgs->bEnable);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_HSIZE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_HSIZE pMVOPArgs = (pstMVOP_GET_HSIZE) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_GetHSize(pInstance);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_VSIZE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_VSIZE pMVOPArgs = (pstMVOP_GET_VSIZE) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_GetVSize(pInstance);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_HSTART:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_HSTART pMVOPArgs = (pstMVOP_GET_HSTART) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_GetHStart(pInstance);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_VSTART:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_VSTART pMVOPArgs = (pstMVOP_GET_VSTART) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_GetVStart(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_IS_INTERLACE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_IS_INTERLACE pMVOPArgs = (pstMVOP_GET_IS_INTERLACE) pArgs;

                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_GetIsInterlace(pInstance);

                UtopiaResourceRelease(pResource);

                printf("XC Exited\n");

                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_IS_HDUPLICATE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_IS_HDUPLICATE pMVOPArgs = (pstMVOP_GET_IS_HDUPLICATE) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_GetIsHDuplicate(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_IS_ENABLE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_IS_ENABLE pMVOPArgs = (pstMVOP_GET_IS_ENABLE) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_GetIsEnable(pInstance, pMVOPArgs->pbEnable);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_OUTPUT_TIMING:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_OUTPUT_TIMING pMVOPArgs = (pstMVOP_GET_OUTPUT_TIMING) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_GetOutputTiming(pInstance, pMVOPArgs->pMVOPTiming);
                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_LIBVER:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_LIBVER pMVOPArgs = (pstMVOP_GET_LIBVER) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_GetLibVer(pInstance, pMVOPArgs->ppVersion);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_CHECK_CAPABILITY:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_CHECK_CAPABILITY pMVOPArgs = (pstMVOP_CHECK_CAPABILITY) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_CheckCapability(pInstance, pMVOPArgs->u16HSize, pMVOPArgs->u16VSize, pMVOPArgs->u16Fps);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_MAX_HOFFSET:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_MAX_HOFFSET pMVOPArgs = (pstMVOP_GET_MAX_HOFFSET) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_GetMaxHOffset(pInstance, pMVOPArgs->u16HSize, pMVOPArgs->u16VSize, pMVOPArgs->u16Fps);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_MAX_VOFFSET:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_MAX_VOFFSET pMVOPArgs = (pstMVOP_GET_MAX_VOFFSET) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_GetMaxVOffset(pInstance, pMVOPArgs->u16HSize, pMVOPArgs->u16VSize, pMVOPArgs->u16Fps);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_DBG_LEVEL:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_DBG_LEVEL pMVOPArgs = (pstMVOP_SET_DBG_LEVEL) pArgs;
                psMVOPInstPri->fpMVOP_SetDbgLevel(pInstance, pMVOPArgs->level);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_INFO:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_INFO pMVOPArgs = (pstMVOP_GET_INFO) pArgs;
                pMVOPArgs->pRet = psMVOPInstPri->fpMVOP_GetInfo(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_STATUS:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_STATUS pMVOPArgs = (pstMVOP_GET_STATUS) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_GetStatus(pInstance, pMVOPArgs->pMVOPStat);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SET_CLK:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_CLK pMVOPArgs = (pstMVOP_SET_CLK) pArgs;
                psMVOPInstPri->fpMVOP_SetClk(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SET_PATTERN:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_PATTERN pMVOPArgs = (pstMVOP_SET_PATTERN) pArgs;
                psMVOPInstPri->fpMVOP_SetPattern(pInstance, pMVOPArgs->enMVOPPattern);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_TILE_FORMAT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_TILE_FORMAT pMVOPArgs = (pstMVOP_SET_TILE_FORMAT) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SetTileFormat(pInstance, pMVOPArgs->eTileFmt);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_DST_INFO:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_DST_INFO pMVOPArgs = (pstMVOP_GET_DST_INFO) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_GetDstInfo(pInstance, pMVOPArgs->pDstInfo, pMVOPArgs->u32SizeofDstInfo);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_FIXVTT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_FIXVTT pMVOPArgs = (pstMVOP_SET_FIXVTT) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SetFixVtt(pInstance, pMVOPArgs->u16FixVtt);


                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_MMIO_MAPBASE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_MMIO_MAPBASE pMVOPArgs = (pstMVOP_SET_MMIO_MAPBASE) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SetMMIOMapBase(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_MIU_SWITCH:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_MIU_SWITCH pMVOPArgs = (pstMVOP_MIU_SWITCH) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_MiuSwitch(pInstance, pMVOPArgs->u8Miu);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_BASEADD:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_BASEADD pMVOPArgs = (pstMVOP_SET_BASEADD) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SetBaseAdd(pInstance, pMVOPArgs->u32YOffset, pMVOPArgs->u32UVOffset, pMVOPArgs->bProgressive, pMVOPArgs->b422pack);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SEL_OP_FIELD:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SEL_OP_FIELD pMVOPArgs = (pstMVOP_SEL_OP_FIELD) pArgs;
                psMVOPInstPri->fpMVOP_SelOPField(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_REGSIZE_FROM_MVD:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_REGSIZE_FROM_MVD pMVOPArgs = (pstMVOP_SET_REGSIZE_FROM_MVD) pArgs;
                psMVOPInstPri->fpMVOP_SetRegSizeFromMVD(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_START_POS:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_START_POS pMVOPArgs = (pstMVOP_SET_START_POS) pArgs;
                psMVOPInstPri->fpMVOP_SetStartPos(pInstance, pMVOPArgs->u16Xpos, pMVOPArgs->u16Ypos);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SET_IMAGE_WIDTH_HIGHT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_IMAGE_WIDTH_HIGHT pMVOPArgs = (pstMVOP_SET_IMAGE_WIDTH_HIGHT) pArgs;
                pMVOPArgs->bRet= psMVOPInstPri->fpMVOP_SetImageWidthHight(pInstance, pMVOPArgs->u16Width, pMVOPArgs->u16Height);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
        case E_MVOP_CMD_SET_VOP_MIRROR_MODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_VOP_MIRROR_MODE pMVOPArgs = (pstMVOP_SET_VOP_MIRROR_MODE) pArgs;
                psMVOPInstPri->fpMVOP_SetVOPMirrorMode(pInstance, pMVOPArgs->bEnable, pMVOPArgs->eMirrorMode);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_INV_OP_VS:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_INV_OP_VS pMVOPArgs = (pstMVOP_INV_OP_VS) pArgs;
                psMVOPInstPri->fpMVOP_InvOPVS(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_FORCE_TOP:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_FORCE_TOP MVOPArgs = (pstMVOP_FORCE_TOP) pArgs;
                psMVOPInstPri->fpMVOP_ForceTop(pInstance, MVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_ENABLE_FREERUN_MODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_ENABLE_FREERUN_MODE pMVOPArgs = (pstMVOP_ENABLE_FREERUN_MODE) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_EnableFreerunMode(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_GET_BASE_ADD:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_BASE_ADD pMVOPArgs = (pstMVOP_GET_BASE_ADD) pArgs;
                psMVOPInstPri->fpMVOP_GetBaseAdd(pInstance, pMVOPArgs->u32YOffset, pMVOPArgs->u32UVOffset);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_INIT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                psMVOPInstPri->fpMVOP_SubInit(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_EXIT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                psMVOPInstPri->fpMVOP_SubExit(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_ENABLE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_ENABLE pMVOPArgs = (pstMVOP_SUB_ENABLE) pArgs;
                psMVOPInstPri->fpMVOP_SubEnable(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_INPUTCFG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_INPUTCFG pMVOPArgs = (pstMVOP_SUB_SET_INPUTCFG) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_SubSetInputCfg(pInstance, pMVOPArgs->in, pMVOPArgs->pCfg);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_OTPUTCFG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_OTPUTCFG pMVOPArgs = (pstMVOP_SUB_SET_OTPUTCFG) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_SubSetOutputCfg(pInstance, pMVOPArgs->pstVideoStatus, pMVOPArgs->bEnHDup);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_ENABLE_BLACK_BG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                psMVOPInstPri->fpMVOP_SubEnableBlackBG(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_ENABLE_UV_SHIFT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_ENABLE_UV_SHIFT pMVOPArgs = (pstMVOP_SUB_ENABLE_UV_SHIFT) pArgs;
                psMVOPInstPri->fpMVOP_SubEnableUVShift(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_MONO_MODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_MONO_MODE pMVOPArgs = (pstMVOP_SUB_SET_MONO_MODE) pArgs;
                psMVOPInstPri->fpMVOP_SubSetMonoMode(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_HSIZE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_HSIZE pMVOPArgs = (pstMVOP_SUB_GET_HSIZE) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_SubGetHSize(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_VSIZE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_VSIZE pMVOPArgs = (pstMVOP_SUB_GET_VSIZE) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_SubGetVSize(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_HSTART:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_HSTART pMVOPArgs = (pstMVOP_SUB_GET_HSTART) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_SubGetHStart(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_VSTART:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_VSTART pMVOPArgs = (pstMVOP_SUB_GET_VSTART) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_SubGetVStart(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_IS_INTERLACE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_IS_INTERLACE pMVOPArgs = (pstMVOP_SUB_GET_IS_INTERLACE) pArgs;
                pMVOPArgs->bRet= psMVOPInstPri->fpMVOP_SubGetIsInterlace(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_IS_HDUPLICATE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_IS_HDUPLICATE pMVOPArgs = (pstMVOP_SUB_GET_IS_HDUPLICATE) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SubGetIsHDuplicate(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_IS_ENABLE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_IS_ENABLE pMVOPArgs = (pstMVOP_SUB_GET_IS_ENABLE) pArgs;
                pMVOPArgs->eRet= psMVOPInstPri->fpMVOP_SubGetIsEnable(pInstance, pMVOPArgs->pbEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_OUTPUT_TIMING:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_OUTPUT_TIMING pMVOPArgs = (pstMVOP_SUB_GET_OUTPUT_TIMING) pArgs;
                pMVOPArgs->eRet= psMVOPInstPri->fpMVOP_SubGetOutputTiming(pInstance, pMVOPArgs->pMVOPTiming);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_CHECK_CAPABILITY:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_CHECK_CAPABILITY pMVOPArgs = (pstMVOP_SUB_CHECK_CAPABILITY) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SubCheckCapability(pInstance, pMVOPArgs->u16HSize, pMVOPArgs->u16VSize, pMVOPArgs->u16Fps);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_MAXHOFFSET:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_MAX_HOFFSET pMVOPArgs = (pstMVOP_SUB_GET_MAX_HOFFSET) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_SubGetMaxHOffset(pInstance, pMVOPArgs->u16HSize, pMVOPArgs->u16VSize, pMVOPArgs->u16Fps);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_MAXVOFFSET:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_MAX_VOFFSET pMVOPArgs = (pstMVOP_SUB_GET_MAX_VOFFSET) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_SubGetMaxVOffset(pInstance, pMVOPArgs->u16HSize, pMVOPArgs->u16VSize, pMVOPArgs->u16Fps);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SUB_SET_CLK:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_CLK pMVOPArgs = (pstMVOP_SUB_SET_CLK) pArgs;
                psMVOPInstPri->fpMVOP_SubSetClk(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SUB_SET_PATTERN:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_PATTERN pMVOPArgs = (pstMVOP_SUB_SET_PATTERN) pArgs;
                psMVOPInstPri->fpMVOP_SubSetPattern(pInstance, pMVOPArgs->enMVOPPattern);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_TILE_FORMAT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_TILE_FORMAT pMVOPArgs = (pstMVOP_SUB_SET_TILE_FORMAT) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SubSetTileFormat(pInstance, pMVOPArgs->eTileFmt);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_DST_INFO:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_DST_INFO pMVOPArgs = (pstMVOP_SUB_GET_DST_INFO) pArgs;
                pMVOPArgs->bRet= psMVOPInstPri->fpMVOP_SubGetDstInfo(pInstance, pMVOPArgs->pDstInfo, pMVOPArgs->u32SizeofDstInfo);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_FIXVTT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_FIXVTT pMVOPArgs = (pstMVOP_SUB_SET_FIXVTT) pArgs;
                pMVOPArgs->bRet= psMVOPInstPri->fpMVOP_SubSetFixVtt(pInstance, pMVOPArgs->u16FixVtt);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_MMIO_MAPBASE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_MMIO_MAPBASE pMVOPArgs = (pstMVOP_SUB_SET_MMIO_MAPBASE) pArgs;
                pMVOPArgs->bRet= psMVOPInstPri->fpMVOP_SubSetMMIOMapBase(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_MIU_SWITCH:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_MIU_SWITCH pMVOPArgs = (pstMVOP_SUB_MIU_SWITCH) pArgs;
                pMVOPArgs->eRet= psMVOPInstPri->fpMVOP_SubMiuSwitch(pInstance, pMVOPArgs->u8Miu);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_BASEADD:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_BASEADD pMVOPArgs = (pstMVOP_SUB_SET_BASEADD) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SubSetBaseAdd(pInstance, pMVOPArgs->u32YOffset, pMVOPArgs->u32UVOffset, pMVOPArgs->bProgressive, pMVOPArgs->b422pack);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_SET_VOP_MIRRORMODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_SET_VOP_MIRROR_MODE pMVOPArgs = (pstMVOP_SUB_SET_VOP_MIRROR_MODE) pArgs;
                psMVOPInstPri->fpMVOP_SubSetVOPMirrorMode(pInstance, pMVOPArgs->bEnable, pMVOPArgs->eMirrorMode);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_ENABLE_FREERUN_MODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_ENABLE_FREERUN_MODE pMVOPArgs = (pstMVOP_SUB_ENABLE_FREERUN_MODE) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SubEnableFreerunMode(pInstance, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_SUB_GET_BASEADD:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_BASE_ADD pMVOPArgs = (pstMVOP_SUB_GET_BASE_ADD) pArgs;
                psMVOPInstPri->fpMVOP_SubGetBaseAdd(pInstance, pMVOPArgs->u32YOffset, pMVOPArgs->u32UVOffset);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_INIT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_INIT pMVOPArgs = (pstMVOP_EX_INIT) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExInit(pInstance, pMVOPArgs->eID, pMVOPArgs->u32InitParam);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_EXIT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_EXIT pMVOPArgs = (pstMVOP_EX_EXIT) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExExit(pInstance, pMVOPArgs->eID, pMVOPArgs->u32ExitParam);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_ENABLE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_ENABLE pMVOPArgs = (pstMVOP_EX_ENABLE) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExEnable(pInstance, pMVOPArgs->eID, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_SET_INPUTCFG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_INPUTCFG pMVOPArgs = (pstMVOP_EX_SET_INPUTCFG) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExInputCfg(pInstance, pMVOPArgs->eID, pMVOPArgs->in, pMVOPArgs->pCfg);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_SET_OTPUTCFG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_OTPUTCFG pMVOPArgs = (pstMVOP_EX_SET_OTPUTCFG) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExOutputCfg(pInstance, pMVOPArgs->eID, pMVOPArgs->pstVideoStatus, pMVOPArgs->bEnHDup);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_EX_SET_CLK:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_CLK pMVOPArgs = (pstMVOP_EX_SET_CLK) pArgs;
                psMVOPInstPri->fpMVOP_ExSetClk(pInstance, pMVOPArgs->eID, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_EX_SET_PATTERN:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_PATTERN pMVOPArgs = (pstMVOP_EX_SET_PATTERN) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExSetPattern(pInstance, pMVOPArgs->eID, pMVOPArgs->enMVOPPattern);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_SET_TILEFORMAT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_TILE_FORMAT pMVOPArgs = (pstMVOP_EX_SET_TILE_FORMAT) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExSetTileFormat(pInstance, pMVOPArgs->eID, pMVOPArgs->eTileFmt);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_ENABLE_UV_SHIFT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_ENABLE_UV_SHIFT pMVOPArgs = (pstMVOP_EX_ENABLE_UV_SHIFT) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExEnableUVShift(pInstance, pMVOPArgs->eID, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_ENABLE_BLACK_BG:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_ENABLE_BLACK_BG pMVOPArgs = (pstMVOP_EX_ENABLE_BLACK_BG) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExEnableBlackBG(pInstance, pMVOPArgs->eID);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_SET_MONO_MODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_MONO_MODE pMVOPArgs = (pstMVOP_EX_SET_MONO_MODE) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExSetMonoMode(pInstance, pMVOPArgs->eID, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_SET_FIXVTT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_FIXVTT pMVOPArgs = (pstMVOP_EX_SET_FIXVTT) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExSetFixVtt(pInstance, pMVOPArgs->eID, pMVOPArgs->u16FixVtt);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_MIU_SWITCH:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_MIU_SWITCH pMVOPArgs = (pstMVOP_EX_MIU_SWITCH) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExMiuSwitch(pInstance, pMVOPArgs->eID, pMVOPArgs->u8Miu);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_SET_VOP_MIRRORMODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_SET_VOP_MIRROR_MODE pMVOPArgs = (pstMVOP_EX_SET_VOP_MIRROR_MODE) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExSetVOPMirrorMode(pInstance, pMVOPArgs->eID, pMVOPArgs->bEnable, pMVOPArgs->eMirrorMode);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_ENABLE_FREERUN_MODE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_ENABLE_FREERUN_MODE pMVOPArgs = (pstMVOP_EX_ENABLE_FREERUN_MODE) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExEnableFreerunMode(pInstance, pMVOPArgs->eID, pMVOPArgs->bEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_OUTPUT_TIMING:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_OUTPUT_TIMING pMVOPArgs = (pstMVOP_EX_GET_OUTPUT_TIMING) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExGetOutputTiming(pInstance, pMVOPArgs->eID, pMVOPArgs->pMVOPTiming);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_IS_ENABLE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_IS_ENABLE pMVOPArgs = (pstMVOP_EX_GET_IS_ENABLE) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_ExGetIsEnable(pInstance, pMVOPArgs->eID, pMVOPArgs->pbEnable);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_HSIZE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_HSIZE pMVOPArgs = (pstMVOP_EX_GET_HSIZE) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_ExGetVStart(pInstance, pMVOPArgs->eID);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_VSIZE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_VSIZE pMVOPArgs = (pstMVOP_EX_GET_VSIZE) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_ExGetVStart(pInstance, pMVOPArgs->eID);


                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_HSTART:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_HSTART pMVOPArgs = (pstMVOP_EX_GET_HSTART) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_ExGetVStart(pInstance, pMVOPArgs->eID);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_VSTART:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_VSTART pMVOPArgs = (pstMVOP_EX_GET_VSTART) pArgs;
                pMVOPArgs->u16Ret = psMVOPInstPri->fpMVOP_ExGetVStart(pInstance, pMVOPArgs->eID);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_IS_INTERLACE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_IS_INTERLACE pMVOPArgs = (pstMVOP_EX_GET_IS_INTERLACE) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_ExGetIsInterlace(pInstance, pMVOPArgs->eID);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_IS_HDUPLICATE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }
                pstMVOP_EX_GET_IS_HDUPLICATE pMVOPArgs = (pstMVOP_EX_GET_IS_HDUPLICATE) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_ExGetIsHDuplicate(pInstance, pMVOPArgs->eID);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_STATUS:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_STATUS pMVOPArgs = (pstMVOP_EX_GET_STATUS) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_ExGetStatus(pInstance, pMVOPArgs->eID, pMVOPArgs->pMVOPStat);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_CHECK_CAPABILITY:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_CHECK_CAPABILITY pMVOPArgs = (pstMVOP_EX_CHECK_CAPABILITY) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_ExCheckCapability(pInstance, pMVOPArgs->eID, pMVOPArgs->u32InParam);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }


        case E_MVOP_CMD_EX_GET_DST_INFO:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_DST_INFO pMVOPArgs = (pstMVOP_EX_GET_DST_INFO) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_ExGetDstInfo(pInstance, pMVOPArgs->eID, pMVOPArgs->pDstInfo, pMVOPArgs->u32SizeofDstInfo);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SEND_BLUE_SCREEN:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SEND_BLUE_SCREEN pMVOPArgs = (pstMVOP_SEND_BLUE_SCREEN) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_SendBlueScreen(pInstance, pMVOPArgs->u16Width, pMVOPArgs->u16Height);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SET_COMMAND:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_COMMAND pMVOPArgs = (pstMVOP_SET_COMMAND) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_SetCommand(pInstance, pMVOPArgs->stHd, pMVOPArgs->eCmd, pMVOPArgs->pPara);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_GET_COMMAND:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_COMMAND pMVOPArgs = (pstMVOP_GET_COMMAND) pArgs;
                pMVOPArgs->eRet = psMVOPInstPri->fpMVOP_GetCommand(pInstance, pMVOPArgs->stHd, pMVOPArgs->eCmd, pMVOPArgs->pPara, pMVOPArgs->u32ParaSize);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SET_FREQUENCY:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_FREQUENCY pMVOPArgs = (pstMVOP_SET_FREQUENCY) pArgs;
                psMVOPInstPri->fpMVOP_SetFrequency(pInstance, pMVOPArgs->eFreq);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_ENABLE_INTERRUPT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_ENABLE_INTERRUPT pMVOPArgs = (pstMVOP_ENABLE_INTERRUPT) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_EnableInterrupt(pInstance, pMVOPArgs->eIntType);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_GET_INT_STATUS:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_GET_INT_STATUS pMVOPArgs = (pstMVOP_GET_INT_STATUS) pArgs;
                pMVOPArgs->u8Ret= psMVOPInstPri->fpMVOP_GetIntStatus(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SUB_ENABLE_INTERRUPT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_ENABLE_INTERRUPT pMVOPArgs = (pstMVOP_SUB_ENABLE_INTERRUPT) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_SubEnableInterrupt(pInstance, pMVOPArgs->eIntType);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_SUB_GET_INT_STATUS:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SUB_GET_INT_STATUS pMVOPArgs = (pstMVOP_SUB_GET_INT_STATUS) pArgs;
                pMVOPArgs->u8Ret= psMVOPInstPri->fpMVOP_SubGetIntStatus(pInstance);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_EX_ENABLE_INTERRUPT:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_ENABLE_INTERRUPT pMVOPArgs = (pstMVOP_EX_ENABLE_INTERRUPT) pArgs;
                pMVOPArgs->bRet = psMVOPInstPri->fpMVOP_ExEnableInterrupt(pInstance, pMVOPArgs->eID, pMVOPArgs->eIntType);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

        case E_MVOP_CMD_EX_GET_INT_STATUS:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_EX_GET_INT_STATUS pMVOPArgs = (pstMVOP_EX_GET_INT_STATUS) pArgs;
                pMVOPArgs->u8Ret= psMVOPInstPri->fpMVOP_ExGetIntStatus(pInstance, pMVOPArgs->eID);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }
#if 1
        case E_MVOP_CMD_SET_POWERSTATE:
            {
                if(UtopiaResourceObtain(pModule, E_MVOP_POOL_ID_INTERNAL, &pResource) != 0)
                {
                    printf("UtopiaResourceObtain fail\n");
    	            return UTOPIA_STATUS_ERR_RESOURCE;
                }

                pstMVOP_SET_POWER_STATE pMVOPArgs = (pstMVOP_SET_POWER_STATE) pArgs;
                pMVOPArgs->u32Ret= psMVOPInstPri->fpMVOP_SetPowerState(pInstance, pMVOPArgs->u16PowerState);

                UtopiaResourceRelease(pResource);
                u32Return = UTOPIA_STATUS_SUCCESS;
                break;
            }

#endif

// OBSOLETE command **END**, please avoid using these

        case E_MVOP_CMD_MAX:
        default:
            {
#if defined (__aarch64__)
                printf("Command %d is not existed\n",u32Cmd);
#else
                printf("Command %d is not existed\n",(int)u32Cmd);
#endif
                u32Return = UTOPIA_STATUS_ERR_INVALID_HANDLE;
                break;
            }
    }
    return u32Return;
}

MS_U32 MVOPClose(void* pInstance)
{
	UtopiaInstanceDelete(pInstance);

	return TRUE;
}



