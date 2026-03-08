/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
/**
 *
 *
 */


#ifndef _DRVGOP_PRIV_H_
#define _DRVGOP_PRIV_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvCH34.h"
#include "drvCH34_v2.h"

typedef struct _CH34_RESOURCE_PRIVATE
{
    MS_U8 dummy;
}CH34_RESOURCE_PRIVATE;

typedef CH34_Drv_Result              (*IOCTL_CH34_INIT)                 (void);
typedef CH34_Drv_Result              (*IOCTL_CH34_SET_MODE)             (E_CH34_CHANNEL, E_CH34_VIDEO_SYS, E_CH34_AUDIO_SYS);
typedef CH34_Drv_Result              (*IOCTL_CH34_WRITE_TABLE)          (MS_U8*);
typedef CH34_Drv_Result              (*IOCTL_CH34_TURN_ONOFF)           (MS_BOOL);

typedef struct _CH34_INSTANT_PRIVATE
{
    IOCTL_CH34_INIT                  fpCH34Init;
    IOCTL_CH34_SET_MODE        	     fpCH34SetMode;
    IOCTL_CH34_WRITE_TABLE  		 fpCH34WriteTable;
    IOCTL_CH34_TURN_ONOFF  		     fpCH34TurnOnOff;
}CH34_INSTANT_PRIVATE;

CH34_Drv_Result MDrv_Ch34_Init_U2(void);
CH34_Drv_Result MDrv_Ch34_SetMode_U2(E_CH34_CHANNEL channel, E_CH34_VIDEO_SYS videosys, E_CH34_AUDIO_SYS audiosys);
CH34_Drv_Result MDrv_Ch34_WriteCH34Table_U2(MS_U8* pTable);
CH34_Drv_Result MDrv_Ch34_TurnOnOff_U2(MS_BOOL bOn);

void CH34RegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 CH34Open(void** ppInstance, const void* const pAttribute);
MS_U32 CH34Close(void* pInstance);
MS_U32 CH34Ioctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
