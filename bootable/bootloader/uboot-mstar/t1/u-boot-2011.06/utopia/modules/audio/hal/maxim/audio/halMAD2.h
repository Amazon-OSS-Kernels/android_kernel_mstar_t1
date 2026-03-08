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

#ifndef _DEV_MAD2_H_
#define _DEV_MAD2_H_
#include "MsTypes.h"
#include "drvAUDIO_if.h"
#include "halMAD.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------



//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//================================================================
//  Structure
//================================================================



//================================================================
//  Enum
//================================================================

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

//=====================================================
// AUDIO_MAD2 System Relational Hal Function
//=====================================================
void    HAL_MAD2_SetMemInfo(void);
void    HAL_MAD2_SetDecCmd(MS_U16 u16DecCmd);
void    HAL_MAD2_SetDecCmd2(MS_U16 u16DecCmd);
MS_U8   HAL_MAD2_GetDecCmd(void);
void    HAL_MAD2_SetFreeRun(MS_U8 u8Freerun);
void    HAL_MAD2_SetPIOCmd(MS_U8 Cmd);
MS_U32  HAL_MAD2_Read_DSP_sram(MS_U16 u16Dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
MS_BOOL HAL_MAD2_Write_DSP_sram(MS_U16 u16Dsp_addr, MS_U32 u32Value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type);
void    HAL_MAD2_DisEn_MIUREQ(void);
void    HAL_MAD2_DisMiuReq(void);
void    HAL_MAD2_RSTMAD_DisEn_MIUREQ(void);

void    HAL_MAD2_Backup_pathreg(void);
void    HAL_MAD2_Restore_pathreg(void);

void    HAL_MAD2_EnableChIRQ(MS_BOOL bEnable);
void    HAL_MAD2_SetDspIDMA(void);
void    HAL_MAD2_TriggerPIO8(void);

MS_U8   HAL_MAD2_GetReloadCodeAck(void);
MS_U8   HAL_MAD2_GetLoadCodeAck(void);
void    HAL_MAD2_SetMcuCmd(MS_U8 cmd);
extern MS_BOOL HAL_AUDSP_CheckSeIdmaReady(MS_U8 u8IdmaChk_type);
MS_BOOL HAL_MAD2_ReLoadCode(AUDIO_DSP_CODE_TYPE u8Type);

void    HAL_MAD2_RebootDsp(void);
extern MS_U8   HAL_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2);
void    HAL_MAD2_XPCM_setParam (XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock);

#endif // _DEV_MAD2_H_
