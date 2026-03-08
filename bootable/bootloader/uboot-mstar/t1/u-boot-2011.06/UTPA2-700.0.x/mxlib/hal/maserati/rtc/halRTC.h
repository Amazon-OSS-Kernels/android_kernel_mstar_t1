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

#ifndef _DEV_RTC_H_
#define _DEV_RTC_H_



#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
//  Local Defines
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
//  Function and Variable
////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    E_RTC_0,
    E_RTC_2,
}E_MS_RTC;

void HAL_RTC_RESET(E_MS_RTC eRtc, MS_BOOL bEnable);
void HAL_RTC_Counter(E_MS_RTC eRtc, MS_BOOL bEnable);
void HAL_RTC_Wrap_Count (E_MS_RTC eRtc, MS_BOOL bEnable);
void HAL_RTC_Loading (E_MS_RTC eRtc, MS_BOOL bEnable);
void HAL_RTC_Reading(E_MS_RTC eRtc, MS_BOOL bEnable);
void HAL_RTC_IntMask(E_MS_RTC eRtc, MS_BOOL bEnable);
void HAL_RTC_IntForce(E_MS_RTC eRtc);
void HAL_RTC_IntClear(E_MS_RTC eRtc);
void HAL_RTC_Set_Frequency(E_MS_RTC eRtc,MS_U32 u32Freq, MS_U32 u32Xtal);
void HAL_RTC_Set_Counter(E_MS_RTC eRtc,MS_U32 u32Val);
void HAL_RTC_Match_Counter(E_MS_RTC eRtc,MS_U32 u32Val);
MS_U16 HAL_RTC_INT_Status(E_MS_RTC eRtc);
MS_U32 HAL_RTC_Read_Counter(E_MS_RTC eRtc);
void HAL_RTC_SetIOMapBase(MS_VIRT virtBase);
MS_VIRT HAL_RTC_GetIOMapBase(void);

#ifdef __cplusplus
}
#endif

#endif // _DEV_WDT_H_

