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
/// @file   drvMBXMsgPool.h
/// @brief  MStar Mailbox msg pool Driver DDI
/// @attention
/// <b>(OBSOLETED) <em>legacy interface is only used by MStar proprietary Mail Message communication\n
/// It's API level for backward compatible and will be remove in the next version.\n
/// Please refer @ref drvGE.h for future compatibility.</em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MBX_MSGPOOL_H
#define _DRV_MBX_MSGPOOL_H

#ifdef _DRV_MBX__MSGPOOL_C
#define INTERFACE
#else
#define INTERFACE extern
#endif



#ifdef __cplusplus
extern "C"
{
#endif

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros
//=============================================================================
#define INVALID_PTR  (-1)
#define IS_VALID_PTR(ptr)  (ptr != INVALID_PTR)

/// MSG Pool return value
typedef MBX_Result MSGPOOL_Result;

#define E_MSGPOOL_SUCCESS                    E_MBX_SUCCESS
#define E_MSGPOOL_ERR_NOT_INITIALIZED       E_MBX_ERR_NOT_INITIALIZED
#define E_MSGPOOL_ERR_NO_MORE_MEMORY        E_MBX_ERR_NO_MORE_MEMORY
#define E_MSGPOOL_ERR_SLOT_BUSY              E_MBX_ERR_SLOT_BUSY
#define E_MSGPOOL_ERR_SLOT_AREADY_OPENNED   E_MBX_ERR_SLOT_AREADY_OPENNED
#define E_MSGPOOL_ERR_SLOT_NOT_OPENNED      E_MBX_ERR_SLOT_NOT_OPENNED
#define E_MSGPOOL_ERR_INVALID_PARAM          E_MBX_ERR_INVALID_PARAM
#define E_MSGPOOL_ERR_HAS_MSG_PENDING       E_MBX_ERR_HAS_MSG_PENDING
#define E_MSGPOOL_ERR_NO_MORE_MSG            E_MBX_ERR_NO_MORE_MSG
#define E_MSGPOOL_ERR_NOT_IMPLEMENTED       E_MBX_ERR_NOT_IMPLEMENTED
#define E_MSGPOOL_UNKNOW_ERROR               E_MBX_UNKNOW_ERROR

//=============================================================================
// Type and Structure Declaration
//=============================================================================
//Enum:
typedef enum
{
    E_MSGQ_INVALID    = 0x00,
    E_MSGQ_NORMAL     = 0x01,
    E_MSGQ_EMPTY      = 0x02,
    E_MSGQ_OVERFLOW   = 0x03
} MSGPOOL_MsgQStatus;

typedef enum
{
    E_MSGQ_R2TASK_INVALID    = 0x00,
    E_MSGQ_R2TASK_TEST     = 0x01,
    E_MSGQ_R2TASK_PNL      = 0x02,
    E_MSGQ_R2TASK_LOGO   = 0x03,
    E_MSGQ_R2TASK_MUSIC   = 0x04,
    E_MSGQ_R2TASK_OSD   = 0x05,
    E_MSGQ_R2TASK_SECURE   = 0x06
} MSGPOOL_R2TaskID;

//Structs:
typedef struct
{
    MBX_Msg mbxMsg; //the msg in slot.
    MS_S16 s16Next;  //next item in pool;
    MS_U16 u16Usage; //the slot is used or free;
}MSGPOOL_MsgPoolItem; //sizeof(MSGPOOL_MsgPoolItem) == 28 bytes

typedef struct
{
    volatile MSGPOOL_MsgPoolItem * pMsgPool; //pointer to the reserved msg pool
    MS_U16 u16Slots; //the total number of MsgPoolItem in the MsgPool
    MS_U16 u16FreeSlots; //the free number of MsgPoolItem in the MsgPool
    MS_U16 u16RegistedSlots; //the registed(resevered for registed msg class) number of MsgPoolItem in the MsgPool.
}MSGPOOL_MsgPoolInfo;

typedef struct
{
    MS_S16 s16MsgFirst; //the first MsgPoolItem slot idx in MsgPool
    MS_S16 s16MsgEnd; //the end MsgPoolItem slot idx in MsgPool

    MS_S16 s16InstantMsgFirst; //the first instant MsgPoolItem slot idx in MsgPool
    MS_S16 s16InstantMsgEnd; //the end instant MsgPoolItem slot idx in MsgPool

    MS_U16 u16MsgNum; //the number of MsgPoolItem in MsgQ;
    MS_U16 u16InstantMsgNum; //the number of instant MsgPoolItem in MsgQ;

    MS_U16 u16MsgQStatus; //the MsgQ Status: invalid; normal, empty; voerflow;
    MS_U16 u16MsgQSize; //the MsgQ size when the msg class is registed; should be no larger than MAX_MBX_QUEUE_SIZE
}MSGPOOL_MsgQMgr; //message queue manager, per class per queue.

////////////////////////////////////////////////////////////////////////////////////////////
// interface to control the Msg Pool:
INTERFACE MSGPOOL_Result MDrv_MSGPOOL_Init(void);
INTERFACE void MDrv_MSGPOOL_DeInit(void);

////////////////////////////////////////////////////////////////////////////////////////////
// interface to control the Msg Queue:
INTERFACE MSGPOOL_Result MDrv_MSGQ_Init(void);
INTERFACE MSGPOOL_Result MDrv_MSGQ_DeInit(MS_BOOL bForceDiscardPendingMsg);

INTERFACE MSGPOOL_Result MDrv_MSGQ_RegisterMSG(MS_S16 s16MsgQID, MS_U16 u16MsgQSize);
INTERFACE MSGPOOL_Result MDrv_MSGQ_UnRegisterMSG(MS_S16 s16MsgQID, MS_BOOL bForceDiscardMsgQueue);
INTERFACE MSGPOOL_Result MDrv_MSGQ_ClearMSG(MS_S16 s16MsgQID);

INTERFACE MSGPOOL_Result MDrv_MSGQ_GetMsgQStatus(MS_S16 s16MsgQID, MBX_MSGQ_Status *pMsgQStatus);

INTERFACE MSGPOOL_Result MDrv_MSGQ_RecvMsg(MS_S16 s16MsgQIdx, MBX_Msg* pMsg, MS_BOOL bInstantMsg);
INTERFACE MSGPOOL_Result MDrv_MSGQ_AddMSG(MBX_Msg* pMbxMsg);

#undef INTERFACE


#ifdef __cplusplus
}
#endif

#endif //_DRV_MBX_MSGPOOL_H

