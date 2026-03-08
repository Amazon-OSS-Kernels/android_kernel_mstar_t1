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
/// @file   drvGFLIP.h
/// @brief  MStar gflip Interface header file
/// @attention
/// <b><em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_GFLIP_H
#define _MDRV_GFLIP_H

#ifdef _MDRV_GFLIP_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

#if defined(__cplusplus)
extern "C" {
#endif
#include "drvGOP.h"
#include "halGOP.h"
#include "mdrv_gflip_st.h"


#define MAX_GOP_GWIN             MAX_GOP0_GWIN
#define MAX_FLIP_ADDR_FIFO      (0x10UL)

//=============================================================================
// Defines & Macros
//=============================================================================

//=============================================================================
// Type and Structure Declaration
//=============================================================================
typedef struct DLL_PACKED
{
    MS_PHY u64Addr; //in
    MS_U32 u32TagId; //in
}GFLIP_INFO, *PGFLIP_INFO;

typedef struct __attribute__((packed))
{
  MS_GOP_CTX_SHARED apiCtxShared;
  GOP_CTX_HAL_SHARED halCtxShared;
}GOP_CTX_DRV_SHARED;
typedef struct DLL_PACKED
{
    MS_GOP_CTX_LOCAL  apiCtxLocal;
    GOP_CTX_DRV_SHARED *pDrvCtxShared;//pointer to shared context paramemetrs
    GOP_CTX_HAL_LOCAL    halCtxLocal;
    MS_U8 *pu8REGMAP_Base[MAX_GOP_SUPPORT];
    MS_U16 u16REGMAP_Offset[MAX_GOP_SUPPORT];
    MS_U32 u32REGMAP_Len[MAX_GOP_SUPPORT];
    //GFLIP parameters
    MS_BOOL bEnableVsyncIntFlip[MAX_GOP_SUPPORT];
#ifdef MSOS_TYPE_LINUX
    MS_S32 s32FdGFlip;
#elif defined(MSOS_TYPE_LINUX_KERNEL)
#else
    GFLIP_INFO GFlipInfo[MAX_GOP_SUPPORT][MAX_GOP_GWIN][MAX_FLIP_ADDR_FIFO];
    MS_U32 u32GFlipInfoReadPtr[MAX_GOP_SUPPORT][MAX_GOP_GWIN];
    MS_U32 u32GFlipInfoWritePtr[MAX_GOP_SUPPORT][MAX_GOP_GWIN];
    MS_U16 u16GOPRefCnt[MAX_GOP_SUPPORT];
    MS_BOOL bGFlipInVsyncLimitation[MAX_GOP_SUPPORT];
    GFLIP_DWININT_INFO gflipDWinIntInfo;
#endif
    MS_BOOL gop_gwin_frwr;
    MS_BOOL gop_dwin_frwr;
    MS_BOOL bGOPBankFwr[SHARED_GOP_MAX_COUNT];
    MS_BOOL b16BPPTileMode[SHARED_GOP_MAX_COUNT];
    MS_BOOL b32BPPTileMode[SHARED_GOP_MAX_COUNT];

    MS_U8 current_gop;
}GOP_CTX_DRV_LOCAL;

//=============================================================================
// Function
//=============================================================================

//Init related:
INTERFACE MS_BOOL MDrv_GFLIP_Init(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_BOOL bEnableVsyncIntFlip);
INTERFACE MS_BOOL MDrv_GFLIP_DeInit(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx);

//Drv Interface related(drv interface):
INTERFACE MS_BOOL MDrv_GFLIP_SetMultiFlipInfo(MS_GOP_CTX_LOCAL*pGOPCtx,MS_GFLIP_MULTIINFO* pMultiinfo);
INTERFACE MS_BOOL MDrv_GFLIP_SetTLBMultiFlipInfo(MS_GOP_CTX_LOCAL*pGOPCtx,MS_TLB_GFLIP_MULTIINFO* pTLBMultiinfo);
INTERFACE MS_BOOL MDrv_GFLIP_SetFlipInfo(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_PHY u64MainAddress, MS_PHY u64SubAddress, MS_U32 u32TagId, MS_U32 * u32QEntry);
INTERFACE MS_BOOL MDrv_GFLIP_SetGwinInfo(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U8 u8GopIdx, MS_U8 u8GwinIdx,DRV_GOP_GWIN_INFO* pinfo);
INTERFACE MS_BOOL MDrv_GFLIP_RestoreFromVsyncLimitation(MS_GOP_CTX_LOCAL *pGOPCtx);
INTERFACE MS_BOOL MDrv_GFLIP_GetDWinIntInfo(MS_GOP_CTX_LOCAL *pGOPCtx, GFLIP_DWININT_INFO *pGFlipDWinIntInfo, MS_U32 u32Timeout);
INTERFACE MS_BOOL MDrv_GFLIP_GetVECaptureState(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
INTERFACE MS_BOOL MDrv_GFLIP_VECaptureWaitOnFrame(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
INTERFACE MS_BOOL MDrv_GFLIP_EnaVECapture(MS_GOP_CTX_LOCAL *pGOPCtx, PMS_DRVGOP_VECAPTURESTATE pstVECapState);
INTERFACE MS_BOOL MDrv_GFLIP_ClearFlipQueue(MS_GOP_CTX_LOCAL *pGOPCtx, MS_U32 u32GopIdx, MS_U32 u32GwinIdx);
INTERFACE MS_BOOL MDrv_GFLIP_SetGPIO3DPin(MS_GOP_CTX_LOCAL*pGOPCtx, MS_U32 u32GPIO3DPin);

#if defined(__cplusplus)
}
#endif

#undef INTERFACE

#endif //_MDRV_GFLIP_H

