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

#ifndef __PANEL_INX_VB1_H__
#define __PANEL_INX_VB1_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>

//-------------------------------------------------------------------------------------------------
//  struction define
//-------------------------------------------------------------------------------------------------
typedef enum
{
    CMD_INX_VB1_SET_PRE_INIT,
    CMD_INX_VB1_SET_FHD,
    CMD_INX_VB1_SET_4K2K,
    CMD_INX_VB1_SET_2D,
    CMD_INX_VB1_SET_3D,
    CMD_INX_VB1_RX_INIT
}PANEL_INX_VB1_CMD_TYPE;

typedef struct
{
    MS_U8 u8Cmd0;
    MS_U8 u8Cmd1;
    MS_U8 u8Cmd2;
    MS_U8 u8Cmd3;
    MS_U8 u8Cmd4;
    MS_U8 u8Cmd5;
    MS_U8 u8Cmd6;
}PANEL_CMD_DATA;

//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Panel_INX_VB1_SWI2C_Init(void);
MS_BOOL MDrv_Panel_INX_VB1_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_VB1_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_VB1_SendCmd(PANEL_INX_VB1_CMD_TYPE eCmdType);
MS_BOOL MDrv_Panel_INX_VB1_Write_Cmd(MS_U8 *pu8CmdData);
MS_BOOL MDrv_Panel_INX_VB1_Print_Cmd(MS_U8 *pu8CmdData);

MS_BOOL MDrv_Panel_INX_VB1_Set_Pre_Init(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_FHD(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_4K2K(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_2D(void);
MS_BOOL MDrv_Panel_INX_VB1_Set_3D(void);
MS_BOOL MDrv_Panel_INX_VB1_RX_INIT(void);
#endif //__PANEL_INX_VB1_H__

