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


#ifndef _DRV_MFE_ST_H_
#define _DRV_MFE_ST_H_

#ifdef __cplusplus
extern "C"
{
#endif


#define MFE_SUPPORT_REDUCE_MODE
// This version of MFE driver supports "reduce mode" which includes
// two flags that can be turned on/off seperately. Refer to
// 'enableReduceBW' and 'enableFastMode' in struct PVR_Info below.


typedef enum {
    MFE_Invalid            = 0x00,
    MFE_Init                 = 0x20,
    MFE_Idle                =0x30,
    MFE_Busy               =0x40,
    MFE_DeInit             =0x50,
    MFE_Error               =0x60
 } MFE_STATE;

typedef enum
{
MFE_CLK_VERY_SLOW = 0,
MFE_CLK_SLOW = 1,
MFE_CLK_MEDIUM = 2,
MFE_CLK_FAST = 3,
} MFE_CLK_LEVEL;


typedef enum
{
BITS_INFO_ZERO = 0,
BITS_INFO_UNKNOW,
BITS_INFO_FRAMEDONE,
BITS_INFO_BUFFULL,
BITS_INFO_IMG_BUFFULL,
} BITS_STATUS_INFO;

typedef enum
{
GETBITS_NOT_READY=0,
GETBITS_READY=1,
} MFE_HW_STATUS;

typedef enum
{
PROGRESSIVE = 0,
INTERLACE_SPLITED,
INTERLACE_NONSPLITED,
} MFE_INTERLACE_INFO;

typedef enum
{
YUVTILE = 0,
MSTTILE = 1,
MFE_YUYV = 2,
MFE_YVYU = 3,
MFE_UYVY = 4,
MFE_VYUY = 5,
} MFE_COLOR_FORMAT_INFO;

#define MAX_OUTBUFFER 10UL

typedef struct {
    MS_PHY miuAddress;    //hardware physical
    MS_VIRT miuPointer;    //need to OR 0xA0000000
    MS_VIRT virtual_addr;
    MS_S32 size;
    BITS_STATUS_INFO status;    // 1: frame_done. 2: buffer full,3: image buffer full.
    MS_S32 is_more_bits; //1: there are more bitstream packet;  0: this is the last bitstream packet
    MS_U8  voptype;
    MS_PHY IVOP_address;
    MS_S32 stuffing_bytes;     //CABAC stuffing byte
} BITSFRAME_INFO;

/* Linear memory area descriptor */
typedef struct MEMMAP
{
    //MS_U32 *miuPointer;
    MS_U8 *miuPointer;
    MS_PHY miuAddress;
    MS_U32 size;
} MEMMAP_t;

typedef struct MEMMAP_CUR
{
    MEMMAP_t Cur_Y0;
    MEMMAP_t Cur_C0;
    MEMMAP_t Cur_Y1;
    MEMMAP_t Cur_C1;
} MEMMAP_CUR_t;

/* madp_mfe crop info */
typedef struct _MFE_CROP_INFO
{
    MS_BOOL crop_en;
    MS_U32 crop_top;
    MS_U32 crop_bottom;
    MS_U32 crop_left;
    MS_U32 crop_right;
} MFE_CROP_INFO_t;

/* madp_mfe api info */
typedef struct _MFE_API_INFO
{
    MS_PHY miu_offset;
    MS_PHY mfe_phy_buf_addr;
    MS_VIRT mfe_vir_buf_ptr;
    MS_U32 mfe_phy_buf_size;
    MS_U32 Encode_stop;
} MFE_API_INFO_t;

typedef struct _MFE_ADV_INFO
{
    MS_U32 low_bandwidth_en;
    MS_U32 input_imi_en;
    MEMMAP_t imi_buf_addr;
} MFE_ADV_INFO_t;

typedef struct{
    MS_U32 is_off;
    MFE_CLK_LEVEL clock_level;
} POWERON_t;

typedef struct _SPSPPS_INFO
{
    MS_U8* pHeaderSPS;
    MS_U32 SPSLen;
    MS_U8* pHeaderPPS;
    MS_U32 PPSLen;
} SPSPPS_INFO_t;

typedef struct _VOL_INFO
{
    MS_U8* pHeader;
    MS_U32 Len;
} VOL_INFO_t;

// bFirst_or_last_byte_of_Frame: 0x00 error state, 0x10 first , 0x01 last at frame, 0x11 one frame
typedef void (*StreamCallback)(void *pCtx, MS_S32 s32FrameType, void *pStreamData, MS_U32 u32DataLen, MS_U32 bFirstOrLastByteOfFrame);

typedef void (*CleanCallback)(MS_U32 u32FrameCoded, MS_VIRT u32YUVPlane);


// MADP-layer "simplified" encoder configuration
// NOTE: "Advanced" configurations are hidden inside DRV-layer.
typedef struct{
    // MFE-compliant
    MS_S32 width;
    MS_S32 height;
    MS_PHY BitsBuffStart;
    MS_U32 BitsBuffSize;
    MS_S32 nOutBuffer;
    MS_S32 OutBufferSize;
    MEMMAP_t InBuffer;
    MEMMAP_t BitstreamOutBuffer;
    MS_S32 BitstreamOutBufferSize;

    // MFE new settings
    //MFE_COLOR_FORMAT_INFO nColorFormat;
    MS_S32 nCodecType;
    MS_S32 nDispWidth, nDispHeight;    // Display resolution
    MS_S32 nBufWidth, nBufHeight;        // Image buffer resolution (aligned to 16 or 32 byte, depends on nCodecType)
    MFE_INTERLACE_INFO bInterlace;
    MFE_COLOR_FORMAT_INFO bColorFormat;
    // GOP
    MS_S32 nPbetweenI;
    MS_S32 nBbetweenP;
    MS_S32 nP_is_infinite;
    // Rate control
    MS_S32 FrameRatex100;
    MS_S32 nBitrate;
    MS_S32 m_bFrameMode;
    MS_S32 out_frametag_top;
    // Video length
    MS_S32 nTotalFrames;    // -1 means infinite. 0 is not allowed.
    MS_S32 TimeIncreamentRes;              // [IN] frame rate
    MS_S32 VopTimeIncreament;              // [IN] frame rate
    //jpeg parameter
    MS_S32 quality;
    MS_S32 fdc_mode;
    MS_S32 exif_enable;
    MFE_CLK_LEVEL MFEClk;

    MFE_API_INFO_t MfeApiInfo;
    MFE_ADV_INFO_t MfeAdvInfo;

    MS_U32 nDropFlag;
    MS_U32 nSkipFlag;
    MS_U32 enableVTRateControl;
    MS_U32 enableSecurityMode;
    MS_U32 enableISR;
    MS_U32 enableCABAC; //main profile

    MFE_CROP_INFO_t  MfeCropInfo;
    MS_U32 enableReduceBW; //Memory BW reduced mode
    MS_U32 enableFastMode; //intra mode inly 0~2, drop i16 and disable fme.
    MFE_STATE mfe_state;
    void* g_pCustomCtx;
    StreamCallback g_StreamCallBackFunc;
    CleanCallback g_CleanCallBackFunc;

    void* mfe_drvHandle;

    MS_S32 reserved0;
    MS_S32 reserved1;
    MS_S32 reserved2;
    MS_S32 reserved3;
    MS_S32 reserved4;
    MS_S32 reserved5;
}PVR_Info;

#define API_HWCAP_VERSION0 1UL

typedef struct{
    MS_U32 u32ApiHW_Version;   //<Version of current structure>
    MS_U16 u16ApiHW_Length;    //<Length of this structure>
    MS_BOOL bIsSupportEncoder;
    MS_U8 u8MaxEncodeFrame;
    MS_U16 u16MaxEncodeWidth;
    MS_U16 u16MaxEncodeHeight;
    MS_BOOL bSupportSubVidWin;
    MS_BOOL bSupportMpeg4;
    MS_BOOL bSupportH264;
    /*New Cap add in struct add at the end*/
}MFE_HwCapV0;

#ifdef __cplusplus
}
#endif

#endif
