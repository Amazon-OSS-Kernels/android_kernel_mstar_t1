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

#ifndef _HAL_CHIP_H_
#define _HAL_CHIP_H_



#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------

typedef enum
{
  E_CHIP_MIU_0 = 0,
  E_CHIP_MIU_1,
  E_CHIP_MIU_2,
  E_CHIP_MIU_3,
  E_CHIP_MIU_NUM,
} CHIP_MIU_ID;

#define ARM_CLOCK_FREQ             1008000000
#define MIPS_CLOCK_FREQ            900000000
#define AEON_CLOCK_FREQ            240000000
#define XTAL_CLOCK_FREQ            12000000

#define HAL_MIU0_BASE               0x00000000UL
#if defined(__AEONR2__)
#define HAL_MIU1_BASE               0x40000000UL // 1512MB
#define HAL_MIU2_BASE               0xC0000000UL //
#else
#define HAL_MIU1_BASE               0x80000000UL // 1512MB
#define HAL_MIU2_BASE               0xC0000000UL //
#endif


#define HAL_MIU0_BUS_BASE           0x20000000UL  // MIU0 Low 256MB
#define HAL_MIU1_BUS_BASE           0xA0000000UL  // MIU1 Low 256MB MUJI: 0xA0000000UL
#define HAL_MIU2_BUS_BASE           0x300000000UL // 64 bit mapping

#define _phy_to_miu_offset(MiuSel, Offset, PhysAddr) if (PhysAddr < HAL_MIU1_BASE) \
                                                        {MiuSel = E_CHIP_MIU_0; Offset = PhysAddr;} \
                                                     else if ((PhysAddr >= HAL_MIU1_BASE) && (PhysAddr < HAL_MIU2_BASE)) \
                                                         {MiuSel = E_CHIP_MIU_1; Offset = PhysAddr - HAL_MIU1_BASE;} \
                                                     else \
                                                         {MiuSel = E_CHIP_MIU_2; Offset = PhysAddr - HAL_MIU2_BASE;}

#define _miu_offset_to_phy(MiuSel, Offset, PhysAddr) if (MiuSel == E_CHIP_MIU_0) \
                                                        {PhysAddr = Offset;} \
                                                     else if (MiuSel == E_CHIP_MIU_1) \
                                                         {PhysAddr = Offset + HAL_MIU1_BASE;} \
                                                     else \
                                                         {PhysAddr = Offset + HAL_MIU2_BASE;}

#define SUPPORT TRUE
#define NONSUPPORT FALSE

#define CHIP_IP_MFE SUPPORT
#define CHIP_IP_VE SUPPORT
#define CHIP_IP_AVD SUPPORT
#define CHIP_IP_DEMOD SUPPORT
#define CHIP_IP_VBI SUPPORT
#define CHIP_IP_VIF SUPPORT
#define CHIP_IP_DMX SUPPORT
#define CHIP_IP_CEC SUPPORT
#define CHIP_IP_MBX SUPPORT
#define CHIP_IP_SWI2C SUPPORT
#define CHIP_IP_BDMA SUPPORT
#define CHIP_IP_CPU SUPPORT
#define CHIP_IP_GPIO SUPPORT
#define CHIP_IP_HWI2C SUPPORT
#define CHIP_IP_IR SUPPORT
#define CHIP_IP_MIU SUPPORT
#define CHIP_IP_MPIF NONSUPPORT
#define CHIP_IP_MSPI SUPPORT
#define CHIP_IP_PM SUPPORT
#define CHIP_IP_PWM SUPPORT
#define CHIP_IP_PWS SUPPORT
#define CHIP_IP_RTC SUPPORT
#define CHIP_IP_SAR SUPPORT
#define CHIP_IP_URDMA SUPPORT
#define CHIP_IP_WDT SUPPORT
#define CHIP_IP_AESDMA SUPPORT
#define CHIP_IP_CA NONSUPPORT
#define CHIP_IP_PCMCIA SUPPORT
#define CHIP_IP_SC SUPPORT
#define CHIP_IP_GPD SUPPORT
#define CHIP_IP_JPEG SUPPORT
#define CHIP_IP_VDEC SUPPORT
#define CHIP_IP_ACE SUPPORT
#define CHIP_IP_DAC NONSUPPORT
#define CHIP_IP_DDC2BI SUPPORT
#define CHIP_IP_DIP SUPPORT
#define CHIP_IP_DLC SUPPORT
#define CHIP_IP_GOP SUPPORT
#define CHIP_IP_HDMITX NONSUPPORT
#define CHIP_IP_MHL SUPPORT
#define CHIP_IP_MVOP SUPPORT
#define CHIP_IP_PNL SUPPORT
#define CHIP_IP_XC SUPPORT
#define CHIP_IP_GFX SUPPORT
#define CHIP_IP_AUDIO SUPPORT
#define CHIP_IP_SERFLASH SUPPORT

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
void    CHIP_InitISR(void);
MS_BOOL CHIP_InISRContext(void);
MS_BOOL CHIP_AttachISR(InterruptNum eIntNum, InterruptCb pIntCb);
MS_BOOL CHIP_DetachISR(InterruptNum eIntNum);
MS_BOOL CHIP_EnableIRQ(InterruptNum eIntNum);
MS_BOOL CHIP_DisableIRQ(InterruptNum eIntNum);
MS_BOOL CHIP_EnableAllInterrupt(void);
MS_BOOL CHIP_DisableAllInterrupt(void);
MS_BOOL CHIP_DebugIRQ(InterruptNum eIntNum, IrqDebugOpt eIrqDebugOpt);
MS_BOOL CHIP_CompleteIRQ(InterruptNum eIntNum);


#ifdef __cplusplus
}
#endif

#endif // _HAL_CHIP_H_

