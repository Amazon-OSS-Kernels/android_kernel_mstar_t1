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

#ifndef _DRVDIP_H_
#define _DRVDIP_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvDIP.h
/// @brief DIP control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#define MAX_NR_BUFF_CNT     2UL
#define MAX_DATA_BUFF_CNT   16UL

typedef struct
{
    MS_U8               u8NR_BufCnt;
    MS_PHY              u32NR_RationBuf;
    MS_PHY              u32NR_DataBuf[MAX_NR_BUFF_CNT];
    MS_U32              u32NR_BufWidth;
    MS_U32              u32NR_BufHeight;
} NR_INFO;

typedef struct
{
    MS_U8               u8DI_BufCnt;
    MS_PHY              u32DI_YBuf[MAX_DATA_BUFF_CNT];
    MS_PHY              u32DI_CBuf[MAX_DATA_BUFF_CNT];
    MS_U32              u32DI_BufWidth;
    MS_U32              u32DI_BufHeight;
    MS_U16              u16Valid_DIBuf;
    MS_U32              u32DI_FrameCnt;
} DI_INFO;


typedef struct
{
    MS_BOOL             Init;
    NR_INFO             NR_Info;
    DI_INFO             DI_Info;

} DIP_MGR;

typedef enum
{
    DIP_ERR_OK,
    DIP_ERR_FAIL,
    DIP_ERR_INVALID_BUFFER_CNT,
    DIP_ERR_INVALID_BUFFER_START,
    DIP_ERR_INVALID_BUFFER_WIDTH,
    DIP_ERR_INVALID_BUFFER_HEIGHT,
    DIP_ERR_INVALID_BUFFER_SZIE
}DIP_ERRCODE;

typedef enum
{
    E_DIP_INPUT_UNKNOWN = 0,
    E_DIP_INPUT_NTSC,
    E_DIP_INPUT_PAL,
    E_DIP_INPUT_WEBCAM,
}DIP_INPUT;

typedef enum
{
    E_DIP_YUV_ORDER_UYVY = 0,
    E_DIP_YUV_ORDER_YUYV = 1,
} DIP_YUV_ORDER;

typedef enum
{
    E_DIP_INIT ,
    E_DIP_ENABLE ,
    E_DIP_ONESHOT ,
    E_DIP_SET_INFO ,
    E_DIP_SET_YUORD ,
    E_DIP_GET_BUFSTUS ,    
    E_DIP_CLR_BUFSTUS ,
    E_DIP_SET_SCANTYPE ,
    E_DIP_SET_INPUTMODE ,
    E_DIP_SET_WEBCAM ,
} DIP_CNTL;

typedef enum
{
    //By HW or SW DIP
    E_DIP_CAP_CONFIG,
}
EN_DIP_CAPS;

typedef enum
{
    E_DIP_CFG_SW =0,
    E_DIP_CFG_HW =1,
}
EN_DIP_Config;

typedef struct
{           
    /// dwin dram starting address (unit: Byte).
    MS_PHY   u32FbAddr0;
    ///  dwin access address high bond (unit: Byte).
    MS_PHY   u32FbAddr1;
    /// dwin width (unit: pix).
    MS_U16 u16W;
    /// height (unit: line).
    MS_U16 u16H;
    
    MS_U8  u8BufCnt;
    
} DIP_PROPERTY;

typedef struct
{           
    /// dwin dram starting address (unit: Byte).
    MS_VIRT u32WebCamAddr0;
    ///  dwin access address high bond (unit: Byte).
    MS_VIRT u32WebCamAddr1;
    /// dwin width (unit: pix).
    MS_U16 u16WebCamW;
    /// height (unit: line).
    MS_U16 u16WebCamH;
    
} DIP_WEBCAM_PROPERTY;

// DIP_ERRCODE MDrv_DIP_GetChipCaps(EN_DIP_CAPS eCapType, MS_U32* pRet, MS_U32 ret_size);

// void MDrv_DIP_Init(MS_U32 u32InputMode);
// void MDrv_DIP_SetFrameInfo(MS_U32 u32FrameX, MS_U32 u32FrameY,
//     MS_U32 u32FrameWidth, MS_U32 u32FrameHeight, MS_BOOL bInterLace);
// void MDrv_DIP_InputMode(MS_U32 u32InputMode);
// void MDrv_DIP_SetNRBuf(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
//                                MS_PHY u32BufStart, MS_PHY u32BufEnd);
// void MDrv_DIP_SetDIBuf(MS_U8 u8BufCnt, MS_U32 u32BufWidth, MS_U32 u32BufHeight,
//                                MS_PHY u32BufStart, MS_PHY u32BufEnd);
// void MDrv_DIP_GetDIInfo(DI_INFO *pDIInfo);
// void MDrv_DIP_GetNRInfo(NR_INFO *pNRInfo);
// void MDrv_DIP_EnableNRDI(MS_BOOL bEnableNR, MS_BOOL bEnableSNR, MS_BOOL bEnableTNR, MS_BOOL bEnableDI);
// void MDrv_DIP_GetDIBufCount(MS_U32 *pu32Count);
// void MDrv_DIP_ClearDIBufStatus(MS_U8 u8Arg);
// void MDrv_DIP_GetDIFrameCount(MS_U32 *pu32FrameCount);
// void MDrv_DIP_GetDIBufStatus(MS_U32 *pu32DIBufStatus);
// void MDrv_DIP_GetDIShow(void);
// void MDrv_DIP_SetMMIOMapBase(void);
// void MDrv_DIP_SetWebCamBuff(MS_U32 u32BufWidth, MS_U32 u32BufHeight,
//                                MS_VIRT u32BufStart, MS_VIRT u32BufEnd);
// void MDrv_DIP_Trigger(void);
// void MDrv_DIP_SetYUVOrder(DIP_YUV_ORDER dipOrder);
// MS_BOOL MDrv_DIP_OneShot(MS_U32 u32BufWidth, MS_U32 u32BufHeight
//     , MS_U32 u32InputBufStart, MS_U32 u32OutputBufStart, MS_U8 u8OutputBufCnt);
// MS_BOOL MDrv_DIP_RegisterXCSetDIPInfo(DIP_ERRCODE (*fpDIP_CB)(MS_U16 cmd,MS_U32 *arg,MS_U16 size));

#ifdef __cplusplus
}
#endif

#endif
