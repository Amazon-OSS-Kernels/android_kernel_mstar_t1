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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   mdrv_iic.h
/// @brief  IIC Driver Interface
/// @author MStar Semiconductor Inc.
///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _MDRV_IIC_IO_H_
#define _MDRV_IIC_IO_H_

#include <asm/types.h>//<asm-mips/types.h>
#include "mdrv_types.h"
#if defined(CONFIG_COMPAT)
#include <linux/compat.h>
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
struct IIC_Param
{
    U8 u8IdIIC;      	/// IIC ID: Channel 1~7
    U8 u8ClockIIC;   	/// IIC clock speed
    U8 u8SlaveIdIIC;    /// Device slave ID
    U8 u8AddrSizeIIC;	/// Address length in bytes
    U8 u8AddrIIC[4];	/// Starting address inside the device
    U8 *u8pbufIIC;     	/// buffer
    U32 u32DataSizeIIC;	/// size of buffer

} __attribute__ ((packed));

#if defined(CONFIG_COMPAT)
struct IIC_Param_Compat
{
    U8 u8IdIIC;      	/// IIC ID: Channel 1~7
    U8 u8ClockIIC;   	/// IIC clock speed
    U8 u8SlaveIdIIC;    /// Device slave ID
    U8 u8AddrSizeIIC;	/// Address length in bytes
    U8 u8AddrIIC[4];	/// Starting address inside the device
    compat_uptr_t u8pbufIIC;     	/// buffer //(4) not using pointer
    U32 u32DataSizeIIC;	/// size of buffer

} __attribute__ ((packed));

typedef struct IIC_Param_Compat  IIC_Param_Compat;
#endif
typedef struct IIC_Param  IIC_Param;

typedef struct IIC_Param IIC_Param_t;

typedef struct{
	u8 u8ChIdx;         ///Channel index
	u8 u8Enable;        ///Enable
	u16 u16Retries;
    u16 u16PadSCL;      ///Pad(Gpio) number for SCL
    u16 u16PadSDA;      ///Pad(Gpio) number for SDA
    u16 u16SpeedKHz;    ///Speed in KHz
	u16 u16DefDelay;
} IIC_BusCfg_t;


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define IIC_IOC_MAGIC               'u'

#if defined(CONFIG_COMPAT)
#define Compat_MDRV_IIC_INIT               _IO(IIC_IOC_MAGIC, 0)
#define Compat_MDRV_IIC_CLOCK              _IOW(IIC_IOC_MAGIC, 2, IIC_Param_Compat)
#define Compat_MDRV_IIC_ENABLE             _IOW(IIC_IOC_MAGIC, 3, IIC_Param_Compat) 
#define Compat_MDRV_IIC_BUSCFG             _IOW(IIC_IOC_MAGIC, 4, IIC_BusCfg_t)
#endif

#define MDRV_IIC_INIT               _IO(IIC_IOC_MAGIC, 0)
#define MDRV_IIC_SET_PARAM          _IOW(IIC_IOC_MAGIC, 1, IIC_Param_t)
#define MDRV_IIC_CLOCK              _IOW(IIC_IOC_MAGIC, 2, IIC_Param_t)
#define MDRV_IIC_ENABLE             _IOW(IIC_IOC_MAGIC, 3, IIC_Param_t) 
#define MDRV_IIC_BUSCFG             _IOW(IIC_IOC_MAGIC, 4, IIC_BusCfg_t)
#define IIC_IOC_MAXNR               5

#define IIC_RW_BUF_SIZE             1024
#define IIC_WR_BUF_SIZE             128		// added for RGB EDID
#define IIC_RD_BUF_SIZE             256		// added for RGB EDID

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
// for software IIC
void MDrv_SW_IIC_SetSpeed(u8 u8ChIIC, u8 u8Speed);
void MDrv_SW_IIC_Init(IIC_BusCfg_t I2CBusCfg[], u8 u8CfgBusNum);
void MDrv_SW_IIC_Init_Bus(IIC_BusCfg_t I2CBusCfg[], u8 u8Bus);
void MDrv_SW_IIC_Init_Setup(void);
void MDrv_SW_IIC_Enable( u8 u8ChIIC, B16 bEnable );
int MDrv_SW_IIC_WriteBytes(u8 u8BusNum, u8 u8SlaveID, u8 u8addrcount, u8* pu8addr, u16 u16size, u8* pu8data);
int MDrv_SW_IIC_WriteBytesStop(u8 u8BusNum, u8 u8SlaveID,u8 AddrCnt, u8* pu8addr, u16 u16size, u8* pBuf, int bGenStop);
int MDrv_SW_IIC_ReadBytes(u8 u8BusNum, u8 u8SlaveID, u8 u8AddrNum, u8* paddr, u16 u16size, u8* pu8data);
int MDrv_SW_IIC_ReadByte(u8 u8BusNum, u8 u8SlaveID, u8 u8RegAddr, u8 *pu8Data);
int MDrv_SW_IIC_WriteByte(u8 u8BusNum, u8 u8SlaveID, u8 u8RegAddr, u8 u8Data);
int MDrv_SW_IIC_Write2Bytes(u8 u8BusNum, u8 u8SlaveID, u8 u8addr, u16 u16data);
u16 MDrv_SW_IIC_Read2Bytes(u8 u8BusNum, u8 u8SlaveID, u8 u8addr);
int MDrv_SW_IIC_Write4Bytes(u8 u8BusNum, u8 u8SlaveID, u32 u32Data, u8 u8EndData);
int MDrv_SW_IIC_WriteByteDirectly(u8 u8BusNum, u8 u8SlaveID, u8 u8Data);
int MDrv_SW_IIC_WriteByteArrayDirectly(u8 u8BusNum, u8 u8SlaveID, u16 u16size, u8* pu8Data, int stop);
int MDrv_SW_IIC_ReadByteArrayDirectly(u8 u8BusNum, u8 u8SlaveID, u16 u16size, u8* pu8Data, int stop);
// for hardware IIC
void MDrv_IIC_Init(void);
void MDrv_HW_IIC_Clock_Select(U8 u8ClockIIC);
S32 MDrv_HW_IIC_WriteBytes(U8 u8SlaveIdIIC, U8 u8AddrSizeIIC, U8 *pu8AddrIIC, U32 u32BufSizeIIC, U8 *pu8BufIIC);
S32 MDrv_HW_IIC_ReadBytes(U8 u8SlaveIdIIC, U8 u8AddrSizeIIC, U8 *pu8AddrIIC, U32 u32BufSizeIIC, U8 *pu8BufIIC);
void MDrv_HW_IIC_Init( void );
#if (defined(CONFIG_MSTAR_TITANIA)||defined(CONFIG_MSTAR_TITANIA2))
#else
B16 MDrv_SW_IIC_ConfigBus(IIC_BusCfg_t* pBusCfg);
int MDrv_SW_IIC_GetSDA(U8 u8ChIIC);
int MDrv_SW_IIC_GetSCL(U8 u8ChIIC);
#endif
#endif