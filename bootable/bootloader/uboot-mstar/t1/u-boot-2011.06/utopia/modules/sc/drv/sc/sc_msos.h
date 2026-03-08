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
/// file    sc_msos.h
/// @brief  SmartCard Driver MsOS Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _SC_MSOS_H_
#define _SC_MSOS_H_


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsOS.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define OS_SC_MUTEX_TIMEOUT         1000UL
#define OS_SC_ENTRY()               if (!OS_OBTAIN_MUTEX(os_SC_MutexID, OS_SC_MUTEX_TIMEOUT)) {   return E_SC_FAIL;   }
#define OS_SC_RETURN(_ret)          { OS_RELEASE_MUTEX(os_SC_MutexID);return (_ret); } // Should be optimized by compiler
#define OS_SC_DELAY(_msec)          OS_DELAY_TASK(_msec)
#define OS_SC_TIME()                OS_SYSTEM_TIME()
#define OS_SC_EXPIRE(_msec)         (OS_SYSTEM_TIME() + (_msec))

#define OS_SC_EVENT_MASK            0x000000FFUL
#define OS_SC_EVENT_CARD            0x00000001UL
#define OS_SC_EVENT_SEND            0x00000010UL
#define OS_SC_EVENT_RECV            0x00000020UL
#define OS_SC_EVENT_MASK2           0x0000FF00UL
#define OS_SC_EVENT_CARD2           0x00000100UL
#define OS_SC_EVENT_SEND2           0x00001000UL
#define OS_SC_EVENT_RECV2           0x00002000UL

#define OS_SC_TX_LOCK()             { OS_OBTAIN_MUTEX(os_SC_MutexID_TX, MSOS_WAIT_FOREVER); }
#define OS_SC_TX_UNLOCK()           { OS_RELEASE_MUTEX(os_SC_MutexID_TX); }

#define OS_SC_SHM_LOCK()             { OS_OBTAIN_MUTEX(os_SC_MutexID_SHM, MSOS_WAIT_FOREVER); }
#define OS_SC_SHM_UNLOCK()           { OS_RELEASE_MUTEX(os_SC_MutexID_SHM); }







//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef MS_BOOL(*P_SC_ISR_Proc)(MS_U8 u8SCID);


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern MS_S32   os_SC_MutexID;
extern MS_S32   os_SC_MutexID_TX;
extern MS_S32   os_SC_MutexID_SHM;

extern MS_BOOL  OS_SC_Init(MS_U8 u8SCID);
extern MS_BOOL  OS_SC_Exit(MS_U8 u8SCID);
extern void     OS_SC_ISR_Enable(MS_U8 u8SCID, P_SC_ISR_Proc pfISRProc);
extern void     OS_SC_ISR_Disable(MS_U8 u8SCID);
extern void     OS_SC_SetEvent(MS_U32 u32Events);
extern MS_U32   OS_SC_WaitEvent(void);
extern void     OS_SC_ClearEvent(MS_U32 u32Events);
extern void     OS_SC_EnterCS(void);
extern void     OS_SC_ExitCS(void);


#endif // _SC_MSOS_H_


