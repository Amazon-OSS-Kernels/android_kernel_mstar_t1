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
//
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDMX_tee.c
/// @brief  Demux TEE API
////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"

#include "drvTSP_tee.h"
#include "apiDMX_tee.h"

#include "utopia.h"
#include "halCHIP.h"

#include "../../drv/mbx/apiMBX_St.h"
#include "../../drv/mbx/apiMBX.h"

//------------------------------------------------------------------------------
// Compile options
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
// for debugging test only

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define DMX_STACK_SIZE          4096
#define PRINTF(x)               //x

//------------------------------------------------------------------------------
// Constant definition
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Internal data structure
//------------------------------------------------------------------------------
typedef enum
{
    E_DMX_REE_TO_TEE_MBX_MSG_NULL,
    E_DMX_REE_TO_TEE_MBX_MSG_FW_LoadCode,               //None parameters
    E_DMX_REE_TO_TEE_MBX_MSG_SET_VqBuf,                 //None parameters
    E_DMX_REE_TO_TEE_MBX_MSG_SET_PvrBuf,                //Param1: Engine id, Param2: Option 1 is reset buffer to 0
    E_DMX_REE_TO_TEE_MBX_MSG_SET_FileinBuf,             //Param1: Engine id, Param2: Buf address, Param3: Buf size
    E_DMX_REE_TO_TEE_MBX_MSG_GET_PvrWPtr                //Param1: Engine id, Param2: Return Buf address
} DMX_REE_TO_TEE_MBX_MSG_TYPE;

typedef enum
{
    E_DMX_TEE_TO_REE_MBX_MSG_NULL,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_INVALID,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_NO_TEE,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS,
    E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL
} DMX_TEE_TO_REE_MBX_ACK_TYPE;

typedef struct _DMX_TEE_PRIV_PARAM
{
    MS_U8   _u8TspFwBufMiuSel;
    MS_U32  _u32TspFwBufSize;
    MS_U32  _u32TspFwBufAddr;

    MS_U8   _u8VQBufMiuSel;
    MS_U32  _u32VQBufSize;
    MS_U32  _u32VQBufAddr;

    MS_U8   _u8PvrBufMiuSel[2];
    MS_U32  _u32PVR_Buf0_Size[2];
    MS_U32  _u32PVR_Buf0_Addr[2];
    MS_U32  _u32PVR_Buf1_Size[2];
    MS_U32  _u32PVR_Buf1_Addr[2];

    MS_U8   _u8FileinMiuSel[3];
    MS_U32  _u32FileinBufAddr[3];
    MS_U32  _u32FileinBufSize[3];
}DMX_TEE_PRIV_PARAM;

//------------------------------------------------------------------------------
// Helper Macros
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Local Variable
//------------------------------------------------------------------------------
MS_U8   _u8DmxTeeStack[DMX_STACK_SIZE];

MS_U8   _u8DmxMbxMsgClass = 0;
MS_S32  _s32Dmx_TEE_EventId = -1;
MS_S32  _s32Dmx_TEE_TaskId = -1;

MBX_Msg _DmxTeeToReeMbxMsg;
MBX_Msg _Dmx_TEE_msg;

static DMX_TEE_PRIV_PARAM stDMX_Tee_Priv_Param =
{
    ._u8TspFwBufMiuSel = 0,
    ._u32TspFwBufSize = 0,
    ._u32TspFwBufAddr = 0,

    ._u8VQBufMiuSel = 0,
    ._u32VQBufSize = 0,
    ._u32VQBufAddr = 0,

    ._u8PvrBufMiuSel = {0,0},
    ._u32PVR_Buf0_Size = {0,0},
    ._u32PVR_Buf0_Addr = {0,0},
    ._u32PVR_Buf1_Size = {0,0},
    ._u32PVR_Buf1_Addr = {0,0},

    ._u8FileinMiuSel = {0,0,0},
    ._u32FileinBufAddr = {0,0,0},
    ._u32FileinBufSize = {0,0,0},
};

//------------------------------------------------------------------------------
// Internal implementation
//------------------------------------------------------------------------------
MS_BOOL _DMX_TEE_BufParamToMbxParam(MS_U32 u32BufAddr, MS_U32 u32Size, MS_U8 u8MiuSel, MBX_Msg *pMsg)
{
    pMsg->u8Parameters[0] = (MS_U8)(u32Size & 0xFFUL);
    pMsg->u8Parameters[1] = (MS_U8)((u32Size >> 8UL) & 0xFFUL);
    pMsg->u8Parameters[2] = (MS_U8)((u32Size >> 16UL) & 0xFFUL);
    pMsg->u8Parameters[3] = (MS_U8)((u32Size >> 24UL) & 0xFFUL);

    pMsg->u8Parameters[4] = (MS_U8)(u32BufAddr & 0xFFUL);
    pMsg->u8Parameters[5] = (MS_U8)((u32BufAddr >> 8UL) & 0xFFUL);
    pMsg->u8Parameters[6] = (MS_U8)((u32BufAddr >> 16UL) & 0xFFUL);
    pMsg->u8Parameters[7] = (MS_U8)((u32BufAddr >> 24UL) & 0xFFUL);
    pMsg->u8Parameters[8] = u8MiuSel;

    pMsg->u8ParameterCount = 9;

    return TRUE;
}

MS_BOOL _DMX_TEE_MbxParamToBufParam(MBX_Msg *pMsg, MS_U32 *pu32BufAddr, MS_U32 *pu32Size, MS_U8* pu8MiuSel)
{
    if(pMsg->u8ParameterCount < 9)
    {
        return FALSE;
    }

    //printf("[%s][%d] %02x %02x %02x %02x %02x %02x %02x %02x\n", __FUNCTION__, __LINE__,
    //    pMsg->u8Parameters[0], pMsg->u8Parameters[1], pMsg->u8Parameters[2], pMsg->u8Parameters[3],
    //    pMsg->u8Parameters[4], pMsg->u8Parameters[5], pMsg->u8Parameters[6], pMsg->u8Parameters[7]);

    *pu32Size = ((MS_U32)(pMsg->u8Parameters[0])) & 0xFFUL;
    *pu32Size += ((((MS_U32)(pMsg->u8Parameters[1])) << 8UL) & 0xFF00UL);
    *pu32Size += ((((MS_U32)(pMsg->u8Parameters[2])) << 16UL) & 0xFF0000UL);
    *pu32Size += ((((MS_U32)(pMsg->u8Parameters[3])) << 24UL) & 0xFF000000UL);

    *pu32BufAddr = ((MS_U32)(pMsg->u8Parameters[4])) & 0xFFUL;
    *pu32BufAddr += ((((MS_U32)(pMsg->u8Parameters[5])) << 8UL) & 0xFF00UL);
    *pu32BufAddr += ((((MS_U32)(pMsg->u8Parameters[6])) << 16UL) & 0xFF0000UL);
    *pu32BufAddr += ((((MS_U32)(pMsg->u8Parameters[7])) << 24UL) & 0xFF000000UL);

    *pu8MiuSel = pMsg->u8Parameters[8];

    return TRUE;
}

void _DMX_TEE_HandleDynamicMBX(MBX_Msg *pMsg, MS_BOOL *pbAddToQueue)
{
    if(pMsg==NULL)
    {
        PRINTF(printf("[DMX]%s,pMsg is a null pointer\n",__FUNCTION__));
        return;
    }

    if (_s32Dmx_TEE_EventId < 0)
    {
        PRINTF(printf("[DMX]%s,DMX TEE event group is not created\n",__FUNCTION__));
        return;
    }
    memcpy(&_Dmx_TEE_msg, pMsg, sizeof(MBX_Msg));
    MsOS_SetEvent(_s32Dmx_TEE_EventId, 1);
    *pbAddToQueue = FALSE;
    return;
}

void _DMX_TEE_Proc(void)
{
    MS_U32 u32Events, u32data = 0;
    MS_U8  u8Index = 0, u8EngId = 0, u8Miusel = 0, u8Opt = 0;
    MBX_Result result;
    MS_U32 u32StartAddr = 0, u32EndAddr = 0;

    PRINTF(printf("DMX TEE_Proc\n"));

    while (1)
    {
        MsOS_WaitEvent(_s32Dmx_TEE_EventId, 1, &u32Events, E_OR_CLEAR, MSOS_WAIT_FOREVER);

        u8Index = _Dmx_TEE_msg.u8Index;

        memset(&_DmxTeeToReeMbxMsg,0,sizeof(MBX_Msg));
        _DmxTeeToReeMbxMsg.eRoleID = E_MBX_CPU_MIPS;
        _DmxTeeToReeMbxMsg.u8Ctrl = 0;
        _DmxTeeToReeMbxMsg.eMsgType = E_MBX_MSG_TYPE_INSTANT;
        _DmxTeeToReeMbxMsg.u8MsgClass = _Dmx_TEE_msg.u8MsgClass;
        _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_SUCCESS;
        _DmxTeeToReeMbxMsg.u8Parameters[0] = 0;
        _DmxTeeToReeMbxMsg.u8ParameterCount = 0;

        switch(u8Index)
        {
            case E_DMX_REE_TO_TEE_MBX_MSG_FW_LoadCode:
                PRINTF(printf("DMX receive load f/w code in R2\n"));
                if((stDMX_Tee_Priv_Param._u32TspFwBufAddr == 0) || (stDMX_Tee_Priv_Param._u32TspFwBufSize == 0))
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                else
                {
                    if (MDrv_TSP_Tee_CopyFw(MsOS_PA2KSEG1(stDMX_Tee_Priv_Param._u32TspFwBufAddr), stDMX_Tee_Priv_Param._u32TspFwBufAddr,
                        stDMX_Tee_Priv_Param._u32TspFwBufSize) == FALSE)
                    {
                        _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    }
                }
                break;
            case E_DMX_REE_TO_TEE_MBX_MSG_SET_VqBuf:
                if((stDMX_Tee_Priv_Param._u32VQBufAddr == 0) || (stDMX_Tee_Priv_Param._u32VQBufSize == 0))
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                else
                {
                    if(MDrv_TSP_Tee_Set_VQBuffer(stDMX_Tee_Priv_Param._u32VQBufAddr, stDMX_Tee_Priv_Param._u32VQBufSize) == FALSE)
                    {
                        _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    }
                }
                break;
            case E_DMX_REE_TO_TEE_MBX_MSG_SET_PvrBuf:
                u8EngId = _Dmx_TEE_msg.u8Parameters[0];
                u8Opt = _Dmx_TEE_msg.u8Parameters[1];
                if(u8EngId >= 2)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    break;
                }
                if(u8Opt == 0)
                {
                    if((stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[u8EngId] == 0) || (stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[u8EngId] == 0))
                    {
                        _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                        break;
                    }
                    else
                    {
                        if(MDrv_TSP_Tee_Set_PVRBuffer(u8EngId, stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[u8EngId], stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[u8EngId],
                            stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr[u8EngId], stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[u8EngId]) == FALSE)
                        {
                            _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                        }
                        else
                        {
                            u32data = stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[u8EngId] + stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[u8EngId];
                            _DMX_TEE_BufParamToMbxParam(stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[u8EngId], u32data, stDMX_Tee_Priv_Param._u8PvrBufMiuSel[u8EngId], &_DmxTeeToReeMbxMsg);
                        }
                    }
                }
                else  //reset buffer to 0
                {
                    if(MDrv_TSP_Tee_Set_PVRBuffer(u8EngId, 0, 0, 0, 0) == FALSE)
                    {
                        _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    }
                    stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[u8EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[u8EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr[u8EngId] = 0;
                    stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[u8EngId] = 0;
                    stDMX_Tee_Priv_Param._u8PvrBufMiuSel[u8EngId] = 0;
                }
                break;
            case E_DMX_REE_TO_TEE_MBX_MSG_SET_FileinBuf:
                u8EngId = _Dmx_TEE_msg.u8Parameters[9];

                PRINTF(printf("[DMX]E_DMX_REE_TO_TEE_MBX_MSG_SET_FileinBuf %d\n", (int)u8EngId));
                if((u8EngId >= 3) && (_DmxTeeToReeMbxMsg.u8ParameterCount != 10))
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    break;
                }

                if(_DMX_TEE_MbxParamToBufParam(&_Dmx_TEE_msg, &u32StartAddr, &u32data, &u8Miusel) == FALSE)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    break;
                }
                PRINTF(printf("[DMX]E_DMX_REE_TO_TEE_MBX_MSG_SET_FileinBuf %d, u32StartAddr %x, size %x\n", (int)u8EngId, (int)u32StartAddr, (int)u32data));

                u32EndAddr = u32StartAddr + u32data;

                if((u8Miusel == stDMX_Tee_Priv_Param._u8FileinMiuSel[u8EngId]) && (u32StartAddr >= stDMX_Tee_Priv_Param._u32FileinBufAddr[u8EngId]) &&
                    (u32EndAddr <= (stDMX_Tee_Priv_Param._u32FileinBufAddr[u8EngId] + stDMX_Tee_Priv_Param._u32FileinBufSize[u8EngId])))
                {
                    if(MDrv_TSP_Tee_Set_FileinBuffer(u8EngId, u32StartAddr, u32data) == FALSE)
                    {
                        _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                    }
                }
                else
                {
                    MDrv_TSP_Tee_Set_FileinBuffer(u8EngId, 0, 0);
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                break;
            case E_DMX_REE_TO_TEE_MBX_MSG_GET_PvrWPtr:
                u8EngId = _Dmx_TEE_msg.u8Parameters[0];
                if(MDrv_TSP_Tee_Get_PVRWriteAddr(u8EngId, &u32StartAddr) == FALSE)
                {
                    _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                }
                else
                {
                    _DmxTeeToReeMbxMsg.u8ParameterCount = 5;
                    _DmxTeeToReeMbxMsg.u8Parameters[0] = stDMX_Tee_Priv_Param._u8PvrBufMiuSel[u8EngId];
                    _DmxTeeToReeMbxMsg.u8Parameters[1] = (MS_U8)(u32StartAddr & 0xFFUL);
                    _DmxTeeToReeMbxMsg.u8Parameters[2] = (MS_U8)((u32StartAddr >> 8UL) & 0xFFUL);
                    _DmxTeeToReeMbxMsg.u8Parameters[3] = (MS_U8)((u32StartAddr >> 16UL) & 0xFFUL);
                    _DmxTeeToReeMbxMsg.u8Parameters[4] = (MS_U8)((u32StartAddr >> 24UL) & 0xFFUL);
                }
                break;
            default:
                _DmxTeeToReeMbxMsg.u8Index = E_DMX_TEE_TO_REE_MBX_ACK_MSG_ACTION_FAIL;
                PRINTF(printf("[DMX]%s,wrong command %d sent from HK\n",__FUNCTION__,(unsigned int)u8Index));
                break;
        }

        result = MApi_MBX_SendMsg(&_DmxTeeToReeMbxMsg);

#if 0
        if (E_MBX_SUCCESS != result)
        {
            PRINTF("[DMX]Ack fail,classid:%x,cmd:%x\n",_DmxTeeToReeMbxMsg.u8MsgClass, _DmxTeeToReeMbxMsg.u8Index);
        }
        else
        {
            PRINTF("[DMX]Ack ok,classid:%x\n",_DmxTeeToReeMbxMsg.u8MsgClass);
        }
#endif
    }
}

//-------------------------------------------------------------------------------------------------
/// Create DMX Tee service task
/// @return TRUE or FALSE for Pass or Fail
/// @note Please call this API when Tee system init, and must create this task before DMX init is called
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_Create_Tee_System(void)
{
    if(_u8DmxMbxMsgClass == 0)
    {
        if(MApi_MBX_GenerateDynamicClass(E_MBX_CPU_MIPS, "DMX_TEE",&_u8DmxMbxMsgClass) != E_MBX_SUCCESS)
        {
            PRINTF(printf("[DMX]DMX GenerateDynamicClass error\n"));
            _u8DmxMbxMsgClass = 0;
            return FALSE;
        }

        if (E_MBX_SUCCESS != MApi_MBX_RegisterMSGWithCallBack(_u8DmxMbxMsgClass,10,_DMX_TEE_HandleDynamicMBX))
        {
            PRINTF(printf("[DMX]TEE Register MBX MSG error\n"));
            _u8DmxMbxMsgClass = 0;
            return FALSE;
        }
    }

    if(_s32Dmx_TEE_EventId == -1)
    {
        _s32Dmx_TEE_EventId = MsOS_CreateEventGroup("DMX_TEE_Event");
        if (_s32Dmx_TEE_EventId < 0)
        {
            PRINTF(printf("[DMX]TEE CreateEventGroup error....\n"));
            return FALSE;
        }
    }

    MDrv_TSP_Tee_Init();

    if(_s32Dmx_TEE_TaskId == -1)
    {
        _s32Dmx_TEE_TaskId = MsOS_CreateTask((TaskEntry)_DMX_TEE_Proc,
                                              NULL,
                                              E_TASK_PRI_MEDIUM,
                                              TRUE,
                                              (void*)_u8DmxTeeStack,
                                              DMX_STACK_SIZE,
                                              "DMX_TEE_Task");
        if (_s32Dmx_TEE_TaskId < 0)
        {
            MsOS_DeleteEventGroup(_s32Dmx_TEE_EventId);
            PRINTF(printf("[DMX]TEE CreateTask error....\n"));
            return FALSE;
        }
    }
    return TRUE;
}

//------------------------------------------------------------------------------
/// GetControl for TEE
/// @param cmd \b IN : TEE user cmd
/// @param param \b IN : TEE control parameters
/// @return MS_BOOL
/// @note For buffer control commend, the input parameters should be DMX_TEE_BUF_Param
//------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_SetControl(DMX_TEE_USER_CMD cmd, void* param)
{
    MS_BOOL ret = FALSE;
    DMX_TEE_BUF_Param *pTeeBuf = (DMX_TEE_BUF_Param*)param;

    if(param == NULL)
    {
        PRINTF(printf("[DMX]%s,Invalid param\n",__FUNCTION__));
    }

    switch(cmd)
    {
        case E_DMX_TSPFW_BUF:
            stDMX_Tee_Priv_Param._u32TspFwBufAddr = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32TspFwBufSize = pTeeBuf->u32BufSize;
            stDMX_Tee_Priv_Param._u8TspFwBufMiuSel = pTeeBuf->u8MiuSel;
            PRINTF(printf("[%s] cmd %x, buf %x %x %d\n", __FUNCTION__, (int)cmd,
                (int)stDMX_Tee_Priv_Param._u32TspFwBufAddr, (int)stDMX_Tee_Priv_Param._u32TspFwBufSize, (int)stDMX_Tee_Priv_Param._u8TspFwBufMiuSel));
            break;
        case E_DMX_TSPVQ_BUF:
            stDMX_Tee_Priv_Param._u32VQBufAddr = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32VQBufSize = pTeeBuf->u32BufSize;
            stDMX_Tee_Priv_Param._u8VQBufMiuSel = pTeeBuf->u8MiuSel;
            break;
        case E_DMX_PVR_BUF:
            if(pTeeBuf->u8EngId >= 2)
            {
                return FALSE;
            }
            stDMX_Tee_Priv_Param._u8PvrBufMiuSel[pTeeBuf->u8EngId] = pTeeBuf->u8MiuSel;
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Addr[pTeeBuf->u8EngId] = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u8EngId] = pTeeBuf->u32BufSize >> 1;
            stDMX_Tee_Priv_Param._u32PVR_Buf1_Addr[pTeeBuf->u8EngId] = pTeeBuf->u32BufAddr + stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u8EngId];
            stDMX_Tee_Priv_Param._u32PVR_Buf1_Size[pTeeBuf->u8EngId] = stDMX_Tee_Priv_Param._u32PVR_Buf0_Size[pTeeBuf->u8EngId];
            break;
        case E_DMX_FILEIN_BUF:
            if(pTeeBuf->u8EngId >= 3)
            {
                return FALSE;
            }
            stDMX_Tee_Priv_Param._u8FileinMiuSel[pTeeBuf->u8EngId] = pTeeBuf->u8MiuSel;
            stDMX_Tee_Priv_Param._u32FileinBufAddr[pTeeBuf->u8EngId] = pTeeBuf->u32BufAddr;
            stDMX_Tee_Priv_Param._u32FileinBufSize[pTeeBuf->u8EngId] = pTeeBuf->u32BufSize;
            break;
        default:
            PRINTF(printf("[DMX][%s]Not support cmd,%d\n",__FUNCTION__, (unsigned int)cmd));
            break;
    }

    return ret;
}
