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

#ifndef _DAC_PRIV_H_
#define _DAC_PRIV_H_
#include "UFO.h"
////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#define DAC_TABLE_TYPE_NUM 16

typedef APIDAC_Result              (*IOCTL_DAC_GET_LIBVER)                  (void*, const MSIF_Version **);
typedef const DAC_ApiInfo *              (*IOCTL_DAC_GET_INFO)              (void*);
typedef MS_BOOL                    (*IOCTL_DAC_GET_STATUS)                  (void*, DAC_ApiStatus *);
typedef MS_BOOL                    (*IOCTL_DAC_SET_DBG_LEVEL)               (void*, MS_U16);
typedef MS_BOOL                    (*IOCTL_DAC_INIT)                        (void*);
typedef void                       (*IOCTL_DAC_ENABLE)                      (void*, MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_CLKINV)                  (void*, MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_YPBPR_OUTPUTTIMING)      (void*, E_OUTPUT_VIDEO_TIMING_TYPE);
typedef void                       (*IOCTL_DAC_SET_OUTPUT_SOURCE)           (void*, E_DAC_OUTPUT_TYPE, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_OUTPUT_LEVE)             (void*, E_DAC_MAX_OUTPUT_LEVEL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_OUTPUT_SWAPSEL)          (void*, E_DAC_SWAP_SEL, MS_BOOL);
typedef void                       (*IOCTL_DAC_ONOFF_SD)                    (void*, E_DAC_SD_ENABLE_CTRL);
typedef void                       (*IOCTL_DAC_ONOFF_HD)                    (void*, E_DAC_HD_ENABLE_CTRL);
typedef void                       (*IOCTL_DAC_CLKSEL)                      (void*, E_OUTPUT_VIDEO_TIMING_TYPE, E_OUTPUT_BIT_TYPE);
typedef void                       (*IOCTL_DAC_DUMP_TABLE)                  (void*, MS_U8 *, MS_U8);
typedef void                       (*IOCTL_DAC_EXIT)                        (void*);
typedef void                       (*IOCTL_DAC_SET_IHALF_OUTPUT)            (void*, MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_QUART_OUTPUT)            (void*, MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_DAC_STATE)               (void*, MS_BOOL, MS_BOOL);
typedef MS_BOOL                    (*IOCTL_DAC_HOTPLUG_DETECT)              (void*, E_DAC_DETECT, E_DAC_DETECT_TYPE, MS_BOOL *);
typedef MS_U32                     (*IOCTL_DAC_SET_POWER_STATE)             (void*, E_DAC_POWER_MODE);
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
typedef MS_BOOL                    (*IOCTL_DAC_GET_OUTPUT_INTERLACETIMING)  (void*);
#endif
typedef MS_BOOL                    (*IOCTL_DAC_SET_WSS_ONOFF)               (void*, MS_BOOL, MS_BOOL);
typedef MS_BOOL                    (*IOCTL_DAC_RESET_WSSDATA)               (void*, MS_BOOL);
typedef MS_BOOL                    (*IOCTL_DAC_SET_WSS_OUTPUT)              (void*, MS_BOOL, MS_BOOL, DAC_SETWSS_INFO);

typedef enum
{
    E_DAC_POOL_ID_INTERNAL_VARIABLE = 0,
    E_DAC_POOL_ID_MAX,
} E_DAC_POOL_ID;

typedef struct __attribute__((__packed__))
{
    MS_BOOL bDACIsYPbPr;

    E_OUTPUT_VIDEO_TIMING_TYPE OutputVideoTimingType_now;
    E_OUTPUT_VIDEO_TIMING_TYPE OutputVideoTimingType_SC0;
    E_OUTPUT_VIDEO_TIMING_TYPE OutputVideoTimingType_SC1;
    E_OUTPUT_BIT_TYPE OutputBitType;

    //Enable
    MS_BOOL bSDDACEnable;
    MS_BOOL bHDDACEnable;
    //OutputType
    MS_U8 SDOutputType;
    MS_U8 HDOutputType;
    //SwapSEL
    MS_U8 SDSwapSEL;
    MS_U8 HDSwapSEL;
    //MaxOutputLevel
    MS_U8 SDMaxOutputLevel;
    MS_U8 HDMaxOutputLevel;
    //EnableCtrl
    E_DAC_SD_ENABLE_CTRL SD_EnableCtrl;
    E_DAC_HD_ENABLE_CTRL HD_EnableCtrl;

    E_DAC_DETECT_TYPE DetectType;
    E_DAC_DETECT Detect;

    //DAC table
    MS_VIRT DACTable[DAC_TABLE_TYPE_NUM];
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy0[DAC_TABLE_TYPE_NUM];
#endif
    MS_U8 u8DACtype[DAC_TABLE_TYPE_NUM];

    MS_U32 u32NPMBase;
    MS_U32 u32PMBase;
} MS_DAC_Info;

typedef struct __attribute__((__packed__))
{
    // flow control related
    DAC_ApiInfo _cstDac_ApiInfo;
    DAC_ApiStatus _stDac_ApiStatus;
    MS_U16 _u16DbgSwitch;
    MS_DAC_Info _stDac_DacPowerState;
} ST_API_DAC;

typedef struct __attribute__((__packed__))
{
    // flow control related
    MS_BOOL bResourceRegistered;
    ST_API_DAC stapiDAC;
} DAC_RESOURCE_PRIVATE;

typedef struct _DAC_INSTANT_PRIVATE
{
    IOCTL_DAC_GET_LIBVER                  fpDACGetLibVer;
    IOCTL_DAC_GET_INFO        	          fpDACGetInfo;
    IOCTL_DAC_GET_STATUS  		  fpDACGetStatus;
    IOCTL_DAC_SET_DBG_LEVEL               fpDACSetDbgLevel;
    IOCTL_DAC_INIT        	          fpDACInit;
    IOCTL_DAC_ENABLE  		          fpDACEnable;
    IOCTL_DAC_SET_CLKINV                  fpDACSetClkInv;
    IOCTL_DAC_SET_YPBPR_OUTPUTTIMING      fpDACSetYPbPrOutputTiming;
    IOCTL_DAC_SET_OUTPUT_SOURCE           fpDACSetOutputSource;
    IOCTL_DAC_SET_OUTPUT_LEVE             fpDACSetOutputLevel;
    IOCTL_DAC_SET_OUTPUT_SWAPSEL          fpDACSetOutputSwapSel;
    IOCTL_DAC_ONOFF_SD  		  fpDACOnOffSD;
    IOCTL_DAC_ONOFF_HD                    fpDACOnOffHD;
    IOCTL_DAC_CLKSEL  		          fpDACClkSel;
    IOCTL_DAC_DUMP_TABLE                  fpDACDumpTable;
    IOCTL_DAC_EXIT        	          fpDACExit;
    IOCTL_DAC_SET_IHALF_OUTPUT            fpDACSetIHalfOutput;
    IOCTL_DAC_SET_QUART_OUTPUT            fpDACSetQuartOutput;
    IOCTL_DAC_SET_DAC_STATE        	  fpDACSetDacState;
    IOCTL_DAC_HOTPLUG_DETECT              fpDACHotPlugDetect;
    IOCTL_DAC_SET_POWER_STATE             fpDACSetPowerState;
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
    IOCTL_DAC_GET_OUTPUT_INTERLACETIMING  fpDACGetOutputInterlaceTiming;
#endif
    IOCTL_DAC_SET_WSS_ONOFF               fpDACSetWSSOnOff;
    IOCTL_DAC_RESET_WSSDATA               fpDACResetWSSData;
    IOCTL_DAC_SET_WSS_OUTPUT              fpDACSetWSSOutput;
}DAC_INSTANT_PRIVATE;

typedef struct
{
    MS_U32 DAC_Reg[2];
}DAC_REGS_SAVE_AREA;

APIDAC_Result MApi_DAC_GetLibVer_U2(void* pInstance, const MSIF_Version **ppVersion);
const DAC_ApiInfo * MApi_DAC_GetInfo_U2(void* pInstance);
MS_BOOL MApi_DAC_GetStatus_U2(void* pInstance, DAC_ApiStatus *pDacStatus);
MS_BOOL MApi_DAC_SetDbgLevel_U2(void* pInstance, MS_U16 u16DbgSwitch);
MS_BOOL MApi_DAC_Init_U2(void* pInstance);
#ifdef UFO_XC_GETOUTPUTINTELACETIMING
MS_BOOL MApi_DAC_GetOutputInterlaceTiming_U2(void* pInstance);
#endif
void MApi_DAC_Enable_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr);
void MApi_DAC_SetClkInv_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr);
void MApi_DAC_SetYPbPrOutputTiming_U2(void* pInstance, E_OUTPUT_VIDEO_TIMING_TYPE eTiming);
void MApi_DAC_SetOutputSource_U2(void* pInstance, E_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr);
void MApi_DAC_SetOutputLevel_U2(void* pInstance, E_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr);
void MApi_DAC_SetOutputSwapSel_U2(void* pInstance, E_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr);
void MApi_DAC_OnOffSD_U2(void* pInstance, E_DAC_SD_ENABLE_CTRL enBit);
void MApi_DAC_OnOffHD_U2(void* pInstance, E_DAC_HD_ENABLE_CTRL enBit);
void MApi_DAC_ClkSel_U2(void* pInstance, E_OUTPUT_VIDEO_TIMING_TYPE eTiming, E_OUTPUT_BIT_TYPE ebits);
void MApi_DAC_DumpTable_U2(void* pInstance, MS_U8 *pDACTable, MS_U8 u8DACtype);
void MApi_DAC_Exit_U2(void* pInstance);
void MApi_DAC_SetIHalfOutput_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr);
void MApi_DAC_SetQuartOutput_U2(void* pInstance, MS_BOOL bEnable,MS_BOOL bIsYPbPr);
void MApi_DAC_SetDacState_U2(void* pInstance, MS_BOOL bEnabled, MS_BOOL bIsYPbPr);
MS_BOOL MApi_DAC_HotPlugDetect_U2(void* pInstance, E_DAC_DETECT SelDAC,E_DAC_DETECT_TYPE DetectType, MS_BOOL *State);
MS_U32 Mapi_DAC_SetPowerState_U2(void* pInstance, E_DAC_POWER_MODE PowerState);
MS_BOOL MApi_DAC_SetWSSOnOff_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr);
MS_BOOL MApi_DAC_ResetWSSData_U2(void* pInstance, MS_BOOL bIsYPbPr);
MS_BOOL MApi_DAC_SetWSSOutput_U2(void* pInstance, MS_BOOL bEnable, MS_BOOL bIsYPbPr, DAC_SETWSS_INFO SetWSS_Data);

void DACRegisterToUtopia(void);
MS_U32 DACOpen(void** ppInstance, const void* const pAttribute);
MS_U32 DACClose(void* pInstance);
MS_U32 DACIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
