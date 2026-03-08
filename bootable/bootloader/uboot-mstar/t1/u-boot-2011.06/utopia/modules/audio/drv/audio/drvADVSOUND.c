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
//  [for Doxygen]
/// file drvADVSOUND.c
/// @brief Subroutine for advance sound effect
/// @author MStarSemi Inc.
////////////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "drvBDMA.h"

#include "drvAUDIO_if.h"
#include "drvAUDIO.h"

#include "./internal/drvADVSOUND.h"
#include "./internal/drvMAD.h"
#include "./internal/drvMAD2.h"
#include "./internal/drvAUDIO_internal.h"

#include "regAUDIO.h"
#include "drvBDMA.h"
#include "halAUDIO.h"
#include "halADVAUDIO.h"


//-------------------------------------------------------------------------------------------------
//  Local Compiler Options
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// Local Functions Prototype
//------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Extern Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_AseSetBinAddress()
/// @brief \b Function \b Description:  This function is used to assign Bin address of advanced sound effect
/// @param u8Index     \b : type index
/// @param BinAddr     \b : address of Bin file
////////////////////////////////////////////////////////////////////////////////
void MDrv_SOUND_AseSetBinAddress(MS_U8 u8Index, MS_U32 BinAddr)
{
    HAL_SOUND_AseSetBinAddress(u8Index, BinAddr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_SetPara()
/// @brief \b Function \b Description:  This function is used to write paramters value into SRAM
/// @param u8Addr1     \b : middle byte of address
/// @param u8Addr2     \b : low byte of address
/// @param u8Value1    \b : high byte of value
/// @param u8Value2    \b : middle byte of value
/// @param u8Value3    \b : low byte of value
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_SetPara(MS_U8 u8Addr1 , MS_U8 u8Addr2, MS_U8 u8Value1, MS_U8 u8Value2, MS_U8 u8Value3)
{
    HAL_SOUND_SetPara(u8Addr1 , u8Addr2, u8Value1, u8Value2, u8Value3);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_TruSurr()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD
/// @param u8TSHDenTruSurr \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_TruSurr(MS_U8 u8TSHDenTruSurr)
{
    HAL_SOUND_TSHD_TruSurr(u8TSHDenTruSurr);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_Limiter()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD Limiter
/// @param u8TSHDenLimiter \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_Limiter(MS_U8 u8TSHDenLimiter)
{
    HAL_SOUND_TSHD_Limiter(u8TSHDenLimiter);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TSHD_SRS3D()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD SRS3D
/// @param u8TSHDenSRS3D \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TSHD_SRS3D(MS_U8 u8TSHDenSRS3D)
{
    HAL_SOUND_TSHD_SRS3D(u8TSHDenSRS3D);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_TruVolume_Enable()
/// @brief \b Function \b Description:  This function is used to enable/disable SRS-HD SRS3D
/// @param u8TSHDenTVol \b :  1: enable, 0: disable
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_TruVolume_Enable(MS_U8 u8TSHDenTVol)
{
    HAL_SOUND_TruVolume_Enable(u8TSHDenTVol);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_SOUND_ADSY_SetPara()
/// @brief \b Function \b Description:  This function is used to set parameter
/// @param u8mode      \b :
/// @param u8value     \b :
////////////////////////////////////////////////////////////////////////////////
void  MDrv_SOUND_ADSY_SetPara(MS_U8 u8mode, MS_U8 u8value)
{
    HAL_SOUND_ADSY_SetPara(u8mode, u8value);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
/// @brief \b Function \b Description: This routine is used to set paramters advanced sound effect.
/// @param <IN>        \b param, u32value1, u16value2
/// @param <OUT>       \b NONE    :
/// @return <OUT>       \b FALSE : Setting Fail
///                               \b TRUE  : Setting OK
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
MS_BOOL MDrv_ADVSOUND_SetParam(ADVSND_PARAM param, MS_U32 u32value1, MS_U16 u16value2)
{
    return(HAL_ADVSOUND_SetParam(param, u32value1, u16value2));
}


////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_AUDIO_COPY_Parameter()
/// @brief \b Function \b Description:  This function is used to copy user parameters to hal
////////////////////////////////////////////////////////////////////////////////
void MDrv_AUDIO_COPY_Parameter(ADVSND_PARAM type, void * Parameter_ptr, MS_U32 size)
{
    HAL_AUDIO_COPY_Parameter(type, Parameter_ptr, size);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief \b Function \b Name: MDrv_DBXTV_SetVolPath(AUDIO_PATH_TYPE path)
/// @brief \b Function \b Description: This routine is used to set dbx TV volume path
/// @param <IN>        \b param, AUDIO_PATH_TYPE
/// @param <OUT>       \b NONE    :
/// @param <RET>       \b  NONE    :
/// @param <GLOBAL>    \b NONE
////////////////////////////////////////////////////////////////////////////////
void MDrv_DBXTV_SetVolPath(AUDIO_OUTPUT_TYPE path){
        Hal_DBXTV_SetVolPath(path);
}
