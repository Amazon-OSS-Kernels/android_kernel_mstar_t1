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
/// @file   apiGOP_SC.h
/// @brief  MStar Graphics Output Path API
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _API_GOPSC_EX_H_
#define _API_GOPSC_EX_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define MSIF_GOPSC_EX_LIBVER              {'0','0'}            //LIB version
#define MSIF_GOPSC_EX_BUILDNUM            {'1','1'}            //Build Number
#define MSIF_GOPSC_EX_CHANGELIST          {'0','0','3','9','1','8','8','7'} //P4 ChangeList Number

typedef enum
{
    EN_Bilinear = 0,
    EN_Cubic
}EN_GOPSC_VScalingMode;

typedef struct
{
    MS_U16 srcWidth;
    MS_U16 srcHeight;
    MS_U16 destWidth;
    MS_U16 destHeight;
    MS_U8 bEnableScaleX;
    MS_U8 bEnableScaleY;
}GOPSC_SizeInfo;

typedef struct
{
    MS_U16 xStart;
    MS_U16 xEnd;
    MS_U16 yStart;
    MS_U16 yEnd;
    MS_U8 bEnableSkipX;
    MS_U8 bEnableSkipY;
}GOPSC_SkipPixelInfo;

typedef struct
{
    MS_U32 u32Version;
    MS_U32 u32Id;
}GOPSC_DeviceInfo;

typedef enum
{
    ///failed
    En_GOPSC_FAIL = 0,
    ///success
    En_GOPSC_OK,
    ///invalid parameter
    En_GOPSC_INVALID_PARAM,
    ///access not allow
    En_GOPSC_NOT_INITIALIZE,
    ///access not allow
    En_GOPSC_NotSupport,
}GOPSC_Result;

typedef enum
{
    None = 0x0,
    SetOutputRateControl = 0x1,
    EnableP2I = 0x2,
    HalfMode = 0x4,
}EN_GOPSCCmd;

typedef enum
{
    EN_Source_GOP,
    EN_Source_OP,
}EN_GOPSC_Source;

typedef struct
{
    EN_GOPSC_VScalingMode en_VScalingMode;
    EN_GOPSC_Source en_Source;
    MS_U16 dlyLineNumber; //(unit : 2-line)
    MS_BOOL P2IEnable;
    MS_BOOL HalfModeEnable; //output frame rate is half input frame rate
    MS_BOOL OutputRateControlEnable;
    MS_BOOL UseP2IFormula; //If GOP use GOPSC result is P2I, scaling factor formula must use change
}GOPSC_Info;

GOPSC_Result MApi_GOPSC_Init(GOPSC_DeviceInfo* DevInfo);
GOPSC_Result MApi_GOPSC_Enable(GOPSC_DeviceInfo* DevInfo, MS_BOOL enable);
GOPSC_Result MApi_GOPSC_GetGOPSCInfo(GOPSC_DeviceInfo* DevInfo, GOPSC_Info* gopscInfo);
GOPSC_Result MApi_GOPSC_SetGOPSCInfo(GOPSC_DeviceInfo* DevInfo, GOPSC_Info* gopscInfo);
GOPSC_Result MApi_GOPSC_SetHVSPSize(GOPSC_DeviceInfo* DevInfo, GOPSC_SizeInfo* sizeInfo, MS_U32 flag);
// GOPSC_Result MApi_GOPSC_SetSkipPixel(GOPSC_DeviceInfo* DevInfo, GOPSC_SkipPixelInfo* skippixelInfo, MS_U32 flag);
// GOPSC_Result MApi_GOPSC_ScalingDownOnce(GOPSC_DeviceInfo* DevInfo, MS_U8 FrameNum);

//MS_BOOL MApi_GOPSC_SetCommand(EN_GOPSCCmd cmds);
//EN_GOPSCCmd MApi_GOPSC_GetCommand(void);

#ifdef __cplusplus
}
#endif

#endif
