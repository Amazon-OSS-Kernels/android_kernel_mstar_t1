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
/// @file   halGFLIP.h
/// @brief  MStar GFLIP Driver DDI HAL Level
/// @attention
/// <b>[MxLib] <em></em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_GFLIP_H
#define _HAL_GFLIP_H

#ifdef _HAL_GFLIP_C
#define INTERFACE
#else
#define INTERFACE extern
#endif

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros
//=============================================================================
#define GFLIP_GOP_IDX_4G             (0x0UL)
#define GFLIP_GOP_IDX_2G             (0x1UL)
#define GFLIP_GOP_IDX_1G             (0x2UL)
#define GFLIP_GOP_IDX_1GX            (0x3UL)
#define GFLIP_GOP_IDX_DWIN           (0x4UL)
#define GFLIP_GOP_IDX_INVALID        (0xFFFFFFFFUL)

#define GOP_DWIN_INT_WADR       GOP_BIT11
#define GOP_DWIN_INT_PROG       GOP_BIT12
#define GOP_DWIN_INT_TF         GOP_BIT13
#define GOP_DWIN_INT_BF         GOP_BIT14
#define GOP_DWIN_INT_VS         GOP_BIT15

#define GOP_DWIN_INTMASK_WADR   GOP_BIT3
#define GOP_DWIN_INTMASK_PROG   GOP_BIT4
#define GOP_DWIN_INTMASK_TF     GOP_BIT5
#define GOP_DWIN_INTMASK_BF     GOP_BIT6
#define GOP_DWIN_INTMASK_VS     GOP_BIT7
#define GOP_DWIN_INT_WADR       GOP_BIT11
#define GOP_DWIN_INT_PROG       GOP_BIT12
#define GOP_DWIN_INT_TF         GOP_BIT13
#define GOP_DWIN_INT_BF         GOP_BIT14
#define GOP_DWIN_INT_VS         GOP_BIT15

//=============================================================================
// HAL Driver Function
//=============================================================================

INTERFACE MS_BOOL MHal_GFLIP_IntEnable(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_BOOL bEnable);
INTERFACE MS_BOOL MHal_GFLIP_IsVSyncInt(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx);
INTERFACE MS_BOOL MHal_GFLIP_SetFlipToGop(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx, MS_U32 u32GwinIdx, MS_U32 u32Addr);

INTERFACE MS_BOOL MHal_GFLIP_ClearDWINIRQ(GOP_CTX_HAL_LOCAL *pGOPHalLocal, GFLIP_DWININT_INFO *pGFlipDWinIntInfo);

INTERFACE MS_BOOL MHal_GFLIP_HandleVsyncLimitation(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx);
INTERFACE MS_BOOL MHal_GFLIP_RestoreFromVsyncLimitation(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U32 u32GopIdx);

INTERFACE MS_BOOL MHal_GFLIP_IsTagIDBack(GOP_CTX_HAL_LOCAL *pGOPHalLocal, MS_U16 u16TagId);

#undef INTERFACE
#endif //_HAL_GFLIP_H

