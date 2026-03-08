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

#ifndef _API_VOIP_MFE_H_
#define _API_VOIP_MFE_H_

#ifdef __cplusplus
extern "C"
{
#endif

// below function is used for T8/T12/Janus2 VOIP project

/* vop coding modes */
// type: I_VOP, P_VOP, or B_VOP
#define I_VOP        0
#define P_VOP        1
#define B_VOP        2


 typedef enum {
    PQ_LOW,
    PQ_NORMAL,
    PQ_HIGH
 } PQ_LEVEL;

 typedef enum {
    VOIP_MPEG4,
    VOIP_H263,
    VOIP_H264
 } VOIP_CODEC_TYPE;


 typedef enum {
    MFE_Invalid            = 0x00,
    MFE_Init                 = 0x20,
    MFE_Idle                =0x30,
    MFE_Busy               =0x40,
    MFE_DeInit             =0x50,
    MFE_Error               =0x60
 } MFE_STATE;

// <<< Calling sequences >>>
//
//      MApi_MFE_SetGOP
//      MApi_MFE_SetBufferInfo
//      MApi_MFE_Initialize
//      MApi_MFE_Encode
//      MApi_MFE_CompressOnePicture     //loop
//      (optional) MApi_MFE_GetVOL
//      (optional) MApi_MFE_ResetBitrate
//      (optional) MApi_MFE_ResetFramerate
//      MApi_MFE_DeInitialize



/*!
 * \brief
 *  create MFE thread, start encoder and wait input frame.
 *
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_Encode(void);

/*!
 * \brief
 *  encode one frame. This function will wake up MFE thread to encode, then return directly without wait encode done. MFE thread will callback when enocde done.
 *
 * \param[in] YUVPlane The image physical address. it must be 256 alignment.
 * \param[in] bForceIframe force this frame tobe I.
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_CompressOnePicture(MS_U32 YUVPlane, MS_BOOL bForceIframe);

/*!
 * \brief
 *  Set memory buffer for MFE.
 *
 * \param[in] s32MiuType miu 0, 1 or both.
 * \param[in] u32MfePA physical address without offset. must be 256 alignment.
 * \param[in] u32MiuOffset miu offset.
 * \param[in] u32MemSize buffer size..
 * \return TRUE if success.
 * \sa
 * \note
 * \warning MIU type must be the same with DIP. You must switch miu before encode.
 */
MS_BOOL MApi_MFE_SetBufferInfo(MS_S32 s32MiuType, MS_U32 u32MfePA, MS_U32 u32MiuOffset, MS_U32 u32MemSize);

/*!
 * \brief
 *  Set memory buffer for MFE.
 *
 * \param[out] Buf get SPS, PPS. size = (Buf[1]<<8)|Buf[0];.
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_GetVOL(MS_S8* Buf);

// bFirst_or_last_byte_of_Frame: 0x00 error state, 0x10 first , 0x01 last at frame, 0x11 one frame
typedef void (*StreamCallback)(void *pCtx, MS_S32 s32FrameType, void *pStreamData, MS_U32 u32DataLen, MS_U32 bFirstOrLastByteOfFrame);

typedef void (*CleanCallback)(MS_U32 u32FrameCoded, MS_U32 u32YUVPlane);

/*!
 * \brief
 *  Set GOP size for MFE.
 *
 * \param[in] PbetweenI GOP size when is_infinite=false. If is_infinite=TRUE, PbetweenI will only used for RateControl.That means there is no I frame(all are pframes) only if MApi_MFE_CompressOnePicture be called and bForceIframe=true.
 * \param[in] is_infinite =TRUE will never get I frame. PbetweenI still must set for ratecontrol, you can set a large number. ex:  (1000,TRUE);
 * \return TRUE if success.
 * \sa
 * \note Each I frame include SPS and PPS for SKYPE requested.
 * \warning GOP size always required by MFE. This function must be called before MApi_MFE_Initialize.
 */
MS_BOOL MApi_MFE_SetGOP(MS_S32 PbetweenI,MS_BOOL is_infinite);

/*!
 * \brief
 *  Changed Bitrate Dynamically. This function will reset RC-Ctl.
 *
 * \param[in] bitrate
 * \return TRUE if success.
 * \sa
 * \note
 * \warning This function may effect the performance. You should not call this function frequently.
 */
MS_BOOL MApi_MFE_ResetBitrate(MS_S32 bitrate);

/*!
 * \brief
 *  Changed Framerate Dynamically. This function will reset RC-Ctl.
 *
 * \param[in] framerate
 * \return TRUE if success.
 * \sa
 * \note
 * \warning This function may effect the performance. You should not call this function frequently.
 */
MS_BOOL MApi_MFE_ResetFramerate(MS_S32 framerate);

/*!
 * \brief
 *  Changed Framerate and Bitrate Dynamically. This function will reset RC-Ctl.
 *
 * \param[in] bitrate
 * \param[in] framerate
 * \return TRUE if success.
 * \sa
 * \note
 * \warning This function may effect the performance. You should not call this function frequently.
 */
MS_BOOL MApi_MFE_ResetBitrateAndFramerate(MS_S32 bitrate,MS_S32 framerate);
/*!
 * \brief
 *  MFE initial function.
 *
 * \param[in] codec_type h263/mpeg4/h264.
 * \param[in] u32Width The image width.
 * \param[in] u32Height The image height.
 * \param[in] picture_quality encode quality, higher quality higher bitrate.
 * \param[in] out_cb Callback function will be called after encoded one frame done.
 * \param[in] mfe_CleanBufStatus Callback function will be called after encoded one frame done.
 * \param[in] pCtx use for Callback function.
 * \return TRUE if success.
 * \sa
 * \note
 * \warning
 */
MS_BOOL MApi_MFE_Initialize(VOIP_CODEC_TYPE codec_type,MS_U32 u32Width, MS_U32 u32Height, PQ_LEVEL picture_quality, StreamCallback out_cb, CleanCallback mfe_CleanBufStatus, void *pCtx);

/*!
 * \brief
 *  MFE deinitial function. delete MFE thread.
 *
 * \return TRUE if success.
 * \sa
 * \note
 * \warning.
 */
MS_BOOL MApi_MFE_DeInitialize(void);

/*!
 * \brief
 *  Get MFE state. Only idle state can call the Dynamic function and compress.
 *
 * \return TRUE if success.
 * \sa
 * \note
 * \warning.
 */
MFE_STATE MApi_MFE_GetState(void);

#ifdef __cplusplus
}
#endif

#endif

