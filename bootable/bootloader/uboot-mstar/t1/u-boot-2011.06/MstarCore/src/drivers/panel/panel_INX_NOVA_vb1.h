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

#ifndef __PANEL_INX_NOVA_VB1_H__
#define __PANEL_INX_NOVA_VB1_H__

#include <MsTypes.h>
#include <apiSWI2C.h>
#include <common.h>

//-------------------------------------------------------------------------------------------------
//  struction define
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_U8 Idx0;
    MS_U8 Idx1;
    MS_U8 Idx2;
    MS_U8 Idx3;
    MS_U8 Idx4;
    MS_U8 Idx5;
    MS_U8 Idx6;
    MS_U8 Idx7;
    MS_U8 Idx8;
}PANEL_INX_NOVA_VB1_CMD_DATA;

typedef enum
{
    CMD_INX_NOVA_VB1_UNLOCK_AHB = 0,
    CMD_INX_NOVA_VB1_SET_UHD_DIVISION_MODE,
    CMD_INX_NOVA_VB1_SET_INPUT_MODE,
    CMD_INX_NOVA_VB1_SET_3D_MODE
}PANEL_INX_NOVA_VB1_CMD_TYPE;

typedef enum
{
    INPUT_FHD_120 = 0,  //FHD@100/120(4LAN,0~3)
    INPUT_FHD_60,       //FHD@50/60(2LAN 0~1)
    INPUT_UHD_60,       //QFHD@50/60(8LAN)
    INPUT_UHD_30        //QFHD@24/30(4LAN 0~3)
}INPUT_TIMMING_TYPE;

typedef enum
{
    INPUT_3D_NONE = 0,  //2D Mode
    INPUT_3D_FS,        //Frame Sequential
    INPUT_3D_TB,        //Top & Bottom
    INPUT_3D_SBS        //Side by Side
}INPUT_3D_TYPE;

typedef enum
{
    UHD_0_DIVISION = 0,
    UHD_1_DIVISION,
    UHD_2_DIVISION
}UHD_DIVISION_TYPE;
//-------------------------------------------------------------------------------------------------
//  Function declare
//-------------------------------------------------------------------------------------------------
void MDrv_Panel_INX_NOVA_VB1_SWI2C_Init(void);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_SendCmd(PANEL_INX_NOVA_VB1_CMD_TYPE eCmdType, MS_U8 u8Data);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Write_Cmd(MS_U8 *pu8CmdData, MS_U8 u8CmdLen);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Read_Data(MS_U8 *pu8CmdData, MS_U8 u8DataLen);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Print_Cmd(MS_BOOL bWrite, MS_U8 *pu8CmdData, MS_U8 u8CmdLen);

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Run_Cmd(PANEL_INX_NOVA_VB1_CMD_DATA *pCmdArray, MS_U8 u8ArraySize);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Check_Cmd(void);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Check_Status(void);

MS_BOOL MDrv_Panel_INX_NOVA_VB1_Unlock_AHB(void);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_UHD_DIVISION(UHD_DIVISION_TYPE eDivisionType);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_INPUT_MODE(INPUT_TIMMING_TYPE eTimmingType);
MS_BOOL MDrv_Panel_INX_NOVA_VB1_Set_3D_MODE(INPUT_3D_TYPE e3DType);

#endif //__PANEL_INX_NOVA_VB1_H__

