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

#ifndef _DEV_WDT_H_
#define _DEV_WDT_H_



#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
//  Local Defines
////////////////////////////////////////////////////////////////////////////////
#define WDT_START_KEY       0x55AA
#define WDT_CLOSE_KEY       0x0

#define MST_XTAL_CLOCK_HZ   (12000000UL)    /* Temp define */
#define MST_XTAL_CLOCK_KHZ  (12000UL)
#define MST_XTAL_CLOCK_MHZ  (12UL)

/*
  *     Use 0xFFFF instead of 0x10000 to allow sec=0 and approximate ceil()
  */
#define HAL_WDTCycles(sec)    ((sec) * MST_XTAL_CLOCK_HZ)
#define HAL_WDTCyclesM(msec)  ((msec) * MST_XTAL_CLOCK_KHZ)
#define HAL_WDTCyclesU(usec)  ((usec) * MST_XTAL_CLOCK_MHZ)

#define calibration             0
#define calibrationMS		0
#define calibrationUS		0

////////////////////////////////////////////////////////////////////////////////
//  Function and Variable
////////////////////////////////////////////////////////////////////////////////

MS_U8 HAL_WDT_ReadByte(MS_U32 u32RegAddr);
MS_U16 HAL_WDT_Read2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_WDT_Read4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_WDT_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_WDT_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_WDT_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_WDT_Start(void);
MS_BOOL HAL_WDT_Stop(void);
MS_BOOL HAL_WDT_Clear(void);
MS_BOOL HAL_WDT_ClearRstFlag(void);
MS_BOOL HAL_WDT_IsReset(void);
MS_BOOL HAL_WDT_IsEnable(void);
MS_BOOL HAL_WDT_SetTimer(MS_U16 sec);
MS_BOOL HAL_WDT_SetTimer_ms(MS_U16 msec);
MS_BOOL HAL_WDT_SetTimer_us(MS_U16 usec);
MS_BOOL HAL_WDT_SetIntTimer(MS_U16 sec);
void HAL_WDT_SetIOMapBase(MS_VIRT u32Base);
MS_VIRT HAL_WDT_GetIOMapBase(void);

#ifdef __cplusplus
}
#endif

#endif // _DEV_WDT_H_
