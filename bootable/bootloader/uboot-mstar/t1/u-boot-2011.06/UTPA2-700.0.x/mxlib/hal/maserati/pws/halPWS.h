/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _HAL_PWS_H_
#define _HAL_PWS_H_

////////////////////////////////////////////////////////////////////////////////
/// @file HALPWS.h
/// @brief power saving hal
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// Header Files
//------------------------------------------------------------------------------
#include "drvPWS.h"

//------------------------------------------------------------------------------
// Define & data type
//------------------------------------------------------------------------------

typedef   struct   {
    MS_U32        u32RegAddr;
    MS_BOOL       bEnable;
    MS_U8         u8BitPos;
    E_PWS_IP_name eIpName;
}E_PWS_IP_MASK_TABLE_INFO;

static const E_PWS_IP_MASK_TABLE_INFO pws_ip_mask_info_table[] =
{

    //========================================================
    //        [ End of the table ]
    //========================================================
    { 0x00000000, FALSE, 0x00, E_PWS_IP_MAX},
};

typedef   struct   {
    MS_U32        u32RegAddr;
    MS_U16        u16RegVal;
    E_PWS_IP_name eIpName;
}E_PWS_IP_TABLE_INFO;

static const E_PWS_IP_TABLE_INFO pws_ip_info_table[] =
{
    //========================================================
    //        [ End of the table ]
    //========================================================
    { 0x00000000, 0x0000, E_PWS_IP_MAX},

};

//------------------------------------------------------------------------------
// Extern function
//------------------------------------------------------------------------------
void   HAL_PWS_SetIOMapBase(MS_U32 u32Base);
MS_U32 HAL_PWS_GetIOMapBase(void);
MS_U16 HAL_PWS_Read2Byte(MS_U32 u32RegAddr);
E_PWS_SouceInfo HAL_PWS_GetSourceInfo(void);
void HAL_PWS_Init(E_PWS_VIF_type eVifType);
E_PWS_Result HAL_PWS_HandleSource(E_PWS_Operation operation,E_PWS_SouceInfo source);
E_PWS_Result HAL_PWS_HandleIP(E_PWS_IpPowerCtrl IpPowerCtrl,const unsigned char *RegName);
E_PWS_Result HAL_PWS_RegisterCallback(MS_U8 index, E_PWS_CallbackCtrl cb_ctrl, unsigned char *RegName, P_PWS_Callback pfCallback);
E_PWS_Result HAL_PWS_CancelCallback(MS_U8 index);

void HAL_PWS_Stop_VDMCU(void);

E_PWS_Result HAL_PWS_IpPowerControl(E_PWS_IP_name eIpName, E_PWS_IpPowerCtrl pwr_type);
#endif  //_HAL_PWS_H_

