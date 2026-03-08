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

#ifndef _HAL_PWM_H_
#define _HAL_PWM_H_

////////////////////////////////////////////////////////////////////////////////
/// @file HALPWM.h
/// @brief Pulse Width Modulation hal
////////////////////////////////////////////////////////////////////////////////

#define PWM_Num     5       /* Max. 6 */
#define MAX_3DPWM_NUM    (4)
////////////////////////////////////////////////////////////////////////////////
// Macro for utility
////////////////////////////////////////////////////////////////////////////////
#define MST_MACRO_START     do {
#define MST_MACRO_END       } while (0)

#define UNUSED( var )       ((void)(var))

////////////////////////////////////////////////////////////////////////////////
// Macro for bitwise
////////////////////////////////////////////////////////////////////////////////
#define _BITMASK(loc_msb, loc_lsb) ((1U << (loc_msb)) - (1U << (loc_lsb)) + (1U << (loc_msb)))
#define BITMASK(x) _BITMASK(1?x, 0?x)
#define BITFLAG(loc) (1U << (loc))

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

#define SETBIT(REG, BIT)   ((REG) |= (1UL << (BIT)))
#define CLRBIT(REG, BIT)   ((REG) &= ~(1UL << (BIT)))
#define GETBIT(REG, BIT)   (((REG) >> (BIT)) & 0x01UL)
#define COMPLEMENT(a)      (~(a))

typedef struct _PWM_3D_RegisterInfo
{
    MS_U32 u32RegOffset;
    MS_U8 u8NumBit;
} PWM_3D_RegisterInfo;

typedef struct _PWM_3D_RegisterOffset
{
    PWM_3D_RegisterInfo regShift;
    PWM_3D_RegisterInfo regDuty;
} PWM_3D_RegisterOffset;

////////////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
void HAL_PWM_SetIOMapBase(MS_VIRT virtBase, MS_VIRT virtBase1);
void HAL_PWM_SetChipTopIOMapBase(MS_VIRT virtBase);
MS_VIRT HAL_PWM_GetIOMapBase(void);
MS_BOOL HAL_PWM_Init(void);
MS_BOOL HAL_PWM_Oen(PWM_ChNum index, MS_BOOL letch);
MS_BOOL HAL_PWM_GetOen(PWM_ChNum index);
MS_BOOL HAL_PWM_UnitDiv(MS_U16 u16DivPWM);
void HAL_PWM_Period(PWM_ChNum index, MS_U32 u32PeriodPWM);
void HAL_PWM_DutyCycle(PWM_ChNum index, MS_U32 u32DutyPWM);
void HAL_PWM_Div(PWM_ChNum index, MS_U16 u16DivPWM);
MS_U16 HAL_PWM_GetDiv(PWM_ChNum index);
void HAL_PWM_Polarity(PWM_ChNum index, MS_BOOL bPolPWM);
MS_BOOL HAL_PWM_GetPolarity(PWM_ChNum index);
void HAL_PWM_VDBen(PWM_ChNum index, MS_BOOL bVdbenPWM);
MS_BOOL HAL_PWM_GetVDBen(PWM_ChNum index);
void HAL_PWM_Vrest(PWM_ChNum index, MS_BOOL bRstPWM);
MS_BOOL HAL_PWM_GetVrest(PWM_ChNum index);
void HAL_PWM_DBen(PWM_ChNum index, MS_BOOL bdbenPWM);
MS_BOOL HAL_PWM_GetDBen(PWM_ChNum index);
void HAL_PWM_RstMux(PWM_ChNum index, MS_BOOL bMuxPWM);
void HAL_PWM_RstCnt(PWM_ChNum index, MS_U8 u8RstCntPWM);
void HAL_PWM_BypassUnit(PWM_ChNum index, MS_BOOL bBypassPWM);
MS_BOOL HAL_PWM01_CntMode(PWM_CntMode CntMode);
MS_BOOL HAL_PWM23_CntMode(PWM_CntMode CntMode);
MS_BOOL HAL_PWM67_CntMode(PWM_CntMode CntMode);
MS_BOOL HAL_PWM_Shift(PWM_ChNum index, MS_U32 u32ShiftPWM);
MS_U32 HAL_PWM_GetShift(PWM_ChNum index);
void HAL_PWM_IMPULSE_EN(PWM_ChNum index, MS_BOOL bdbenPWM);
void HAL_PWM_ODDEVEN_SYNC(PWM_ChNum index, MS_BOOL bdbenPWM);
void HAL_PWM_Nvsync(PWM_ChNum index, MS_BOOL bNvsPWM);
void HAL_PWM_Align(PWM_ChNum index, MS_BOOL bAliPWM);
MS_BOOL HAL_PWM_IsSupport3D(PWM_ChNum index);
void HAL_PWM_SetMultiDiff(MS_BOOL bEnable);
MS_BOOL HAL_PWM_Set3D_DiffWaveform(PWM_ChNum index, MS_U8 u8WaveformIndex, MS_U32 u32Shift, MS_U32 u32Duty);
void HAL_PM_PWM_Enable(void);
void HAL_PM_PWM_Period(MS_U16 u16PeriodPWM);
void HAL_PM_PWM_DutyCycle(MS_U16 u16DutyPWM);
void HAL_PM_PWM_Div(MS_U8 u8DivPWM);
void HAL_PM_PWM_Polarity(MS_BOOL bPolPWM);
void HAL_PM_PWM_DBen(MS_BOOL bdbenPWM);
MS_U32 HAL_PWM_GetPeriod(PWM_ChNum index);
MS_U32 HAL_PWM_GetDutyCycle(PWM_ChNum index);
MS_BOOL HAL_PWM_INV_3D_Flag(MS_BOOL bInvPWM);
void HAL_PWM_LR_RST_SEL(PWM_ChNum index, N_LR_SYNC_SEL eLR_Sync);
#endif // _HAL_PWM_H_

