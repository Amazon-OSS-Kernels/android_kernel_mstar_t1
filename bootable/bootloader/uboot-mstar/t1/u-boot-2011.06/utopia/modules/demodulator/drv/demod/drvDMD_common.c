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
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDMD_common.c
/// @brief  DMD Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
// Common Definition
#ifdef MSOS_TYPE_LINUX_KERNEL
#include <linux/string.h>
#else
#include <string.h>
#include <stdio.h>
#include <math.h>
#endif
#include "MsCommon.h"
#include "MsVersion.h"
#include "MsOS.h"
#include "drvSYS.h"
#include "drvDMD_common.h"
#include "halDMD_INTERN_common.h"
#include "drvDMD_VD_MBX.h"
#include "ULog.h"
#include "drvMSPI.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#define SPI_DEVICE_BUFFER_SIZE           256
//////////////MSPI Define//////////////////
#define READ_INDEX  0
#define WRITE_INDEX 1
#define CMD_INDEX 0
#define ADDR_INDEX 1
//SSPI COMMAND
#define RIU_W_CMD      0x1A
#define RIU_W1_CMD     0x1D
#define RIU_R_CMD       0x18
#define RIU_R1_CMD      0x1C
#define RIU_RT_CMD      0x11
#define RIU_R1T_CMD      0x15
#define MIU_W_CMD      0x25
#define MIU_R_CMD   0x20
#define MIU_ST_CMD       0x21
#define CFG_W_CMD       0x05
#define CFG_R_CMD       0x00


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
/*
#ifdef CONFIG_AMAZON
extern s_MDRV_DMD_INTERFACE_FUNCTION  sMdrvDmdInterfaceFunc;
#endif
*/
MS_U8   u8DMD_I2C_SLAVE_BUS;
MS_U8   u8DMD_I2C_SLAVE_ADDR;
MS_BOOL   bIS_EXTERN_DMD;
s_I2C_Interface_func   sI2cInterfaceFunc;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------
#ifdef MS_DEBUG
#define DMD_DBG(x)          (x)
#else
#define DMD_DBG(x)          //(x)
#endif
//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_DMD_PreInit(void)
{
    DMD_DBG(ULOGD("DEMOD","MDrv_DMD_PreInit\n"));
    /*
    #ifdef CONFIG_AMAZON
    sMdrvDmdInterfaceFunc.MDrv_DMD_I2C_Channel_Change  = MDrv_DMD_I2C_Channel_Change;
    sMdrvDmdInterfaceFunc.MDrv_DMD_I2C_Channel_Set        = MDrv_DMD_I2C_Channel_Set;
    sMdrvDmdInterfaceFunc.MDrv_DMD_ReadReg                     = MDrv_DMD_ReadReg;
    sMdrvDmdInterfaceFunc.MDrv_DMD_WriteReg                     = MDrv_DMD_WriteReg;
    sMdrvDmdInterfaceFunc.MDrv_DMD_WriteRegs                   = MDrv_DMD_WriteRegs; 
    sMdrvDmdInterfaceFunc.Log10Approx                                 = Log10Approx;
    #endif
    */
    HAL_DMD_RegInit();

    //HAL_DMD_TS1_Tristate(TRUE);
    MDrv_SYS_SetAGCPadMux(E_SYS_DTV_AGC_PAD_SET_ALL_OFF);

    return TRUE;
}

void MDrv_DMD_RFAGC_Tristate(MS_BOOL bEnable)
{
    HAL_DMD_RFAGC_Tristate(bEnable);
}

void MDrv_DMD_IFAGC_Tristate(MS_BOOL bEnable)
{
    HAL_DMD_IFAGC_Tristate(bEnable);
}

MS_BOOL MDrv_DMD_TS_GetClockRate(void *fTS_CLK)
{
    // TODO: TS output is only valid when demod is locked
//    return HAL_DMD_TS_GetClockRate(fTS_CLK);
return true;
}

MS_BOOL MDrv_DMD_TSO_Clk_Control(MS_U8 *u8cmd_array)
{
    return HAL_DMD_TSO_Clk_Control(u8cmd_array);
}

MS_BOOL MDrv_DMD_ReadReg(MS_U32 u32Reg, MS_U8 *u8Value)
{
    MS_BOOL bResult = FALSE;  
    if (bIS_EXTERN_DMD == FALSE)
        bResult = MDrv_SYS_DMD_VD_MBX_ReadReg(u32Reg, u8Value);
    else //external demod
        bResult = HAL_DMD_IIC_ReadByte((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), u32Reg, u8Value);
    return bResult;
}
MS_BOOL MDrv_DMD_WriteReg(MS_U32 u32Reg, MS_U8 u8Value)
{
    MS_BOOL bResult = FALSE;  
    if (bIS_EXTERN_DMD == FALSE)
        bResult = MDrv_SYS_DMD_VD_MBX_WriteReg(u32Reg, u8Value);
    else //external demod
        bResult = HAL_DMD_IIC_WriteByte((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), u32Reg, u8Value);
    return bResult;  
}

MS_BOOL MDrv_DMD_WriteRegs(MS_U32 u32Reg, MS_U8 *u8Value, MS_U8 u8Length)
{
    MS_BOOL bResult = FALSE;  
    if (bIS_EXTERN_DMD == FALSE)
        bResult = MDrv_SYS_DMD_VD_MBX_WriteReg(u32Reg, (*u8Value));
    else //external demod
        bResult = HAL_DMD_IIC_WriteBytes((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), u32Reg, u8Value, u8Length);
    return bResult;  

}

MS_BOOL MDrv_DMD_I2C_Channel_Change(MS_U8 ch_num)
{
    MS_BOOL bResult = FALSE;  
    bResult = HAL_DMD_I2C_Channel_Change((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), ch_num);

    return bResult;  

}

MS_BOOL MDrv_DMD_I2C_Channel_Set(MS_U8  ch_num)
{
    MS_BOOL bResult = FALSE;  

    bResult = HAL_DMD_I2C_Channel_Set((u8DMD_I2C_SLAVE_BUS << 8 | u8DMD_I2C_SLAVE_ADDR), ch_num);
    return bResult;  

}

MS_BOOL MDrv_DMD_SSPI_Init(MS_U8  u8DeviceNum)
{

	//bryan temp mark 
	#if(0)
    if(MDrv_MSPI_Init_Ext(u8DeviceNum) == E_MSPI_OK)
        return TRUE;
    else
        return FALSE;
	#else
     return true;
     #endif
}

MS_BOOL MDrv_DMD_SSPI_MIU_Writes(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    MS_BOOL bRet = TRUE;
    MS_U8 Wdata[5];
    
    Wdata[0] = MIU_W_CMD;
    Wdata[1] = u32Addr & 0xFF;
    Wdata[2] = (u32Addr >> 8) & 0xFF;
    Wdata[3] = (u32Addr >> 16)& 0xFF;
    Wdata[4] = (u32Addr >> 24);
    //ULOGD("DEMOD","MDrv_SS_MIU_Writes\n");
    //bryan temp mark
/*
    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    // send write address
    MDrv_MSPI_Write(Wdata,sizeof(Wdata));
    // send data
    MDrv_MSPI_Write(pdata,u16Size);
    MDrv_MSPI_SlaveEnable(FALSE);
*/
    return bRet;
}

MS_BOOL MDrv_DMD_SSPI_MIU_Reads(MS_U32 u32Addr, MS_U8 *pdata, MS_U16 u16Size)
{
    
    MS_BOOL bRet = TRUE;
    MS_U8 Rdata[SPI_DEVICE_BUFFER_SIZE];
    MS_U16 dataLen, i, j=0;

    do
    {
        dataLen = (u16Size>16?16:u16Size);//(len>24?24:len);
        
        Rdata[0] = MIU_R_CMD;
        Rdata[1] = u32Addr & 0xFF;
        Rdata[2] = (u32Addr >> 8) & 0xFF;
        Rdata[3] = (u32Addr >> 16)& 0xFF;
        Rdata[4] = (u32Addr >> 24);
        Rdata[5] = dataLen+1;
        //ULOGD("DEMOD","MDrv_SS_MIU_Reads, addr=0x%lx, dataLen=%d\n", u32Addr, dataLen);

        // send read command to read data
        //bryan temp mark
        #if(0)
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,6);
        MDrv_MSPI_SlaveEnable(FALSE);

        // read operation
        Rdata[0] = MIU_ST_CMD;
        MDrv_MSPI_SlaveEnable(TRUE);
        MDrv_MSPI_Write(Rdata,1);
        //ULOGD("DEMOD","Read Operation\n");
        MDrv_MSPI_Read(Rdata, dataLen+1);
        MDrv_MSPI_SlaveEnable(FALSE);
	#endif
        if(Rdata[0] != 0x0A)
        {
            ULOGD("DEMOD","MDrv_SS_MIU_Reads fail, status=0x%x\n", Rdata[0] );
            return false;
        }

        for (i=1; i<dataLen+1; i++, j++) 
        {
            pdata[j] = Rdata[i];
            //ULOGD("DEMOD","%x, ", pdata[j]);
        }

        u16Size -= dataLen;
        u32Addr += dataLen;
        //ULOGD("DEMOD","u16Size=%d,  u32Addr=0x%lx\n", u16Size, u32Addr);
    }while(u16Size);

    return bRet;
}

MS_BOOL MDrv_DMD_SSPI_RIU_Write8(MS_U16 u16Addr, MS_U8 data)
{
    MS_BOOL bRet = TRUE;
    MS_U8 Wdata[4];

    Wdata[0] = RIU_W1_CMD;
    Wdata[1] = u16Addr & 0xFF;
    Wdata[2] = (u16Addr >> 8) & 0xFF;
    Wdata[3] = data;
    //ULOGD("DEMOD","MDrv_SS_RIU_Write\n");
//bryan temp mark
#if(0)
    // Write operation
    MDrv_MSPI_SlaveEnable(TRUE);
    
    // send write address & data
    MDrv_MSPI_Write(Wdata,4);
    
    MDrv_MSPI_SlaveEnable(FALSE);
#endif
    return bRet;
}

MS_BOOL MDrv_DMD_SSPI_RIU_Read8(MS_U16 u16Addr, MS_U8 *pdata)
{
    MS_BOOL bRet = TRUE;
    MS_U8 Rdata[5];
    
    Rdata[0] = RIU_R1T_CMD;
    Rdata[1] = u16Addr & 0xFF;
    Rdata[2] = (u16Addr >> 8) & 0xFF;
    Rdata[3] = 0x00;
    //ULOGD("DEMOD","MDrv_SS_RIU_Read8\n");
    //bryan temp mark
    #if(0)
    MDrv_MSPI_SlaveEnable(TRUE);
    // send read command to read data
    MDrv_MSPI_Write(Rdata,4);
    // read operation
    //ULOGD("DEMOD","Read Operation\n");
    MDrv_MSPI_Read(pdata, 1);
    MDrv_MSPI_SlaveEnable(FALSE);
    #endif
    
    return bRet;

}
