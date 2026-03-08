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
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
///
/// @file   apiDMX_tee.h
/// @brief  MStar Demux Driver Interface for Tee
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------
/*! \defgroup DMX TEE modules
  */
//-------------------------------------------------

#ifndef __API_DMX_TEE_H__
#define __API_DMX_TEE_H__

#include "MsCommon.h"


#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// Pocily Mamager Pipie id definitation
#define DMX_PIPE_NULL                                       0xFFFFFFFF
#define DMX_PIPE_TSPFW                                      0x00000001
#define DMX_PIPE_TSPVQ                                      0x00000002
#define DMX_PIPE_PVR_0                                      0x80000000
#define DMX_PIPE_PVR_1                                      0x80000001
#define DMX_PIPE_PVR_2                                      0x80000002
#define DMX_PIPE_PVR_3                                      0x80000003
#define DMX_PIPE_FILE_0                                     0x40000000
#define DMX_PIPE_FILE_1                                     0x40000001
#define DMX_PIPE_FILE_2                                     0x40000002
#define DMX_PIPE_FILE_3                                     0x40000003
#define DMX_PIPE_MMFI_0                                     0x41000000
#define DMX_PIPE_MMFI_1                                     0x41000001

#ifdef MSOS_TYPE_OPTEE
typedef struct
{
    MS_U32 ID[4]; //id[3]: 0, id[2]=address_h, od[1]=address_l, id[0]=length
}DMX_TEE_RESOURCE;
#endif //MSOS_TYPE_OPTEE

typedef struct _DMX_TEE_BUF_Param
{
    MS_U32  u32EngId;             // Engine ID
    MS_U32  u32Opt;               // Option value or MIU select number
    MS_U32  u32BufSize;          // Phsical Buffer Size
    MS_U32  u32BufAddr;          // Phsical Buffer Address
    MS_U32  u32BufAddr_H;        // Phsical Buffer Address_H
} DMX_TEE_BUF_Param;

typedef enum
{
    E_DMX_TEECMD_SET_TSPFW_BUF,                ///< For TSP FW buffer control
    E_DMX_TEECMD_SET_TSPVQ_BUF,                ///< For TSP VQ buffer control
    E_DMX_TEECMD_SET_PVR_BUF,                  ///< For TSP PVR buffer control
    E_DMX_TEECMD_SET_FILEIN_BUF,               ///< For TSP Finein buffer control
    E_DMX_TEECMD_SET_MMFI_BUF,                 ///< For TSP MMFI buffer control
} DMX_TEE_USER_CMD;

typedef enum
{
    E_DMX_REE_TO_TEE_CMD_NULL,
    E_DMX_REE_TO_TEE_CMD_FW_LoadCode,               //None parameters
    E_DMX_REE_TO_TEE_CMD_SET_VqBuf,                 //None parameters
    E_DMX_REE_TO_TEE_CMD_SET_PvrBuf,                //Param1: Engine id, Param2: Option 1 is reset buffer to 0
    E_DMX_REE_TO_TEE_CMD_SET_FileinBuf,             //Param1: Engine id, Param2: Buf address, Param3: Buf size
    E_DMX_REE_TO_TEE_CMD_GET_PvrWPtr,               //Param1: Engine id, Param2: Return Buf address
    E_DMX_REE_TO_TEE_CMD_SET_MMFIBuf                //Param1: Engine id, Param2: Return Buf address
} DMX_REE_TO_TEE_CMD_TYPE;

//-----------------------------------------------------------------------------
//  Functions
//-----------------------------------------------------------------------------


//Used for TEE
//-------------------------------------------------------------------------------------------------
/// Create DMX Tee service task
/// @ingroup DMX_Tee
/// @return TRUE or FALSE for Pass or Fail
/// @note Please call this API when Tee system init, and must create this task before DMX init is called
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_NUTTX
MS_BOOL MApi_DMX_TEE_Create_Tee_System(void);
#endif

//------------------------------------------------------------------------------
/// SetControl for TEE
/// @ingroup DMX_Tee
/// @param cmd \b IN : TEE user cmd
/// @param param \b IN : TEE control parameters
/// @return MS_BOOL
/// @note Tee set DMX setting inform to DMX
//------------------------------------------------------------------------------
MS_BOOL MApi_DMX_TEE_SetControl(DMX_TEE_USER_CMD cmd, void* param);

//------------------------------------------------------------------------------
/// Process REE commend
/// @param cmd \b IN : Ree Commend
/// @param param \b IN : TEE control parameters
/// @return MS_U32 Error code
/// @note For buffer control commend, the input parameters should be DMX_TEE_BUF_Param
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_TEE_ProcReeCmd(DMX_REE_TO_TEE_CMD_TYPE cmd, void* param);

#ifdef MSOS_TYPE_OPTEE
//------------------------------------------------------------------------------
/// Get secure resource by Pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param pu32MaxNRetRes \b IN : Pointer to store max request resource number
/// @param pResource \b OUT : Pointer to store resource data
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_GetResourceByPipeID(MS_U32 u32PipeID, MS_U32* pu32MaxNRetRes, DMX_TEE_RESOURCE* pResource);

//------------------------------------------------------------------------------
/// Configure secure resource by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Error code
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_ConfigPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);

//------------------------------------------------------------------------------
/// Check secure resource status by pipe ID
/// @param u32PipeID \b IN : Pipe ID
/// @param u32SecureDMA \b IN : 0 -> non-secure dma mode; 1 -> secure dma mode
/// @param u32OperationMode \b IN : 0 -> unlock output; 1 -> lock output
/// @return MS_U32 Status: 0 -> Configure failed; 1 -> Configure OK
//------------------------------------------------------------------------------
MS_U32 MApi_DMX_CheckPipe(MS_U32 u32PipeID, MS_U32 u32SecureDMA, MS_U32 u32OperationMode);
#endif //MSOS_TYPE_OPTEE

#ifdef __cplusplus
}
#endif

#endif // #ifndef __API_DMX_TEE_H__
