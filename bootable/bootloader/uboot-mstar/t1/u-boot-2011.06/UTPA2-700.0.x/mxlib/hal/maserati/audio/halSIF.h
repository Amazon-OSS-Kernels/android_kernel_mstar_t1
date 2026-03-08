/**
 * Copyright (c) 2006 <96> 2016 MStar Semiconductor, Inc.
 * This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef _DEV_SIF_H_
#define _DEV_SIF_H_

#include "MsTypes.h"

// for standard status
// [3:2]: 00:NTSC 01:A2 10:NICAM 11:OTHER
// BIT7: AUTO MODE
enum
{
    IS_MAIN_CARRIER= 0x01,
    IS_A2= 0x02,
    IS_NICAM= 0x04
};

enum
{
    AU_CARRIER1_IGNORE = 0,
    AU_CARRIER1_KOREA  = 1,
    AU_CARRIER1_BTSC   = 2,
    AU_CARRIER1_JAPAN  = 3
};
// 6.5 MHz
enum
{
    AU_CARRIER2_L  = 0x10,
    AU_CARRIER2_DK = 0x20
};

enum
{
    AU_INFO_CARRIER_1_AMP,
    AU_INFO_CARRIER_1_VAR,
    AU_INFO_CARRIER_2_AMP,
    AU_INFO_CARRIER_2_VAR,
    AU_INFO_CARRIER_STATE,
    AU_INFO_ERROR_COUNT,
    AU_INFO_BTSC_MAIN_AMP,
    AU_INFO_BTSC_MAIN_VAR,
    AU_INFO_EIAJ_MAIN_AMP,
    AU_INFO_EIAJ_MAIN_VAR,
    AU_INFO_NICAM_BIT_ERROR,
    AU_INFO_AM_MAIN_AMP,
};

typedef enum
{
   AUD_SIF_42M,
   AUD_SIF_43M,
   AUD_SIF_44M,
   AUD_SIF_48M
} AU_SIF_CLOCK_Type;

//sif threshold address
#define BTSC_THRESHOLD_ADDR_SE         BTSC_THRESHOLD_Addr_2
#define M_A2_THRESHOLD_ADDR_SE         ADDR_a2_stdM_thr_2
#define BG_A2_THRESHOLD_ADDR_SE        ADDR_a2_stdBG_thr_2
#define DK_A2_THRESHOLD_ADDR_SE        ADDR_a2_stdDK_thr_2
#define I_FM_THRESHOLD_ADDR_SE         ADDR_a2_stdI_thr_2
#define L_AM_THRESHOLD_ADDR_SE         ADDR_am_thr_2
#define NICAM_BGDKL_THRESHOLD_ADDR_SE  ADDR_nicam_stdBG_pherr_thr_2
#define NICAM_I_THRESHOLD_ADDR_SE      ADDR_nicam_stdI_pherr_thr_2

#define HIDEV_M_THRESHOLD_ADDR_SE      ADDR_hidev_stdM_thr_2
#define HIDEV_BG_THRESHOLD_ADDR_SE     ADDR_hidev_stdBG_thr_2
#define HIDEV_DK_THRESHOLD_ADDR_SE     ADDR_hidev_stdDK_thr_2
#define HIDEV_I_THRESHOLD_ADDR_SE      ADDR_hidev_stdI_thr_2

//sif gain address
#define A2_OUTPUT_GAIN_X_ADDR_SE   ADDR_fm_stdX_gain_2
#define NICAM_OUTPUT_GAIN_ADDR_SE  ADDR_nicam_gain_2
#define AM_OUTPUT_GAIN_ADDR_SE     ADDR_am_gain_2
#define A2_OUTPUT_GAIN_M_ADDR_SE   ADDR_fm_stdM_gain_2
#define BTSC_OUTPUT_GAIN_ADDR_SE   BTSC_OUTPUT_GAIN_Addr_2
#define BTSC_MTS_OUTPUT_GAIN_SE    MTS_OUTPUT_GAIN_Addr_2

//sif sub-carrier set
#define SIF_MODE_NICAM          0x40
#define SIF_MODE_A2             0x20
#define SIF_MODE_HIDEV          0x10
#define SIF_MODE_MONO           0x00

//sif hidev bandwidth type
#define HIDEV_PFIR_BW_NARROW    0x10
#define HIDEV_PFIR_BW_MIDDLE    0x20
#define HIDEV_PFIR_BW_WIDE      0x30

//Mailbox definition

#define M2D_MBOX_SIF_CMD_STANDARD             MB_2DC0
#define M2D_MBOX_SIF_CMD_PFIRBANDWIDTH        MB_2DC2
#define M2D_MBOX_SIF_CMD_MODE1                MB_2DC4
#define M2D_MBOX_SIF_CMD_MODE2                MB_2DC6
#define M2D_MBOX_DBG_CMD2               MB_2D9E
#define D2M_MBOX_SIF_DETECTION_RESULT         MB_2DA0
#define D2M_MBOX_SIF_STATUS_INFO           MB_2DA2
#define D2M_MBOX_SIF_STATUS_MODE1             MB_2DA4
#define D2M_MBOX_SIF_STATUS_MODE2             MB_2DA6
#define D2M_MBOX_SIF_STATUS_NICAM_INFO     MB_2DA8
#define D2M_MBOX_SIF_STATUS_NICAM_PARITY_ERR_CNT    MB_2DAA

#define AU_CMD_TONE             M2S_MBOX_SINE_GEN//0x2D28
#define AU_CMD_STANDARD         M2D_MBOX_SIF_CMD_STANDARD
#define AU_CMD_PFIRBANDWIDTH    M2D_MBOX_SIF_CMD_PFIRBANDWIDTH     //HiDEV[5:4], A2 carrier2 pfir[1:0]
#define AU_CMD_AGC              M2D_MBOX_SIF_CMD_PFIRBANDWIDTH+1  //[0] enable/disable AGC, [1]reset AGC
#define AU_CMD_MODE1            M2D_MBOX_SIF_CMD_MODE1
#define AU_CMD_MODE2            M2D_MBOX_SIF_CMD_MODE2
#define AU_CMD_DEBUG            M2D_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_CMD          M2D_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_DATA_H       M2D_MBOX_DBG_CMD1
#define AU_CMD_DBG_DATA_M       M2D_MBOX_DBG_CMD2+1
#define AU_CMD_DBG_DATA_L       M2D_MBOX_DBG_CMD2
#define AU_CMD_TONE             M2S_MBOX_SINE_GEN

#define AU_STATUS_STANDARD      D2M_MBOX_SIF_DETECTION_RESULT
#define AU_STATUS_MODE1         D2M_MBOX_SIF_STATUS_MODE1
#define AU_STATUS_MODE2         D2M_MBOX_SIF_STATUS_MODE2
#define AU_STATUS_DEBUG1        D2M_MBOX_DBG_RESULT1
#define AU_STATUS_DEBUG2        D2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_H         D2M_MBOX_DBG_RESULT1
#define AU_STATUS_DBG_M         D2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_L         D2M_MBOX_DBG_RESULT2

#define AU_CMD_STANDARD_SE           M2S_MBOX_SIF_CMD_STANDARD
#define AU_CMD_PFIRBANDWIDTH_SE      M2S_MBOX_SIF_CMD_PFIRBANDWIDTH     //HiDEV[5:4], A2 carrier2 pfir[1:0]
#define AU_CMD_AGC_SE                M2S_MBOX_SIF_CMD_PFIRBANDWIDTH+1  //[0] enable/disable AGC, [1]reset AGC
#define AU_CMD_MODE1_SE              M2S_MBOX_SIF_CMD_MODE1
#define AU_CMD_MODE2_SE              M2S_MBOX_SIF_CMD_MODE2
#define AU_CMD_DEBUG_SE              M2S_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_CMD_SE            M2S_MBOX_DBG_CMD1+1
#define AU_CMD_DBG_DATA_H_SE         M2S_MBOX_DBG_CMD1
#define AU_CMD_DBG_DATA_M_SE         M2S_MBOX_DBG_CMD2+1
#define AU_CMD_DBG_DATA_L_SE         M2S_MBOX_DBG_CMD2
#define AU_CMD_TONE_SE               M2S_MBOX_SINE_GEN

#define AU_STATUS_STANDARD_SE        S2M_MBOX_SIF_DETECTION_RESULT
#define AU_STATUS_MODE1_SE           S2M_MBOX_SIF_STATUS_MODE1
#define AU_STATUS_MODE2_SE           S2M_MBOX_SIF_STATUS_MODE2
#define AU_STATUS_DEBUG1_SE          S2M_MBOX_DBG_RESULT1
#define AU_STATUS_DEBUG2_SE          S2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_H_SE           S2M_MBOX_DBG_RESULT1
#define AU_STATUS_DBG_M_SE           S2M_MBOX_DBG_RESULT2+1
#define AU_STATUS_DBG_L_SE           S2M_MBOX_DBG_RESULT2

#define SIF_ADC_FROM_VIF_PATH    0x08

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    HAL_SIF_Init(void);
void    HAL_SIF_TriggerSifPLL(void);

MS_U8   HAL_SIF_GetDspType(void);
void    HAL_SIF_SetDspCodeType(AUDIO_DSP_CODE_TYPE dspCodeType);
AUDIO_DSP_CODE_TYPE HAL_SIF_GetDspCodeType(void);

MS_BOOL HAL_SIF_SetSoundMode(AUDIO_SIF_SOUND_MODE u8Mode_type);
MS_BOOL HAL_SIF_StartAutoDetect(void);

MS_U32  HAL_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 u16Value);
MS_BOOL HAL_SIF_GetOrginalGain(void);
MS_BOOL HAL_SIF_SetPrescale(AUDIO_SIF_GAIN_TYPE gainType, MS_S32 db_value);

MS_U8   HAL_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2);
MS_BOOL HAL_SIF_ReLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType);
MS_BOOL HAL_SIF_SetSystem(AUDIO_DSP_CODE_TYPE dspCodeType);

void HAL_SIF_BackupMailbox(void);
void HAL_SIF_RestoreMailbox(void);
MS_BOOL HAL_SIF_CheckDecSys(En_DVB_decSystemType enDecSystem);
#endif // _DEV_SIF_H_
