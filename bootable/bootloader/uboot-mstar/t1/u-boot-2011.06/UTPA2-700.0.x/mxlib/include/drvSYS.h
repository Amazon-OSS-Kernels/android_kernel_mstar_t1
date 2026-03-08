/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//
//
//
//
//
//
//
//
//
//******************************************************************************
////////////////////////////////////////////////////////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvSYS.h
/// @brief  System Control Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SYS SYS interface
    \ingroup  G_PERIPHERAL

    \brief

    This drvSYS.h provide functions like WDT, FlushMemory, and Chip-Related utility.

    <b>Features</b>

    - WDT control function
    - System Memory control function
    - System Chip-Related function

    <b> Operation Code Flow: </b> \n
    -# Prepare SYS setting for each operation
    -# Set and start SYS in command handle
    -# Trigger SYS

     \defgroup G_SYS_INIT Initialization Task relative
     \ingroup  G_SYS
     \defgroup G_SYS_COMMON Common Task relative
     \ingroup  G_SYS
     \defgroup G_SYS_CONTROL Control relative
     \ingroup  G_SYS
     \defgroup G_SYS_OTHER  other relative
     \ingroup  G_SYS
*/

#ifndef _DRV_SYS_H_
#define _DRV_SYS_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "MsDevice.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

#define SYS_ENCODER_MFE		0x00000001UL
#define SYS_ENCODER_VE		0x00000002UL

#define SYS_FRONTEND_AVD	0x00000001UL
#define SYS_FRONTEND_DEMOD	0x00000002UL
#define SYS_FRONTEND_VBI	0x00000004UL
#define SYS_FRONTEND_VIF	0x00000008UL

#define SYS_SYSTEM_CEC		0x00000001UL
#define SYS_SYSTEM_MBX		0x00000002UL
#define SYS_SYSTEM_SWI2C	0x00000004UL
#define SYS_SYSTEM_BDMA		0x00000008UL
#define SYS_SYSTEM_CPU		0x00000010UL
#define SYS_SYSTEM_GPIO		0x00000020UL
#define SYS_SYSTEM_HWI2C	0x00000040UL
#define SYS_SYSTEM_IR		0x00000080UL
#define SYS_SYSTEM_MIU		0x00000100UL
#define SYS_SYSTEM_MPIF		0x00000200UL
#define SYS_SYSTEM_MSPI		0x00000400UL
#define SYS_SYSTEM_PM		0x00000800UL
#define SYS_SYSTEM_PWM		0x00001000UL
#define SYS_SYSTEM_PWS		0x00002000UL
#define SYS_SYSTEM_RTC		0x00004000UL
#define SYS_SYSTEM_SAR		0x00008000UL
#define SYS_SYSTEM_URDMA	0x00010000UL
#define SYS_SYSTEM_WDT		0x00020000UL

#define SYS_CA_AESDMA		0x00000001UL
#define SYS_CA_CA			0x00000002UL
#define SYS_CA_PCMCIA		0x00000004UL
#define SYS_CA_SC			0x00000008UL

#define SYS_CODEC_GPD		0x00000001UL
#define SYS_CODEC_JPEG		0x00000002UL
#define SYS_CODEC_VDEC		0x00000004UL

#define SYS_DISPLAY_ACE		0x00000001UL
#define SYS_DISPLAY_DAC		0x00000002UL
#define SYS_DISPLAY_DDC2BI	0x00000004UL
#define SYS_DISPLAY_DIP		0x00000008UL
#define SYS_DISPLAY_DLC		0x00000010UL
#define SYS_DISPLAY_GOP		0x00000020UL
#define SYS_DISPLAY_HDMITX	0x00000040UL
#define SYS_DISPLAY_MHL		0x00000080UL
#define SYS_DISPLAY_MVOP	0x00000100UL
#define SYS_DISPLAY_PNL		0x00000200UL
#define SYS_DISPLAY_XC		0x00000400UL

#define SYS_GFX_GFX			0x00000001UL
#define SYS_AUDIO_AUDIO		0x00000001UL
#define SYS_STORAGE_SERFLASH	0x00000001UL
#define SYS_DEMUX_DMX		0x00000001UL

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
/// Version string.
    #define MSIF_SYS_LIB_CODE               {'S','Y','S','_'} //Lib code
    #define MSIF_SYS_LIBVER                 {'0','1'}      //LIB version
    #define MSIF_SYS_BUILDNUM               {'0','9' }    //Build Number
    #define MSIF_SYS_CHANGELIST             {'0','0','6','3','0','6','5','2'} //P4 ChangeList Number

#define SYS_DRV_VERSION                 /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_SYS_LIB_CODE,                  /* IP__                                             */  \
    MSIF_SYS_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_SYS_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_SYS_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

#define SYS_LIB_NAME_MAX            16UL      ///< Maximum length of library name
#define SYS_LIB_TIME_MAX            16UL     ///< Maximum length of time
#define SYS_LIB_DATE_MAX            16UL      ///< Maximum length of date
#define SYS_BOARD_MODEL_MAX         32UL      ///< Maximum length of board name
#define SYS_PLATFORM_CHIP_MAX       32UL      ///< Maximum length of playform name
#define SYS_EFUSE_DID_LENGTH         4UL      ///< efuse device ID length


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

/// System information
typedef struct _SYS_Info
{
    struct
    {
        MS_U16    DeviceId;                           ///< Device ID
        MS_U8     Version;                            ///< Version
        MS_U8     Revision;                           ///< Revision
        MS_PHY  MIU1Base;                           ///< MIU1Base
        MS_PHY  MIU1BusBase;                        ///< MIU1Base
        MS_PHY  MIU2Base;                           ///< MIU2Base
        MS_PHY  MIU2BusBase;                        ///< MIU3Base
        MS_PHY  MIU3Base;                           ///< MIU2Base
        MS_PHY  MIU3BusBase;                        ///< MIU3Base
    } Chip;                                         ///< Chip information

    struct
    {
        MS_U8   Time[SYS_LIB_TIME_MAX];             ///< Library Built Time
        MS_U8   Date[SYS_LIB_DATE_MAX];             ///< Library Built Date
    } SWLib;                                        ///< Software Library Information

} SYS_Info;


typedef struct _SYS_Status
{
    MS_BOOL     WDT_Active;                         ///< Watchdog Timer

} SYS_Status;

typedef enum _SYS_DbgLv
{
    E_SYS_DBGLV_NONE           //no debug message
    ,E_SYS_DBGLV_ERR_ONLY      //show error only
    ,E_SYS_DBGLV_REG_DUMP      //show error & reg dump
    ,E_SYS_DBGLV_INFO          //show error & informaiton
    ,E_SYS_DBGLV_ALL           //show error, information & funciton name
}SYS_DbgLv;

typedef enum
{
    E_SYS_QUERY_H264_SUPPORTED = 0x00,
    E_SYS_QUERY_DVBC_SUPPORTED = 0x10,
    E_SYS_QUERY_MM_SUPPORTED = 0x11,
    E_SYS_QUERY_TTXNICAM_SUPPORTED = 0x12,
    E_SYS_QUERY_TCON_SUPPORTED = 0x13,
    E_SYS_QUERY_DTV_SUPPORTED = 0x14,
    E_SYS_QUERY_DTVCC_SUPPORTED = 0x15,
    E_SYS_QUERY_GINGA_NCL_SUPPORTED = 0x16,
    E_SYS_QUERY_GINGA_JAVA_SUPPORTED = 0x17,
    E_SYS_QUERT_HD_SUPPORTED = 0x18,
    E_SYS_QUERY_MHL_SUPPORTED = 0x19,
    E_SYS_QUERY_DVBT_SUPPORTED = 0x1A,
    E_SYS_QUERY_ISDBT_SUPPORTED = 0x1B,
    E_SYS_QUERY_HBBTV_SUPPORTED = 0x1C,
    E_SYS_QUERY_TTS_SUPPORTED = 0x1D,
    E_SYS_QUERY_DOLBY_HDR_SUPPORTED = 0x1E,
    E_SYS_QUERY_SECURED_IC_SUPPORTED = 0x200,
} E_SYS_QUERY;

typedef enum
{
    E_SYS_ATV_AGC_PAD_SET,
    E_SYS_DTV_AGC_PAD_SET,
    E_SYS_DTV_AGC_PAD_SET_ALL_OFF,
}SYS_AGC_PAD_SET;

typedef enum
{
    E_TS0_PAD_SET,
    E_TS1_PAD_SET,
    E_TS2_PAD_SET,
    E_TS3_PAD_SET,
    E_CA_CI_PAD_SET,
    E_SC_PAD_SET,
    E_MSPI_PAD_SET,
    E_TS4_PAD_SET,
    E_TS5_PAD_SET,
    E_TSO_PAD_SET,
    E_PAD_SET_MAX
}SYS_PAD_MUX_SET;

typedef enum
{
    E_PARALLEL_IN,
    E_SERIAL_IN,
    E_PARALLEL_IN_2nd_PORT,
    E_PARALLEL_OUT,
    E_CA_CI_PAD_CA,
    E_CA_CI_PAD_CI,
    E_CA_CI_PAD_NONE,
    E_TRI_STATE_ON,
    E_TRI_STATE_OFF,
    E_SC_PAD_PCM,
    E_SC_PAD_TS1,
    E_MSPI_PAD_ON,
    E_MSPI_PAD_GPIO,
    E_SERIAL_IN_3_WIRE,
    E_PARALLEL_OUT_SER2PAR,
    E_PARALLEL_OUT_INTDMD,
    E_PARALLEL_OUT_TSO,
    E_PARALLEL_OUT_SER2PAR1,
    E_MSPI_PAD_ON_2nd_PORT,
    E_PAD_DRIVING_4MA = 0x00001000,
    E_PAD_DRIVING_8MA = 0x00001001,
    E_PAD_SEL_MAX
}SYS_PAD_SEL;

typedef enum
{
    E_PCM_CD_SINGLE,
    E_PCM_CD_OR,
    E_PCM_CD_MAX
}SYS_PCM_CD_MODE;

typedef enum
{
    E_SYS_DOLBY_VERSION = 0x00,
    E_SYS_DOLBY_CONTROL_BIT = 0x01,
    E_SYS_DOLBY_REVERSE_BIT = 0x02,

} E_SYS_DOLBY_HASH_INFO;

typedef enum
{
    E_SYS_CHIP_TV,
    E_SYS_CHIP_STB,
    E_SYS_CHIP_MAX,
} E_SYS_CHIP_TYPE;

typedef enum
{
	E_SYS_IP_ENCODER,
	E_SYS_IP_FRONTEND,
	E_SYS_IP_DEMUX,
	E_SYS_IP_SYSTEM,
	E_SYS_IP_CA,
	E_SYS_IP_CODEC,
	E_SYS_IP_DISPLAY,
	E_SYS_IP_AUDIO,
	E_SYS_IP_GFX,
	E_SYS_IP_STORAGE,
} E_SYS_IP_TYPE;

typedef enum
{
	E_SYS_IO_SWI2C_READ     = 0x00,
	E_SYS_IO_SWI2C_WRITE    = 0x01,
	E_SYS_IO_MAX            = 0xFF,
} E_SYS_IO_PROC;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_Init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_GlobalInit(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
const SYS_Info* MDrv_SYS_GetInfo(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8           MDrv_SYS_GetChipRev(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16          MDrv_SYS_GetChipID(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WDTEnable(MS_BOOL bEnable);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WDTClear(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_WDTLastStatus(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WDTSetTime(MS_U32 u32Ms);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_ResetChip(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_ResetCPU(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_SetDbgLevel(SYS_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_FlushMemory(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_ReadMemory(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_VIFWriteByteByVDMbox(MS_U32 u32Reg, MS_U8 u8Val);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_VIFWriteByteMaskByVDMbox(MS_U32 u32Reg, MS_U8 u8Val, MS_U8 u8Mask);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_VIFWriteRegBitByVDMbox(MS_U32 u32Reg, MS_U8 bEnable, MS_U8 u8Mask);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U8           MDrv_SYS_VIFReadByteByVDMbox(MS_U32 u32Reg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16          MDrv_SYS_VIFRead2ByteByVDMbox(MS_U32 u32Reg);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_Query(E_SYS_QUERY id);

#ifdef _SYS_DAC_GAIN_CTRL_
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_EnhanceDACGain(MS_U8 u8DAC_ID, MS_BOOL bEnable);
#endif
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_GetSoftwareVersion(MS_SW_VERSION_INFO *pSoftwareVersionInfo);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_SetAGCPadMux(SYS_AGC_PAD_SET eAgcPadMux);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetPadMux(SYS_PAD_MUX_SET ePadMuxType,SYS_PAD_SEL ePadSel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetPCMCardDetectMode(SYS_PCM_CD_MODE ePCMCDMode);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_DisableDebugPort(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_EnableDebugPort(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetTSOutClockPhase(MS_U16 u16Val);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_SetTSClockPhase(SYS_PAD_MUX_SET ePadMuxType, MS_U16 u16Val);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32          MDrv_SYS_SetPowerState(EN_POWER_MODE u16PowerState);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32          MDrv_SYS_QueryDolbyHashInfo(E_SYS_DOLBY_HASH_INFO index);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
E_SYS_CHIP_TYPE MDrv_SYS_GetChipType(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_SetChipType(E_SYS_CHIP_TYPE Type);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void MDrv_SYS_GetDolbyKeyCustomer(MS_U8 * u8pkey);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U16          MDrv_SYS_ReadBrickTerminatorStatus(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SYS_WriteBrickTerminatorStatus(MS_U16 u16Status);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_GetEfuseDid(MS_U16 *u16efuse_did);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_OTHER
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SYS_ReadEfuseHDCPKey(MS_U16 u16ReadAddr, MS_U32 *u32HDCPKey);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SYS_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32          MDrv_SYS_GetIpList(E_SYS_IP_TYPE TYPE);

//FOR INTERNAL USE ONLY
MS_U32          MDrv_SYS_RegisterIoProc(E_SYS_IO_PROC dwIoType, void *pdwIoFunction);

char* MDrv_SYS_GetUtopiaReleaseLabel(void);

#ifdef __cplusplus
}
#endif


#endif // _DRV_SYS_H_

