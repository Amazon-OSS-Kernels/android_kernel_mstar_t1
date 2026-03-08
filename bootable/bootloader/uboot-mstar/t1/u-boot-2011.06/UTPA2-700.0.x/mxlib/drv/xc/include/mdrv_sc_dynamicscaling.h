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

#ifndef _MDRV_SC_DYNAMICSCALING_H_
#define _MDRV_SC_DYNAMICSCALING_H_

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

#ifdef _MDRV_SC_DYNAMICSCALING_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

INTERFACE MS_BOOL MDrv_XC_GetDynamicScalingStatus(void *pInstance);
INTERFACE E_APIXC_ReturnValue MDrv_XC_Get_DSForceIndexSupported(void *pInstance, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue MDrv_XC_Set_DSIndexSourceSelect(void *pInstance, E_XC_DS_INDEX_SOURCE eDSIdxSrc, SCALER_WIN eWindow);
INTERFACE E_APIXC_ReturnValue MDrv_XC_Set_DSForceIndex(void *pInstance, MS_BOOL bEnable, MS_U8 u8Index, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_Is_DSForceIndexEnabled(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_EnableIPMTuneAfterDS(void *pInstance, MS_BOOL bEnable);
INTERFACE MS_BOOL MDrv_XC_Set_DynamicScaling(void *pInstance, MS_PHY u32MemBaseAddr, MS_U8 u8MIU_Select, MS_U8 u8IdxDepth, MS_BOOL bOP_On, MS_BOOL bIPS_On, MS_BOOL bIPM_On, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_Is_SupportSWDS(void *pInstance);
#if (SUPPORT_KERNEL_DS == 1)
INTERFACE MS_U8 MDrv_XC_GetSWDSIndex(void *pInstance, SCALER_WIN eWindow);
INTERFACE MS_BOOL MDrv_XC_FireSWDSIndex(void *pInstance, SCALER_WIN eWindow);
#endif
#undef INTERFACE
#endif //_MDRV_SC_DYNAMICSCALING_H_

