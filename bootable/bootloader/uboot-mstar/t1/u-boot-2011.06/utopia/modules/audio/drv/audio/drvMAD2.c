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
//******************************************************************************
//
//  [Module Name]:
//      drvMAD2.c
//  [Abstract]:
//      This module contains code for Audio 2nd DSP driver
//      procedure and subroutin
//  [Reversion History]:
//      Initial release:    15 July, 2005
//
//  [Doxygen]
/// file drvMAD2.c
/// @brief Subroutine for Audio 2nd DSP
/// @author MStarSemi Inc.
//*******************************************************************************

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

// Internal
#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "./internal/drvSOUND.h"
#include "./internal/drvMAD.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvAUDIO_internal.h"

#include "halAUDIO.h"
#include "halMAD.h"
#include "halMAD2.h"
#include "halSIF.h"
#include "regAUDIO.h"

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define H2BYTE(value)             ((MS_U8)((value) / 0x10000))

extern MS_U8  g_Dsp2CodeType;
extern MS_U32 MadBaseBufferAdr;

//Temp mask by Cathy extern MS_U8 CheckIPControlBit(MS_U8  u8Bit);


MS_U8  DVB2_IP_CONTROL_BIT[] =
{
    255,                         //MPEG_AD
    11,                          //AC3_AD
    12,                          //AC3P_AD
    255,                         //AAC_AD
    13,                          //DDE
    255,                         //SBC
    255,                         //MP3
    255,                         //MPEG_EN
    255,                         //DTSE
    255,                         //XPCM
    255,                         //KTV
    255,                         //KTV2
    255,                         //NONE
};

//======================================================================
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_SetMemInfo()
/// @brief \b Function \b Description:  This routine is used to set the SE-DSP memory information
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_SetMemInfo(void)
{
    HAL_MAD2_SetMemInfo();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_SetDecCmd()
/// @brief \b Function \b Description: This routine is to set  the DVB2 decoder command.
/// @param u8DecCmd    \b system command    :
///                                         AU_DVB2_DECCMD_STOP,           //0
///                                         AU_DVB2_DECCMD_PLAY,           //1
///                                         AU_DVB2_DECCMD_PLAYFILETSP = 2,
///                                         AU_DVB2_DECCMD_RESYNC,
///                                         AU_DVB2_DECCMD_PLAYFILE = 4,
///                                         AU_DVB2_DECCMD_PAUSE = 6
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_SetDecCmd(AU_DVB2_DECCMD u8DecCmd)
{
    HAL_MAD2_SetDecCmd((AU_DVB_DECCMD)u8DecCmd);
}

///////////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_GetDecCmd()
/// @brief \b Function \b Description: This routine is to get  the DVB2 decode command.
/// @return AU_DVB_DECCMD \b command type:
///                                 AU_DVB_DECCMD_STOP,      //0
///                                 AU_DVB_DECCMD_PLAY,      //1
///                                 AU_DVB_DECCMD_PLAYFILETSP = 2,
///                                 AU_DVB_DECCMD_RESYNC,
///                                          ....etc
///////////////////////////////////////////////////////////////////////////////////////
AU_DVB_DECCMD MDrv_MAD2_GetDecCmd(void)
{
    return (AU_DVB_DECCMD) (HAL_MAD2_GetDecCmd()&0xF);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_SetFreeRun()
/// @brief \b Function \b Description: This routine is to set  the DVB2 decoder in free run or AV-sync mode.
/// @param u8FreeRun   \b AU_DVB2_FreeRunMode_AVsync  0--normal AV sync
///                       AU_DVB2_FreeRunMode_FreeRun 1--free-run mode
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_SetFreeRun(AU_DVB2_FreeRunMode u8FreeRun)
{
    HAL_MAD2_SetFreeRun((MS_U8)u8FreeRun);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_RebootDsp(void)
/// @brief \b Function \b Description:  This routine reboot Sound Effect DSP.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_RebootDsp(void)
{
    HAL_MAD2_RebootDsp();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_DisEn_MIUREQ(void)
{
    HAL_MAD2_DisEn_MIUREQ();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_DisMiuReq(void)
{
    HAL_MAD2_DisMiuReq();
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_DisEn_MIUREQ()
/// @brief \b Function \b Description: This routine is to reset DVB2 MIU request command.
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_RSTMAD_DisEn_MIUREQ(void)
{
    HAL_MAD2_RSTMAD_DisEn_MIUREQ();

}



////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_XPCM2_CheckIntStatus()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value for int status by IDMA
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    MS_U32 sram value for interrupt status
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U16 MDrv_MAD2_XPCM2_CheckIntStatus(void)
{
    return (MS_U16)HAL_MAD2_Read_DSP_sram(0x1494, DSP_MEM_TYPE_DM);

}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_XPCM_setParam()
/// @brief \b Function \b Description: This routine configures necessary parameters when playing XPCM data
/// @param audioType   \b type: 1--lpcm
///                             2--MS-ADPCM
///                             17--IMA-ADPCM
/// @param Channels    \b channel num: 0-- mono
///                                    1-- stereo
/// @param sampleRate  \b sample rate: 8, 11.025, 16, 22.05, 44.1, 48 (KHz)
/// @param bitsPerSample \b bit per sample:  8--8 bits
///                                         16--16 bits
/// @param blockSize \b block size
/// @param samplePerBlock \b samples per block
/// @return MS_U8    \b TRUE--parameters are correct
///                     FALSE--parameters are incorrect
////////////////////////////////////////////////////////////////////////////////
void MDrv_MAD2_XPCM_setParam (XPCM_TYPE audioType, MS_U8  channels, MS_U16 sampleRate,
                                                    MS_U8  bitsPerSample, MS_U16 blockSize, MS_U16 samplePerBlock)
{
   HAL_MAD2_XPCM_setParam (audioType,channels, sampleRate,
                                                     bitsPerSample, blockSize, samplePerBlock);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_Read_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Read DSP internal sram value by IDMA
/// @param <IN>        \b u16Dsp_addr    : DSP internal sram address
/// @param <IN>        \b dm    :    0--DSP_MEM_TYPE_PM
///                                1--DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b value :    MS_U32 sram value
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_U32 MDrv_MAD2_Read_DSP_sram(MS_U16 u16Dsp_addr,AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
    return (HAL_MAD2_Read_DSP_sram(u16Dsp_addr, dsp_memory_type));
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_MAD2_Write_DSP_sram()
/// @brief \b Function \b Description: This routine is used to Write DSP internal sram value by IDMA
/// @param <IN>        \b u16Dsp_addr    : DSP internal sram address
/// @param <IN>        \b u32Value     : data want to write
/// @param <IN>        \b dm        :    0-- write to DSP_MEM_TYPE_PM
///                                        1-- write to DSP_MEM_TYPE_DM
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b NONE  :
/// @param <GLOBAL>    \b NONE    :
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_MAD2_Write_DSP_sram(MS_U16 u16Dsp_addr, MS_U32 u32Value, AUDIO_DSP_MEMORY_TYPE dsp_memory_type)
{
    return(HAL_MAD2_Write_DSP_sram(u16Dsp_addr, u32Value, dsp_memory_type));
}


