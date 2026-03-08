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

#ifndef __DRV_URSA_9_C__
#define __DRV_URSA_9_C__

//-------------------------------------------------------------------------------------------------
//  Include files
//-------------------------------------------------------------------------------------------------
#include "ursa_9.h"
#include <panel/panel.h>
#include <MsSystem.h>

//-------------------------------------------------------------------------------------------------
//  Define
//-------------------------------------------------------------------------------------------------
#define URSA_9_CMD_DATA_SUM             (7)

#define URSA_9_I2C_BUS_NUM              (1)
#if defined(ENABLE_MSTAR_NAPOLI_BD_MST189B_D01A_S) && (ENABLE_MSTAR_NAPOLI_BD_MST189B_D01A_S == 1)
#define URSA_9_DDC2BI_MODE_ADDR         (0x240)
#else
#define URSA_9_DDC2BI_MODE_ADDR         (0x040)
#endif

#define URSA_URSA9_PANEL_INIT_DELAY           (1200)  //1200ms

typedef enum
{
    UC_INIT_RX_OSD_2LANE,
    UC_INIT_RX_OSD_4LANE,
    UC_INIT_RX_VIDEO_2LANE,
    UC_INIT_RX_VIDEO_4LANE,
    UC_SET_IN_OSD_FULLHD,
    UC_SET_IN_OSD_4K2K,
    UC_SET_IN_VIDEO_FULLHD,
    UC_SET_IN_VIDEO_4K2K,
    UC_SET_LVDS_RX_RECONFIG,//     = 0x106,
    UC_SET_3D_MODE,            //  = 0x301,
    UC_WRITE_REGISTER_1,           //0x31B
    UC_WRITE_REGISTER_2,
    UC_WRITE_REGISTER_3,
    UC_WRITE_REGISTER_MASK_1,
    UC_WRITE_REGISTER_MASK_2,
    UC_URSA_MOD_POWER_ON,
    UC_INIT_RX_VIDEO_8LANE,
    UC_URSA_GAIN_PATCH,
    UC_URSA_MONACO_PATCH,
    UC_URSA_DISABLE_OSD
}URSA_9_CMD_TYPE;

static SWI2C_BusCfg Ursa_9_I2cBusCfg[URSA_9_I2C_BUS_NUM]=
{
    // Bus-0
    #if (ENABLE_MSTAR_BD_MST205A_D01A_S_MONACO == 1)
    {PAD_GPIO28, PAD_GPIO29, 480},
    #else
    {PAD_DDCR_CK, PAD_DDCR_DA, 400},//IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    #endif
    // Bus-1
   // {PAD_TGPIO2, PAD_TGPIO3, 400},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-2
   // {PAD_GPIO36, PAD_GPIO37, 480},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Bus-3
   // {PAD_GPIO28, PAD_GPIO29, 480},   //IS_SW_I2C  /SCL_PAD /SDA_PAD /Delay
    // Others, add other bus here
};

//-------------------------------------------------------------------------------------------------
//  Function define
//-------------------------------------------------------------------------------------------------
void MDrv_Ursa_9_SWI2C_Init(void)
{
    static MS_U8 is_first_init = FALSE;
    unsigned long lpanelVCCOnTimer = 0;
    unsigned long ltotalTime = 0;
    unsigned long lursaDelayTime = 0;

    UBOOT_TRACE("IN\n");
    if(is_first_init == FALSE)
    {
        UBOOT_DEBUG("[URSA9] Initialize SW I2C\n");
        // according to spec, before sending i2c command to U9, we must delay about 1200ms
        lpanelVCCOnTimer = MApi_PNL_GetPanelVccOnTime();
        ltotalTime = MsSystemGetBootTime();
        if(ltotalTime > lpanelVCCOnTimer)
        {
            lursaDelayTime = ltotalTime - lpanelVCCOnTimer;
            if(lursaDelayTime < URSA_URSA9_PANEL_INIT_DELAY)
            {
                lursaDelayTime = URSA_URSA9_PANEL_INIT_DELAY - lursaDelayTime;
                printf("[%s][%d]ursa9 delay time: %ld\n", __FUNCTION__, __LINE__, lursaDelayTime);
                mdelay(lursaDelayTime);
            }
        }
        MApi_SWI2C_Init(Ursa_9_I2cBusCfg, URSA_9_I2C_BUS_NUM);
        is_first_init = TRUE;
    }
    else
    {
        UBOOT_ERROR("[URSA9] SW I2C is already initialized\n");
        return;
    }
    UBOOT_TRACE("OK\n");
}

MS_BOOL MDrv_Ursa_9_SWI2C_WriteBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_WriteBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

MS_BOOL MDrv_Ursa_9_SWI2C_ReadBytes(MS_U16 u16BusNumSlaveID, MS_U8 u8addrnum, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pu8data)
{
    return MApi_SWI2C_ReadBytes(u16BusNumSlaveID, u8addrnum, pu8addr, u16size, pu8data);
}

void CalCheckSum(MS_U8 *pBuf, MS_U8 u8BufLen )
{
    UBOOT_TRACE("IN\n");
    MS_U8 u8CheckSum =0;

    while( u8BufLen > 1 )//calculate pBuf[1]~pBuf[End] do not count pBuf[0],it is checksum
    {
        -- u8BufLen;
        u8CheckSum += pBuf[u8BufLen] & 0x0F;
    }
    u8CheckSum &= 0x0F;
    pBuf[0] &= 0x0F;
    pBuf[0] |= ((u8CheckSum )<<4);
    UBOOT_TRACE("OK\n");
}

MS_BOOL MDrv_Ursa_9_Write_Cmd(MS_U8 *pu8CmdData)
{
    MS_U16 u16BusNumSlaveID = URSA_9_DDC2BI_MODE_ADDR;
    // MUST re-calculator check sum pre I2C CMD.
    CalCheckSum(pu8CmdData, MFC_CMD_DATA_SUM);
    printf("[Ursa9]Write cmd:%x,%x,%x,%x,%x,%x,%x\n", pu8CmdData[0], pu8CmdData[1], pu8CmdData[2], pu8CmdData[3], pu8CmdData[4], pu8CmdData[5], pu8CmdData[6]);
    return MDrv_Ursa_9_SWI2C_WriteBytes(u16BusNumSlaveID, 0, NULL, URSA_9_CMD_DATA_SUM, pu8CmdData);
}

MS_BOOL MDrv_Ursa_9_SendCmd(URSA_9_CMD_TYPE eCmdType)
{
    UBOOT_TRACE("IN\n");
    MS_U8 pCmdData[URSA_9_CMD_DATA_SUM] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    MS_BOOL bUnkown = FALSE;
    MS_BOOL bRet = FALSE;

    switch(eCmdType)
    {
        case UC_INIT_RX_OSD_2LANE:
            {
                //0xE0, 0x01, 0x02, 0x05, 0x4C, 0x5C, 0x00
            #if (ENABLE_MSTAR_NAPOLI==0)
                pCmdData[0] = 0xE0;
                pCmdData[1] = 0x01;
                pCmdData[2] = 0x02;
                pCmdData[3] = 0x05;
                pCmdData[4] = 0x8C;
                pCmdData[5] = 0x9C;
                pCmdData[6] = 0x00;

            #else
                pCmdData[0] = 0xE0;
                pCmdData[1] = 0x01;
                pCmdData[2] = 0x02;
                pCmdData[3] = 0x05;
                pCmdData[4] = 0x4C;
                pCmdData[5] = 0x5C;
                pCmdData[6] = 0x00;
            #endif
            }
            break;
        case UC_INIT_RX_OSD_4LANE:
            {
                //0xF0, 0x01, 0x02, 0x0F, 0x46, 0x57, 0x00
            #if (ENABLE_MSTAR_NAPOLI==0)
                pCmdData[0] = 0x60;
                pCmdData[1] = 0x01;
                pCmdData[2] = 0x02;
                pCmdData[3] = 0x0F;
                pCmdData[4] = 0x89;
                pCmdData[5] = 0xAB;
                pCmdData[6] = 0x00;
            #else
                pCmdData[0] = 0xF0;
                pCmdData[1] = 0x01;
                pCmdData[2] = 0x02;
                pCmdData[3] = 0x0F;
                pCmdData[4] = 0x46;
                pCmdData[5] = 0x57;
                pCmdData[6] = 0x00;
            #endif
            }
            break;
        case UC_INIT_RX_VIDEO_2LANE:
            {
                //0xC0, 0x02, 0x11, 0xC0, 0xCC, 0xC1, 0xCC}
            #if (ENABLE_MSTAR_NAPOLI==0)
                pCmdData[0] = 0x50;
                pCmdData[1] = 0x02;
                pCmdData[2] = 0x11;
                pCmdData[3] = 0x44;
                pCmdData[4] = 0x44;
                pCmdData[5] = 0x55;
                pCmdData[6] = 0x55;
            #else
                pCmdData[0] = 0xC0;
                pCmdData[1] = 0x02;
                pCmdData[2] = 0x11;
                pCmdData[3] = 0xC0;
                pCmdData[4] = 0xCC;
                pCmdData[5] = 0xC1;
                pCmdData[6] = 0xCC;
            #endif
            }
            break;
        case UC_INIT_RX_VIDEO_4LANE:
            {
                //0xD0, 0x02, 0x55, 0xC0, 0xC2, 0xC1, 0xC3}
                pCmdData[0] = 0xD0;
                pCmdData[1] = 0x02;
                pCmdData[2] = 0x55;
                pCmdData[3] = 0xC0;
                pCmdData[4] = 0xC2;
                pCmdData[5] = 0xC1;
                pCmdData[6] = 0xC3;
            }
            break;
        case UC_INIT_RX_VIDEO_8LANE:
            {
                //0xD0, 0x02, 0x55, 0xC0, 0xC2, 0xC1, 0xC3}
            #if (ENABLE_MSTAR_NAPOLI==0)
                pCmdData[0] = 0x70;
                pCmdData[1] = 0x02;
                pCmdData[2] = 0xFF;
                pCmdData[3] = 0x40;
                pCmdData[4] = 0x62;
                pCmdData[5] = 0x51;
                pCmdData[6] = 0x73;
            #else
                pCmdData[0] = 0x70;
                pCmdData[1] = 0x02;
                pCmdData[2] = 0xFF;
                pCmdData[3] = 0x62;
                pCmdData[4] = 0x40;
                pCmdData[5] = 0x73;
                pCmdData[6] = 0x51;
            #endif
            }
            break;
        case UC_SET_IN_OSD_FULLHD:
            {
                //0xA1, 0x04, 0x03, 0x07, 0x80, 0x04, 0x38
                pCmdData[0] = 0xA1;
                pCmdData[1] = 0x04;
                pCmdData[2] = 0x03;
                pCmdData[3] = 0x07;
                pCmdData[4] = 0x80;
                pCmdData[5] = 0x04;
                pCmdData[6] = 0x38;
            }
            break;
        case UC_SET_IN_OSD_4K2K:
            {
                //0xE1, 0x04, 0x03, 0x0F, 0x00, 0x08, 0x70
                pCmdData[0] = 0xE1;
                pCmdData[1] = 0x04;
                pCmdData[2] = 0x03;
                pCmdData[3] = 0x0F;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x08;
                pCmdData[6] = 0x70;
            }
            break;
        case  UC_SET_IN_VIDEO_FULLHD:
            {
                //0x81, 0x05, 0x07, 0x80, 0x04, 0x38, 0x00
                pCmdData[0] = 0x81;
                pCmdData[1] = 0x05;
                pCmdData[2] = 0x07;
                pCmdData[3] = 0x80;
                pCmdData[4] = 0x04;
                pCmdData[5] = 0x38;
                pCmdData[6] = 0x00;
            }
            break;
        case  UC_SET_IN_VIDEO_4K2K:
            {
                //0xC1, 0x05, 0x0F, 0x00, 0x08, 0x70, 0x00
                pCmdData[0] = 0xC1;
                pCmdData[1] = 0x05;
                pCmdData[2] = 0x0F;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x08;
                pCmdData[5] = 0x70;
                pCmdData[6] = 0x00;
            }
            break;
        case UC_SET_LVDS_RX_RECONFIG:
            {
                //0x61, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0x61;
                pCmdData[1] = 0x06;
                pCmdData[2] = 0x00;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x00;
            }
            break;
        case UC_SET_3D_MODE:
            {
                //0x81,0x05,0x07,0x80,0x04,0x38,0x00
                pCmdData[0] = 0x13;
                pCmdData[1] = 0x01;
                pCmdData[2] = 0x00;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x00;
            }
            break;
        case UC_WRITE_REGISTER_1:
            {
                //0x23,0x1B,0x10,0x27,0xC2,0xEE,0xD0,
                pCmdData[0] = 0x23;
                pCmdData[1] = 0x1B;
                pCmdData[2] = 0x10;
                pCmdData[3] = 0x27;
                pCmdData[4] = 0xC2;
                pCmdData[5] = 0xEE;
                pCmdData[6] = 0xD0;
            }
            break;
        case UC_WRITE_REGISTER_2:
            {
                //0x23,0x1B,0x10,0x27,0x40,0x40,0xD0
                pCmdData[0] = 0x23;
                pCmdData[1] = 0x1B;
                pCmdData[2] = 0x10;
                pCmdData[3] = 0x27;
                pCmdData[4] = 0x40;
                pCmdData[5] = 0x40;
                pCmdData[6] = 0xD0;
            }
            break;
        case UC_WRITE_REGISTER_3:
            {
                //0x33,0x1B,0x10,0x27,0xC3,0xEE,0xD0
                pCmdData[0] = 0x33;
                pCmdData[1] = 0x1B;
                pCmdData[2] = 0x10;
                pCmdData[3] = 0x27;
                pCmdData[4] = 0xC3;
                pCmdData[5] = 0xEE;
                pCmdData[6] = 0xD0;
            }
            break;
        case UC_WRITE_REGISTER_MASK_1:
            {
                //0x33,0x1C,0x13,0x00,0x04,0x40,0x40
                pCmdData[0] = 0x33;
                pCmdData[1] = 0x1C;
                pCmdData[2] = 0x13;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x04;
                pCmdData[5] = 0x40;
                pCmdData[6] = 0x40;
            }
            break;
        case UC_WRITE_REGISTER_MASK_2:
            {
                //0x33,0x1C,0x13,0x00,0x04,0x00,0x40
                pCmdData[0] = 0x33;
                pCmdData[1] = 0x1C;
                pCmdData[2] = 0x13;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x04;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x40;
            }
            break;
        case UC_URSA_MOD_POWER_ON:
            {
               //URSA MOD Power On command: 0x93, 0x08, 0x01, 0x00, 0x00, 0x00, 0x00
                pCmdData[0] = 0x93;
                pCmdData[1] = 0x08;
                pCmdData[2] = 0x01;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x00;

            }
            break;
        case UC_URSA_GAIN_PATCH:
            {

                pCmdData[0] = 0x99;
                pCmdData[1] = 0x08;
                pCmdData[2] = 0x01;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x00;

            }
            break;
        case UC_URSA_MONACO_PATCH:
            {

                pCmdData[0] = 0x01;
                pCmdData[1] = 0x10;
                pCmdData[2] = 0x01;
                pCmdData[3] = 0x10;
                pCmdData[4] = 0xFF;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x00;

            }
            break;
        case UC_URSA_DISABLE_OSD:
            {

                pCmdData[0] = 0x41;
                pCmdData[1] = 0x04;
                pCmdData[2] = 0x00;
                pCmdData[3] = 0x00;
                pCmdData[4] = 0x00;
                pCmdData[5] = 0x00;
                pCmdData[6] = 0x00;

            }
            break;
        default:
            bUnkown = TRUE;
            break;
    }

    if(!bUnkown)
    {
        bRet  = MDrv_Ursa_9_Write_Cmd(pCmdData);
        printf("[Ursa9]Send cmd:%d, return:%d\n", eCmdType, bRet);
    }
    else
    {
        bRet = FALSE;
        printf("[Ursa9]Can not find the command, please check!\n");
    }
    UBOOT_TRACE("OK\n");
    return bRet;
}

//-------------------------------------------------------------------------------------------------
//  Member Function
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Ursa_9_Set_Lane_VB1_per_init(MS_U8 u8OsdLane, MS_U8 u8VideoLane)
{
    UBOOT_TRACE("IN\n");
    MS_BOOL bRet = FALSE;
#if(ENABLE_OSDC_Init==1)
    #if (ENABLE_MSTAR_MUJI==1 || ENABLE_MSTAR_MONACO==1)
    // Muji OC use 2 lane only
    bRet = MDrv_Ursa_9_SendCmd(UC_INIT_RX_OSD_2LANE);
    #else
    if (u8OsdLane == 4)
    {
        bRet = MDrv_Ursa_9_SendCmd(UC_INIT_RX_OSD_4LANE);
    }
    else
    {
        bRet = MDrv_Ursa_9_SendCmd(UC_INIT_RX_OSD_2LANE);
    }
    #endif
    if(bRet != TRUE)
    {
        UBOOT_ERROR("send ursa9 cmd fail\n");
        return FALSE;
    }
    udelay(50);
#endif

    if (u8VideoLane == 8)
    {
        bRet = MDrv_Ursa_9_SendCmd(UC_INIT_RX_VIDEO_8LANE);
    }
    else if (u8VideoLane == 4)
    {
        bRet = MDrv_Ursa_9_SendCmd(UC_INIT_RX_VIDEO_4LANE);
    }
    else
    {
        bRet = MDrv_Ursa_9_SendCmd(UC_INIT_RX_VIDEO_2LANE);
    }
    if(bRet != TRUE)
    {
        UBOOT_ERROR("send ursa9 cmd fail\n");
        return FALSE;
    }
    udelay(50);
    MDrv_Ursa_9_SendCmd(UC_SET_LVDS_RX_RECONFIG);
    udelay(50);
    UBOOT_TRACE("OK\n");
    if(bRet != TRUE)
    {
        UBOOT_ERROR("send ursa9 cmd fail\n");
        return FALSE;
    }
    return bRet;
}

MS_BOOL MDrv_Ursa_9_Set_Lane_VB1(MS_U16 u16OsdWidth, MS_U16 u16VideoWidth)
{

    UBOOT_TRACE("IN\n");
    MS_BOOL bRet = FALSE;

    if (u16VideoWidth > 1920)
    {
        bRet = MDrv_Ursa_9_SendCmd(UC_SET_IN_VIDEO_4K2K);
    }
    else
    {
        bRet = MDrv_Ursa_9_SendCmd(UC_SET_IN_VIDEO_FULLHD);
    }
    if(bRet != TRUE)
    {
        UBOOT_ERROR("send ursa9 cmd fail\n");
        return FALSE;
    }
    #if (ENABLE_MSTAR_MONACO == 1)
    MDrv_Ursa_9_SendCmd(UC_URSA_MONACO_PATCH);
    #endif
#if(ENABLE_OSDC_Init==1)
    udelay(50);
    #if (ENABLE_MSTAR_MUJI==1 || ENABLE_MSTAR_MONACO==1)
    // Muji OC use 2 lane only
    MDrv_Ursa_9_SendCmd(UC_SET_IN_OSD_FULLHD);
    #else
    if (u16OsdWidth > 1920)
    {
        MDrv_Ursa_9_SendCmd(UC_SET_IN_OSD_4K2K);
    }
    else
    {
        MDrv_Ursa_9_SendCmd(UC_SET_IN_OSD_FULLHD);
    }
    #endif
#endif
    udelay(50);
    MDrv_Ursa_9_SendCmd(UC_SET_3D_MODE);
    udelay(50);
    MDrv_Ursa_9_SendCmd(UC_URSA_MOD_POWER_ON);
    #if (ENABLE_MSTAR_NAPOLI==1)
    udelay(50);
    MDrv_Ursa_9_SendCmd(UC_URSA_GAIN_PATCH);
    #endif
    #if(ENABLE_OSDC_Init==0)
    udelay(50);
    MDrv_Ursa_9_SendCmd(UC_URSA_DISABLE_OSD);
    #endif
    UBOOT_TRACE("OK\n");

    return bRet;

}
#endif //__DRV_URSA_9_C_
