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
/// @file   apiVOIPMFE.h
/// @brief  MStar Multi Format Video Encoder driver
/// @version 1.0
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup VDEC VDEC modules

    *! \defgroup VENC VENC(video encoder) interface.
     *  \ingroup VDEC
    This is the MStar Multi Format Video Encoder driver
 
    <b>Calling sequences</b> 
    
   -# MApi_MFE_Initialize
   -# MApi_MFE_Encode
   -# MApi_MFE_CompressOnePicture     //loop
   -# (optional) MApi_MFE_GetVOL
   -# (optional) MApi_MFE_SetConfig
   -# (optional) MApi_MFE_GetConfig
   -# MApi_MFE_DeInitialize
    
 *! \defgroup MFE Multi Format Video Encoder driver
 *  \ingroup VENC

 * *! \defgroup MFE_BASIC MFE driver basic.
 *  \ingroup MFE

 *! \defgroup MFE_CONFIG MFE driver set/get configure.
 *  \ingroup MFE

 *! \defgroup MFE_OPERATION MFE driver operation.
 *  \ingroup MFE

 *! \defgroup MFE_toBeRemove MFE driver API to be removed.
 *  \ingroup MFE
 */

#ifndef _API_VOIP_MFE_H_
#define _API_VOIP_MFE_H_

#ifdef __cplusplus
extern "C"
{
#endif

// below function is used for T8/T12/Janus2 VOIP project

/* vop coding modes */
// type: I_VOP, P_VOP, or B_VOP
#define I_VOP        0UL
#define P_VOP        1UL
#define B_VOP        2UL


 typedef enum {
    PQ_LOW,
    PQ_NORMAL,
    PQ_HIGH
 } PQ_LEVEL;

 typedef enum {
    VOIP_MPEG4,
    VOIP_H263,
    VOIP_H264,
    VOIP_JPEG
 } VOIP_CODEC_TYPE;

typedef enum {
    MPEG4_ENC = 0,
    H263_ENC = 1,
    H264_ENC = 2,
    JPEG_ENC = 3,
    UNKNOWN_TYPE = 0xFF
} MFE_CODEC_TYPE;


/*
typedef enum {
    MFE_RET_OK                      = 1,
    MFE_RET_FAIL                    = -1000,
    MFE_RET_CLOSE_FAIL              = -1002,

    MFE_RET_DEC_INIT_FAIL           = -2000,

    MFE_RET_INVALID_PARAM           = -4000
} MFE_ERROR_CODE;
*/

typedef enum {
    MFE_ENC_SETCONF_FRAME_TYPE = 100,
    MFE_ENC_SETCONF_CHANGE_FRAME_RATE,
    MFE_ENC_SETCONF_CHANGE_BIT_RATE,
    MFE_ENC_SETCONF_OUT_BSP_BUFFER,
    MFE_ENC_SETCONF_FRAME_TAG,
    MFE_ENC_SETCONF_JPE_QUALITY,
    MFE_ENC_SETCONF_CHANGE_BIT_RATE_AND_FRAME_RATE,
    MFE_ENC_SETCONF_RESET_SPSPPS,
    MFE_ENC_SETCONF_SET_COLORFORMAT,

    MFE_ENC_GETCONF_FRAME_TAG = 300,
    MFE_ENC_GETCONF_OUT_BSP_BUFFER,
    MFE_ENC_GETCONF_JPE_QUALITY,
    MFE_ENC_GETCONF_DROP,
} MFE_ENC_CONF;

// <<< Calling sequences >>>
//
//      MApi_MFE_Initialize
//      MApi_MFE_Encode
//      MApi_MFE_CompressOnePicture     //loop
//      (optional) MApi_MFE_GetVOL
//      (optional) MApi_MFE_SetConfig
//      (optional) MApi_MFE_GetConfig
//      MApi_MFE_DeInitialize


//-------------------------------------------------------------------------------------------------
/// Notify driver to start encoder.
/// @ingroup MFE_toBeRemove
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_MFE_Encode(void);

//-------------------------------------------------------------------------------------------------
/// Encode one frame.
/// @ingroup MFE_BASIC
/// @param  YUVPlane               \b IN: YUVPlane The image physical address. it must be 256 alignment.
/// @param  bForceIframe           \b IN: bForceIframe force this frame tobe I.
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_MFE_CompressOnePicture_ex(MEMMAP_CUR_t YUVPlane, MS_BOOL bForceIframe);

//-------------------------------------------------------------------------------------------------
/// Get bitstream header.
/// @ingroup MFE_BASIC
/// @param  header_info              \b OUT: return header size and length.
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_MFE_GetVOL_ex(void *header_info);

//-------------------------------------------------------------------------------------------------
/// De-initialize MFE driver
/// @ingroup MFE_BASIC
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_MFE_DeInitialize(void);

//-------------------------------------------------------------------------------------------------
/// Get MFE driver state.
/// @ingroup MFE_operation
/// @return MFE driver state
//-------------------------------------------------------------------------------------------------
// MFE_STATE MApi_MFE_GetState(void);

//MS_BOOL MApi_MFE_GenSPSPPS(MS_U32 pHeaderSPS,MS_U32 SPSLen,MS_U32 pHeaderPPS,MS_U32 PPSLen);


MS_BOOL MApi_MFE_Initialize_ex(PVR_Info* in_Info,
StreamCallback out_cb,CleanCallback mfe_CleanBufStatus, void *ctx);
//-------------------------------------------------------------------------------------------------
/// Set config to MFE driver.
/// @ingroup MFE_CONFIG
/// @param  conf_type              \b IN: set config type.
/// @param  value                  \b IN: set config value.
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_MFE_SetConfig(MFE_ENC_CONF conf_type, void *value);

//-------------------------------------------------------------------------------------------------
/// Get config from MFE driver.
/// @ingroup MFE_CONFIG
/// @param  conf_type              \b IN: get config type.
/// @param  value                  \b OUT: return config value.
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_MFE_GetConfig(MFE_ENC_CONF conf_type, void *value);

//-------------------------------------------------------------------------------------------------
/// Get MFE HW capability.
/// @ingroup MFE_CONFIG
/// @param  pDrv                 \b OUT: Get HW capability.
/// @param  len                  \b OUT: Get HW capability length of struct.
/// @return TRUE : succeed
/// @return FALSE : fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MApi_MFE_GetHWCap(MFE_HwCapV0 *pDrv, MS_U32 *len);
#ifdef __cplusplus
}
#endif

#endif

