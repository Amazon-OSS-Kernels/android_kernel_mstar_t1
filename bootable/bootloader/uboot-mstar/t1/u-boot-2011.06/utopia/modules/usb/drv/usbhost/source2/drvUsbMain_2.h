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

	#define mbFUSBH200_HALFSPEEDEnable_Set()		  (mwHost20Bit_Set_Port2(0x40,BIT2))
	#define mbFUSBH200_Interrupt_OutPut_High_Set()	  (mwHost20Bit_Set_Port2(0x40,BIT3))
       #define mbFUSBH200_Interrupt_OutPut_Low_Set()     (mwHost20Bit_Clr_Port2(0x40,BIT3))
	#define mbFUSBH200_VBUS_OFF_Set()		          (mwHost20Bit_Set_Port2(0x40,BIT4))
	#define mbFUSBH200_VBUS_ON_Set()   		          (mwHost20Bit_Clr_Port2(0x40,BIT4))
	#define mbFUSBH200_VBUS_VLD_Rd()   		          (mwHost20Port_Port2(0x40)&BIT8)


extern void FUSBH200_Driver_VBUS(void);
//extern void FUSBH200_Drop_VBUS(void);
extern U8 MDrv_UsbGetMaxLUNCount_Port2(void);
//#define Enable_Issue_TestPacket
//#define Issue_TestPacket
//#define Enable_Burning_Test
//#define Enable_SOF_Only
#define ENABLE_CBI_HOST
//#define Enable_Issue_TestPacketByHW
//#define ENABLE_HOST_TEST
#define DEVICE_ENUM_SEGMENT

void UTMI2_ORXBYTE(U8 offset,U8 val);
void UTMI2_ANDXBYTE(U8 offset,U8 val);
void UTMI2_SETXBYTE(U8 offset,U8 val);
U8 UTMI2_READXBYTE(U8 offset);
void UHC2_ORXBYTE(U8 offset,U8 val);
void UHC2_ANDXBYTE(U8 offset,U8 val);
void UHC2_SETXBYTE(U8 offset,U8 val);
U8 UHC2_READXBYTE(U8 offset);
void USBC2_ORXBYTE(U8 offset,U8 val);
void USBC2_ANDXBYTE(U8 offset,U8 val);
void USBC2_SETXBYTE(U8 offset,U8 val);
U8 USBC2_READXBYTE(U8 offset);

extern U32  gUHC2_BASE;
extern U32  gUTMI_BASE;
extern U32  gUTMI2_BASE;
extern U32  gUSBC2_BASE;

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
##No_CPU_type_for_USB2_KSEG02KSEG1
#endif

#endif
