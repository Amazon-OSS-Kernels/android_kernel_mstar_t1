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
////////////////////////////////////////////////////////////////////////////////////////////////////
/// file   drvFQ.c
/// @brief  FQ Driver Interface
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "MsCommon.h"
#include "MsVersion.h"
#include "regFQ.h"
#include "halFQ.h"

#include "drvFQ.h"
#include "drvMMIO.h"
#include "asmCPU.h"

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <unistd.h>
    #include <fcntl.h> // O_RDWR
#endif

//--------------------------------------------------------------------------------------------------
//  Debug Macro
//--------------------------------------------------------------------------------------------------
#define MS_DEBUG 1
#if MS_DEBUG
#define FQ_DBG(_fmt,_args...)             {printf("[%s][%d]",__FUNCTION__,__LINE__); printf(_fmt,##_args);}
#else
#define FQ_DBG(_args)                     { }
#endif

//--------------------------------------------------------------------------------------------------
//  Driver Compiler Option
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//  Local Defines
//--------------------------------------------------------------------------------------------------
#define _FQ_ENTRY()       if (!MsOS_ObtainMutex(_s32FQMutexId, MSOS_WAIT_FOREVER))      \
                            {                                                               \
                                _u32LastErr = DRVFQ_FUNC_ERROR;                           \
                                return DRVFQ_FUNC_ERROR;                                  \
                            }                                                               \
                            if (_bSuspend)                                                  \
                            {                                                               \
                                _FQ_RETURN(DRVFQ_FAIL);                                 \
                            }

#define _FQ_RETURN(_ret)  if(MsOS_ReleaseMutex(_s32FQMutexId)) {return _ret;}           \
                            else                                                            \
                            {                                                               \
                                _u32LastErr = DRVFQ_FUNC_ERROR;                           \
                                return DRVFQ_FUNC_ERROR;                                  \
                            }

#define FQ_NUM_CHECK_RET(idx) if(idx >= FQ_NUM ) \
                            { printf("[FQ][ERR] FQ Engine [%d] not exist ! \n", (unsigned int)idx);   \
                             return 0; }

//--------------------------------------------------------------------------------------------------
//  Local variable
//--------------------------------------------------------------------------------------------------
static MS_S32                           _s32FQMutexId = -1;
static MS_U32                           _u32LastErr;
static MS_BOOL                          _bSuspend = FALSE;
//MS_U32                                  _u32FQIntNum = ; //for intr
static MS_BOOL                          _bFQInitReady = FALSE;
//--------------------------------------------------------------------------------------------------
//  Forward declaration
//--------------------------------------------------------------------------------------------------
static void _FQ_RegStateReset(MS_U32 u32FQEng)
{
    HAL_FQ_PVR_SetBuf(u32FQEng, 0, 0);
    HAL_FQ_SkipRushData(u32FQEng, FIQ_CFG11_SKIP_RUSH_DATA_PATH_NON);
    HAL_FQ_PVR_SetRushAddr(u32FQEng, 0);
}

static void _FQ_Init(MS_U32 u32FQEng, MS_U8 u8AddrMode)
{
    HAL_FQ_SWReset(u32FQEng, FALSE);
    HAL_FQ_Bypass(u32FQEng, FALSE);
    HAL_FQ_AddrMode(u32FQEng, u8AddrMode);
}

static void _FQ_Exit(MS_U32 u32FQEng)
{
    HAL_FQ_SWReset(u32FQEng, TRUE);
    HAL_FQ_Bypass(u32FQEng, TRUE);
    HAL_FQ_AddrMode(u32FQEng, FALSE);
}

// General API
//--------------------------------------------------------------------------------------------------
/// Initialize FQ driver and FQ engine
/// @return FQ_Result
/// @note
/// It should be called before calling any other FQ DDI functions.
/// Driver task is in @ref E_TASK_PRI_SYS level.
//--------------------------------------------------------------------------------------------------
FQ_Result MDrv_FQ_Init(MS_U32 u32FQEng, MS_U8 u8AddrMode)
{
    MS_VIRT virtBank;
    MS_PHY  phyBankSize;

    FQ_NUM_CHECK_RET(u32FQEng);

    if (FALSE == MDrv_MMIO_GetBASE(&virtBank, &phyBankSize, MS_MODULE_HW))
    {
        FQ_DBG("MDrv_MMIO_GetBASE fail\n");
        MS_ASSERT(0);
    }

    if(!HAL_FQ_SetBank(virtBank))
    {
        _FQ_RETURN(DRVFQ_FAIL);
    }

    _FQ_Init(u32FQEng, u8AddrMode);

    _FQ_RegStateReset(u32FQEng);
    //default for PCR filter & RASP/PVR, we skip rush data to avoid the older data.
    HAL_FQ_SkipRushData(u32FQEng, FIQ_CFG11_SKIP_PCR_RUSH_DATA | FIQ_CFG11_SKIP_RASP_RUSH_DATA
                                | FIQ_CFG11_SKIP_PVR1_RUSH_DATA|FIQ_CFG11_SKIP_PVR2_RUSH_DATA|FIQ_CFG11_SKIP_PVR3_RUSH_DATA);
    if (-1 == _s32FQMutexId)
        _s32FQMutexId = MsOS_CreateMutex(E_MSOS_FIFO , "FQ_Mutex" , MSOS_PROCESS_SHARED);
    else
        FQ_DBG("Create _s32FQMutexId more than once\n");

    _FQ_ENTRY();
    _bFQInitReady = TRUE;
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_IsInit(MS_U32 u32FQEng, MS_BOOL* pbIsInit)
{
    _FQ_ENTRY();
    *pbIsInit = _bFQInitReady;
    _FQ_RETURN(DRVFQ_OK);
}

//--------------------------------------------------------------------------------------------------
/// Close FQ driver and reset software/register state
/// @return FQ_Result
/// @note
//--------------------------------------------------------------------------------------------------
FQ_Result MDrv_FQ_Exit(MS_U32 u32FQEng)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    if (_s32FQMutexId != -1)
        _FQ_ENTRY();

    if(_bFQInitReady == TRUE)
    {
        _FQ_RegStateReset(u32FQEng);
        _FQ_Exit(u32FQEng);

        _bFQInitReady = FALSE;
    }

    if (_s32FQMutexId != -1)
    {
        MsOS_ReleaseMutex(_s32FQMutexId);
        MsOS_DeleteMutex(_s32FQMutexId);
        _s32FQMutexId = -1;
    }

    return DRVFQ_OK;
}

FQ_Result MDrv_FQ_SetBuffer(MS_U32 u32FQEng, MS_PHY phyBufStart, MS_U32 u32BufSize)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    _FQ_ENTRY();
    HAL_FQ_PVR_SetBuf(u32FQEng, phyBufStart, u32BufSize);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_Start(MS_U32 u32FQEng, MS_BOOL bStart)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    _FQ_ENTRY();

    if (!bStart)
        HAL_FQ_PVR_Stop(u32FQEng);
    else
        HAL_FQ_PVR_Start(u32FQEng);

    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_PVR_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyRushAddr)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    _FQ_ENTRY();
    HAL_FQ_PVR_SetRushAddr(u32FQEng, phyRushAddr);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_SkipRushData(MS_U32 u32FQEng, MS_U32 u32SkipPath)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    _FQ_ENTRY();
    HAL_FQ_SkipRushData(u32FQEng, u32SkipPath);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_RushEnable(MS_U32 u32FQEng)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    _FQ_ENTRY();
    HAL_FQ_Rush_Enable(u32FQEng);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_TimeStampSetRecordStamp(MS_U32 u32FQEng, MS_U32 u32Stamp)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    _FQ_ENTRY();
    HAL_FQ_SetPVRTimeStamp(u32FQEng, u32Stamp);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_TimeStampGetRecordStamp(MS_U32 u32FQEng, MS_U32* u32Stamp)
{
    FQ_NUM_CHECK_RET(u32FQEng);
    _FQ_ENTRY();
    *u32Stamp = HAL_FQ_GetPVRTimeStamp(u32FQEng);
    _FQ_RETURN(DRVFQ_OK);
}

FQ_Result MDrv_FQ_SetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf eTsSrc)
{
    MS_U32 u32FQSrc = 0;
    MS_U32 u32Ret = DRVFQ_OK;

    FQ_NUM_CHECK_RET(u32FQEng);

    _FQ_ENTRY();

    switch(eTsSrc)
    {
        case E_DRVFQ_SRC_TSIF0:
            u32FQSrc = 0;
            break;
        case E_DRVFQ_SRC_TSIF1:
            u32FQSrc = 1;
            break;
        case E_DRVFQ_SRC_TSIF2:
            u32FQSrc = 2;
            break;
        case E_DRVFQ_SRC_TSIF3:
            u32FQSrc = 3;
            break;
        case E_DRVFQ_SRC_TSIFFI:
            u32FQSrc = 7;
            break;
        default:
            _FQ_RETURN(DRVFQ_INVALID_PARAM);
    }

    if(!HAL_FQ_SetMuxSwitch(u32FQEng,u32FQSrc))
        u32Ret = DRVFQ_FAIL;

    _FQ_RETURN(u32Ret);
}

FQ_Result MDrv_FQ_GetMuxSwitch(MS_U32 u32FQEng, DrvFQ_SrcIf* peTsSrc)
{
    MS_U32 u32FQSrc = 0xFF;
    MS_U32 u32Ret = DRVFQ_OK;

    FQ_NUM_CHECK_RET(u32FQEng);

    _FQ_ENTRY();

    u32FQSrc = HAL_FQ_GetMuxSwitch(u32FQEng);

    switch(u32FQSrc)
    {
        case 0:
            *peTsSrc = E_DRVFQ_SRC_TSIF0;
            break;
        case 1:
            *peTsSrc = E_DRVFQ_SRC_TSIF1;
            break;
        case 2:
            *peTsSrc = E_DRVFQ_SRC_TSIF2;
            break;
        case 3:
            *peTsSrc = E_DRVFQ_SRC_TSIF3;
            break;
        case 7:
            *peTsSrc = E_DRVFQ_SRC_TSIFFI;
            break;
        default:
            *peTsSrc = 0xFF;
            _FQ_RETURN(DRVFQ_FAIL);
    }

    _FQ_RETURN(u32Ret);
}

#ifdef MSOS_TYPE_LINUX_KERNEL
FQ_Result MDrv_FQ_Suspend(void)
{
    if(_bFQInitReady == TRUE)
    {
        HAL_FQ_SaveRegs();
    }

    return DRVFQ_OK;
}

FQ_Result MDrv_FQ_Resume(void)
{
    if(_bFQInitReady == TRUE)
    {
        HAL_FQ_RestoreRegs();
    }

    return DRVFQ_OK;
}
#endif  //MSOS_TYPE_LINUX_KERNEL


