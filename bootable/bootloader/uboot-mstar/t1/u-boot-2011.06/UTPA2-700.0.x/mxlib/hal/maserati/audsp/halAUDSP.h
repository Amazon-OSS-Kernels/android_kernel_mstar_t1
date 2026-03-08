/**
 * Copyright (c) 2006 <96> 2016 MStar Semiconductor, Inc.
 * This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _DEV_AUDSP_H_
#define _DEV_AUDSP_H_
#include "MsCommon.h"
#include "MsTypes.h"
#include "drvAUDIO_if.h"
#include "../../drv/audio/internal/drvAUDIO_internal.h"

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

#define code
#define BYTE MS_U8

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////

typedef struct
{
    MS_U32            cm_addr;
    MS_U32            cm_len;
    MS_U8            *cm_buf;

    MS_U32            pm_addr;
    MS_U32            pm_len;
    MS_U8            *pm_buf;

    MS_U32            cache_addr;
    MS_U32            cache_len;
    MS_U8            *cache_buf;

    MS_U32            prefetch_addr;
    MS_U32            prefetch_len;
    MS_U8            *prefetch_buf;

    char            *AlgName;
    AUDIO_DSP_CODE_TYPE    dspCodeType;
} AUDIO_ALG_INFO, *PAUDIO_ALG_INFO;

typedef struct
{
    AUDIO_ALG_INFO* pau_info;
    MS_U8 DSP_select;
}LOAD_CODE_INFO;
//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//================================================================
//  Structure
//================================================================

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//void    HAL_AUDIO_Set_MMIO_BASE(MS_U32 u32_MMIO_MapBase);

//================================================================
//  Basic Register read/write function
//================================================================
extern MS_U8   HAL_AUDIO_ReadByte(MS_U32 u32RegAddr);
extern MS_U8   HAL_AUDIO_DecReadByte(MS_U32 u32RegAddr);
extern MS_U8   HAL_AUDIO_SeReadByte(MS_U32 u32RegAddr);

extern MS_U16  HAL_AUDIO_ReadReg(MS_U32 u32RegAddr);
extern MS_U16  HAL_AUDIO_DecReadReg(MS_U32 u32RegAddr);
extern MS_U16  HAL_AUDIO_SeReadReg(MS_U32 u32RegAddr);

extern void    HAL_AUDIO_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
extern void    HAL_AUDIO_DecWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
extern void    HAL_AUDIO_SeWriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);

extern void    HAL_AUDIO_WriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
extern void    HAL_AUDIO_DecWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);
extern void    HAL_AUDIO_SeWriteReg(MS_U32 u32RegAddr, MS_U16 u16Val);

extern void    HAL_AUDIO_WriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);
extern void    HAL_AUDIO_DecWriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);
extern void    HAL_AUDIO_SeWriteMaskReg(MS_U32 u32RegAddr, MS_U16 u16Mask, MS_U16 u16Val);

extern void    HAL_AUDIO_WriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);
extern void    HAL_AUDIO_DecWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);
extern void    HAL_AUDIO_SeWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);

extern void    _HAL_AUDIO_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);

extern void    HAL_AUDIO_WriteMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum, MS_U16 u16Data);
extern MS_U16  HAL_AUDIO_ReadMailBox(MS_BOOL bDspType, MS_U8 u8ParamNum);
extern MS_PHY HAL_AUDIO_GetDspMadBaseAddr(MS_U8 u8Index);
extern MS_PHY HAL_AUDIO_GetDspBinBaseAddr(MS_U8 u8Index);
extern MS_BOOL MDrv_FLASH_CheckWriteDone(void);
extern void    HAL_AUDIO_AbsWriteMaskByte(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_U8 u8Val);

MS_BOOL HAL_AUDSP_DspLoadCodeSegment(MS_U32 dsp_addr, MS_U8  *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select);
MS_BOOL HAL_AUDSP_DspVerifySegmentCode(MS_U32 dsp_addr, MS_U8 *dspCode_buf, MS_U32 dspCode_buflen, MS_U8 DSP_select);

MS_BOOL HAL_AUDSP_CheckDecIdmaReady(MS_U8 u8IdmaChk_type);
MS_BOOL HAL_AUDSP_CheckSeIdmaReady(MS_U8 u8IdmaChk_type);
MS_U32 HAL_AUDSP_GetDspBinBaseAddr(MS_U8 u8Index);
void HAL_AUDSP_SetDspCodeTypeLoaded(AUDIO_DSP_CODE_TYPE dspCodeType);
AUDIO_DSP_CODE_TYPE HAL_AUDSP_GetDspCodeTypeLoaded(void);
MS_BOOL HAL_AUDSP_DspLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
void HAL_AUDSP_SetDspLoadCodeInfo(AUDIO_ALG_INFO *pau_info, MS_U8 DSP_select);
LOAD_CODE_INFO* HAL_AUDSP_GetDspLoadCodeInfo(void);
MS_BOOL HAL_AUDSP_DspLoadCode2(MS_U8 DSP_select, void * info);
#endif // _DEV_AUDSP_H_

