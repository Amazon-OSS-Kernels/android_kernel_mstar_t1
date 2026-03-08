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

#define FORCE_MY_GPIO_MODE 1

#define _IIC_C_

#include <linux/string.h>
#include "Board.h"
#include "drvIIC.h"
#include "drvGPIO.h"
#include "drvMMIO.h"
#include "drvHWI2C.h"




void MDrv_IIC_InitPort0(HWI2C_PORT ePort)
{
    MS_BOOL bResult = FALSE;
    HWI2C_UnitCfg hwI2CCfg;
    memset(&hwI2CCfg, 0x00, sizeof(HWI2C_UnitCfg));
#ifdef BD_CUS75_MBQ213A
#if (MS_BOARD_TYPE_SEL == BD_CUS75_MBQ213A) // To improve tuner and demod lock time
    hwI2CCfg.eSpeed = E_HWI2C_HIGH;
#else
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
#endif
#else
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
#endif

    hwI2CCfg.sI2CPin.u32Reg = 0;
    hwI2CCfg.sI2CPin.u8BitPos = 0;
    hwI2CCfg.sI2CPin.bEnable = 0;
    hwI2CCfg.ePort = ePort;
    bResult = MDrv_HWI2C_Init(&hwI2CCfg);
    if(bResult)
    {
        printf("MDrv_IIC_InitPort0: OK\n");
    }
    else
    {
        printf("MDrv_IIC_InitPort0: FAIL\n");
        return;
    }

    MDrv_HWI2C_SelectPort(hwI2CCfg.ePort);
    MDrv_HWI2C_SetClk(hwI2CCfg.eSpeed);
    MDrv_HWI2C_SetReadMode(E_HWI2C_READ_MODE_DIRECTION_CHANGE);

}

void MDrv_IIC_InitPort1(HWI2C_PORT ePort)
{
    MS_BOOL bResult;
    HWI2C_UnitCfg hwI2CCfg;
    memset(&hwI2CCfg, 0x00, sizeof(HWI2C_UnitCfg));
#ifdef BD_CUS75_MBQ213A
#if (MS_BOARD_TYPE_SEL == BD_CUS75_MBQ213A) // To improve tuner and demod lock time
    hwI2CCfg.eSpeed = E_HWI2C_HIGH;
#else
    hwI2CCfg.eSpeed = E_HWI2C_SLOW;//E_HWI2C_NORMAL
#endif
#endif
    hwI2CCfg.sI2CPin.u32Reg = 0;
    hwI2CCfg.sI2CPin.u8BitPos = 0;
    hwI2CCfg.sI2CPin.bEnable = 0;
    hwI2CCfg.ePort = ePort;
    bResult = MDrv_HWI2C_Init(&hwI2CCfg);
    if(bResult)
    {
        printf("MDrv_IIC_InitPort1: OK\n");
    }
    else
    {
        printf("MDrv_IIC_InitPort1: FAIL\n");
        return;
    }

    MDrv_HWI2C_SelectPort1(hwI2CCfg.ePort);
    MDrv_HWI2C_SetClkP1(hwI2CCfg.eSpeed);
    MDrv_HWI2C_SetReadModeP1(E_HWI2C_READ_MODE_DIRECTION_CHANGE);

}

void MDrv_IIC_Init(HWI2C_PORT ePort)
{
    printf("MDrv_IIC_Init  PORT  %d \n", ePort);

    if (E_HWI2C_PORT_NOSUP == ePort)
    {
        return;
    }

    if(ePort >= E_HWI2C_PORT_1)
    {
         MDrv_IIC_InitPort1(ePort);
    }
    else
    {
         MDrv_IIC_InitPort0(ePort);
    }

}



MS_BOOL MDrv_IIC_ReadBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    return MDrv_HWI2C_ReadBytes(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
}


MS_BOOL MDrv_IIC_ReadBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    return MDrv_HWI2C_ReadBytesP1(u16BusNumSlaveID, (MS_U32)ucSubAdr, paddr, (MS_U32)ucBufLen, pBuf);
}

MS_BOOL MDrv_IIC_ReadBytes(HWI2C_PORT ePort, MS_U16 u16BusNumSlaveID, MS_U8 ucSubAdr, MS_U8* paddr, MS_U16 ucBufLen, MS_U8* pBuf)
{
    //HB_printf("MDrv_IIC_ReadBytes  PORT  %d \n", ePort);
    if(ePort >= E_HWI2C_PORT_1)
    {
        return MDrv_IIC_ReadBytesPort1(u16BusNumSlaveID,ucSubAdr, paddr,ucBufLen, pBuf);
    }
    else
    {
        return MDrv_IIC_ReadBytesPort0(u16BusNumSlaveID,ucSubAdr, paddr,ucBufLen, pBuf);
    }
}


MS_BOOL MDrv_IIC_WriteBytesPort0(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    return MDrv_HWI2C_WriteBytes(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
}

MS_BOOL MDrv_IIC_WriteBytesPort1(MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    return MDrv_HWI2C_WriteBytesP1(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
}

MS_BOOL MDrv_IIC_WriteBytes(HWI2C_PORT ePort, MS_U16 u16BusNumSlaveID, MS_U8 AddrCnt, MS_U8* pu8addr, MS_U16 u16size, MS_U8* pBuf)
{
    //HB_printf("MDrv_IIC_WriteBytes  PORT  %d \n", ePort);
    if(ePort >= E_HWI2C_PORT_1)
    {
        return MDrv_IIC_WriteBytesPort1(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
    }
    else
    {
        return MDrv_IIC_WriteBytesPort0(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
    }
}



MS_BOOL MDrv_IIC_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    MS_BOOL ret;
    ret = MDrv_HWI2C_ReadBytes(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
    return ret;
}

MS_BOOL MDrv_IIC1_Read(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    return MDrv_HWI2C_ReadBytesP1(u16BusNumSlaveID, (MS_U32)AddrCnt, pu8addr, (MS_U32)u16size, pBuf);
}

MS_BOOL MDrv_IIC_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    MS_BOOL ret;
    ret = MDrv_IIC_WriteBytesPort0(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
    return ret;

}

MS_BOOL MDrv_IIC1_Write(MS_U16 u16BusNumSlaveID, MS_U8* pu8addr, MS_U8 AddrCnt, MS_U8* pBuf,  MS_U16 u16size)
{
    return MDrv_IIC_WriteBytesPort1(u16BusNumSlaveID,AddrCnt, pu8addr,u16size, pBuf);
}


MS_BOOL MDrv_IIC_SelectCLK(HWI2C_PORT ePort, HWI2C_CLKSEL eSpeed)
{
    if(ePort >= E_HWI2C_PORT_1)
    {
        return MDrv_HWI2C_SetClkP1(eSpeed);

    }
    else
    {
        return MDrv_HWI2C_SetClk(eSpeed);
    }
}


void IIC_Stop(void)
{
    printf("IIC_N");
}

MS_BOOL IIC_Start(void)
{
    printf("IIC_N");
    return 0;
}

MS_BOOL IIC_SendByte(MS_U8 u8dat)
{
    printf("IIC_N");
    return 0;
}

HWI2C_PORT getI2CPort(MS_U8 drv_frontend_index)
{
    HWI2C_PORT ePort;
    switch(drv_frontend_index)
    {
        case 0:
            ePort = FRONTEND_TUNER_PORT0;
        break;
        case 1:
            ePort = FRONTEND_TUNER_PORT1;
        break;
        case 2:
            ePort = FRONTEND_TUNER_PORT2;
        break;
        case 3:
            ePort = FRONTEND_TUNER_PORT3;
        break;
        default:
            ePort = FRONTEND_TUNER_PORT0;
            break;
    }
    return ePort;
}


#undef _IIC_C_

