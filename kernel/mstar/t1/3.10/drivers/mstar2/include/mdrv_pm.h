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
/// @file   Mdrv_mtlb.h
/// @brief  MTLB Driver Interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// Linux Mhal_mtlb.h define start
// -----------------------------------------------------------------------------
#ifndef __DRV_PM_H__
#define __DRV_PM_H__

//-------------------------------------------------------------------------------------------------
//  Structure and Enum
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_PM_FAIL       = 0,      // fail
    E_PM_OK         = 1,      // success
    E_PM_TIMEOUT    = 2,      // timeout

} PM_Result;

typedef struct _MSTAR_PM_DEV{
    unsigned long flag;
}MSTAR_PM_DEV;

#define CRC_KERNEL_BUF   (3)
#define MAX_BUF_WAKE_IR 32
#define MAX_BUF_WAKE_IR2 16
#define MAX_BUF_WAKE_MAC_ADDRESS 6
#define PM_POWERDOWN_CFG_OFFSET 0xA0

/// Define PM wake-up parameter
typedef struct
{
    /// For PM IR Wake-up
    u8 bPmWakeEnableIR         : 1;
    /// For PM SAR Wake-up
    u8 bPmWakeEnableSAR        : 1;
    /// For PM GPIO0 Wake-up
    u8 bPmWakeEnableGPIO0      : 1;
    /// For PM GPIO1 Wake-up
    u8 bPmWakeEnableGPIO1      : 1;
    /// For PM UART1 Wake-up
    u8 bPmWakeEnableUART1      : 1;
    /// For PM SYNC Wake-up
    u8 bPmWakeEnableSYNC       : 1;
    /// For PM EasySYNC Wake-up
    u8 bPmWakeEnableESYNC      : 1;

    /// For PM RTC0 Wake-up
    u8 bPmWakeEnableRTC0       : 1;
    /// For PM RTC1 Wake-up
    u8 bPmWakeEnableRTC1       : 1;
    /// For PM DVI0 Wake-up
    u8 bPmWakeEnableDVI0       : 1;
    /// For PM DVI1 Wake-up
    u8 bPmWakeEnableDVI2       : 1;
    /// For PM CEC Wake-up
    u8 bPmWakeEnableCEC        : 1;
    /// For PM AVLINK Wake-up
    u8 bPmWakeEnableAVLINK     : 1;
    /// For PM WOL Wake-up
    u8 bPmWakeEnableWOL        : 1;

    ///For PM IR Wake-up key define
    u8 u8PmWakeIR[MAX_BUF_WAKE_IR];
    ///For PM IR Wake-up key 2 define
    u8 u8PmWakeIR2[MAX_BUF_WAKE_IR2];
    ///For PM WOL Wake-up MAC Addr define
    u8 u8PmWakeMACAddress[MAX_BUF_WAKE_MAC_ADDRESS];

    ///For STR Power Mode, defined in EN_PM_STR_MODE
    u8 u8PmStrMode;
    ///For STR CRC check, TRUE for MIU1, FALSE for MIU0
    u8 bLxCRCMiu[CRC_KERNEL_BUF];
    ///For STR CRC check, memory address
    u32 u32LxCRCAddress[CRC_KERNEL_BUF];
    ///For STR CRC check, memory size
    u32 u32LxCRCSize[CRC_KERNEL_BUF];

}PM_WakeCfg_t ;

/// Define PM mode
typedef struct
{
    /// Power Down Mode
    u8 u8PowerDownMode;
    u8 u8WakeAddress;

} PM_PowerDownCfg_t ;

/// Define PM configuration
typedef struct
{

    PM_WakeCfg_t stPMWakeCfg;
    PM_PowerDownCfg_t stPMPowerDownCfg;

} PM_Cfg_t;

#define DRAM_MMAP_SZ                (0x00010000UL)      /* 64KB */

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

PM_Result MDrv_PM_CopyBin2Sram();
PM_Result MDrv_PM_SetSRAMOffsetForMCU(void);
void MDrv_SetDram(u32 u32Addr, u32 u32Size);
void MDrv_SetData(u32 u32Addr, u32 u32Size);
PM_Result MDrv_PM_CopyBin2TempBuffer(void);
PM_Result MDrv_PM_CopyBin2PhysicalDram(void);
void MDrv_PM_RunTimePM_Disable_PassWord(void);
// Luke add for Maxim API change.
PM_Result MDrv_PM_GetCfg(PM_Cfg_t* pstPMCfg);
PM_Result MDrv_PM_SetCfg(PM_Cfg_t* pstPMCfg);

#endif

