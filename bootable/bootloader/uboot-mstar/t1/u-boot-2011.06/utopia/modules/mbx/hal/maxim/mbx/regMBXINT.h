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
/// @file   regMBXINT.h
/// @brief  MStar Mailbox Driver DDI
/// @attention
/// <b><em>For INT_FIQMASK_AEON2MIPS, maybe has issues with big-endient, but seems pm won't use this bit\n
/// so just skip it now. </em></b>
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MHAL_MBX_INTERRUPT_REG_H
#define _MHAL_MBX_INTERRUPT_REG_H

//=============================================================================
// Includs
//=============================================================================

//=============================================================================
// Defines & Macros
//=============================================================================
#define RIU_MAP _virtRIUBaseAddrMBXINT

#define RIU     ((MS_U16 volatile *) RIU_MAP)
#define RIU8    ((MS_U8  volatile *) RIU_MAP)

//#############################
//# host0: 51               (bank : 1019h)
//# host1: ARM CA9 Core0    (bank : 1019h)
//# host2: ARM CA9 Core1    (bank : 1211h)
//# host3: SECURE-R2        (bank : 1019h)
//#############################
#define REG_FIQ_MASK_BASE               (0xC80<<1)//0x101900-0x100000
#define FIQ_REG(address)                RIU[address*2+REG_FIQ_MASK_BASE]
#define REG_FIQ_H1_48_63                0x0027
    // host3 to x
    #define INT_FIQMASK_H2_PM           BIT(14)
    #define INT_FIQMASK_H2_H1           BIT(13)
    #define INT_FIQMASK_H2_AEON         BIT(12)

#define REG_FIQ_H1_32_47                0x0026
    // host0 to x
    #define INT_FIQMASK_PM_H1           BIT(6)
    #define INT_FIQMASK_PM_H2           BIT(5)
    #define INT_FIQMASK_PM_AEON         BIT(4)
    // host2 to x
    #define INT_FIQMASK_AEON_PM         BIT(2)
    #define INT_FIQMASK_AEON_H1         BIT(1)
    #define INT_FIQMASK_AEON_H2         BIT(0)
    // host1 to x
    #define INT_FIQMASK_H1_PM           BIT(10)
    #define INT_FIQMASK_H1_H2           BIT(9)
    #define INT_FIQMASK_H1_AEON         BIT(8)


#define REG_FIQS_H1_32_47               0x002e
#define REG_FIQS_H1_48_63               0x002f


//#############################
//# host0: 51               (bank : 1019h)
//# host1: ARM CA9 Core0    (bank : 1019h)
//# host2: SECURE-R2        (bank : 1019h)
//# host3: ARM CA9 Core1    (bank : 1211h)
//#############################
#define REG_CPU_INT_BASE                (0x2A0<<1)//(0x100540-0x100000)
#define CPU_INT_REG(address)            RIU[address*2+REG_CPU_INT_BASE]
#define REG_INT_PMFIRE                  0x0000 //51
    #define INT_PM_H1                   BIT(0)
    #define INT_PM_H2                   BIT(1)
    #define INT_PM_AEON                 BIT(2)

#define REG_INT_H1FIRE                  0x0002 //R2
    #define INT_H1_PM                   BIT(0)
    #define INT_H1_H2                   BIT(1)
    #define INT_H1_AEON                 BIT(2)

#define REG_INT_H2FIRE                  0x0004 //AMR1
    #define INT_H2_PM                   BIT(0)
    #define INT_H2_H1                   BIT(1)
    #define INT_H2_AEON                 BIT(2)

#define REG_INT_AEONFIRE                0x0006 //AMR0
    #define INT_AEON_PM                 BIT(0)
    #define INT_AEON_H1                 BIT(1)
    #define INT_AEON_H2                 BIT(2)

#if 1//frcr2_integration###
//################
//
// FRC - FIQ
//
// Host0: to nonPM_intr
// Host1: frc_r2
//
#define REG_FRC_FIQ_MASK_BASE           (0x300500UL) //(0x400500-0x100000)
#define FIQ_FRCREG(address)             RIU[address*2+REG_FRC_FIQ_MASK_BASE]
//For Host 0
#define REG_FRCFIQ_H0_0_15              0x0004 //mask
#define REG_FRCFIQS_H0_0_15             0x000C //final status
//For Host 1
#define REG_FRCFIQ_H1_0_15              0x0024 //mask
#define REG_FRCFIQS_H1_0_15             0x002C //final status
    #define INT_FRC_FIQMASK_HST0TOHST1  BIT(0)
    #define INT_FRC_FIQMASK_HST1TOHST0  BIT(3)

    
//################
//
// FRC CPU Interrupt
//
#define REG_FRC_CPUINT_BASE             (0x300640UL) //(0x400640-0x100000)
#define FRC_CPU_INT_REG(address)         RIU[address*2+REG_FRC_CPUINT_BASE]
#define REG_FRCINT_HKCPUFIRE            0x0000 //hst0 to hst1
    #define INT_HKCPU_FRCR2             BIT(0)
#define REG_FRCINT_FRCR2FIRE            0x0002 //hst1 to hst0
    #define INT_FRCR2_HKCPU             BIT(0)
#endif


#endif //_MHAL_MBX_INTERRUPT_REG_H
