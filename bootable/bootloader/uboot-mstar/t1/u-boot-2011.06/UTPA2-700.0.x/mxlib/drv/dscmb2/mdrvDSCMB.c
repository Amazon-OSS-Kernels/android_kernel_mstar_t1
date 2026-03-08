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
///////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   mdrvDSCMB.c
/// @brief   API Interface
/// @attention
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsTypes.h"
#include <stdio.h>
#include <string.h>
#include "drvDSCMB_v2.h"
#include "drvDSCMB_private.h"

#include "utopia.h"
#include "utopia_dapi.h"
#include "MsOS.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------
void* pModuleDscmb = NULL;

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DSCMB_MSG(_f, _a...)                             //ULOGD("DSCMB", _f, ##_a)

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
MS_BOOL* _bFreeSlot;
stDscmbTspMap (*DscmbTspMap)[HAL_DSCMB_KTE_MAX];
stTspFltInfo* _u32PidFlt2Dscmb;

//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
void DSCMBRegisterToUtopia(void)
{
    DSCMB_RESOURCE_PRIVATE * pResPri = NULL;
    void* psResource = NULL;

    // 1. deal with module

    // create a DSCMB module
    // a. size = 8
    // b. Dst: pModuleDscmb
    UtopiaModuleCreate(MODULE_DSCMB, 8, &pModuleDscmb);
    // register a dscmb module
    UtopiaModuleRegister(pModuleDscmb);
    //setup module functions
    UtopiaModuleSetupFunctionPtr(pModuleDscmb, (FUtopiaOpen)DSCMBOpen, (FUtopiaClose)DSCMBClose, (FUtopiaIOctl)DSCMBIoctl);

    // 2. deal with resource
    // create and allocate resource private data(drv Global shared data)
    UtopiaResourceCreate("DSCMB", sizeof(DSCMB_RESOURCE_PRIVATE), &psResource);
    UtopiaResourceRegister(pModuleDscmb, psResource, E_DSCMB_RESOURCE);
    UtopiaModuleAddResourceEnd(pModuleDscmb, E_DSCMB_RESOURCE);

    UtopiaResourceObtain(pModuleDscmb, E_DSCMB_RESOURCE, &psResource);
    UtopiaResourceGetPrivate(psResource, (void**)&pResPri);

    if(pResPri->u32Magic != MAGIC_NUM)
    {
        // not regiser yet
        pResPri->u32Magic = MAGIC_NUM;
        pResPri->bInited_Drv = FALSE;
        pResPri->u32Cavid = DEFAULT_CAVID;
    }

    UtopiaResourceRelease(psResource);
}

MS_U32 DSCMBOpen(void** ppInstance, MS_U32 u32ModuleVersion, void* pAttribute)
{
    DSCMB_MSG("DSCMB open \n");

    if(ppInstance == 0)
    {
        return UTOPIA_STATUS_PARAMETER_ERROR;
    }

    DSCMB_INSTANT_PRIVATE*    psDscmbInstPri = NULL;

    //create instance
    //1.  Allocate instant private data, prepare argument variable.
    //2.  Dst: ppInstance
    UtopiaInstanceCreate(sizeof(DSCMB_INSTANT_PRIVATE), ppInstance);

    //get instant private data pointer
    // 1. Dst: psDscmbInstPri
    UtopiaInstanceGetPrivate(*ppInstance, (void**)&psDscmbInstPri);

    //reset argument variables.
//    memset(&psDscmbInstPri->pInitVector, 0x0, DSCMB_IV_LENGTH);
//    memset(&psDscmbInstPri->pCipherKey, 0x0, DSCMB_KEY_LENGTH);
    memset(&psDscmbInstPri, 0, sizeof(DSCMB_INSTANT_PRIVATE));

    return UTOPIA_STATUS_SUCCESS;
}

MS_U32 DSCMBIoctl(void* pInstance, MS_U32 u32Cmd, void* pu32Args)
{
    void* pstRes = NULL;
    MS_U32 u32Ret = UTOPIA_STATUS_FAIL;
    DSCMB_RESOURCE_PRIVATE* pstResPri  = NULL;
    DSCMB_INSTANT_PRIVATE*  pstInstPri = NULL;

    UtopiaResourceObtain(pModuleDscmb, E_DSCMB_RESOURCE, &pstRes);
    UtopiaResourceGetPrivate(pstRes, (void**)&pstResPri);

    if((FALSE == pstResPri->bInited_Drv)  && (u32Cmd != E_MDRV_CMD_DSCMB_Init))
    {
        UtopiaResourceRelease(pstRes);
        return UTOPIA_STATUS_FAIL;
    }

    //set pointer from resource private to local file pointer variable
    //the local file pointer variable will extern to drvDSCMB.c
    _bFreeSlot       = pstResPri->bFreeSlot;
    _u32PidFlt2Dscmb = pstResPri->u32PidFlt2Dscmb;
    DscmbTspMap      = pstResPri->DscmbTspMap;

    switch(u32Cmd)
    {
        // General API
        case E_MDRV_CMD_DSCMB_Init:
        {
            if(pstResPri->bInited_Drv == FALSE)
            {
                if(TRUE == _MDrv_DSCMB2_Init())
                {
                     pstResPri->bInited_Drv = TRUE;
                }
                else
                {
                    DSCMB_MSG("E_MDRV_CMD_DSCMB_Init fail\n");
                }
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_Init\n");

            u32Ret = UTOPIA_STATUS_SUCCESS;
        }
        break;

        case E_MDRV_CMD_DSCMB_Exit:
        {
            if(TRUE == _MDrv_DSCMB2_Exit())
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_Exit\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_SetPowerState:
        {
            EN_POWER_MODE * pPowereMode = (EN_POWER_MODE *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_SetPowerState(*pPowereMode))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_SetPowerState,\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltAlloc:
        {
            DSCMB_FLTALLOC* pFltAlloc = (DSCMB_FLTALLOC *)pu32Args;

            pFltAlloc->u32DscmbId = _MDrv_DSCMB2_FltAlloc_Ex(pFltAlloc->u32EngId,
                                                             pFltAlloc->eFltType);
            if(pFltAlloc->u32DscmbId != DRV_DSCMB_FLT_NULL)
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltAlloc\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltFree:
        {
            DSCMB_FLTFREE * pFltFree = (DSCMB_FLTFREE *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltFree(pFltFree->u32EngId, pFltFree->u32DscmbId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltFree\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltConnectFltId:
        {
            DSCMB_CONNECTFLT * pConnectFlt = (DSCMB_CONNECTFLT *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltConnectFltId(pConnectFlt->u32EngId,
                                                    pConnectFlt->u32DscmbId,
                                                    pConnectFlt->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltConnectFltId\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_FltConnectPid:
        {
            DSCMB_CONNECTPID * pstConnectPid = (DSCMB_CONNECTPID *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltConnectPid(
                        pstConnectPid->u32EngId,
                        pstConnectPid->u32DscmbId,
                        pstConnectPid->u32Pid))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltConnectPid\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltDisconnectFltId:
        {
            DSCMB_DISCONNECTFLT * pDisconnectFlt = (DSCMB_DISCONNECTFLT *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltDisconnectFltId(pDisconnectFlt->u32EngId,
                                                       pDisconnectFlt->u32DscmbId,
                                                       pDisconnectFlt->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltDisconnectFltId\n");
        }
        break;


        case E_MDRV_CMD_DSCMB_FltDisconnectPid:
        {
            DSCMB_DISCONNECTPID * pstDisconnectPid = (DSCMB_DISCONNECTPID *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltDisconnectPid_Ex(pstDisconnectPid->u32EngId,
                    pstDisconnectPid->u32DscmbId,
                    pstDisconnectPid->u32Pid))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltDisconnectPid\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltDscmb:
        {
            DSCMB_FLTDSCMB * pstFltDscmb = (DSCMB_FLTDSCMB *)pu32Args;

            if(TRUE == _MDrv_DSCMB2_FltDscmb(pstFltDscmb->u32EngId,
                                             pstFltDscmb->u32DscmbId,
                                             pstFltDscmb->bDscmb))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltDscmb\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltTypeSet:
        {
            DSCMB_TYPESET *pstTypeSet = (DSCMB_TYPESET *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltTypeSet(pstTypeSet->u32EngId,
                                               pstTypeSet->u32DscmbId,
                                               pstTypeSet->eType))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltTypeSet\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltKeySet:
        {
            DSCMB_FLTKEYSET * pstFltKeySet = (DSCMB_FLTKEYSET *)pu32Args;

            if(TRUE == _MDrv_DSCMB2_FltKeySet(pstFltKeySet->u32EngId,
                                              pstFltKeySet->u32DscmbId,
                                              pstFltKeySet->eKeyType,
                                              pstFltKeySet->pu8Key))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltKeySet\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltKeyReset:
        {
            DSCMB_FLTKEYRESET * pstFltKeyReset = (DSCMB_FLTKEYRESET *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_FltKeyReset(pstFltKeyReset->u32EngId,
                                                pstFltKeyReset->u32DscmbId,
                                                pstFltKeyReset->eKeyType))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltKeyReset\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_FltIVSet:
        {
            UtopiaInstanceGetPrivate(pInstance, (void**)&pstInstPri);
            DSCMB_FLTIVSET * pstFltIVSet = (DSCMB_FLTIVSET *)pu32Args;

            if(TRUE == _MDrv_DSCMB2_FltIVSet(pstFltIVSet->u32EngId,
                                             pstFltIVSet->u32DscmbId,
                                             pstFltIVSet->eKeyType,
                                             pstFltIVSet->pu8IV))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_FltIVSet\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_HDCP2_SetRiv:
        {
            DSCMB_HDCP2_SETRIV * pstSetRiv = (DSCMB_HDCP2_SETRIV *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_HDCP2_SetRIV(pstSetRiv->u32EngId,
                                                 pstSetRiv->pu8Riv))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_HDCP2_SetRiv\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_Multi2_SetRound:
        {
            DSCMB_MULTI2_SETROUND * pstSetRnd = (DSCMB_MULTI2_SETROUND *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_Multi2_SetRound(pstSetRnd->u32EngId,
                                                    pstSetRnd->u32Round))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
        }
        break;

        case E_MDRV_CMD_DSCMB_Multi2_SetSysKey:
        {
            DSCMB_MULTI2_SETSYSKEY * psSetSysKey = (DSCMB_MULTI2_SETSYSKEY *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_Multi2_SetSystemKey(psSetSysKey->u32EngId,
                    (MS_U8*)psSetSysKey->pu8SysKey))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
        }
        break;

        case E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus:
        {
            DSCMB_DSCMBSTATUS * pDscmbStatus = (DSCMB_DSCMBSTATUS *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_PidFlt_ScmbStatus(pDscmbStatus->u32EngId,
                    pDscmbStatus->u32DmxFltId,
                    pDscmbStatus->pScmbLevel))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_PidFlt_ScmbStatus\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_PVR_RecCtrl:
        {
            DSCMB_PVR_RECCTRL * pPvrRecCtrl = (DSCMB_PVR_RECCTRL *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_PVR_RecCtrl(pPvrRecCtrl->u32EngId,
                                                      pPvrRecCtrl->bEnable))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_PVR_RecCtrl\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_SetDefaultCAVid:
        {
            DSCMB_DEFAULT_CAVID * pu32DefaultVid = (DSCMB_DEFAULT_CAVID *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_SetDefaultCAVid(pu32DefaultVid->u32EngId, pu32DefaultVid->u32CAVid))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_SetDefaultCAVid\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetAlgo:
        {
            DSCMB_ENGALGO * pAlgo = (DSCMB_ENGALGO *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetAlgo(pAlgo->u32EngId, pAlgo->u32DscmbId, pAlgo->eEngType, pAlgo->stConfig))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetAlgo\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetKey:
        {
            DSCMB_ENGKEY * pKey = (DSCMB_ENGKEY *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetKey(pKey->u32EngId, pKey->u32DscmbId, pKey->eEngType, pKey->eKeyType, pKey->pu8Key))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetKey\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngResetKey:
        {
            DSCMB_ENGRESETKEY * pKey = (DSCMB_ENGRESETKEY *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngResetKey(pKey->u32EngId, pKey->u32DscmbId, pKey->eEngType, pKey->eKeyType))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngResetKey\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetIV:
        {
            DSCMB_ENGIV * pIV = (DSCMB_ENGIV *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetIV(pIV->u32EngId, pIV->u32DscmbId, pIV->eKeyType, pIV->pu8IV))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetIV\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetSwitch:
        {
            DSCMB_ENGSWITCH * pSwitch = (DSCMB_ENGSWITCH *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetSwitch(pSwitch->u32EngId, pSwitch->u32DscmbId, pSwitch->eUppSwitch, pSwitch->eLowSwitch))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetSwitch\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_EngSetFSCB:
        {
            DSCMB_ENGFSCB * pFSCB = (DSCMB_ENGFSCB *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_EngSetFSCB(pFSCB->u32EngId, pFSCB->u32DscmbId, pFSCB->eForceSCB))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_EngSetFSCB\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_KLadder_AtomicExec:
        {
            DSCMB_KL_ATOMICEXEC * pKLArgs = (DSCMB_KL_ATOMICEXEC *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_KLadder_AtomicExec(pKLArgs->KLCfg,
                                                       pKLArgs->ACPU_Out,
                                                       pKLArgs->u32Status))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_KLadder_AtomicExec\n");

        }
        break;

        case E_MDRV_CMD_DSCMB_KLadder_ETSI:
        {
            DSCMB_KL_ETSI * pKLArgs = (DSCMB_KL_ETSI *)pu32Args;
            if(TRUE == _MDrv_DSCMB2_KLadder_ETSI(pKLArgs->KLCfg,
                                                 pKLArgs->ACPU_Out,
                                                 pKLArgs->pu8Nonce,
                                                 pKLArgs->pu8Response,
                                                 pKLArgs->u32Status))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_KLadder_ETSI\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_QueryCap:
        {
            DSCMB_QUERYCAP* pGetCap = (DSCMB_QUERYCAP*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_GetCap(pGetCap->u32EngId,
                                           pGetCap->eQueryType,
                                           pGetCap->pInput,
                                           pGetCap->pOutput))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_GetCap\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_SetDBGLevel:
        {
            MS_U32* pDBGArgs = (MS_U32*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_SetDBGLevel(*pDBGArgs))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_SetDBGLevel\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_GetLibVer:
        {
            const MSIF_Version** pLibVerArgs = (const MSIF_Version**)pu32Args;
            if(TRUE == _MDrv_DSCMB2_GetLibVer(pLibVerArgs))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }

            DSCMB_MSG("E_MDRV_CMD_DSCMB_GetLibVer\n");
        }
        break;

        case E_MDRV_CMD_DSCMB_GetConnectStatus:
        {
            DSCMB_CONNECTSTATUS* pStatusArgs = (DSCMB_CONNECTSTATUS*)pu32Args;
            if(TRUE == _MDrv_DSCMB2_GetConnectStatus (pStatusArgs->u32EngId,
                                                      pStatusArgs->u32DscmbId,
                                                      pStatusArgs->u32DmxFltId))
            {
                u32Ret = UTOPIA_STATUS_SUCCESS;
            }
            DSCMB_MSG("E_MDRV_CMD_DSCMB_GetConnectStatus\n");
        }
        break;

        default:
            DSCMB_MSG("DSCMBIoctl - Unknown commend 0x%x!!!\n", (int)u32Cmd);
            return UTOPIA_STATUS_FAIL;
    }


    UtopiaResourceRelease(pstRes);

    return u32Ret;
}

MS_U32 DSCMBClose(void* pInstance)
{

    UtopiaInstanceDelete(pInstance);

    return UTOPIA_STATUS_SUCCESS;
}

