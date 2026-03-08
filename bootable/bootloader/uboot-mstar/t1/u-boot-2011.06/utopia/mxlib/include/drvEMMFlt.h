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

#ifndef _DRVEMMFLT_H_
#define _DRVEMMFLT_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvEMMFlt.h
/// @brief to filter EMM packet
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "UFO.h"

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
#define MSIF_EMMFLT_LIB_CODE	{'E','M','M','F'}    //Lib code
#define MSIF_EMMFLT_LIBVER		{'0','1'}            //LIB version
#define MSIF_EMMFLT_BUILDNUM    {'0','1'}            //Build Number
#define MSIF_EMMFLT_CHANGELIST  {'0','0','3','4','8','0','3','3'} //P4 ChangeList Number

#define EMMRASP_SHAREINT

#define EMMFLT_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_EMMFLT_LIB_CODE,                  /* IP__                                             */  \
    MSIF_EMMFLT_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_EMMFLT_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_EMMFLT_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

typedef MS_U32                         EMMFLT_RESULT;

typedef void (*EMMFLT_Notify)(MS_U32 conn, MS_U32 reason, void *pData);


#define DRV_EMMFLT_OK                   TRUE
#define DRV_EMMFLT_FAIL                 FALSE
#define DRV_EMMFLT_INVALID_REQUEST      3



#define EMM_SRC_FILEIN      0
#define EMM_SRC_LIVEIN      1

typedef enum
{
    EMM_ENGINE = 0,
    RASP_ENGINE,
    MAX_ENGINE,
}EMMRASP_e;

typedef enum _EMM_EVENT_
{
    EMM_EVENT_ARRIVE = 0,
    EMM_EVENT_OVERFLOW,
} EMM_EVENT_e;


#if defined(UFO_PUBLIC_HEADER_500_3)
typedef enum _EMM_SrcType_
{
    EMM_SRC_TS0             = 0,
    EMM_SRC_TS1,
    EMM_SRC_TS2,            // 2
    EMM_SRC_TS3,
    EMM_SRC_TS4,
    EMM_SRC_TS5,            // 5
    EMM_SRC_TS6,            // 6
    EMM_SRC_TSO,            // 7
    EMM_SRC_INTERDEMOD_0,   // 8
    EMM_SRC_INTERDEMOD_1,   // 9
    EMM_SRC_MEMORY,         // 10
}EMM_SRCTYPE_e;
#else
typedef enum _EMM_SrcType_
{
    EMM_SRC_TS0 = 0,
    EMM_SRC_TS1,
    EMM_SRC_TS2,
    EMM_SRC_TS3,
    EMM_SRC_INTERDEMOD_0,
    EMM_SRC_INTERDEMOD_1,
    EMM_SRC_MEMORY,
}EMM_SRCTYPE_e;
#endif

typedef enum
{
    EMM_TIDMODE_INGORE = 0x0,
    EMM_TIDMODE_ACCEPT_ALL,
    EMM_TIDMODE_ACCEPT_ADDRESS,
    EMM_TIDMODE_RESERVED,
} EMMDRV_TIDMODE_e;

typedef enum _EMM_OutType_
{
    EMM_OUT_NORMAL = 0,
    EMM_OUT_184BYTES,
    EMM_OUT_PACKETNUM,
}EMM_OUTTYPE_e;

typedef struct _EMMFLT_Require_
{
    MS_U32 u32DstBufSize;
    MS_U32 u32AlignBytes;
    MS_U8  u8EMMNumber;
} EMMFLT_Require_t;


typedef struct _EMM_FILTER_DEF
{
    MS_U8   filter_type;         //this can be set to LABEL_00, 01, 02, 03
    MS_U8   filter_address[4];   //
    MS_U8   filter_address_mask[4];

} EMM_FILTER_DEF_t;


////////////////////////////////////////////////////////////////////////////////
// Extern Function
////////////////////////////////////////////////////////////////////////////////

EMMFLT_RESULT MDrv_EMMFLT_Init(EMMFLT_Require_t *pRequire);
EMMFLT_RESULT MDrv_EMMFLT_GetHwBufCnt(MS_U32 *pHwBufCnt);
EMMFLT_RESULT MDrv_EMMFLT_SetDbgLevel(MS_U32 u32Level);
EMMFLT_RESULT MDrv_EMMFLT_SetNotify(EMMFLT_Notify Notify);
EMMFLT_RESULT MDrv_EMMFLT_SetEmmPID(MS_U32 connection,MS_U16 u16EmmPID);
EMMFLT_RESULT MDrv_EMMFLT_SetTidMode(MS_U32 connection, MS_U8 u8TidValue, EMMDRV_TIDMODE_e eTIDMODE);
EMMFLT_RESULT MDrv_EMMFLT_EnableEmmTID(MS_U32 connection, MS_BOOL bEnable);
EMMFLT_RESULT MDrv_EMMFLT_SetFilterCfg(MS_U32 connection, MS_U8 FilerIndex, EMM_FILTER_DEF_t *pFilter);
EMMFLT_RESULT MDrv_EMMFLT_Fire(MS_U32 connection);

EMMFLT_RESULT MDrv_EMMFLT_SetDstBufInfo(MS_U32 x_connection, MS_U32 u32BufAddr, MS_U32 u32BufSize);
EMMFLT_RESULT MDrv_EMMFLT_EmmProcessingFinished(MS_U8 *emm_buf/* RPC SIZECONST(256) */);
EMMFLT_RESULT MDrv_EMMFLT_Deinit(void);
EMMFLT_RESULT MDrv_EMMFLT_SrcSelect(MS_U32 x_connection, MS_U32 u32SrcFrom, EMM_SRCTYPE_e eSrcType);
EMMFLT_RESULT MDrv_EMMFLT_StopEmm(MS_U32 x_connection);
EMMFLT_RESULT MDrv_EMMFLT_EmmProcessingFinished(MS_U8 *emm_buf/* RPC SIZECONST(256) */);
EMMFLT_RESULT MDrv_EMMFLT_SetOutputType(MS_U32 x_connection, EMM_OUTTYPE_e eOutType);
EMMFLT_RESULT MDrv_EMMFLT_ConnectCheck(MS_U32 connection);

EMMFLT_RESULT MDrv_EMMFLT_SetExtendConfig(MS_U32 x_connection, MS_U16 type, MS_U16 extendbytes, MS_U16 syncbyte);
EMMFLT_RESULT MDrv_EMMFLT_InputMode(MS_U32 x_connection, MS_BOOL bSerial);

void EMMRASP_IntAttach(EMMRASP_e eEng, MS_BOOL Enable);

#ifdef __cplusplus
}
#endif
#endif//_DRVEMMFLT_H_

