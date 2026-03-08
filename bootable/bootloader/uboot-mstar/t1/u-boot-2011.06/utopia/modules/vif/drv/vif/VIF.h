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


#ifndef _VIF_H_
#define _VIF_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"
#include "MsTypes.h"

#include "drvVIF_v2.h"

//****************************************************************************
// Public attributes.
//****************************************************************************

#ifdef _VIF_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif

typedef struct
{
    BYTE Vif_Clamp_L;
    BYTE Vif_Clamp_H;
    BYTE Vif_Clamp_Gain_L;
    BYTE Vif_Clamp_Gain_H;

    BYTE Vif_Cr_Rate_1;
    BYTE Vif_Cr_Rate_2;
    BYTE Vif_Cr_Rate_3;
    BYTE Vif_Cr_Rate_Inv;

    BYTE Vif_N_A1_C0_L;
    BYTE Vif_N_A1_C0_H;
    BYTE Vif_N_A1_C1_L;
    BYTE Vif_N_A1_C1_H;
    BYTE Vif_N_A1_C2_L;
    BYTE Vif_N_A1_C2_H;
    BYTE Vif_N_A2_C0_L;
    BYTE Vif_N_A2_C0_H;
    BYTE Vif_N_A2_C1_L;
    BYTE Vif_N_A2_C1_H;
    BYTE Vif_N_A2_C2_L;
    BYTE Vif_N_A2_C2_H;

    BYTE Vif_SOS_11_C0_L;
    BYTE Vif_SOS_11_C0_H;
    BYTE Vif_SOS_11_C1_L;
    BYTE Vif_SOS_11_C1_H;
    BYTE Vif_SOS_11_C2_L;
    BYTE Vif_SOS_11_C2_H;
    BYTE Vif_SOS_11_C3_L;
    BYTE Vif_SOS_11_C3_H;
    BYTE Vif_SOS_11_C4_L;
    BYTE Vif_SOS_11_C4_H;
    BYTE Vif_SOS_12_C0_L;
    BYTE Vif_SOS_12_C0_H;
    BYTE Vif_SOS_12_C1_L;
    BYTE Vif_SOS_12_C1_H;
    BYTE Vif_SOS_12_C2_L;
    BYTE Vif_SOS_12_C2_H;
    BYTE Vif_SOS_12_C3_L;
    BYTE Vif_SOS_12_C3_H;
    BYTE Vif_SOS_12_C4_L;
    BYTE Vif_SOS_12_C4_H;

    BYTE VifSos21FilterC0_L;
    BYTE VifSos21FilterC0_H;
    BYTE VifSos21FilterC1_L;
    BYTE VifSos21FilterC1_H;
    BYTE VifSos21FilterC2_L;
    BYTE VifSos21FilterC2_H;
    BYTE VifSos21FilterC3_L;
    BYTE VifSos21FilterC3_H;
    BYTE VifSos21FilterC4_L;
    BYTE VifSos21FilterC4_H;
    BYTE VifSos22FilterC0_L;
    BYTE VifSos22FilterC0_H;
    BYTE VifSos22FilterC1_L;
    BYTE VifSos22FilterC1_H;
    BYTE VifSos22FilterC2_L;
    BYTE VifSos22FilterC2_H;
    BYTE VifSos22FilterC3_L;
    BYTE VifSos22FilterC3_H;
    BYTE VifSos22FilterC4_L;
    BYTE VifSos22FilterC4_H;
    BYTE VifSos31FilterC0_L;
    BYTE VifSos31FilterC0_H;
    BYTE VifSos31FilterC1_L;
    BYTE VifSos31FilterC1_H;
    BYTE VifSos31FilterC2_L;
    BYTE VifSos31FilterC2_H;
    BYTE VifSos31FilterC3_L;
    BYTE VifSos31FilterC3_H;
    BYTE VifSos31FilterC4_L;
    BYTE VifSos31FilterC4_H;
    BYTE VifSos32FilterC0_L;
    BYTE VifSos32FilterC0_H;
    BYTE VifSos32FilterC1_L;
    BYTE VifSos32FilterC1_H;
    BYTE VifSos32FilterC2_L;
    BYTE VifSos32FilterC2_H;
    BYTE VifSos32FilterC3_L;
    BYTE VifSos32FilterC3_H;
    BYTE VifSos32FilterC4_L;
    BYTE VifSos32FilterC4_H;

    BYTE Vif_Agc_Ref;
    BYTE Vif_CR_K_Sel;
    BYTE Vif_CR_KpKi;
}VIFSTRParameter;

//****************************************************************************
// Public functions.
//****************************************************************************
INTERFACE void VIF_Version(void);
INTERFACE void VIF_SetClock(BOOL bEnable);
INTERFACE void VIF_Init(VIFInitialIn * pVIF_InitData, DWORD u32InitDataLen);
INTERFACE void VIF_Exit(void);
INTERFACE void VIF_Reset(void);
INTERFACE void VIF_Handler(BOOL bAutoScan);
INTERFACE void VIF_SetSoundSystem(VIFSoundSystem ucSoundSystem);
INTERFACE void VIF_SetIfFreq(IfFrequencyType u16IfFreq);
INTERFACE U8 VIF_Read_CR_FOE(void);
INTERFACE U8 VIF_Read_CR_LOCK_STATUS(void);
INTERFACE void VIF_BypassDBBAudioFilter(BOOL bEnable);
INTERFACE void VIF_SetFreqBand(FrequencyBand u8FreqBand);
INTERFACE BOOL VIF_GetInputLevelIndicator(void);
INTERFACE BOOL VIF_SetParameter(VIF_PARA_GROUP paraGroup, void * pVIF_Para, DWORD u32DataLen);
INTERFACE void VIF_ShiftClk(BYTE VifShiftClk);
INTERFACE MS_U32 VIF_SetPowerState(EN_POWER_MODE u16PowerState);
INTERFACE void VIF_WriteByte(U32 u32Reg, U8 u8Val );
INTERFACE U8 VIF_ReadByte(U32 u32Reg );


//-------------------------------- functions ------------------------------------------------------------

typedef void (*IOCTL_VIF_Version)(void);
typedef void (*IOCTL_VIF_SetClock)(BOOL);
typedef void (*IOCTL_VIF_Init)(VIFInitialIn *, DWORD);
typedef void (*IOCTL_VIF_Exit)(void);
typedef void (*IOCTL_VIF_Reset)(void);
typedef void (*IOCTL_VIF_Handler)(BOOL);
typedef void (*IOCTL_VIF_SetSoundSystem)(VIFSoundSystem);
typedef void (*IOCTL_VIF_SetIfFreq)(IfFrequencyType);
typedef U8 (*IOCTL_VIF_CR_FOE)(void);
typedef U8 (*IOCTL_VIF_LOCK_STATUS)(void);
typedef void (*IOCTL_VIF_Bypass_DBBAudioFilter)(BOOL);
typedef void (*IOCTL_VIF_SetFreqBand)(FrequencyBand);
typedef BOOL (*IOCTL_VIF_GetInputLevelIndicator)(void);
typedef BOOL (*IOCTL_VIF_SetParameter)(VIF_PARA_GROUP, void *, DWORD);
typedef void (*IOCTL_VIF_ShiftClk)(BYTE);
typedef MS_U32 (*IOCTL_VIF_SetPowerState)(EN_POWER_MODE);
typedef void (*IOCTL_VIF_WriteByte)(U32, U8);
typedef U8 (*IOCTL_VIF_ReadByte)(U32);


typedef struct _VIF_INSTANT_PRIVATE
{
    IOCTL_VIF_Version                                  fpVIFVersion;
    IOCTL_VIF_SetClock                                fpVIFSetClock;
    IOCTL_VIF_Init                                        fpVIFInit;
    IOCTL_VIF_Exit                                        fpVIFExit;
    IOCTL_VIF_Reset                                     fpVIFReset;
    IOCTL_VIF_Handler                                  fpVIFHandler;
    IOCTL_VIF_SetSoundSystem                     fpVIFSetSoundSystem;
    IOCTL_VIF_SetIfFreq                                fpVIFSetIfFreq;
    IOCTL_VIF_CR_FOE                                  fpVIFCRFOE;
    IOCTL_VIF_LOCK_STATUS                        fpVIFLOCKSTATUS;
    IOCTL_VIF_Bypass_DBBAudioFilter            fpVIFBypassDBBAudioFilter;
    IOCTL_VIF_SetFreqBand                           fpVIFSetFreqBand;
    IOCTL_VIF_GetInputLevelIndicator             fpVIFGetInputLevelIndicator;
    IOCTL_VIF_SetParameter                          fpVIFSetParameter;
    IOCTL_VIF_ShiftClk                                  fpVIFShiftClk;
    IOCTL_VIF_SetPowerState                        fpVIFSetPowerState;
    IOCTL_VIF_WriteByte                               fpVIFWriteByte;
    IOCTL_VIF_ReadByte                               fpVIFReadBtye;
    
}VIF_INSTANT_PRIVATE;


void VIFRegisterToUtopia(void);
MS_U32 VIFOpen(void** pInstantTmp, MS_U32 u32ModuleVersion, void* pAttribute);
MS_U32 VIFClose(void* pInstantTmp);
MS_U32 VIFIoctl(void* pInstantTmp, MS_U32 u32Cmd, void* pArgs);
MS_U32 VIFStr(MS_U32 u32PowerState, void* pModule);


#undef INTERFACE

#ifdef __cplusplus
}
#endif

#endif //_VIF_H_

