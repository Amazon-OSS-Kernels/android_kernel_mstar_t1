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


#ifndef __DRV_URSA_10_C__
#define __DRV_URSA_10_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_10.h"
#include <MsTypes.h>
#include <command.h>
#include <apiSWI2C.h>
#include <MsDebug.h>
//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_CMD_DATA_SUM              (7)
#define URSA_I2C_BUS_NUM               (1)
#define URSA_DDC2BI_MODE_ADDR            (0x40)
#define URSA_I2C_SUB_ADDR              (0)


static SWI2C_BusCfg Ursa_I2cBusCfg[URSA_I2C_BUS_NUM]=
{
    // Bus-0
    {PAD_DDCR_CK, PAD_DDCR_DA, 400}, //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-1
    //{PAD_TGPIO2, PAD_TGPIO3, 400},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Others, add other bus here
};

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------


static MS_BOOL MDrv_Ursa_SWI2C_WriteBytes(MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(URSA_DDC2BI_MODE_ADDR, URSA_I2C_SUB_ADDR, NULL, URSA_CMD_DATA_SUM, pu8data);
}

static MS_BOOL MDrv_Ursa_SWI2C_ReadBytes( MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(URSA_DDC2BI_MODE_ADDR, URSA_I2C_SUB_ADDR, NULL, URSA_CMD_DATA_SUM, pu8data);
}

//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------


void MDrv_Ursa_10_Lvds_init(void)
{
    MS_U8 u10_cmd_init[6][7] =
    {
        {0x80,0x10,0x43,0x00,0xD8,0x28,0x15},
        {0x40,0x11,0x08,0x98,0x04,0x6F,0xA0},
        {0x40,0x12,0x07,0x80,0x04,0x38,0x2F},
        {0x40,0x13,0x50,0x05,0x3C,0x00,0x00},
        {0x01,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x5D,0x01,0x01,0x01,0x00,0x02,0x00}
    };
    MS_U8 cmd_index;


    UBOOT_TRACE("IN\n");
    MApi_SWI2C_Init(Ursa_I2cBusCfg, URSA_I2C_BUS_NUM);
    for(cmd_index = 0; cmd_index < 6; cmd_index++)
    {
        UBOOT_DEBUG("[U10] send LVDS init cmd_%d--->\n", cmd_index);
        if(!MDrv_Ursa_SWI2C_WriteBytes(u10_cmd_init[cmd_index]))
        {
            UBOOT_ERROR("[U10]Send cmd: FAILED !!!\n");
        }
    }

    UBOOT_TRACE("OK\n");
}


#endif //__DRV_URSA_10_C__
