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
////////////////////////////////////////////////////////////////////////////////

#ifndef _DAC_PRIV_H_
#define _DAC_PRIV_H_

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

typedef APIDAC_Result              (*IOCTL_DAC_GET_LIBVER)                  (const MSIF_Version **);
typedef const DAC_ApiInfo *              (*IOCTL_DAC_GET_INFO)                    (void);
typedef MS_BOOL                    (*IOCTL_DAC_GET_STATUS)                  (DAC_ApiStatus *);
typedef MS_BOOL                    (*IOCTL_DAC_SET_DBG_LEVEL)               (MS_U16);
typedef MS_BOOL                    (*IOCTL_DAC_INIT)                         (void);
typedef void                       (*IOCTL_DAC_ENABLE)                       (MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_CLKINV)                  (MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_YPBPR_OUTPUTTIMING)     (E_OUTPUT_VIDEO_TIMING_TYPE);
typedef void                       (*IOCTL_DAC_SET_OUTPUT_SOURCE)           (E_DAC_OUTPUT_TYPE, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_OUTPUT_LEVE)             (E_DAC_MAX_OUTPUT_LEVEL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_OUTPUT_SWAPSEL)          (E_DAC_SWAP_SEL, MS_BOOL);
typedef void                       (*IOCTL_DAC_ONOFF_SD)                     (E_DAC_SD_ENABLE_CTRL);
typedef void                       (*IOCTL_DAC_ONOFF_HD)                     (E_DAC_HD_ENABLE_CTRL);
typedef void                       (*IOCTL_DAC_CLKSEL)                       (E_OUTPUT_VIDEO_TIMING_TYPE, E_OUTPUT_BIT_TYPE);
typedef void                       (*IOCTL_DAC_DUMP_TABLE)                   (MS_U8 *, MS_U8);
typedef void                       (*IOCTL_DAC_EXIT)                         (void);
typedef void                       (*IOCTL_DAC_SET_IHALF_OUTPUT)            (MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_QUART_OUTPUT)            (MS_BOOL, MS_BOOL);
typedef void                       (*IOCTL_DAC_SET_DAC_STATE)               (MS_BOOL, MS_BOOL);
typedef MS_BOOL                    (*IOCTL_DAC_HOTPLUG_DETECT)              (E_DAC_DETECT, E_DAC_DETECT_TYPE, MS_BOOL *);

typedef enum
{
    E_DAC_POOL_ID_INTERNAL_VARIABLE = 0,
    E_DAC_POOL_ID_MAX,
} E_DAC_POOL_ID;

typedef struct
{
    // flow control related
    DAC_ApiInfo _cstDac_ApiInfo;
    DAC_ApiStatus _stDac_ApiStatus;
    MS_U16 _u16DbgSwitch;
} ST_API_DAC;

typedef struct
{
    // flow control related
    MS_BOOL bResourceRegistered;
    ST_API_DAC stapiDAC;
} DAC_RESOURCE_PRIVATE;

typedef struct _DAC_INSTANT_PRIVATE
{
    IOCTL_DAC_GET_LIBVER                  fpDACGetLibVer;
    IOCTL_DAC_GET_INFO        	          fpDACGetInfo;
    IOCTL_DAC_GET_STATUS  		          fpDACGetStatus;
    IOCTL_DAC_SET_DBG_LEVEL               fpDACSetDbgLevel;
    IOCTL_DAC_INIT        	              fpDACInit;
    IOCTL_DAC_ENABLE  		              fpDACEnable;
    IOCTL_DAC_SET_CLKINV                  fpDACSetClkInv;
    IOCTL_DAC_SET_YPBPR_OUTPUTTIMING      fpDACSetYPbPrOutputTiming;
    IOCTL_DAC_SET_OUTPUT_SOURCE  		  fpDACSetOutputSource;
    IOCTL_DAC_SET_OUTPUT_LEVE             fpDACSetOutputLevel;
    IOCTL_DAC_SET_OUTPUT_SWAPSEL          fpDACSetOutputSwapSel;
    IOCTL_DAC_ONOFF_SD  		          fpDACOnOffSD;
    IOCTL_DAC_ONOFF_HD                    fpDACOnOffHD;
    IOCTL_DAC_CLKSEL  		              fpDACClkSel;
    IOCTL_DAC_DUMP_TABLE                  fpDACDumpTable;
    IOCTL_DAC_EXIT        	              fpDACExit;
    IOCTL_DAC_SET_IHALF_OUTPUT  		  fpDACSetIHalfOutput;
    IOCTL_DAC_SET_QUART_OUTPUT            fpDACSetQuartOutput;
    IOCTL_DAC_SET_DAC_STATE        	      fpDACSetDacState;
    IOCTL_DAC_HOTPLUG_DETECT              fpDACHotPlugDetect;
}DAC_INSTANT_PRIVATE;

APIDAC_Result MApi_DAC_GetLibVer_U2(const MSIF_Version **ppVersion);
const DAC_ApiInfo * MApi_DAC_GetInfo_U2(void);
MS_BOOL MApi_DAC_GetStatus_U2(DAC_ApiStatus *pDacStatus);
MS_BOOL MApi_DAC_SetDbgLevel_U2(MS_U16 u16DbgSwitch);
MS_BOOL MApi_DAC_Init_U2(void);
void MApi_DAC_Enable_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
void MApi_DAC_SetClkInv_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
void MApi_DAC_SetYPbPrOutputTiming_U2(E_OUTPUT_VIDEO_TIMING_TYPE eTiming);
void MApi_DAC_SetOutputSource_U2(E_DAC_OUTPUT_TYPE enOutputType, MS_BOOL bIsYPbPr);
void MApi_DAC_SetOutputLevel_U2(E_DAC_MAX_OUTPUT_LEVEL enLevel, MS_BOOL bIsYPbPr);
void MApi_DAC_SetOutputSwapSel_U2(E_DAC_SWAP_SEL enSwap,MS_BOOL bIsYPbPr);
void MApi_DAC_OnOffSD_U2(E_DAC_SD_ENABLE_CTRL enBit);
void MApi_DAC_OnOffHD_U2(E_DAC_HD_ENABLE_CTRL enBit);
void MApi_DAC_ClkSel_U2(E_OUTPUT_VIDEO_TIMING_TYPE eTiming, E_OUTPUT_BIT_TYPE ebits);
void MApi_DAC_DumpTable_U2(MS_U8 *pDACTable, MS_U8 u8DACtype);
void MApi_DAC_Exit_U2(void);
void MApi_DAC_SetIHalfOutput_U2(MS_BOOL bEnable, MS_BOOL bIsYPbPr);
void MApi_DAC_SetQuartOutput_U2(MS_BOOL bEnable,MS_BOOL bIsYPbPr);
void MApi_DAC_SetDacState_U2(MS_BOOL bEnabled, MS_BOOL bIsYPbPr);
MS_BOOL MApi_DAC_HotPlugDetect_U2(E_DAC_DETECT SelDAC,E_DAC_DETECT_TYPE DetectType, MS_BOOL *State);

void DACRegisterToUtopia(void);
MS_U32 DACOpen(void** ppInstance, const void* const pAttribute);
MS_U32 DACClose(void* pInstance);
MS_U32 DACIoctl(void* pInstance, MS_U32 u32Cmd, void* pArgs);

#ifdef __cplusplus
}
#endif
#endif // _DRVBDMA_PRIV_H_
