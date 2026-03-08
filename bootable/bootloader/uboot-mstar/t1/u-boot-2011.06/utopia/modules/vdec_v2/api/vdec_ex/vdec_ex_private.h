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

#ifndef _API_VDEC_EX_V2_PRIV_H_
#define _API_VDEC_EX_V2_PRIV_H_

#if !defined(MSOS_TYPE_NUTTX) || defined(SUPPORT_X_MODEL_FEATURE)
////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "apiVDEC_EX.h"
#include "apiVDEC_EX_v2.h"
////////////////////////////////////////////////////////////////////////////////
// Define for VDEC layer
////////////////////////////////////////////////////////////////////////////////

#define SUPPORT_DECODER_NUM_MAX     2
#define VDEC_EX_V2_BIT(_bit_)      (1 << (_bit_))
#define MAX_NUM_OF_STORE_USER_SET_CMD  100
#define MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG 4
//#define MAX_NUM_OF_STORE_USER_SET_CMD_ARG MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG+1
////////////////////////////////////////////////////////////////////////////////
// Enum for VDEC layer
////////////////////////////////////////////////////////////////////////////////

enum
{
    E_VDEC_EX_POOL_ID_INTERNAL = 0, 
    E_VDEC_EX_POOL_ID_MAX,
} E_VDEC_EX_POOL_ID;


////////////////////////////////////////////////////////////////////////////////
// Function pointer for VDEC layer
////////////////////////////////////////////////////////////////////////////////
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_GetFreeStream)        (void*,MS_U32,VDEC_EX_Stream,VDEC_EX_CodecType);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_Init)                 (VDEC_StreamId*,VDEC_EX_InitParam*);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_SetControl)           (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**,MS_BOOL);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_GetControl)           (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_PreSetControl)        (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
typedef VDEC_EX_Result      (*IOCTL_VDEC_EX_PostSetControl)       (VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);

////////////////////////////////////////////////////////////////////////////////
// Structure for VDEC layer
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    MS_U8 u8DecoderInUse[SUPPORT_DECODER_NUM_MAX];
}VDEC_EX_RESOURCE_PRIVATE;


typedef struct
{
    // instance variable
    MS_U8                                Index_of_decoder;                             
    // Function pointer
    IOCTL_VDEC_EX_GetFreeStream          fpVDEC_EX_GetFreeStream;
    IOCTL_VDEC_EX_Init                   fpVDEC_EX_Init;
    IOCTL_VDEC_EX_SetControl             fpVDEC_EX_SetControl;
    IOCTL_VDEC_EX_GetControl             fpVDEC_EX_GetControl;
    IOCTL_VDEC_EX_PreSetControl          fpVDEC_EX_PreSetControl;
    IOCTL_VDEC_EX_PostSetControl         fpVDEC_EX_PostSetControl;
}VDEC_EX_INSTANCE_PRIVATE;


typedef struct
{
    VDEC_EX_V2_User_Cmd cmd;
    MS_VIRT              u32Arg[MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG];
    //void*               pStructArg;
}VDEC_EX_RECORD_SET_CMD;

typedef struct
{
    MS_U8 u8Idx;
    VDEC_EX_V2_User_Cmd cmd_id;
    void** param;
    MS_U32  u32Arg_num;
    MS_BOOL structArg_num;  
    //MS_U32 struct_size;
    MS_U8  position[MAX_NUM_OF_STORE_USER_SET_CMD_U32ARG]; // index 0~4 for U32ARG position, index 5 for structARG position
}VDEC_EX_RESUME_SET_CMD;



////////////////////////////////////////////////////////////////////////////////
// Function for VDEC layer
////////////////////////////////////////////////////////////////////////////////

VDEC_EX_Result MApi_VDEC_EX_V2_GetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
VDEC_EX_Result MApi_VDEC_EX_V2_SetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**,MS_BOOL);
VDEC_EX_Result MApi_VDEC_EX_V2_GetFreeStream(void*,MS_U32,VDEC_EX_Stream,VDEC_EX_CodecType);
VDEC_EX_Result MApi_VDEC_EX_V2_Init(VDEC_StreamId*,VDEC_EX_InitParam*);
VDEC_EX_Result MApi_VDEC_EX_V2_PreSetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);
VDEC_EX_Result MApi_VDEC_EX_V2_PostSetControl(VDEC_StreamId*,VDEC_EX_V2_User_Cmd,void**);

#ifdef __cplusplus
}
#endif

#endif
#undef _API_VDEC_EX_V2_PRIV_H_
#endif // _API_VDEC_EX_V2_PRIV_H_
