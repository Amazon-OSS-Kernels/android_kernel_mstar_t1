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
/***************************************************************************
 *
 * Copyright ?2008-2010 MStar Semiconductor, Inc. All Rights Reserved
 *
 **************************************************************************/
/** \file
 * \brief This is the header file to configure the FrHsl module.
 * This file contains default definitions of platform-specific macros to
 * customise the HSL hardware, interface and logging routines.  All macros
 * defined here can be overridden/'overloaded' in a platform-specific header file.
 */
#if defined (MSOS_TYPE_LINUX)
#ifndef FRHSLCFG_H
#define FRHSLCFG_H


#include "MsTypes.h"
#include <unistd.h>
#include <pthread.h>

#include <dlfcn.h>


#ifdef  __cplusplus
extern "C" {
#endif

/*******************************************************************************
** Constants
*******************************************************************************/

#define ENABLE_ALL_GROUP_IDS            1
#define DISABLE_ALL_GROUP_IDS           0

/*******************************************************************************
** Macros
*******************************************************************************/

/** Macro that can be redefined to allow code to be added at the
 *  start of the HSL flow control interrupt.
 *  Macro that can be redefined to allow code to be added at the
 *  start of the HSL flow control interrupt.
 */
#if !defined (M_FrHslFlowControlIntBegin)
# define M_FrHslFlowControlIntBegin()
#endif

/** Macro that can be redefined to allow code to be added at the
 *  end of the HSL flow control interrupt.
 *  Macro that can be redefined to allow code to be added at the
 *  end of the HSL flow control interrupt.
 */
#if !defined (M_FrHslFlowControlIntEnd)
# define M_FrHslFlowControlIntEnd()
#endif


/*******************************************************************************
* Alternative macros
*******************************************************************************/


/** \def FR_HSL_V2_UNITIALISED_LOG_ID
 *  Log point id assigned when using MDF to capture a new log point.
 *  Log point id assigned when using MDF to capture a new log point.
 *  This value should be used when adding a log point with a new MDF
 *  compatible log point id but using HSL version 2 macros.
 *  e.g. M_FrHslPrintf1(FR_HSL_V2_UNITIALISED_LOG_ID, NEW_MDF_COMPAT_ID,"NewMdfCompaqId: %#.4x", arg1);
 *  NEW_MDF_COMPAT_ID is the new log point added for MDF to be able to decode it.
 */
#define FR_HSL_V2_UNITIALISED_LOG_ID ((MS_U16)0x0000)

/** \def FR_HSL_V2_UNPROCESSED_LOG_ID
 *  Log point id assigned when an HSL version 2 statement has not been processed by silox.exe.
 *  Log point id assigned when an HSL version 2 statement has not been processed by silox.exe.
 *  This ID is used as part of stub functions which will print a default HSL log message when
 *  new HSL log points have been added but have not been processed by silox.exe.
 */
#define FR_HSL_V2_UNPROCESSED_LOG_ID ((MS_U16)0x0002)


// large part of the data are bookmarks (8 * BUFFER_SEGMENTS_COUNT)
#define SYS_STATE_BUFSIZE (1024)

#define TX_BUFFER_SIZE (5*1024)
#define MAX_OUTPUT_THREADS 1
// MS_U8 used to store the next bm number - max count 255
#define BUFFER_SEGMENTS_COUNT 10
#define BUFFER_DUMP_SEGMENTS 3
#define BUFFER_FREE_SPACE_PERCENTAGE 15
#define PENDING_WRITE_LIST_LEN 255
#define ASSERT_RT_CLOCK_FAIL_TW 1000

/*******************************************************************************
** Global Data
*******************************************************************************/
typedef struct ST_HSL_CONTROLLER* p_HslController;

typedef struct ST_LIST_NODE
{
    MS_U32 u32bufferIndex;
    MS_U32 u32timestamp;
    struct ST_LIST_NODE *prev;
    struct ST_LIST_NODE *next;
} stListNode;

typedef struct
{
    MS_U32 address;
    MS_U32 timerWrapDelta;
} stRBbookmark;

// this structure need to save in ring buffer
// after crashing, it can be used to output the remaining message
// make sure the size of this structure MUST less than SYS_STATE_BUFSIZE
typedef struct ST_RB_SYSTEM_STATE
{
    MS_U32 ssize;               // size of system state reserved RAM area
    MS_U32 sbufStart;           // system state RAM start address
    MS_U32 sbufEnd;             // system state RAM address end

    MS_U32  bufSize;            // size of ringbuffer
    MS_U32  bufStart;           // ring buffer start address
    MS_U32  bufEnd;             // ring buffer end address

    // runtime constants
    MS_U32  bufMinFreeBytes;     // defines a threshold for triggering data dumping
    MS_U32  bufSegmentSize;

    // access to the following is protected by the bufferMutex
    MS_U32  bufUsedEnd;         // end of the currently used buffer space (adjusted
    MS_U32  bufWritePos;        // current write position in ring buffer
    MS_U32  bufReadPos;         // position of output pointer
    MS_U32  bufReadLimit;       // last byte of valid fully written data update after write finishes
    MS_U32  bufFlags;
    MS_U32  hslLastTwTime;      // timestamp of the last timerwrap

    // controlled by outputCondMutex
    MS_U32  bufBytesToSend;     // amount of data to be transfered from the ring buffer
    stRBbookmark bufBookmarks[BUFFER_SEGMENTS_COUNT];
    MS_U8    dumpFlag;            // marks a necessity to insert a log point informing about a data dump into the output stream.
    MS_U8   syncMarker;         // countdown for writing sync marker
    MS_U8   nextBookmark;
    MS_U8    flushOnConnect;        // indicates whether the complete buffer contents should be flushed on connection
    MS_U8    dynamicAlloc;        // indicates the type of allocation and a necessity to release memory on exit. (0 static/1 dynamic)
} stSystemState;

// manages the data associated with a specific thread instance.
typedef void * (*start_routine)(void *);

typedef enum
{
    read_thread=0,
    write_thread,
    output_thread,
    prMS_S32_thread
}THREADTYPE;

typedef struct
{
    MS_S32          streamId;
    pthread_t       threadId;
    MS_S32          priority;
    THREADTYPE  typeId;
    start_routine   funcPtr;
    void*               funcParam; // ptr to array of bytes to log.
} hslThread;


typedef struct ST_HSL_CONTROLLER
{
    // system state pointer
    stSystemState* pSystemState;                // location of structure for system state
    MS_U32 u32Port;  // port on which connections will be accepted
    MS_U8 u8Instance;
    MS_BOOL bInit;

    // Global variables
    //TODO!: Initialize output threads
    hslThread logThreads[MAX_OUTPUT_THREADS];
/*    =
    {
            {0, 0, 0, output_thread, bufferWriteThread, NULL}
    }; */

    stListNode pendingWriteList[PENDING_WRITE_LIST_LEN];
    stListNode *pendingWriteHead; //TODO: initialize pointers with zeros
    MS_U16 pendingWriteLength;

    volatile MS_BOOL thread_finalize;

    pthread_mutex_t  bufferMutex;            // mutex for accessing the ring buffer
    pthread_mutex_t  inputMutex;             // mutex synchronising the input threads
    pthread_mutex_t  outputCondMutex;        // mutex for I/O sync
    pthread_cond_t   outputCond;             // ring buffer output sync condition

    pthread_mutex_t pendingWriteMutex;       // mutex for the pending write linked list;

    pthread_t threadCmdRecvStart;
    pthread_mutex_t socketReadMutex;   // mutex variable for socket read

    MS_BOOL g_AutoTestRun;
    int hServerSocket, hClientSocket;

} stHslController;

/*******************************************************************************
** Global Function Prototypes
*******************************************************************************/

void FrHslResetConnection(stHslController* const _hsl);
MS_BOOL FrHslInitialiseBegin(void);
MS_BOOL FrHslInitialiseEnd(void);
MS_BOOL FrHslStop(void);
void FrHslWritePayload (const MS_BOOL bLast, const MS_U16 u16Payload, MS_U8 *pu8Buff);
void FrHslWriteHeader(const MS_U8 u8MsgType, const MS_U16 u16ID, MS_U8 *pu8Buff, MS_U32 u32Time);
MS_BOOL FrHslCommand(p_HslController _hsl, CmdInfo* const pCmd);

#define HSL_MAX_LIBRARY_COUNT 50
#define HSL_MAX_THREAD_COUNT 200
#define HSL_MAX_THREAD_NAME 17
#define HSL_MAX_LIBRARY_NAME 100

typedef struct LibraryShortInfoTag
{
    MS_U16 rec_no;
    MS_U16 rec_total;
    void*  base_address;
    void*  on_load_addr;
    char   name[HSL_MAX_LIBRARY_NAME];
} ST_LIBRARY_SHORT_INFO;
extern ST_LIBRARY_SHORT_INFO gDynamicLibraryInfo[HSL_MAX_LIBRARY_COUNT];
extern MS_U16 gDynamicLibraryCount;

typedef struct ThreadInfoTag
{
    MS_U32  thread_ID;
    char   thread_name[HSL_MAX_THREAD_NAME];
} ST_THREAD_INFO;
extern ST_THREAD_INFO gThreadInfo[HSL_MAX_THREAD_COUNT];
extern MS_U16 gThreadCount;

#define MAX_SELFTEST_THREAD_CNT 20
extern pthread_t gTestThreads[MAX_SELFTEST_THREAD_CNT];
extern MS_BOOL gRunLoadTest;
void *HslSelfTestThreadFun(void* pArg);

#ifdef  __cplusplus
}
#endif

#endif
/* END OF FILE */
#endif
