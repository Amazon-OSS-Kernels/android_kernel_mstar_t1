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
///////////////////////////////////////////////////////////////////////////////
//
//	File name: drvUsbMain.H
//	Version: 1.0
//	Date: 2004/12/08
//
//	Company: Faraday Tech. Corp.
//
//	Description:
///////////////////////////////////////////////////////////////////////////////
#ifndef Main_Host_Only__H
#define Main_Host_Only__H

#include "../drvUsbHostConfig.h"

//=================== 1.Condition Definition  ============================================================
//========================================================================================================

	#define mbFUSBH200_HALFSPEEDEnable_Set()		  (mwHost20Bit_Set(0x40,BIT2))
	#define mbFUSBH200_Interrupt_OutPut_High_Set()	  (mwHost20Bit_Set(0x40,BIT3))
       #define mbFUSBH200_Interrupt_OutPut_Low_Set()     (mwHost20Bit_Clr(0x40,BIT3))
	#define mbFUSBH200_VBUS_OFF_Set()		          (mwHost20Bit_Set(0x40,BIT4))
	#define mbFUSBH200_VBUS_ON_Set()   		          (mwHost20Bit_Clr(0x40,BIT4))
	#define mbFUSBH200_VBUS_VLD_Rd()   		          (mwHost20Port(0x40)&BIT8)


extern void FUSBH200_Driver_VBUS(void);
//extern void FUSBH200_Drop_VBUS(void);
extern U8 MDrv_UsbGetMaxLUNCount(void);
//#define Enable_Issue_TestPacket
//#define Issue_TestPacket
//#define Enable_Burning_Test
//#define Enable_Performance_Read_Test
//#define Enable_Performance_Write_Test
//#define Enable_SOF_Only
#define ENABLE_CBI_HOST
//#define Enable_Issue_TestPacketByHW
//#define ENABLE_HOST_TEST
#define DEVICE_ENUM_SEGMENT

void UTMI_ORXBYTE(U8 offset,U8 val);
void UTMI_ANDXBYTE(U8 offset,U8 val);
void UTMI_SETXBYTE(U8 offset,U8 val);
U8 UTMI_READXBYTE(U8 offset);
void UHC_ORXBYTE(U8 offset,U8 val);
void UHC_ANDXBYTE(U8 offset,U8 val);
void UHC_SETXBYTE(U8 offset,U8 val);
U8 UHC_READXBYTE(U8 offset);
void USBC_ORXBYTE(U8 offset,U8 val);
void USBC_ANDXBYTE(U8 offset,U8 val);
void USBC_SETXBYTE(U8 offset,U8 val);
U8 USBC_READXBYTE(U8 offset);

extern U32  gUHC_BASE;
extern U32  gUTMI_BASE;
extern U32  gUSBC_BASE;

//#define  UHC_XBYTE(x)       (XBYTE[gUHC_BASE+x])
#define  UHC_XWORD(offset) 	  *((UINT16 volatile  *) ( gUHC_BASE+(offset)*2))

#define  USBC_XBYTE(x)       (XBYTE[gUSBC_BASE+x])
#if defined(CPU_TYPE_AEON)

#define KSEG02KSEG1(addr)  ((void *)((U32)(addr)|0x80000000))
#define KSEG12KSEG0(addr)  ((void *)((U32)(addr)&~0x80000000))

#elif defined(CPU_TYPE_MIPS)

#undef KSEG02KSEG1
#if 0
#define KSEG02KSEG1(addr)  MsOS_PA2KSEG1(MsOS_VA2PA((U32)addr))
#else
#define KSEG02KSEG1(addr)  ((void *)((U32)(addr) | 0xA0000000))
#endif

#undef KSEG12KSEG0
#if 0
#define KSEG12KSEG0(addr)  MsOS_PA2KSEG0(MsOS_VA2PA((U32)addr))
#else
#define KSEG12KSEG0(addr)  ((void *)((U32)(((addr) & ~0x20000000) | 0x80000000)))
#endif

#elif defined(CPU_TYPE_ARM)
#undef KSEG02KSEG1
#define KSEG02KSEG1(addr)  MsOS_PA2KSEG1(MsOS_VA2PA((U32)addr))

#undef KSEG12KSEG0
#define KSEG12KSEG0(addr)  MsOS_PA2KSEG0(MsOS_VA2PA((U32)addr))

#else
#No_CPU_type_for_USB_KSEG02KSEG1
#endif


#endif
