/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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

#include <common.h>
#include <command.h>
#include <config.h>
#include <MsTypes.h>
#include <MsOS.h>
#include <MsDebug.h>
#include <MsSystem.h>

#include <apiAUDIO.h>
#include <drvAUDIO_if.h>
#include <drvAUDIO.h>

#define AUDIO_DELAY1MS(x)    mdelay(x)

//=============================================
// Set audio related input & output port connectivity
//=============================================
void MsHalAudio_SetInternalPath(void)
{
        MDrv_AUDIO_SetInputPath(AUDIO_DSP1_DVB_INPUT, AUDIO_PATH_MAIN);      //Set input source to decoder 1

        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_I2S_OUTPUT);       // TV:2R1D use internal path D
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT0_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT1_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT2_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_AUOUT3_OUTPUT);
        MDrv_AUDIO_SetInternalPath(INTERNAL_CH8, AUDIO_SPDIF_OUTPUT);
}

//=============================================
// Set volume for all output port
//=============================================
void MsHalAudio_SetOutputVolume(MS_U16 u16Vol)
{
    MS_U8 u8VolHi = 0 , u8VolLo = 0 ;

    u8VolHi = (MS_U8)(u16Vol>>8);
    u8VolLo = (MS_U8)(u16Vol&0xFF);

    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_I2S, u8VolHi, u8VolLo );
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT0, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT1, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT2, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_AUOUT3, u8VolHi, u8VolLo );   //Set volume
    MApi_AUDIO_SetAbsoluteVolume( AUDIO_T3_PATH_SPDIF, u8VolHi, u8VolLo );   //Set volume//AUDIO_T3_PATH_I2S
}

//=============================================
// Play Melody in this function
//=============================================
void MsHalAudio_MelodyPlayStart(void)
{
  //======================================================
  // New SRC arch chip , play Music with MM new mode
  //======================================================
    MApi_AUDIO_SetSystem(MSAPI_AUD_DVB_MP3);   // Set Decoder type = MP3
    MApi_AUDIO_MM2_initAesInfo(AU_DEC_ID1);
    AUDIO_DELAY1MS(5);
    MApi_AUDIO_SetCommand(MSAPI_AUD_DVB_DECCMD_PLAYMM_FILE2);
    AUDIO_DELAY1MS(5);
    MApi_AUDIO_MM2_inputAesFinished(AU_DEC_ID1, 0x10000-0x20, 0, 0);
}

//=============================================
// Mute/unmute all audio output port
//=============================================
void MsHalAudio_SetOutputMute(MS_BOOL bMute)
{
    if(bMute == 1)
    {
        MApi_AUDIO_SetMute(AUDIO_PATH_MAIN, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT0, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT1, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT2, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT3, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_I2S, 1);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, 1);
    }
    else
    {
        MApi_AUDIO_SetMute(AUDIO_PATH_MAIN, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT0, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT1, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT2, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_AUOUT3, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_I2S, 0);
        MApi_AUDIO_SetMute(AUDIO_T3_PATH_SPDIF, 0);
    }
}

//=============================================
// Get ES buffer abs start address
//=============================================
MS_U32 MsHalAudio_GetEsStartAddr(void)
{
    MS_U32 u32EsAddress = 0;

    u32EsAddress = MApi_AUDIO_GetCommAudioInfo(Audio_Comm_infoType_Get_UNI_ES_Base);

    return u32EsAddress;
}


