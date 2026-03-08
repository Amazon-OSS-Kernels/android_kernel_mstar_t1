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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   apiHDMITx_NDS.h
/// @brief  HDMI Tx API header file for NDS
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_HDMITX_NDS_H_
#define _API_HDMITX_NDS_H_

#include "hdmi.h"


#ifdef CUSTOMER_NDS
//17.7.7.5 Ioctl: Get Events
DLL_PUBLIC MS_BOOL MApi_HDMITx_Get_Events(MS_U32 *events, MS_BOOL bEraseEvent);
//17.7.7.8 Ioctl: Get Connection Status
DLL_PUBLIC MS_BOOL MApi_HDMITx_Get_Rx_CONNECTION_STATUS(HDMIConnectionStatus *status);
//17.7.7.9 Ioctl: Get EEDID Block
DLL_PUBLIC MS_BOOL MApi_HDMITx_Get_EEDID_Block(HDMIGetEEDIDBlock *eedid);
//17.7.7.10 Ioctl: Start
DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_HDMI_Protocol(MS_U8 protocol);
//17.7.7.11 Ioctl: Stop
DLL_PUBLIC MS_BOOL MApi_HDMITx_Stop(void);
//17.7.7.14 Ioctl: Set Video Code
DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_Video_Code(MS_U8 *videoCode);
//17.7.7.15 Ioctl: Set Pixel Format
DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_Pixel_Format(MS_U8 *pixelFormat);
//17.7.7.16 Ioctl: Set Aspect Ratio
DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_Aspect_Ratio(HDMIAspectRatio *aspectRatio);
//17.7.7.17 Ioctl: Set Colorimetry
DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_Colorimetry(HDMIColorimetry *colorimetryData);
//17.7.7.18 Ioctl: Set AVMUTE
//17.7.7.19 Ioctl: Clear AVMUTE
DLL_PUBLIC MS_BOOL MApi_HDMITx_Set_AVMUTE(MS_BOOL bflag);
//17.7.7.20 Ioctl: HDCP Start Authentication
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Start_Authentication(void);
//17.7.7.21 Ioctl: HDCP Stop Authentication
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Stop_Authentication(void);
//17.7.7.22 Ioctl: HDCP Get Bcaps
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Get_BCaps(HDMIHDCPBCaps *arg);
//17.7.7.23 Ioctl: HDCP Get Bstatus
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Get_BStatus(HDMIHDCPBStatus *arg);
//17.7.7.24 Ioctl: HDCP Send SRM List
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Send_SRM(DigitalCPSrm *srm);
DLL_PUBLIC void MApi_HDMITx_HDCP_Reset_SRM(void);
//17.7.7.25 Ioctl: HDCP Encryption Control
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Encryption_Enable(MS_U8 control);
//17.7.7.26 Ioctl: HDCP Get Status
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Get_HDCP_Status(MS_U8 *status);
//17.7.7.27 Ioctl: HDCP Configure
DLL_PUBLIC MS_BOOL MApi_HDMITx_HDCP_Configure(HDMIHDCPConfiguration *arg);
//17.7.7.28 Ioctl: CEC Read
DLL_PUBLIC MS_BOOL MApi_HDMITx_CEC_Read(HDMICECRead *readMsgs);
//17.7.7.29 Ioctl: CEC Write
DLL_PUBLIC MS_BOOL MApi_HDMITx_CEC_Write(HDMICECMessage *msg);

#endif //CUSTOMER_NDS


#if 0


#include "MsTypes.h"
//#include "halHDMITx.h"
//#include "drvHDMITx.h"
//#include "regHDMITx.h"



#ifdef __cplusplus
extern "C"
{
#endif

#define CUSTOMER_NDS

#ifdef CUSTOMER_NDS
#ifndef __HDMI_H__
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


/* HDMI Event definitions. */
#define HDMI_EVENT_HOTPLUGGED                                  (0x00000001U)
#define HDMI_EVENT_HOTUNPLUG                                     (0x00000002U)
#define HDMI_EVENT_CEC_RECEIVED                                (0x00000010U)
#define HDMI_EVENT_CP_AUTHENTICATED                        (0x00100000U)
#define HDMI_EVENT_CP_AUTHENTICATION_FAILED          (0x00200000U)
#define HDMI_EVENT_CP_KSV_REVOKED                          (0x00400000U)
#define HDMI_EVENT_CP_KSV_INVALID                              (0x00800000U)
#define HDMI_EVENT_CP_LINK_INTEGRITY_FAILED            (0x01000000U)
#define HDMI_EVENT_CP_LINK_INTEGRITY_NO_ACK          (0x02000000U)

/* HDCP Protection Status */
#define HDMI_STATUS_NOT_AUTHENTICATED (1)
#define HDMI_STATUS_AUTHENTICATED (2)
#define HDMI_STATUS_NOT_AUTHENTICATED_KSV_INVALID (3)
#define HDMI_STATUS_NOT_AUTHENTICATED_KSV_REVOKED (4)
#define HDMI_STATUS_NOT_AUTHENTICATED_LINK_INTEGRITY_FAILED (5)
#define HDMI_STATUS_NOT_AUTHENTICATED_LINK_INTEGRITY_NO_ACK (6)
#define HDMI_STATUS_AUTHENTICATION_DISABLED (7)

/* Capabilities Versions */
#define HDMI_CAP_V0 (0x00000001)

/* HDMI Specification Versions */
#define HDMI_SPEC_HDMI_1_0 (0x01)
#define HDMI_SPEC_HDMI_1_1 (0x02)
#define HDMI_SPEC_HDMI_1_2 (0x03)
#define HDMI_SPEC_HDMI_1_2a (0x04)
#define HDMI_SPEC_HDMI_1_3 (0x05)
#define HDMI_SPEC_HDMI_1_3a (0x06)

/* HDCP Specification Versions*/
#define HDMI_SPEC_HDCP_1_0 (0x01)
#define HDMI_SPEC_HDCP_1_1 (0x02)
#define HDMI_SPEC_HDCP_1_2 (0x03)
#define HDMI_SPEC_HDCP_1_3 (0x04)

/* HDMI Features */
#define HDMI_FEATURE_CEC (0x00000001)
#define HDMI_FEATURE_EXTENDED_COLORIMETRY (0x00000002)
#define HDMI_FEATURE_PIXEL_FORMAT_RGB (0x00000010)
#define HDMI_FEATURE_PIXEL_FORMAT_YCbCr422 (0x00000020)
#define HDMI_FEATURE_PIXEL_FORMAT_YCbCr444 (0x00000040)

#define HDMI_EEDID_BLOCK_SIZE (128)

/* HDMI Protocol */
#define HDMI_PROTOCOL_DVI (1)
#define HDMI_PROTOCOL_HDMI (2)

/* HDMI Pixel Format */
#define HDMI_PIXEL_FORMAT_RGB (0x00)
#define HDMI_PIXEL_FORMAT_YCbCr422 (0x20)
#define HDMI_PIXEL_FORMAT_YCbCr444 (0x40)

/* Source Aspect Ratio Definitions */
#define HDMI_SOURCE_ASPECT_RATIO_NO_DATA (0x00)
#define HDMI_SOURCE_ASPECT_RATIO_4_3 (0x10)
#define HDMI_SOURCE_ASPECT_RATIO_16_9 (0x20)
/* Active Format Descriptor Definitions */
#define HDMI_AFD_SAME_AS_PICTURE (0x08)
#define HDMI_AFD_4_3_CENTER (0x09)
#define HDMI_AFD_16_9_CENTER (0x0A)
#define HDMI_AFD_14_9_CENTER (0x0B)
/* AFDInfo Valid Definitions */
#define HDMI_AFDINFO_INVALID (0x00)
#define HDMI_AFDINFO_VALID (0x10)
/* BarInfo Valid Definitions */
#define HDMI_BARINFO_INVALID (0x00)
#define HDMI_BARINFO_VERTICAL_VALID (0x04)
#define HDMI_BARINFO_HORIZONTAL_VALID (0x08)
#define HDMI_BARINFO_BOTH_VALID (0x0C)
/* ScanInfo Valid Definitions */
#define HDMI_SCANINFO_NO_DATA (0x00)
#define HDMI_SCANINFO_OVERSCANNED (0x01)
#define HDMI_SCANINFO_UNDERSCANNED (0x02)
/* Scaling Definitions */
#define HDMI_SCALING_NO_SCALING (0x00)
#define HDMI_SCALING_HORIZONTAL (0x01)
#define HDMI_SCALING_VERTICAL (0x02)
#define HDMI_SCALING_HORIZONTAL_VERTICAL (0x03)

/* Colorimetry Definitions */
#define HDMI_COLORIMETRY_NONE (0x00)
#define HDMI_COLORIMETRY_SMPTE_170M (0x40)
#define HDMI_COLORIMETRY_ITU_709 (0x80)
#define HDMI_COLORIMETRY_EXTENDED (0xC0)
/* Extended Colorimetry Definitions */
#define HDMI_EXTENDED_COLORIMETRY_XVCC601 (0x00)
#define HDMI_EXTENDED_COLORIMETRY_XVCC709 (0x10)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// cccc
/* HDMI CapsEx Structure */
typedef struct
{
    MS_U32 version;
    void* capability;
    MS_U32 capabilitySize;
} HDMICapsEx;

/// cccc
/* Struct HDMICapsV0 */
typedef struct
{
    MS_U32 features;                         // HDMI Features
    MS_U8 numBlenderConnections;  // The number of different blender connections supported by this device
    MS_U8 numSPDIFConnections;    // The number of different SPDIF Formatter connections supported by this HDMI device.
    MS_U8 hdmiVersion;                     // HDMI Specification Versions
    MS_U8 hdcpVersion;                     // HDCP Specification Versions
} HDMICapsV0;

/// cccc
typedef struct
{
    MS_U8 attached;
    MS_U8 powered;
} HDMIConnectionStatus;

/// cccc
typedef struct
{
    MS_U32 blockNumber;
    MS_U8 eedid[HDMI_EEDID_BLOCK_SIZE];
    MS_U32 timeOut;
} HDMIGetEEDIDBlock;

/// cccc
typedef struct
{
    MS_U8 pictureAspectRatio;
    MS_U8 activeFormatDescriptor;
    MS_U16 topBarEndsLine;
    MS_U16 bottomBarStartsLine;
    MS_U16 leftBarEndsPixel;
    MS_U16 rightBarStartsPixel;
    MS_U8 barinfoValidity;
    MS_U8 afdValidity;
    MS_U8 scanInfo;
    MS_U8 scalingInfo;
} HDMIAspectRatio;

/// cccc
typedef struct
{
    MS_U8 colorimetry;
    MS_U8 extendedColorimetry;
} HDMIColorimetry;

/// cccc
typedef struct
{
    MS_U8 bcaps;
    MS_U32 timeOut;
} HDMIHDCPBCaps;

/// cccc
typedef struct
{
    MS_U16 bstatus;
    MS_U32 timeOut;
} HDMIHDCPBStatus;

typedef struct
{
    MS_U32 length;
    MS_U8 *data;
} DigitalCPSrm;

/// cccc
typedef struct
{
    MS_U32 encryptionStartTime;
    MS_U8 checkPjIntegrity;
} HDMIHDCPConfiguration;


// cccc
typedef struct
{
	MS_U8 header;
	MS_U8 messageLength ;
	MS_U8 message[16];
} HDMICECMessage;

/// cccc
typedef struct
{
	HDMICECMessage *messages ;
	MS_U8 numMessages;
	MS_U8 numMessagesReturned;
} HDMICECRead;

#endif
//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//17.7.7.5 Ioctl: Get Events
MS_S8 MApi_HDMITx_Get_Events(MS_U32 *events);
//17.7.7.8 Ioctl: Get Connection Status
MS_S8 MApi_HDMITx_Get_Rx_CONNECTION_STATUS(HDMIConnectionStatus *status);
//17.7.7.9 Ioctl: Get EEDID Block
MS_S8 MApi_HDMITx_Get_EEDID_Block(HDMIGetEEDIDBlock *eedid);
//17.7.7.10 Ioctl: Start
MS_S8 MApi_HDMITx_Set_HDMI_Protocol(MS_U8 protocol);
//17.7.7.11 Ioctl: Stop
MS_S8 MApi_HDMITx_Stop(void);
//17.7.7.14 Ioctl: Set Video Code
MS_S8 MApi_HDMITx_Set_Video_Code(MS_U8 *videoCode);
//17.7.7.15 Ioctl: Set Pixel Format
MS_S8 MApi_HDMITx_Set_Pixel_Format(MS_U8 *pixelFormat);
//17.7.7.16 Ioctl: Set Aspect Ratio
MS_S8 MApi_HDMITx_Set_Aspect_Ratio(HDMIAspectRatio *aspectRatio);
//17.7.7.17 Ioctl: Set Colorimetry
MS_S8 MApi_HDMITx_Set_Colorimetry(HDMIColorimetry *colorimetryData);
//17.7.7.18 Ioctl: Set AVMUTE
//17.7.7.19 Ioctl: Clear AVMUTE
MS_S8 MApi_HDMITx_Set_AVMUTE(MS_BOOL bflag);
//17.7.7.20 Ioctl: HDCP Start Authentication
MS_S8 MApi_HDMITx_HDCP_Start_Authentication(void);
//17.7.7.21 Ioctl: HDCP Stop Authentication
MS_S8 MApi_HDMITx_HDCP_Stop_Authentication(void);
//17.7.7.22 Ioctl: HDCP Get Bcaps
MS_S8 MApi_HDMITx_HDCP_Get_BCaps(HDMIHDCPBCaps *arg);
//17.7.7.23 Ioctl: HDCP Get Bstatus
MS_S8 MApi_HDMITx_HDCP_Get_BStatus(HDMIHDCPBStatus *arg);
//17.7.7.24 Ioctl: HDCP Send SRM List
MS_S8 MApi_HDMITx_HDCP_Send_SRM(DigitalCPSrm *srm);
//17.7.7.25 Ioctl: HDCP Encryption Control
MS_S8 MApi_HDMITx_HDCP_Encryption_Enable(MS_U8 control);
//17.7.7.26 Ioctl: HDCP Get Status
MS_S8 MApi_HDMITx_HDCP_Get_HDCP_Status(MS_U8 *status);
//17.7.7.27 Ioctl: HDCP Configure
MS_S8 MApi_HDMITx_HDCP_Configure(HDMIHDCPConfiguration *arg);
//17.7.7.28 Ioctl: CEC Read
MS_S8 MApi_HDMITx_CEC_Read(HDMICECRead *readMsgs);
//17.7.7.29 Ioctl: CEC Write
MS_S8 MApi_HDMITx_CEC_Write(HDMICECMessage *msg);

#endif //CUSTOMER_NDS

#ifdef __cplusplus
}
#endif

#endif
#endif // _API_HDMITX_NDS_H_


