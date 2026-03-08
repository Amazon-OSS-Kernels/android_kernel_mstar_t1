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
/// @file   drvDMD_DVBT_INTERN.h
/// @brief  DVBT Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_DVBT2_V2_H_
#define _DRV_DVBT2_V2_H_

//#include "MsCommon.h"
//#include "drvDMD_common.h"
#include "drvDMD_INTERN_DVBT2.h"
#ifdef __cplusplus
extern "C"
{
#endif
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
typedef enum {
    DMD_DVBT2_DRV_CMD_Init                        ,
    DMD_DVBT2_DRV_CMD_Exit                        ,
    DMD_DVBT2_DRV_CMD_SetDbgLevel                 ,
    DMD_DVBT2_DRV_CMD_GetInfo                     ,
    DMD_DVBT2_DRV_CMD_GetLibVer                   ,
    DMD_DVBT2_DRV_CMD_GetFWVer                    ,
    DMD_DVBT2_DRV_CMD_GetReg                      ,
    DMD_DVBT2_DRV_CMD_SetReg                      ,
    DMD_DVBT2_DRV_CMD_SetSerialControl            ,
    DMD_DVBT2_DRV_CMD_SetReset                    ,
    DMD_DVBT2_DRV_CMD_SetConfig                   ,
    DMD_DVBT2_DRV_CMD_SetActive                   ,
    DMD_DVBT2_DRV_CMD_GetLock                     ,
//    DMD_DVBT2_DRV_CMD_GetSignalStrengthWithRFPower,
//    DMD_DVBT2_DRV_CMD_GetSignalQualityWithRFPower ,
    DMD_DVBT2_DRV_CMD_GetSNR                      ,
    DMD_DVBT2_DRV_CMD_GetPostLdpcBer              ,
    DMD_DVBT2_DRV_CMD_GetPreLdpcBer               ,
    DMD_DVBT2_DRV_CMD_GetPacketErr                ,
    DMD_DVBT2_DRV_CMD_GetL1Info                   ,
    DMD_DVBT2_DRV_CMD_GetFreqOffset               ,
    DMD_DVBT2_DRV_CMD_NORDIG_SSI_Table_Write      ,
    DMD_DVBT2_DRV_CMD_NORDIG_SSI_Table_Read       ,
    DMD_DVBT2_DRV_CMD_SetPowerState               ,
    DMD_DVBT2_DRV_CMD_GetPlpBitMap                ,
    DMD_DVBT2_DRV_CMD_GetPlpGroupID								,
    DMD_DVBT2_DRV_CMD_SetPlpID                    ,

} DMD_DVBT2_DRV_COMMAND;
/// For demod init


typedef struct DLL_PACKED _DMD_DVBT2_InitData_Transform
{
    // tuner parameter
    MS_U8 u8SarChannel;
    /*
    DMD_RFAGC_SSI *pTuner_RfagcSsi;
    MS_U16 u16Tuner_RfagcSsi_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_LoRef;
    MS_U16 u16Tuner_IfagcSsi_LoRef_Size;
    DMD_IFAGC_SSI *pTuner_IfagcSsi_HiRef;
    MS_U16 u16Tuner_IfagcSsi_HiRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_LoRef;
    MS_U16 u16Tuner_IfagcErr_LoRef_Size;
    DMD_IFAGC_ERR *pTuner_IfagcErr_HiRef;
    MS_U16 u16Tuner_IfagcErr_HiRef_Size;
    DMD_SQI_CN_NORDIGP1 *pSqiCnNordigP1;
    MS_U16 u16SqiCnNordigP1_Size;
    */

    // register init
    MS_U8 *u8DMD_DVBT2_DSPRegInitExt; // TODO use system variable type
    MS_U8 u8DMD_DVBT2_DSPRegInitSize;
    MS_U8 *u8DMD_DVBT2_InitExt; // TODO use system variable type

    MS_U32  u32EqStartAddr;
    MS_U32  u32TdiStartAddr;
    MS_U32  u32DjbStartAddr;
    MS_U32  u32FwStartAddr;
} DMD_DVBT2_InitData_Transform;


typedef struct DLL_PACKED _DVBT2_INIT_PARAM
{
	DMD_DVBT2_InitData_Transform *pDMD_DVBT2_InitData;
	MS_U32 u32InitDataLen;
	MS_BOOL ret;
}DVBT2_INIT_PARAM, *PDVBT2_INIT_PARAM;


typedef struct DLL_PACKED _DVBT2_EXIT_PARAM
{
	MS_BOOL ret;
}DVBT2_EXIT_PARAM, *PDVBT2_EXIT_PARAM;


typedef struct DLL_PACKED _DVBT2_SETDBGLEVEL_PARAM
{
	DMD_T2_DbgLv u8DbgLevel;
	MS_BOOL ret;
}DVBT2_SETDBGLEVEL_PARAM, *PDVBT2_SETDBGLEVEL_PARAM;

typedef struct DLL_PACKED _DVBT2_GETINFO_PARAM
{
	DMD_DVBT2_INFO_TYPE eInfoType;
	DMD_DVBT2_Info* pInfo;
	MS_BOOL ret;
}DVBT2_GETINFO_PARAM, *PDVBT2_GETINFO_PARAM;


typedef struct DLL_PACKED _DVBT2_GETLIBVER_PARAM
{
	const MSIF_Version **ppVersion;
	MS_BOOL ret;
}DVBT2_GETLIBVER_PARAM, *PDVBT2_GETLIBVER_PARAM;


typedef struct DLL_PACKED _DVBT2_GETFWVER_PARAM
{
	MS_U16 *ver;
	MS_BOOL ret;
}DVBT2_GETFWVER_PARAM, *PDVBT2_GETFWVER_PARAM;

typedef struct DLL_PACKED _DVBT2_GETREG_PARAM
{
	MS_U16 u16Addr;
	MS_U8 *pu8Data;
	MS_BOOL ret;
}DVBT2_GETREG_PARAM, *PDVBT2_GETREG_PARAM;

typedef struct DLL_PACKED _DVBT2_SETREG_PARAM
{
	MS_U16 u16Addr;
	MS_U8 u8Data;
	MS_BOOL ret;
}DVBT2_SETREG_PARAM, *PDVBT2_SETREG_PARAM;

typedef struct DLL_PACKED _DVBT2_SETSERIALCONTROL_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBT2_SETSERIALCONTROL_PARAM,*PDVBT2_SETSERIALCONTROL_PARAM;

typedef struct DLL_PACKED _DVBT2_SETCONFIG_PARAM
{
	DMD_DVBT2_RF_CHANNEL_BANDWIDTH BW;
	MS_BOOL bSerialTS;
       MS_U8 u8PlpID;
	MS_BOOL ret;
}DVBT2_SETCONFIG_PARAM,*PDVBT2_SETCONFIG_PARAM;

typedef struct DLL_PACKED _DVBT2_SETACTIVE_PARAM
{
	MS_BOOL bEnable;
	MS_BOOL ret;
}DVBT2_SETACTIVE_PARAM,*PDVBT2_SETACTIVE_PARAM;

typedef struct DLL_PACKED _DVBT2_GETLOCK_PARAM
{
	DMD_DVBT2_GETLOCK_TYPE eType;
	DMD_T2_LOCK_STATUS *eLockStatus;
	MS_BOOL ret;
}DVBT2_GETLOCK_PARAM, *PDVBT2_GETLOCK_PARAM;

#if 0
typedef struct DLL_PACKED _DVBT2_GETSIGNALSTRENGTHWITHRFPOWER_PARAM
{
	MS_U16 *u16Strength;
	float fRFPowerDbm	;
	MS_BOOL ret;
}DVBT2_GETSIGNALSTRENGTHWITHRFPOWER_PARAM,*PDVBT2_GETSIGNALSTRENGTHWITHRFPOWER_PARAM;

typedef struct DLL_PACKED _DVBT2_GETSIGNALQUALITYWITHRFPOWER_PARAM
{
	MS_U16 *u16Quality;
	float fRFPowerDbm;
	MS_BOOL ret;
}DVBT2_GETSIGNALQUALITYWITHRFPOWER_PARAM,*PDVBT2_GETSIGNALQUALITYWITHRFPOWER_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_GETSNR_PARAM
{
	float *fSNR;
	MS_BOOL ret;
}DVBT2_GETSNR_PARAM,*PDVBT2_GETSNR_PARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETSNR_PARAM
{
    MS_U16 *u16_snr100;
    MS_U8 *snr_cali;
    MS_U8 *u8_gi;
    MS_BOOL ret;
}DVBT2_GETSNR_PARAM,*PDVBT2_GETSNR_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_GETPOSTLDPCBER_PARAM
{
	float *ber;
	MS_BOOL ret;
}DVBT2_GETPOSTLDPCBER_PARAM,*PDVBT2_GETPOSTLDPCBER_PARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETPOSTLDPCBER_PARAM
{
    MS_U32 *BitErr_reg;
    MS_U16 *BitErrPeriod_reg;
    MS_U16 *FecType;
    MS_BOOL ret;
}DVBT2_GETPOSTLDPCBER_PARAM,*PDVBT2_GETPOSTLDPCBER_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_GETPRELDPCBERPARAM
{
	float *ber;
	MS_BOOL ret;
}DVBT2_GETPRELDPCBERPARAM,*PDVBT2_GETPRELDPCBERPARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETPRELDPCBERPARAM
{
    MS_U32 *BitErr_reg;
    MS_U16 *BitErrPeriod_reg;
    MS_U16 *FecType;
    MS_BOOL ret;
}DVBT2_GETPRELDPCBERPARAM,*PDVBT2_GETPRELDPCBERPARAM;
#endif

typedef struct DLL_PACKED _DVBT2_GETPACKETERRPARAM
{
	MS_U16 *pktErr;
	MS_BOOL ret;
}DVBT2_GETPACKETERRPARAM,*PDVBT2_GETPACKETERRPARAM;


typedef struct DLL_PACKED _DVBT2_GETL1INFO_PARAM
{
	MS_U16 *u16Info;
	DMD_DVBT2_SIGNAL_INFO eSignalType;
	MS_BOOL ret;
}DVBT2_GETL1INFO_PARAM,*PDVBT2_GETL1INFO_PARAM;

#if 0
typedef struct DLL_PACKED _DVBT2_GETFREQOFFSET_PARAM
{
	float *pFreqOff;
	MS_BOOL ret;
}DVBT2_GETFREQOFFSET_PARAM,*PDVBT2_GETFREQOFFSET_PARAM;
#else
typedef struct DLL_PACKED _DVBT2_GETFREQOFFSET_PARAM
{
    MS_U32 *CfoTd_reg;
    MS_U32 *CfoFd_reg;
    MS_U32 *Icfo_reg;
    MS_U8 *fft_reg;
    MS_BOOL ret;
}DVBT2_GETFREQOFFSET_PARAM,*PDVBT2_GETFREQOFFSET_PARAM;
#endif

#if 0
typedef struct DLL_PACKED _DVBT2_NORDIG_SSI_TABLE_WRITE_PARAM
{
	DMD_T2_CONSTEL constel;
	DMD_T2_CODERATE code_rate;
	float write_value;
	MS_BOOL ret;
}DVBT2_NORDIG_SSI_TABLE_WRITE_PARAM,*PDVBT2_NORDIG_SSI_TABLE_WRITE_PARAM;

typedef struct DLL_PACKED _DVBT2_NORDIG_SSI_TABLE_READ
{
	DMD_T2_CONSTEL constel;
	DMD_T2_CODERATE code_rate;
	float *read_value;
	MS_BOOL ret;
}DVBT2_NORDIG_SSI_TABLE_READ_PARAM,*PDVBT2_NORDIG_SSI_TABLE_READ;
#endif

typedef struct DLL_PACKED _DVBT2_SETPOWERSTATE_PARAM
{
	EN_POWER_MODE u16PowerState;
	MS_U32 ret;
}DVBT2_SETPOWERSTATE_PARAM,*PDVBT2_SETPOWERSTATE_PARAM;

typedef struct DLL_PACKED _DVBT2_GETPLPBITMAP_PARAM
{
	MS_U8* u8PlpBitMap;
	MS_BOOL ret;
}DVBT2_GETPLPBITMAP_PARAM,*PDVBT2_GETPLPBITMAP_PARAM;

typedef struct DLL_PACKED _DVBT2_GETPLPGROUPID_PARAM
{
	MS_U8 u8PlpID;
	MS_U8* u8GroupID;
	MS_BOOL ret;
}DVBT2_GETPLPGROUPID_PARAM,*PDVBT2_GETPLPGROUPID_PARAM;

typedef struct DLL_PACKED _DMD_DVBT2_SETPLPID_PARAM
{
	MS_U8 u8PlpID;
	MS_U8 u8GroupID;
	MS_BOOL ret;
}DMD_DVBT2_SETPLPID_PARAM,*PDMD_DVBT2_SETPLPID_PARAM;










/*************************************************/


MS_U32 DVBT2Open(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 DVBT2Close(void* pInstantTmp);
MS_U32 DVBT2Ioctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);



#ifdef __cplusplus
}
#endif

#endif // _DRV_DVBT2_H_