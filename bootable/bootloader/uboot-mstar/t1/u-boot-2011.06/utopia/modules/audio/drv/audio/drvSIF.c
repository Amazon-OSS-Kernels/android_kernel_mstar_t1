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
#define _AUSIF_C_

///////////////////////////////////////////////////////////////////////////////
//
/// file drvSIF.c
/// @brief SIF control driver
///////////////////////////////////////////////////////////////////////////////
//
//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include "ULog.h"
#include "MsCommon.h"
#include "MsTypes.h"

// Internal
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "./internal/drvMAD.h"
#include "./internal/drvSIF.h"
#include "./internal/SifInitialTable.h"
#include "./internal/drvAUDIO_internal.h"

#include "regAUDIO.h"
#include "halAUDIO.h"
#include "halSIF.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "drvSYS.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define HINIBBLE(u8Data)    ((u8Data >> 4) & 0x0F)
#define LONIBBLE(u8Data)    ((u8Data) & 0x0F)
MS_U8  g_ucCarrierStableCnt = 0, g_ucPreCarrierStatus=0;

extern AU_CHIP_TYPE gAudioChipType;
extern void    HAL_MAD2_DisEn_MIUREQ(void);

//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------
extern AUDIO_SHARED_VARS2 * g_AudioVars2;


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_ReLoadCode()
/// @brief \b Function \b Description: This routine is used to reload the SIF algorithm code
/// @param dspCodeType     \b type    :  sif algorithm type
/// @return MS_BOOL    \b TRUE --DSP reload code okay
///                       FALSE--DSP reload code fail
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_ReLoadCode(AUDIO_DSP_CODE_TYPE dspCodeType)
{
    return HAL_SIF_ReLoadCode(dspCodeType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: HAL_SIF_Init()
/// @brief \b Function \b Description: This routine is the initialization for SIF
////////////////////////////////////////////////////////////////////////////////
void MDrv_SIF_Init(void)
{
    HAL_SIF_Init();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function  \b Name  : MDrv_SIF_SetSoundMode
/// @brief \b Function  \b Description : This routine is to set the sound mode for SIF module.
/// @param u8ModeType   \b : Sound Mode type
/// @return MS_BOOL     \b : set SIF sound mode
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetSoundMode(AUDIO_SIF_SOUND_MODE u8ModeType)
{
   return (HAL_SIF_SetSoundMode(u8ModeType));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_SetSystem()
/// @brief \b Function \b Description: This routine is to set  sif decoder system.
/// @param u8System_type \b system codec type  :
///                                  AU_SIF_BTSC,             //0
///                                  AU_SIF_PALSUM,           //1
///                                  AU_SIF_EIAJ,             //2
///                                  ......etc
/// @return MS_BOOL \b Set System ok or failure
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetSystem(En_DVB_decSystemType SystemType)
{
    AUDIO_DSP_CODE_TYPE dspCodeType; 
    dspCodeType = HAL_AUDIO_Convert_EnDVBdecSystemType_to_AudioDspCodeType(SystemType);

    return HAL_SIF_SetSystem(dspCodeType);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_StartAutoDetection
/// @brief \b Function \b Description : This routine is to start to detect the sound system for SIF Palsum module.
/// @return MS_BOOL    \b : start auto tuning
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_StartAutoDetection (void)
{
    return HAL_SIF_StartAutoDetect();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_SetThreshold()
/// @brief \b Function \b Description:  This routine is the subfunction of setting SIF threshold
/// @param ThrTbl  \b :          Thresholds Table.
///                             From MApi_AUDIO_SIF_SetThreshold(THR_TBL_TYPE code *ThrTbl)
/// @param start_i  \b :         Thresholds Table starting index
///                - BTSC_TBL_INDEX
///                - A2_M_TBL_INDEX
///                - A2_M_TBL_INDEX
///                - A2_DK_TBL_INDEX
///                - FM_I_TBL_INDEX
///                - AM_TBL_INDEX
///                - NICAM_BG_TBL_INDEX
///                - NICAM_I_TBL_INDEX
///                - HIDEV_M_TBL_INDEX
///                - HIDEV_BG_TBL_INDEX
///                - HIDEV_DK_TBL_INDEX
///                - HIDEV_I_TBL_INDEX
/// @param num    \b :            Thresholds Table Length
///                - BTSC_TBL_LENGTH
///                - A2_M_TBL_LENGTH
///                - A2_M_TBL_LENGTH
///                - A2_DK_TBL_LENGTH
///                - FM_I_TBL_LENGTH
///                - AM_TBL_LENGTH
///                - NICAM_BG_TBL_LENGTH
///                - NICAM_I_TBL_LENGTH
///                - HIDEV_M_TBL_LENGTH
///                - HIDEV_BG_TBL_LENGTH
///                - HIDEV_DK_TBL_LENGTH
///                - HIDEV_I_TBL_LENGTH
/// @param standard_type  \b :         Standard Type
///                          - BTSC_STANDARD
///                          - A2_M_STANDARD
///                          - A2_BG_STANDARD
///                          - A2_DK_STANDARD
///                          - A2_I_STANDARD
///                          - AM_STANDARD
///                          - NICAM_BG_STANDARD
///                          - NICAM_I_STANDARD
///                          - HIDEV_M_STANDARD
///                          - HIDEV_BG_STANDARD
///                          - HIDEV_DK_STANDARD
///                          - HIDEV_I_STANDARD
////////////////////////////////////////////////////////////////////////////////
void MDrv_SIF_SetThreshold(THR_TBL_TYPE *ThrTbl, MS_U8 start_i, MS_U8 num, AUDIO_SIF_SYSTEM_TYPE standard_type)
{
    MS_U8 i;
    MS_U16 value;
    AUDIO_SIF_SYSTEM_TYPE system_type = standard_type;

    system_type |= WRITE_THRESHOLD;  // coverity 8855
    ThrTbl = ThrTbl+start_i;
    for(i=start_i; i<(start_i+num); i++)
    {
        value =( (0x00FF & ThrTbl->HiByteValue) <<8)|(0x00FF & ThrTbl->LowByteValue);
        HAL_SIF_AccessThreshold(system_type,(AUDIO_SIF_THRESHOLD_TYPE)(i-start_i),value);  // coverity 8855
        ThrTbl++;
    }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_AccessThreshold
/// @brief \b Function \b Description : This routine to read/write SIF threshold
/// @param rw_standard_type \b :   standard type
/// @param u8Threshold_type \b :   Threshold  type
/// @param u16Value         \b : write DRAM data value
/// @return  MS_U32         \b : return DRAM data value
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_SIF_AccessThreshold(AUDIO_SIF_SYSTEM_TYPE rw_standard_type, AUDIO_SIF_THRESHOLD_TYPE  u8Threshold_type, MS_U16 u16Value)
{
    return HAL_SIF_AccessThreshold(rw_standard_type, u8Threshold_type, u16Value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_GetOrginalGain
/// @brief \b Function \b Description : This routine to read current SIF gain
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_GetOrginalGain(void)
{
    return HAL_SIF_GetOrginalGain();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_SetPrescale
/// @brief \b Function \b Description : This routine is to set the SIF Prescale.
/// @param u8Type      \b :
///                             SET_PRESCALE_A2_FM
///                            SET_PRESCALE_NICAM
///                             SET_PRESCALE_AM
///                             SET_PRESCALE_HIDEV
///                             SET_PRESCALE_FM_M
///                             SET_PRESCALE_HIDEV_M
///                             SET_PRESCALE_BTSC
///                             SET_PRESCALE_BTSC_MONO
///                             SET_PRESCALE_BTSC_STEREO
///                             SET_PRESCALE_BTSC_SAP
/// @param db_value    \b   :   The prescale value, unit is 0.25dB.
///                             0   : 0db
///                             1   :  0.25dB
///                             -1  : -0.25dB
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetPrescale(AUDIO_SIF_GAIN_TYPE gainType, MS_S32 db_value)
{
    return HAL_SIF_SetPrescale(gainType, db_value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_IsPALType
/// @brief \b Function \b Description : This routine to check if the Palsum is in pal_type state or not
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_IsPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    MS_U8  sub_carrier_std;

    if (!MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_PAL))
    {
        return FALSE;
    }

    sub_carrier_std = HAL_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_CMD, NULL, NULL) & 0xF0;
    switch( pal_type )
        {
        case AU_SIF_PAL_MONO:
            if (sub_carrier_std > SIF_MODE_HIDEV )
                return FALSE;
            else if ((HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_ENABLE, NULL, NULL) == TRUE) != (sub_carrier_std == SIF_MODE_HIDEV))
                return FALSE;
            break;
        case AU_SIF_PAL_HIDEV:
            if (sub_carrier_std != SIF_MODE_HIDEV )
                return FALSE;
            break;
        case AU_SIF_PAL_A2:
            if (sub_carrier_std != SIF_MODE_A2 )
                return FALSE;
            break;
        case AU_SIF_PAL_NICAM:
            if (sub_carrier_std != SIF_MODE_NICAM )
                return FALSE;
            break;
        default:
            return FALSE;// UT
        }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_SetPALType
/// @brief \b Function \b Description : Set Palsum to the pal_type state
/// @return MS_BOOL    \b :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_SetPALType(AUDIO_SIF_PAL_TYPE pal_type)
{
    MS_U8  sif_standard;
    MS_U8  u8NicamSupport = TRUE;

    if (!MDrv_SIF_CheckDecSys(MSAPI_AUD_ATV_PAL))
    {
        return FALSE;
    }

    sif_standard = HAL_SIF_SendCmd(AU_SIF_CMD_GET_STANDARD_CMD, NULL, NULL) & 0x0F;

    switch (pal_type) {
        case AU_SIF_PAL_MONO:
            AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "Utopia Set MONO \r\n");
            if (HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_ENABLE, NULL, NULL) == TRUE) {
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_HIDEV, NULL);
                HAL_AUDIO_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_FILTER_BW_LEVEL, NULL, NULL));
                HAL_SIF_SendCmd(AU_SIF_CMD_RESET_FC_TRACKING, NULL, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, TRUE, NULL);
            }
            else {
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_MONO, NULL);
                HAL_AUDIO_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
            }
            break;
        case AU_SIF_PAL_HIDEV:
            HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_HIDEV, NULL);
            HAL_AUDIO_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, HAL_SIF_SendCmd(AU_SIF_CMD_GET_HIDEV_FILTER_BW_LEVEL, NULL, NULL));
            HAL_SIF_SendCmd(AU_SIF_CMD_RESET_FC_TRACKING, NULL, NULL);
            HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, TRUE, NULL);
            break;
        case AU_SIF_PAL_A2:
            HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
            HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_A2, NULL);
            HAL_AUDIO_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
            break;
        case AU_SIF_PAL_NICAM:
            if (gAudioChipType == AU_CHIP_M12 || gAudioChipType == AU_CHIP_Euler) {
                if (TRUE == MDrv_SYS_Query(E_SYS_QUERY_TTXNICAM_SUPPORTED))
                {
                    u8NicamSupport= TRUE;
                }
                else
                {
                    u8NicamSupport= FALSE;
                }
            }
            if(u8NicamSupport) {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "NICAM support, code = %x\n",u8NicamSupport);
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard|SIF_MODE_NICAM, NULL);
                HAL_AUDIO_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
                break;
            }
            else {
                AUDIO_PRINT(AUDIO_DEBUG_LEVEL_DEBUG, "NICAM not support, code = %x\n",u8NicamSupport);
                HAL_SIF_SendCmd(AU_SIF_CMD_ENABLE_FC_TRACKING, FALSE, NULL);
                HAL_SIF_SendCmd(AU_SIF_CMD_SET_SUB_CARRIER_STD, sif_standard, NULL);
                HAL_AUDIO_WriteMaskByte(AU_CMD_PFIRBANDWIDTH, 0x30, 0x00);
                break;
            }
    }
    return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_SetStandardType
/// @brief \b Function \b Description : This routine to set sif audio system
/// @param sif_type    \b : sif type
////////////////////////////////////////////////////////////////////////////////
void MDrv_SIF_SetStandardType(AUDIO_SIF_SET_STANDARD sif_type)
{
    HAL_SIF_SendCmd(AU_SIF_CMD_SET_STD, sif_type, NULL);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name  : MDrv_SIF_GetDspCodeType
/// @brief \b Function \b Description : This routine to get audio DSP type
/// @return AUDIO_DSP_CODE_TYPE  \b : audio dsp code type
////////////////////////////////////////////////////////////////////////////////
AUDIO_DSP_CODE_TYPE MDrv_SIF_GetDspCodeType(void)
{
    return HAL_SIF_GetDspCodeType();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_GetDspType()
/// @brief \b Function \b Description:  This function is used to get the DSP(DSP_DEC or DSP_SE) which SIF module used.
/// @param <IN>        \b NONE    :
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b MS_U8   : DSP which SIF modules (DSP_DEC or DSP_SE)
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_SIF_GetDspType(void)
{
    return HAL_SIF_GetDspType();
}
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_SendCmd()
/// @brief \b Function \b Description: This routine is to set play/stop command for SIF.
/// @param u8DecCmd    \b command type   :
///                                AU_DVB_DECCMD_STOP,      //0
///                                AU_DVB_DECCMD_PLAY,      //1
////////////////////////////////////////////////////////////////////////////////
MS_U8 MDrv_SIF_SendCmd(AU_SIF_CMD u8DecCmd, MS_U8 comm_arg1, MS_U8 comm_arg2)
{
    MS_U8 u8Return = 0;
    u8Return = HAL_SIF_SendCmd(u8DecCmd, comm_arg1, comm_arg2);

    return u8Return;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SIF_CheckDecSys()
/// @brief \b Function \b Description: This function will check whether current sif decoder equals giving DecSystem type
/// @param <IN>        \b   enDecSystem : DecSystem type
/// @param <RET>       \b   BOOL        : True or False
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_SIF_CheckDecSys(En_DVB_decSystemType enDecSystem)
{
    return(HAL_SIF_CheckDecSys(enDecSystem));
}

