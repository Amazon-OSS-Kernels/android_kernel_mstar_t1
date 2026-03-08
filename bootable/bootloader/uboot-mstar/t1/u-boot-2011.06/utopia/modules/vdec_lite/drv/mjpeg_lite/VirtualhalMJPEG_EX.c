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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    halMJPEG.c
/// @brief  MJPEG hal interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#if (!defined(MSOS_TYPE_NUTTX) && !defined(MSOS_TYPE_OPTEE)) || defined(SUPPORT_X_MODEL_FEATURE)

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#include "MsCommon.h"
#include "MsOS.h"
#include "halCHIP.h"
// Internal Definition
//#include "regCHIP.h"
#include "drvMMIO.h" //get RIU base
#include "drvMJPEG.h"
#include "VirtualhalMJPEG.h"

#include "regVPU_EX.h" // For VPU
#include "halVPU_EX.h"


#include "drvSYS.h"

#include "../hvd_lite/fwHVD_if.h"
#define NJPD_FW_VERSION  "NJPD.FW.00130062"

#ifdef VDEC3
#define MAX_MJPEG_DEC_NUM 4
#else
#define MAX_MJPEG_DEC_NUM 2
#endif


static HAL_MJPEG_Stream _stMJPEGStream[MAX_MJPEG_DEC_NUM] =
{
#ifdef VDEC3
    {E_HAL_MJPEG_N_STREAM0, FALSE},
    {E_HAL_MJPEG_N_STREAM0+1, FALSE},
    {E_HAL_MJPEG_N_STREAM0+2, FALSE},
    {E_HAL_MJPEG_N_STREAM0+3, FALSE},
#else
    {E_HAL_MJPEG_MAIN_STREAM0, FALSE},
    {E_HAL_MJPEG_SUB_STREAM0, FALSE},
#endif
};

/*================================ MJPEG =====================================*/

/******************************************************************************/
///Stop VPU
/******************************************************************************/
void VirtualHal_MJPEG_StopVPU(MS_U32 u32Id)
{
    //HAL_VPU_PowerCtrl(FALSE);
    HAL_VPU_EX_DeInit(u32Id);
}

/******************************************************************************/
///Release VPU
/******************************************************************************/
void VirtualHal_MJPEG_ReleaseVPU(MS_U32 u32Id)
{
    HAL_VPU_EX_SwRstRelse(u32Id);
}


/******************************************************************************/
///Check VPU MailBox ready or not
///@return TRUE or FALSE
///    - TRUE, MailBox is free
///    - FALSE, MailBox is busy
///@param u8MBox \b IN: MailBox to check
///     - VPU_HI_MBOX0,
///     - VPU_HI_MBOX1,
///     - VPU_RISC_MBOX0,
///     - VPU_RISC_MBOX1,
/******************************************************************************/
MS_BOOL VirtualHal_MJPEG_IsVPUMBoxReady(MS_U32 u32Id, MS_U8 u8MBox)
{
    MS_BOOL bResult = FALSE;
    bResult = HAL_VPU_EX_MBoxRdy(u32Id, (MS_U32)u8MBox);
    return bResult;
}

/******************************************************************************/
///Read message from VPU
///@return TRUE or FALSE
///    - TRUE, success
///    - FALSE, failed
///@param u8MBox \b IN: MailBox to read
///    - VPU_RISC_MBOX0
///    - VPU_RISC_MBOX1
///@param u32Msg \b OUT: message read
/******************************************************************************/
MS_BOOL VirtualHal_MJPEG_ReadVPUMBox(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 *u32Msg)
{
    MS_BOOL bResult = TRUE;
    bResult = HAL_VPU_EX_MBoxRead(u32Id, (MS_U32)u8MBox, u32Msg);
    return bResult;
}

///-----------------------------------------------------------------------------
/// Send message to VPU
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
/// @param u8MBox \b IN: MailBox
///     - VPU_HI_MBOX0,
///     - VPU_HI_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL VirtualHal_MJPEG_SendVPUMBox(MS_U32 u32Id, MS_U8 u8MBox, MS_U32 u32Msg)
{
    MS_BOOL bResult = TRUE;
    MS_U8 u8TaskId = HAL_VPU_EX_GetTaskId(u32Id);
    u32Msg |= (u8TaskId << 24);

    bResult = HAL_VPU_EX_MBoxSend(u32Id, (MS_U32)u8MBox, u32Msg);
    return bResult;
}

///-----------------------------------------------------------------------------
/// Clear VPU RISC MBox
/// @return TRUE or FALSE
///     - TRUE, Success
///     - FALSE, Failed
/// @param u8MBox \b IN: MailBox
///     - VPU_RISC_MBOX0,
///     - VPU_RISC_MBOX1,
///-----------------------------------------------------------------------------
MS_BOOL VirtualHal_MJPEG_ClearVPUMBox(MS_U32 u32Id, MS_U8 u8MBox)
{
    HAL_VPU_EX_MBoxClear(u32Id, (MS_U32)u8MBox);
    return TRUE;
}

MJPEG_MSTAR_CHIP_ID VirtualHal_MJPEG_Get_Chip_ID(void)
{
    return E_MJPEG_MSTAR_CHIP_T8;
}

HAL_MJPEG_StreamId VirtualHal_MJPEG_GetFreeStream(HAL_MJPEG_StreamType eStreamType)
{
    MS_U32 i = 0;

    if (eStreamType == E_HAL_MJPEG_MAIN_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_MJPEG_MAIN_STREAM_MAX - E_HAL_MJPEG_MAIN_STREAM_BASE) +
              (E_HAL_MJPEG_SUB_STREAM_MAX - E_HAL_MJPEG_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_MJPEG_MAIN_STREAM_BASE & _stMJPEGStream[i].eStreamId) && (FALSE == _stMJPEGStream[i].bUsed))
            {
                return _stMJPEGStream[i].eStreamId;
            }
        }
    }
    else if (eStreamType == E_HAL_MJPEG_SUB_STREAM)
    {
        for (i = 0;
             i <
             ((E_HAL_MJPEG_MAIN_STREAM_MAX - E_HAL_MJPEG_MAIN_STREAM_BASE) +
              (E_HAL_MJPEG_SUB_STREAM_MAX - E_HAL_MJPEG_SUB_STREAM_BASE)); i++)
        {
            if ((E_HAL_MJPEG_SUB_STREAM_BASE & _stMJPEGStream[i].eStreamId) && (FALSE == _stMJPEGStream[i].bUsed))
            {
                return _stMJPEGStream[i].eStreamId;
            }
        }
    }
#ifdef VDEC3
    else if ((eStreamType >= E_HAL_MJPEG_N_STREAM) && (eStreamType < E_HAL_MJPEG_N_STREAM + MAX_MJPEG_DEC_NUM))
    {
        i = eStreamType - E_HAL_MJPEG_N_STREAM;
        if (!_stMJPEGStream[i].bUsed)
            return _stMJPEGStream[i].eStreamId;
    }
#endif

    return E_HAL_MJPEG_STREAM_NONE;
}


void VirtualHal_MJPEG_Init(MS_U8 u8DrvId)
{
    _stMJPEGStream[u8DrvId].bUsed = TRUE;

}

void VirtualHal_MJPEG_DeInit(MS_U8 u8DrvId)
{
    _stMJPEGStream[u8DrvId].bUsed = FALSE;
}


#endif
