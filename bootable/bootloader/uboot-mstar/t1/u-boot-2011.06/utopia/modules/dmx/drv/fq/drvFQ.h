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
////////////////////////////////////////////////////////////////////////////////////////////////////
/// file   drvFQ.h
/// @brief  FQ Driver Interface
/// @attention
////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _DRVFQ_H_
#define _DRVFQ_H_

#ifdef __cplusplus
extern "C"
{
#endif

//--------------------------------------------------------------------------------------------------
//  Define
//--------------------------------------------------------------------------------------------------                                               // Transport stream null PID
//--------------------------------------------------------------------------------------------------
//  Driver Capability
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//  Type and Structure
//--------------------------------------------------------------------------------------------------

/// FQ DDI return value
/// @name TSP_Result
/// @ref TSP_Result
/// return value
/// @{
typedef enum
{
    DRVFQ_FAIL                   = 0,
    DRVFQ_OK,
    DRVFQ_INVALID_PARAM,
    DRVFQ_FUNC_ERROR,
} FQ_Result;

//--------------------------------------------------------------------------------------------------
//  Function Prototype
//--------------------------------------------------------------------------------------------------

// Initialization API
FQ_Result MDrv_FQ_Init(MS_U32 u32FQEng, MS_U8 u8AddrMode);
FQ_Result MDrv_FQ_Exit(MS_U32 u32FQEng);

// FQ API
FQ_Result MDrv_FQ_IsInit(MS_U32 u32FQEng, MS_BOOL* pbIsInit);
FQ_Result MDrv_FQ_SetBuffer(MS_U32 u32FQEng, MS_PHY phyBufStart, MS_U32 u32BufSize);
FQ_Result MDrv_FQ_Start(MS_U32 u32FQEng, MS_BOOL bStart);
FQ_Result MDrv_FQ_SkipRushData(MS_U32 u32FQEng, MS_U32 u32SkipPath);
FQ_Result MDrv_FQ_RushEnable(MS_U32 u32FQEng);
FQ_Result MDrv_FQ_TimeStampSetRecordStamp(MS_U32 u32FQEng, MS_U32 u32Stamp);
FQ_Result MDrv_FQ_TimeStampGetRecordStamp(MS_U32 u32FQEng, MS_U32* u32Stamp);
FQ_Result MDrv_FQ_SetRushAddr(MS_U32 u32FQEng, MS_PHY phyRushAddr);
FQ_Result MDrv_FQ_GetReadAddr(MS_U32 u32FQEng, MS_PHY* pphyReadAddr);
FQ_Result MDrv_FQ_GetWriteAddr(MS_U32 u32FQEng, MS_PHY* pphyWriteAddr);


#ifdef MSOS_TYPE_LINUX_KERNEL
FQ_Result MDrv_FQ_Suspend(void);
FQ_Result MDrv_FQ_Resume(void);
#endif  //MSOS_TYPE_LINUX_KERNEL


#ifdef __cplusplus
} // closing brace for extern "C"
#endif
#endif // _DRVFQ_H_
