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

#ifndef DRV_SC_MENULOAD_H
#define DRV_SC_MENULOAD_H

#include "mhal_menuload.h"

#define MLOAD_MUTEX
#define MLOAD_MUTEX_DBG	0


#ifdef DRV_SC_MENULOAD_C
#define INTERFACE
INTERFACE MS_S32            _MLOAD_MUTEX = -1;
#else
#define INTERFACE extern
INTERFACE MS_S32            _MLOAD_MUTEX;
#endif

#define _MLOAD_MUTEX_TIME_OUT                 MSOS_WAIT_FOREVER

#ifdef MLOAD_MUTEX
#if(MLOAD_MUTEX_DBG)
#define _MLOAD_ENTRY(pInstance)                                                                  \
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_MLD);                                      \
            printf("0,==========================Prepare to get mutex\n");                             \
            printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
            if(!MsOS_ObtainMutex(_MLOAD_MUTEX, _MLOAD_MUTEX_TIME_OUT))                     \
            {                                                                        \
                printf("==========================\n");                              \
                printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
            }\
            printf("[%s][%06d] menuload mutex got\n",__FUNCTION__,__LINE__);

#define _MLOAD_RETURN(pInstance)                                                                \
            printf("1,==========================\n");                              \
            printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
            MsOS_ReleaseMutex(_MLOAD_MUTEX);                                          \
            printf("==========================prepare to release semaphore\n");                    \
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_MLD);                           \
            printf("[%s][%06d] menuload mutex released\n",__FUNCTION__,__LINE__);
                                //return _ret;
#else
#define _MLOAD_ENTRY(pInstance)                                                                  \
            _XC_SEMAPHORE_ENTRY(pInstance,E_XC_ID_MLD);                  \
            if(!MsOS_ObtainMutex(_MLOAD_MUTEX, _MLOAD_MUTEX_TIME_OUT))                     \
            {                                                                        \
            }
#define _MLOAD_RETURN(pInstance)                                                                    \
            MsOS_ReleaseMutex(_MLOAD_MUTEX);                                               \
            _XC_SEMAPHORE_RETURN(pInstance,E_XC_ID_MLD);
                                //return _ret;

#endif
#else // #if not def MLOAD_MUTEX
#define _MLOAD_ENTRY()         while(0)
#define _MLOAD_RETURN()    while(0)
#endif // #ifdef _MLOAD_MUTEX

typedef struct __attribute__((packed))
{
    MS_U16 u16WPoint;
    MS_U16 u16RPoint;
    MS_U16 u16FPoint;
    MS_PHY PhyAddr;
    MS_U16 u16MaxCmdCnt;
    MS_BOOL bEnable;
}MS_MLoad_Info;

MS_BOOL MDrv_XC_MLoad_Check_Done(void *pInstance);
void MDrv_XC_MLoad_Wait_HW_Done(void *pInstance);
void  MDrv_XC_MLoad_Init(void *pInstance, MS_PHY phyAddr);
void MDrv_XC_MLoad_Trigger(void *pInstance, MS_PHY startAddr, MS_U16 u16CmdCnt);
void MDrv_XC_MLoad_AddCmd(void *pInstance, MS_U32 u32Cmd);
MS_U32 MDrv_XC_MLoad_GetCmd(MS_PHY DstAddr); //NO_NEED
MS_U64 MDrv_XC_MLoad_GetCmd_64Bits(MS_PHY DstAddr);
MS_BOOL MDrv_XC_MLoad_BufferEmpty(void *pInstance);
MS_BOOL MDrv_XC_MLoad_KickOff(void *pInstance);
MS_BOOL MDrv_XC_MLoad_GetCaps(void *pInstance);
void MDrv_XC_MLoad_AddNull(void *pInstance);
void MDrv_XC_MLoad_Add_32Bits_Cmd(void *pInstance,MS_U32 u32Cmd, MS_U16 u16Mask);
void MDrv_XC_MLoad_Add_64Bits_Cmd(void *pInstance,MS_U64 u64Cmd, MS_U16 u16Mask);
MS_BOOL MDrv_XC_MLoad_WriteCommand(void *pInstance,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MApi_XC_MLoad_WriteCmd_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MDrv_XC_MLoad_WriteCommand_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);


void MApi_XC_MLoad_Init(MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void MApi_XC_MLoad_Cus_Init(EN_MLOAD_CLIENT_SELECT eMloadSelect, MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void MApi_XC_MLoad_Enable(MS_BOOL bEnable);
void MApi_XC_MLoad_Cus_Enable(EN_MLOAD_CLIENT_SELECT eMloadSelect, MS_BOOL bEnable);
MLOAD_TYPE MApi_XC_MLoad_GetStatus(void);
MS_BOOL MApi_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmeidate);
MS_BOOL MDrv_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmediate);
MS_BOOL MApi_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
MS_BOOL MDrv_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);

void MApi_XC_MLG_Init(MS_PHY PhyAddr, MS_U32 u32BufByteLen);
void MApi_XC_MLG_Enable(MS_BOOL bEnable);
MS_BOOL MApi_XC_MLG_GetCaps(void);
MLG_TYPE MApi_XC_MLG_GetStatus(void);
void MApi_XC_MLG_Fire(MS_U8 *pR, MS_U8 *pG, MS_U8 *pB, MS_U16 u16Count, MS_U16 *pMaxGammaValue);
MLOAD_TYPE MDrv_XC_MLoad_GetStatus(void *pInstance);
void MDrv_XC_MLoad_Enable(void *pInstance,MS_BOOL bEnable);
MS_BOOL MDrv_XC_MLoad_set_IP_trig_p(void *pInstance, MS_U16 u16train, MS_U16 u16disp);
MS_BOOL MDrv_XC_MLoad_get_IP_trig_p(void *pInstance, MS_U16 *pu16Train, MS_U16 *pu16Disp);
void MDrv_XC_MLoad_set_trigger_sync(void *pInstance, MLoad_Trigger_Sync eTriggerSync);

#undef INTERFACE
#endif

