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

////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file   apiDMX.c
/// @brief  Demux API
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __API_DMX_DBG_H__
#define __API_DMX_DBG_H__

#ifndef MSOS_TYPE_LINUX_KERNEL
#include <string.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"

#define DMX_DEBUG 0

#if DMX_DEBUG
MS_BOOL _DMX_DBG_Record_Stop(void);
MS_BOOL _DMX_DBG_Record_Start(const char[], MS_U32*, MS_U32, MS_PHYADDR, MS_U32, MS_BOOL, MS_BOOL);
void _DMX_DBG_LogSel(MS_U32 u32Flag);
void _DMX_DBG_PidMonitor(MS_U32 *u32Pids, MS_U32 u32Num);
void _DMX_Debug(char path[]);
MS_BOOL MApi_DMX_DBG_IsFltMonitor(MS_U32 u32DmxId);


typedef struct
{
    int         id;
    const char *str;
} DMX_DMX_INPUT_STR, DMX_AV_FIFO_STR;

typedef struct
{
    MS_U32      u32DmxId;
    MS_U32      u32TimeCreate;
//    MS_U32      u32TimeLastEnable;
//    MS_U32      u32TimeLastDisable;
//    MS_U32      u32TimeActive; // enable time
    MS_U32      u32Int_rdy;
    MS_U32      u32Int_ovf;
    MS_U32      u32Int_crc;
    MS_U32      u32CB_rdy;
    MS_U32      u32CB_ovf;
} DMX_FLT_DBG_INFO;
//------------------------------------------------------------------------------
// Debug Macros
//------------------------------------------------------------------------------
#define COLOR_NONE "\033[m"
#define COLOR_RED "\033[0;32;31m"
#define COLOR_GREEN "\033[0;32;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_BLUE "\033[0;32;34m"
#define COLOR_BROWN "\033[0;33m"

#define COLOR_DBG   COLOR_RED
#define dmx_dbg_print(fmt, ...) ULOGD("DMX", COLOR_DBG fmt COLOR_NONE, ##__VA_ARGS__)

//#define DMX_DBG_ENTRY() MsOS_ObtainMutex(s32DmxDbgMutex, MSOS_WAIT_FOREVER)
//#define DMX_DBG_EXIT() MsOS_ReleaseMutex(s32DmxDbgMutex)

#define DMX_DBG_ENTRY()
#define DMX_DBG_EXIT()

extern MS_U32 u32PidMonitor[];
extern MS_U32 u32NumFltMonitor;
extern MS_S32 s32DmxDbgMutex;

#ifndef dmx_dbg_IsFltMonitor
#define dmx_dbg_IsFltMonitor(u32DmxId, bRet) {\
        int tempii = 0; \
        bRet = FALSE; \
        if((u32NumFltMonitor == 0) && (u32DmxDbgFltId == 0xFFFFFFFF))bRet = TRUE; \
        if((u32DmxId == u32DmxDbgFltId) && (u32DmxDbgFltId != 0xFFFFFFFF))bRet = TRUE;\
        for(tempii = 0; tempii <u32NumFltMonitor; tempii++) {                             \
            DMX_DBG_ENTRY();                                                             \
            if(_pdmx_res->_FltList[(u32DmxId)].u16Pid == (MS_U16)u32PidMonitor[tempii]) \
                {DMX_DBG_EXIT();bRet = TRUE;}                                          \
            DMX_DBG_EXIT();}}

#endif

//------------------------------------------------------------------------------
// Constant definition
//------------------------------------------------------------------------------

#define MAX_NUM_CP_DATA_DUMP 128

    #define DMX_DBG_LOG_FLT_OPEN    0x00000001
    #define DMX_DBG_LOG_FLT_PID     0x00000002
    #define DMX_DBG_LOG_FLT_CLOSE   0x00000004
    #define DMX_DBG_LOG_FLT_ENABLE  0x00000008
    #define DMX_DBG_LOG_FLT_INFO    0x00000010
    #define DMX_DBG_LOG_FLT_CB      0x00000020

    #define DMX_DBG_LOG_FILEIN      0x00000080 // file in operation

    #define DMX_DBG_LOG_COPYDATA    0x00000100 // section buffer address
    #define DMX_DBG_LOG_COPYDATA1   0x00000200 // dump section data (console / file)

    #define DMX_DBG_LOG_FLOWSET     0x00000400
    #define DMX_DBG_LOG_FLT_SEC_OVF 0x00000800
    #define DMX_DBG_LOG_FLT_SEC_RDY 0x00001000
    #define DMX_DBG_LOG_STC_CHANGE  0x00002000
    #define DMX_DBG_LOG_PCR         0x00004000

    #define DMX_DBG_LOG_INTR        0x00080000
    #define DMX_DBG_LOG_FI_READ     0x00100000 // file in read address
    #define DMX_DBG_LOG_PVR_WRITE   0x00200000 // pvr write address
    #define DMX_DBG_LOG_AVFIFO      0x00400000 // a/v fifo reset and status

    #define DMX_DBG_LOG_AV_DISCONTI 0x02000000
    #define DMX_DBG_LOG_AV_DROP     0x04000000
    #define DMX_DBG_LOG_LOCK        0x08000000
    #define DMX_DBG_LOG_STC         0x10000000
    #define DMX_DBG_LOG_AV_PKT      0x20000000
    #define DMX_DBG_LOG_AV_OVF      0x40000000
    #define DMX_DBG_LOG_AEON_ALIVE  0x80000000

//------------------------------------------------------------------------------
// Global Variable
//------------------------------------------------------------------------------
#endif // DMX_DEBUG
#endif // __API_DMX_DBG_H__
