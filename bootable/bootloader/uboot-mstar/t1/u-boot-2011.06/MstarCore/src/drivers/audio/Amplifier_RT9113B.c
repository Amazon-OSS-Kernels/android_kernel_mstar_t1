/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
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

 //wjq20100112>>>
#include <common.h>
#include <audio/Amplifier_RT9113B.h>
#include <linux/types.h>
#include <apiSWI2C.h>
#include <MsUboot.h>
#include <drvIIC.h>

#define RT9113B_ADDR        0x36

#ifndef I2C_BUS_IDX
#define I2C_BUS_IDX         0
#endif

#define RT9113B_BUSID     (((MS_U16) I2C_BUS_IDX << 8) | RT9113B_ADDR)
#define  MAX_DATA_LENGTH   20

MAPI_U8  g_ucRT9113BRegAddr;


/****************************
        amp init regs
****************************/
MAPI_U8 AmpReadTbl[] =
{
    //DataLength        Address     DataN       DataN+1...
    1, 0x0a, 0xff,
    0x00,
};

MAPI_U8 AmpInitTblRa188b[] =
{
    1, 0x80, 0x80,
    1, 0x12, 0xD9,
    1, 0x13, 0x06,
    1, 0x84, 0x90,
    1, 0x16, 0x06,
    1, 0x18, 0x13,
    1, 0x81, 0x14,
    1, 0x19, 0x56,     // SW Volume default +16 dB
    1, 0x13, 0x02,
    1, 0x05, 0x02,
    1, 0x74, 0x00,
    1, 0x73, 0x8c,
    1, 0x73, 0x8c,
    1, 0x15, 0x44,
    2, 0x07, 0x01, 0x10,
    0x00,
};

MAPI_U8 AmpInitTblTka188e[] =
{
    //DataLength        Address     DataN       DataN+1...
    1, 0x05, 0x40,
    1, 0x80, 0x80,
    1, 0x11, 0xA9,
    1, 0x12, 0xD8,
    1, 0x13, 0x02,
    1, 0x15, 0x04,
    1, 0x16, 0x05,
    1, 0x18, 0x12,
    1, 0x81, 0x14,
    1, 0x82, 0x39,
    1, 0x05, 0x02,
    1, 0x15, 0xC4,
    1, 0x19, 0x56,
    1, 0x07, 0x10,     // SW Volume default +16 dB
    4, 0x57, 0x00, 0x01, 0xCC, 0x00,
    1, 0x03, 0xB0,
    4, 0x21, 0x3F, 0x7F, 0x00, 0xA9,
    4, 0x22, 0x07, 0xE5, 0x01, 0xFF,
    1, 0x70, 0xF1,
    4, 0x45, 0x00, 0x08, 0x5A, 0x8B,
    8, 0x3A, 0x00, 0x18, 0x12, 0x78, 0x00, 0x7F, 0xFF, 0x51,
    8, 0x3D, 0x00, 0x70, 0x0F, 0xE8, 0x00, 0x7F, 0xF9, 0x2C,
    8, 0x47, 0x00, 0x70, 0x0F, 0xE8, 0x00, 0x7F, 0xE0, 0x69,
    8, 0xAC, 0x00, 0x70, 0x0F, 0xE8, 0x00, 0x7F, 0xFF, 0x3E,
    0x00,
};

MAPI_U8 AmpmasterSWMute[] =
{
    1, 0x06, 0x03,
    0x00,
};

MAPI_U8 AmpmasterSWUnMute[] =
{
    1, 0x06, 0x00,
    0x00,
};

MAPI_BOOL Amplifier_RT9113_Init(void)
{
    MAPI_U8 * Pstr=NULL;
    MAPI_U8 DataLength = 0;
    printf("*****Amplifier_Init****\n");
    Audio_Amplifier_ON();
    mdelay(2);
    Adj_Volume_On();
    mdelay(40);

    Pstr = AmpReadTbl;
    DataLength = *Pstr;
    g_ucRT9113BRegAddr = *(++Pstr);
    Pstr++;
    if(MApi_SWI2C_ReadBytes(RT9113B_BUSID, 1, &g_ucRT9113BRegAddr, DataLength, Pstr) != TRUE)
    {
        printf("MApi_SWI2C_ReadBytes FAIL!!\n");
        return MAPI_FALSE;
    }
    printf("==========RT9113B chip id is:%d==========\n", AmpReadTbl[2]);

    if(AmpReadTbl[2] == 0x00)       //chip revision ID(0x00-TKA188E,0xff-RA188B)
    {
        Pstr = AmpInitTblTka188e;
    }
    else
    {
        Pstr = AmpInitTblRa188b;
    }

    do
    {
        DataLength = *Pstr;
        if(DataLength>MAX_DATA_LENGTH)
        {
            printf("** RT9113 set command error!!\n");
            break;
        }
        g_ucRT9113BRegAddr = *(++Pstr);
        Pstr++;
        if (MApi_SWI2C_WriteBytes(RT9113B_BUSID,1,&g_ucRT9113BRegAddr,DataLength, Pstr) == FALSE)
        {
            printf("** RT9113 write fail = %d\n",g_ucRT9113BRegAddr);
        }
        mdelay(2);

        Pstr = Pstr + DataLength;
    }while(*Pstr != 0);

    return MAPI_TRUE;
}


MAPI_BOOL Amplifier_RT9113_SWMute(MAPI_BOOL bMute)
{
    MAPI_U8 * Pstr;
    MAPI_U8 DataLength = 0;

    printf("******Amplifier_RT9113_SWMute(%d)\n",bMute);
    if(bMute)
    {
        Pstr=AmpmasterSWMute;
        do
        {
            DataLength = *Pstr;
            if(DataLength>20)
            {
                printf("9113 set command error!!\n");
                break;
            }
            g_ucRT9113BRegAddr = *(++Pstr);
            Pstr++;

            if (MApi_SWI2C_WriteBytes(RT9113B_BUSID,1, &g_ucRT9113BRegAddr, DataLength, Pstr) == FALSE)
            {
                printf("9113write fail = %d\n",g_ucRT9113BRegAddr);
            }
            mdelay(2);
            Pstr = Pstr + DataLength;
        }while(*Pstr != 0);
    }
    else
    {

        Pstr=AmpmasterSWUnMute;
        do
        {
            DataLength = *Pstr;
            if(DataLength>20)
            {
                printf("9113 set command error!!\n");
                break;
            }
            g_ucRT9113BRegAddr = *(++Pstr);
            Pstr++;

            if (MApi_SWI2C_WriteBytes(RT9113B_BUSID,1, &g_ucRT9113BRegAddr, DataLength, Pstr) == FALSE)
            {
                printf("9113write fail = %d\n",g_ucRT9113BRegAddr);
            }
            mdelay(2);

            Pstr = Pstr + DataLength;
        }while(*Pstr != 0);
    }
    return TRUE;
}

MAPI_BOOL Amplifier_RT9113_Mute(MAPI_BOOL bMute)
{
    // TODO: HW no use
    return TRUE;
}


