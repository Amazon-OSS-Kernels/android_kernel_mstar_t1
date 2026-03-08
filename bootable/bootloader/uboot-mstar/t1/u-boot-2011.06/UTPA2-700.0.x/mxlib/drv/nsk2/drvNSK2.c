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
///////////////////////////////////////////////////////////////////////////////
#define _DRVNSK2_C

////////////////////////////////////////////////////////////////////////////////
/// file drvNSK2.c
/// @brief
////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include "string.h"
#endif

#include "MsCommon.h"
#include "MsVersion.h"
#include "drvMMIO.h"
#include "MsOS.h"
#include "asmCPU.h"
#include "halCHIP.h"

#include "drvNSK2.h"
#include "halNSK2.h"
#include "drvNSK2Type.h"


////////////////////////////////////////////////////////////////////////////////
// Local defines & local structures
////////////////////////////////////////////////////////////////////////////////

static MS_U32 _g32NSK2DbgLv = NSK2_DBGLV_DEBUG;

#define DRVNSK2_DBG(lv, x, args...)   if (lv <= _g32NSK2DbgLv ) \
                                        {printf(x, ##args);}

#define NSK2_TASK_STACK_SIZE        16*1024

#define NDS_NSK2_IRQ                E_INT_IRQ_CA_NSK_INT//E_INT_IRQ_CA_SVP

#define NSK2_INT_EVENT_0            0x00000001
#define NSK2_INT_GROUP              0x0000FFFF


////////////////////////////////////////////////////////////////////////////////
// Local Variables
////////////////////////////////////////////////////////////////////////////////
static MS_U32  u32NSK2_Stack[NSK2_TASK_STACK_SIZE/sizeof(MS_U32)];
static MS_S32  s32NSK2_TaskId = -1;
static MS_S32  s32NSK2_EventId = -1;


static NSK2_IntNotify _gNs2IntNtf = NULL;


////////////////////////////////////////////////////////////////////////////////
// Global Variables
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// External funciton
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
// Local Function
////////////////////////////////////////////////////////////////////////////////


static void _NSK2_Isr(void)
{
    //MsOS_DisableInterrupt(NDS_NSK2_IRQ);
    MsOS_SetEvent(s32NSK2_EventId, NSK2_INT_EVENT_0);
}


static void _NSK2_Isr_Task(void)
{
    MS_U32              u32Reg;
    MS_U32              u32Events = 0;

    while(1)
    {
        MsOS_WaitEvent(s32NSK2_EventId, NSK2_INT_GROUP, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);
        //MsOS_DisableInterrupt(NDS_NSK_IRQ);
        DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"s32NSK2_EventId = %ld\n",s32NSK2_EventId);

        if (u32Events & NSK2_INT_EVENT_0)
        {
            u32Reg = HAL_NSK2_GetIntStatus();

            HAL_NSK2_ClearInt(u32Reg);
            if(_gNs2IntNtf != NULL)
            {
                _gNs2IntNtf(u32Reg, 0);
            }
        }
        else
        {
            u32Reg = HAL_NSK2_GetIntStatus();

            HAL_NSK2_ClearInt(u32Reg);
        }
        MsOS_EnableInterrupt(NDS_NSK2_IRQ);

        MsOS_DelayTask(1);
    }
}


////////////////////////////////////////////////////////////////////////////////
// Global Function
////////////////////////////////////////////////////////////////////////////////


MS_U32 MDrv_NSK2_Init(void)
{
    //init clock, register bank
    MS_U32 u32Base, u32BankSize;      // Non-PM bank

    DRVNSK2_DBG(NSK2_DBGLV_INFO,"enter %s \n", __FUNCTION__);

    s32NSK2_EventId = MsOS_CreateEventGroup("NDS_NSK2_Event");
    if (s32NSK2_EventId < 0)
    {
        return FALSE;
    }

    s32NSK2_TaskId = MsOS_CreateTask((TaskEntry)_NSK2_Isr_Task,
                                    (MS_U32)NULL,
                                    E_TASK_PRI_HIGH,
                                    TRUE,
                                    u32NSK2_Stack,
                                    NSK2_TASK_STACK_SIZE,
                                    "NDS_NSK2_Task");
    if (s32NSK2_TaskId < 0)
    {
        MsOS_DeleteEventGroup(s32NSK2_EventId);
        return FALSE;
    }


    if (FALSE == MDrv_MMIO_GetBASE(&u32Base, &u32BankSize, MS_MODULE_HW))
    {
        DRVNSK2_DBG(NSK2_DBGLV_EMERG,"%s, %d MDrv_MMIO_GetBASE (NonPM base)fail\n", __FUNCTION__,__LINE__);
        MS_ASSERT(0);
    }

    HAL_NSK2_SetBase(u32Base); //set non pm bank
    HAL_NSK2_Init();

    //HAL_NSK2_SWReset();

    MsOS_DisableInterrupt(NDS_NSK2_IRQ);
    MsOS_DetachInterrupt(NDS_NSK2_IRQ);
    MsOS_AttachInterrupt(NDS_NSK2_IRQ, (InterruptCb)_NSK2_Isr);
    MsOS_EnableInterrupt(NDS_NSK2_IRQ);


    return TRUE;
}

MS_U32 MDrv_NSK2_ColdReset(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_ColdReset();
    return TRUE;
}

MS_U32 MDrv_NSK2_Compare(MS_U32 StartAddr, MS_U32 CompareLens, MS_U32 CompareSim,
                         MS_U32 Mask, MS_U32 ExpectResult)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_Compare(StartAddr, CompareLens, CompareSim, Mask,  ExpectResult);
}

MS_U32 MDrv_NSK2_CompareMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 CompareLens,
                            MS_U32 CompareSim, MS_U32 ExpectResult, void *pGolden)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CompareMem(reserved, StartAddr, CompareLens, CompareSim, ExpectResult, pGolden);
}

MS_U32 MDrv_NSK2_WriteMem(MS_U32 reserved, MS_U32 StartAddr, MS_U32 WriteLens, void *pWriteData)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_WriteMem(reserved,StartAddr,WriteLens,pWriteData);
    return TRUE;
}


MS_U32 MDrv_NSK2_WriteSFR(MS_U32 StartAddr, MS_U32 Data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_WriteSFR(StartAddr, Data);
    return TRUE;
}

MS_U32 MDrv_NSK2_BasicInitializationComplete(void)
{
    //the chip should manager internal variables NSKactive and ConcurrencyMode
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);

    HAL_NSK2_NSKBasicInitializationComplete();
    return TRUE;
}

MS_U32 MDrv_NSK2_WriteESA(MS_U8 ESASelect, MS_U8 ESASubSelect, MS_U8 pid_no)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);

    HAL_NSK2_WriteESA(ESASelect, ESASubSelect, pid_no);
    return TRUE;

}

MS_U32 MDrv_NSK2_WriteTransportKey(MS_U8 SCB, MS_U8 ForceSCB, void *pLabel, MS_U8 pid_no )
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    DRVNSK2_DBG(NSK2_DBGLV_INFO,"SCB = %x, ForceSCB = %x\n",SCB,ForceSCB);


    HAL_NSK2_WriteTransportKey(SCB,ForceSCB,pLabel,pid_no);
    return TRUE;
}

MS_U32 MDrv_NSK2_CompareKTE(MS_U32 reserved_1, MS_U32 reserved_2, void *pLabel)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CompareKTE(reserved_1, reserved_2, pLabel);
}

MS_U32 MDrv_NSK2_CompareOut(MS_U32 reserved_1, MS_U32 reserved_2, MS_U32 HighDWord, MS_U32 LowDWord)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    return HAL_NSK2_CompareOut( reserved_1, reserved_2, HighDWord, LowDWord);
}

MS_U32 MDrv_NSK2_SetRNG(MS_U32 reserved_1, MS_U16 RNG_Value)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);

    HAL_NSK2_SetRNG( reserved_1, RNG_Value);
    return TRUE;
}


MS_U32 MDrv_NSK2_DriveKteAck(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s \n", __FUNCTION__);
    HAL_NSK2_DriveKteAck();
    return TRUE;
}

MS_U32 MDrv_NSK2_WriteSCPUKey(void)
{
    HAL_NSK2_WriteSCPUKey();
	return TRUE;

}

MS_U32 MDrv_NSK2_WriteM2MKey(void *pIV, MS_U8 SubAlgo)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, SubAlgo = %x \n", __FUNCTION__,SubAlgo);

    //can't find the algorithm meaning
    HAL_NSK2_WriteM2MKey(pIV,SubAlgo);
    return TRUE;
}

// TODO: need to remove and check by MW
MS_U32 MDrv_NSK2_FillJTagPswd(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_SetJTagPswd();
    return TRUE;
}

// TODO: need to remove and check by MW
MS_U32 MDrv_NSK2_CheckPubOTPConfig(void *pCheck)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_CheckPubOTPConfig(pCheck);
}

// TODO:Check OTP control password here
MS_U32 MDrv_NSK2_UnlockOTPCtrl(void)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    HAL_NSK2_UnlockOTPCtrl();
    return TRUE;
}


MS_U32 MDrv_NSK2_ReadData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_ReadData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData8(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData8(MS_U32 addr_offset, MS_U32 data_size, MS_U8 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData8(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_ReadData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_ReadData32(addr_offset, data_size, data);
}

MS_U32 MDrv_NSK2_WriteData32(MS_U32 addr_offset, MS_U32 data_size, MS_U32 *data)
{
    DRVNSK2_DBG(NSK2_DBGLV_DEBUG,"%s \n", __FUNCTION__);
    return HAL_NSK2_WriteData32(addr_offset, data_size, data);
}


MS_U32 MDrv_NSK2_SetIntNotify(NSK2_IntNotify IntNotify)
{
    _gNs2IntNtf = IntNotify;
    return TRUE;
}


MS_U32 MDrv_NSK2_ExcuteCmd(MS_U32 command, MS_U32 control,
                           MS_U32 register_offset, MS_U32 data_size,
                           const MS_U8    *data)
{
    MS_U32 u32Status;
    HAL_NSK2_WriteControl(control);

    if(data_size != 0)
    {
        DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s data_size = %lx\n", __FUNCTION__,data_size);
        u32Status = HAL_NSK2_WriteData8(register_offset, data_size, ( MS_U8 *)data);
        if(TRUE != u32Status)
        {
            DRVNSK2_DBG(NSK2_DBGLV_ERR,"%s, write excute cmd error", __FUNCTION__);
        }
    }

    HAL_NSK2_WriteCommand(command);
    return TRUE;
}




MS_U32 MDrv_NSK2_GetMaxXConn(void)
{
    return HAL_NSK2_GetMaxXConn();
}

MS_U32 MDrv_NSK2_CMChannelNum(void)
{
    return HAL_NSK2_CMChannelNum();
}

MS_U32 MDrv_NSK2_GetOTPFieldValue(MS_U8 u8Tag, MS_U8 *pLength, MS_U32 *pRetValue)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, u8Tag = %x\n", __FUNCTION__, u8Tag);

    return HAL_NSK2_GetOTPFieldValue(u8Tag, pLength, pRetValue);
}

void MDrv_NSK2_SetDbgLevel(MS_U32 u32Level)
{
    _g32NSK2DbgLv = u32Level;
    DRVNSK2_DBG(NSK2_DBGLV_INFO, "%s level: %lx\n", __FUNCTION__, u32Level);
    HAL_NSK2_SetDbgLevel(u32Level);
}

void MDrv_NSK2_SetPollingCnt(MS_U32 u32Cnt)
{
    HAL_NSK2_SetPollingCnt(u32Cnt);
}

void MDrv_NSK2_RunFree(MS_BOOL bRunFree)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO,"%s, bRunFree = %d\n", __FUNCTION__, bRunFree);

	HAL_NSK2_RunFree(bRunFree);
}

void MDrv_NSK2_PushSlowClock ( MS_BOOL HaltClk, MS_U32 TenSecond)
{
	HAL_NSK2_PushSlowClock(HaltClk, TenSecond);
}

void MDrv_NSK2_GetRNGThroughPut(void *pRngData, MS_U32 u32DataSize, MS_BOOL bDump)
{
    DRVNSK2_DBG(NSK2_DBGLV_INFO, "%s enter\n", __FUNCTION__);
    HAL_NSK2_GetRNGThroughPut(pRngData, u32DataSize, bDump);
}

#if 0
void MDrv_NSK2_BurstLen(MS_U32 u32PVREng, MS_U32 u32BurstMode)
{
	HAL_NSK2_BurstLen(u32PVREng, u32BurstMode);
}
#endif

//=====================================
// =========== Debug Usage=================
//=====================================
void DBG_NSK2_AllTSPPidFilter(void)
{
    HAL_NSK2_AllTSPPidFilter();
}

void DBG_NSK2_ChangePidFilter(MS_U32 pid_no, MS_U32 Data)
{
    HAL_NSK2_ChangePidFilter(pid_no, Data);
}

