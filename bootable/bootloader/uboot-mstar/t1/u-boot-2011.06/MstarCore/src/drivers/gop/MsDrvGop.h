/**
* Copyright (c) 2006 ¡V 2016 MStar Semiconductor, Inc.
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

#ifndef _MS_DRV_GOP_
#define _MS_DRV_GOP_
#include <apiGFX.h>
#include <apiGOP.h>
#include <MsTypes.h>

typedef struct
{
    MS_U8   m_u8SettingType;//0:mux, 1:layer
    MS_U16  m_u16SettingCounts;
    MS_U16  m_u16Setting0GopIndex;
    MS_U16  m_u16Setting1GopIndex;
    MS_U16  m_u16Setting2GopIndex;
    MS_U16  m_u16Setting3GopIndex;
    MS_U16  m_u16Setting4GopIndex;
}st_gopmux_para;

typedef struct
{
    MS_U32  m_u32PWMPeriod;
    MS_U16  m_u16DivPWM;
    MS_U32  m_u32PWMDuty;
    MS_U8   m_bPolPWM;
    MS_U32  m_u32maxPWM;
    MS_U32  m_u32minPWM;
    MS_U16  m_u16PWMPort;
}st_pwm_setting;

void MsDrvGFX_Init(void);
void MsDrvGOP_Init(MS_U8 u8logoGopIdx);
void MsDrvGOP_Show(MS_U8 u8logoGopIdx, GFX_BufferInfo *dst_info,MS_BOOL bHorStretch, MS_BOOL bVerStretch,MS_U16 u16DispX,MS_U16 u16DispY);
void MsDrvGOP_Show1(MS_U8 u8logoGopIdx,GFX_BufferInfo *dst_info , MS_U8 u8HorStretch, MS_U8 u8VerStretch,MS_U16 u16DispX,MS_U16 u16DispY,MS_U32 u32HorStretch_After_scaling , MS_U32 u32VorStretch_After_scaling );
int MsDrvGFX_Blt(GFX_DrawRect *rect,GFX_BufferInfo *src_info, GFX_BufferInfo *dst_info);
void MsDrvGOP_Hide(void);
#endif