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

#ifndef __DRV_URSA_6M38_H__
#define __DRV_URSA_6M38_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>
#include <MsDebug.h>
#include <mstarstr.h>

//-------------------------------------------------------------------------------------------------
//  struction define
//-------------------------------------------------------------------------------------------------
typedef enum
{
    UC_INIT_RX_ALL_1,
    UC_INIT_RX_ALL_2,
    UC_SET_IN_VIDEO_SIZE,//        = 0x105,
    UC_SET_LVDS_RX_RECONFIG,//     = 0x106,
    UC_SET_3D_MODE,            //  = 0x301,
    UC_WRITE_REGISTER_1,           //0x31B
    UC_WRITE_REGISTER_2,
    UC_WRITE_REGISTER_3,
    UC_WRITE_REGISTER_MASK_1,
    UC_WRITE_REGISTER_MASK_2,
    UC_DISABLE_AUTO_MUTE

}URSA_6M38_CMD_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Ursa_6M38_SWI2C_Init(void);
MS_BOOL MDrv_Ursa_6M38_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Ursa_6M38_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Ursa_6M38_SendCmd(URSA_6M38_CMD_TYPE eCmdType);
MS_BOOL MDrv_Ursa_6M38_Write_Cmd(MS_U8 *pu8CmdData);


void MDrv_Ursa_6M38_Set_2_lane_VB1(void);
void MDrv_Ursa_6M38_Set_2_lane_VB1_per_init(void);
void MDrv_Ursa_6M38_4K2K_init(void);
void MDrv_Ursa_6M38_4K2K_DisableAutoMute(void);
void MDrv_Ursa_6M38_4K2K60_init(void);
void MDrv_Ursa_6M38_Mute(MS_BOOL bMute);

// for KS2 panel
void MDrv_KS2_Panel_Unlock(void);
void MDrv_KS2_Panel_Bootlogo(MS_U8 is_4k2k_mode);
void MDrv_KS2_Panel_AutoMuteMode(void);
void MDrv_KS2_Panel_DeviceCmdWrite(MS_U8 *reg);

#endif //__DRV_URSA_6M38_H__

