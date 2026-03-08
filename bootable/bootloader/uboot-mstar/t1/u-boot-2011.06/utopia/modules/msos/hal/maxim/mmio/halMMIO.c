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
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    halMMIO.c
/// @brief  memory map io (MMIO) HAL
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#include "MsCommon.h"
#include "halMMIO.h"
#include "drvMMIO.h"

// for getting mapped IO base from DRV
extern MS_VIRT   _virtPM_Bank         ;
extern MS_U32   _u32PM_Bank_SIZE    ;
extern MS_VIRT   _virtNonPM_Bank      ;
extern MS_U32   _u32NonPM_Bank_SIZE ;
extern MS_VIRT   _virtFRC_Bank      ; //frcr2_integration###
extern MS_U32   _u32FRC_Bank_SIZE ; //frcr2_integration###

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//  Global Variables
//--------------------------------------------------------------------------------------------------
MS_VIRT virt_ge0_mmio_base;


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MSOS_TYPE_LINUX
    // assume linux always running on mips
    #define HAL_MMIO_PM_BASE            0x1f000000UL
    #define HAL_MMIO_PM_SIZE            0x00A00000UL
    #define HAL_MMIO_NONPM_BASE         0x1f200000UL
    #define HAL_MMIO_NONPM_SIZE         0x01000000UL
    #define HAL_MMIO_FLASH_BASE0        0x14000000UL
    #define HAL_MMIO_FLASH_SIZE0        0x01000000UL
    #define HAL_MMIO_FRC_BASE           0x1f800000UL //frcr2_integration###
    #define HAL_MMIO_FRC_SIZE           0x00013600UL //frcr2_integration###
#elif defined(MSOS_TYPE_LINUX_KERNEL)
    #ifdef CONFIG_UTOPIA_FRAMEWORK_KERNEL_DRIVER_64BIT
	    extern ptrdiff_t mstar_pm_base;
	    #define RIU_BASE    mstar_pm_base
	#else
		#define RIU_BASE     0xfd000000UL
	#endif
    #define HAL_MMIO_PM_BASE            RIU_BASE
    #define HAL_MMIO_PM_SIZE            0x00A00000UL
    #define HAL_MMIO_NONPM_BASE         RIU_BASE+0x200000UL
    #define HAL_MMIO_NONPM_SIZE         0x01000000UL
    #define HAL_MMIO_FLASH_BASE0        0x14000000UL
    #define HAL_MMIO_FLASH_SIZE0        0x01000000UL
    #define HAL_MMIO_FRC_BASE           RIU_BASE+0x800000UL //0x1f800000UL //frcr2_integration###
    #define HAL_MMIO_FRC_SIZE           0x00013600UL //frcr2_integration###
#else
    #if defined (MCU_AEON)
        #define HAL_MMIO_PM_BASE        0xFA000000UL
        #define HAL_MMIO_PM_SIZE        0x00007B80UL
        #define HAL_MMIO_NONPM_BASE     0xFA200000UL
        #define HAL_MMIO_NONPM_SIZE     0x00025600UL
        #define HAL_MMIO_FLASH_BASE0    0xA1000000UL // non-cache // 0xA1000000 for cache
        #define HAL_MMIO_FLASH_SIZE0    0x1000000UL
        #define HAL_MMIO_FRC_BASE       0xFA800000UL //frcr2_integration###
        #define HAL_MMIO_FRC_SIZE       0x00013600UL //frcr2_integration###
    #elif defined (MCU_ARM_CA7)
        #define HAL_MMIO_PM_BASE        0x1f000000UL
        #define HAL_MMIO_PM_SIZE        0x00007B80UL
        #define HAL_MMIO_NONPM_BASE     0x1f200000UL
        #define HAL_MMIO_NONPM_SIZE     0x00025600UL
        #define HAL_MMIO_FLASH_BASE0    0x14000000UL
        #define HAL_MMIO_FLASH_SIZE0    0x1000000UL
        #define HAL_MMIO_FRC_BASE       0x1f800000UL //frcr2_integration###
        #define HAL_MMIO_FRC_SIZE       0x00013600UL //frcr2_integration###
    #elif defined (MCU_ARM_CA53)
        #define HAL_MMIO_PM_BASE        0x1f000000UL
        #define HAL_MMIO_PM_SIZE        0x00007B80UL
        #define HAL_MMIO_NONPM_BASE     0x1f200000UL
        #define HAL_MMIO_NONPM_SIZE     0x00025600UL
        #define HAL_MMIO_FLASH_BASE0    0x14000000UL
        #define HAL_MMIO_FLASH_SIZE0    0x1000000UL
        #define HAL_MMIO_FRC_BASE       0x1f800000UL //frcr2_integration###
        #define HAL_MMIO_FRC_SIZE       0x00013600UL //frcr2_integration###
    #else
        #error "Please choose MCU";
    #endif
#endif


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_U16  HAL_MMIO_GetType(MS_U32 u32Module)
{
    switch (u32Module)
    {
    //HAL_MMIO_PM_BANK
    case MS_MODULE_PM       :
    case MS_MODULE_IR       :
    case MS_MODULE_ISP      :
    case MS_MODULE_PWS      :
        return DRV_MMIO_PM_BANK;

    //HAL_MMIO_NONPM_BANK
    case MS_MODULE_HW       :
    case MS_MODULE_CHIPTOP  :
    case MS_MODULE_MIU      :
    case MS_MODULE_ACE      :
    case MS_MODULE_AUDIO    :
    case MS_MODULE_AVD      :
    case MS_MODULE_BDMA     :
    case MS_MODULE_DLC      :
    case MS_MODULE_DMD      :
    case MS_MODULE_GE       :
    case MS_MODULE_GOP      :
    case MS_MODULE_GPIO     :
    case MS_MODULE_HVD      :
    case MS_MODULE_HWI2C    :
    case MS_MODULE_IRQ      :
    case MS_MODULE_JPD      :
    case MS_MODULE_MBX      :
    case MS_MODULE_MFE      :
    case MS_MODULE_MHEG5    :
    case MS_MODULE_MVD      :
    case MS_MODULE_MVOP     :
    case MS_MODULE_RVD      :
    case MS_MODULE_TSP      :
    case MS_MODULE_UART     :
    case MS_MODULE_VPU      :
    case MS_MODULE_XC       :
    case MS_MODULE_PCMCIA   :
    case MS_MODULE_PFSH     :
    case MS_MODULE_PNL      :
    case MS_MODULE_PWM      :
    case MS_MODULE_SEM      :
    case MS_MODULE_VBI      :
    case MS_MODULE_VIF      :
    case MS_MODULE_DIP      :
	case MS_MODULE_MPIF     :
	case MS_MODULE_MMFILEIN :
    case MS_MODULE_GPD      :
    case MS_MODULE_TSO      :
    case MS_MODULE_CMDQ     :
        return DRV_MMIO_NONPM_BANK;

    case MS_MODULE_SC       :
        return DRV_MMIO_SC_BANK;

     //HAL_MMIO_FLASH_BANK0
    case MS_MODULE_FLASH    :
        return DRV_MMIO_FLASH_BANK0;
    case MS_MODULE_FRC      : //frcr2_integration###
        return DRV_MMIO_FRC_BANK; //frcr2_integration###

    default:
        return 0xFFFF; //undefine type
    }

    return 0xFFFF; //undefine type
}


MS_BOOL HAL_MMIO_GetBase(MS_PHY* virtBaseAddr, MS_PHY* pu32BaseSize, MS_U32 u32BankType)
{
    MS_BOOL bRet = TRUE;

    *virtBaseAddr = 0;
    *pu32BaseSize = 0;
    switch (u32BankType)
    {
    case DRV_MMIO_PM_BANK:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_PM_BASE;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_PM_SIZE;
        break;
    case DRV_MMIO_NONPM_BANK:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_NONPM_BASE;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_NONPM_SIZE;
        break;
    case DRV_MMIO_FLASH_BANK0:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_FLASH_BASE0;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_FLASH_SIZE0;
        break;
    case DRV_MMIO_FRC_BANK:
        *virtBaseAddr = (MS_PHY)HAL_MMIO_FRC_BASE;
        *pu32BaseSize = (MS_PHY)HAL_MMIO_FRC_SIZE;
        break;
    default:
//        MS_ASSERT(0);
        bRet = FALSE;
        break;
    }

    return bRet;
}


// @NOTE: Only run after MMIO_Init
MS_BOOL HAL_MMIO_GetIPBase(MS_VIRT *virtBaseAddr, MS_U16 u16BankType)
{
    *virtBaseAddr = 0;

    // if MMIO_Init is not yet initialized.
    if ( (_u32PM_Bank_SIZE == 0x0) || (_u32NonPM_Bank_SIZE == 0x0) || (_u32FRC_Bank_SIZE == 0x0) ) //frcr2_integration###
    {
        return FALSE;
    }

    switch (u16BankType)
    {
    case DRV_MMIO_SC_BANK:
        *virtBaseAddr =_virtNonPM_Bank  + 0x00005200; // 0xBF220C00, 0xBF220C80
        break;
    default:
        return FALSE;
    }

    return TRUE;
}

#if defined(__aarch64__) || defined(__arm__)
#ifndef MSOS_TYPE_LINUX
static void _chip_flush_miu_pipe(void)
{
    unsigned int    dwReadData = 0;


     //toggle the flush miu pipe fire bit
    *(volatile unsigned int *)(HAL_MMIO_PM_BASE + (0x10188A << 1)) &= ~(0x0001);
    *(volatile unsigned int *)(HAL_MMIO_PM_BASE + (0x10188A << 1)) |= 0x0001;

    do
    {
        dwReadData = *(volatile unsigned int *)(HAL_MMIO_PM_BASE+ (0x1018A0 << 1));
        dwReadData &= BIT(12);  //Check Status of Flush Pipe Finish

    } while(dwReadData == 0);
}
#endif
#endif

void HAL_MMIO_FlushMemory(void)
{
#if defined(__aarch64__) || defined(__arm__)
#ifndef MSOS_TYPE_LINUX
    _chip_flush_miu_pipe();
#endif
#endif
}

void HAL_MMIO_ReadMemory(void)
{
#if defined(__aarch64__) || defined(__arm__)
#ifndef MSOS_TYPE_LINUX
    _chip_flush_miu_pipe();
#endif
#endif
}


