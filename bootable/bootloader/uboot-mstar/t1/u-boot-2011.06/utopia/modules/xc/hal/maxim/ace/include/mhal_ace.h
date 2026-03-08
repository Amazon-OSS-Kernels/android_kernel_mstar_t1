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
//==============================================================================
#ifndef MHAL_ACE_H
#define MHAL_ACE_H

/// Define XC ACE device number
typedef enum
{
    XC_ACE_DEVICE0 = 0,
    XC_ACE_MAX_DEVICE_NUM
} XC_ACE_DEVICE_NUM;

#define XC_ACE_SUPPORT_DEVICE_NUM XC_ACE_MAX_DEVICE_NUM

typedef enum
{
    E_HALACE_DEVICE0_XC_BANK_OFFSET    = 0,
} ACE_HAL_DEVICE_XC_BANK_OFFSET;

void Hal_ACE_DMS(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bisATV );
void Hal_ACE_GetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix);
void Hal_ACE_SetColorMatrix(void* pInstance, MS_BOOL bScalerWin, MS_U16* pu16Matrix);
MS_BOOL Hal_ACE_SetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8 u8Val);
MS_BOOL Hal_ACE_GetIHC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IHC_COLOR_TYPE eIHC, MS_U8* pu8Val);
MS_BOOL Hal_ACE_SetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eICC, MS_U8 u8Val);
MS_BOOL Hal_ACE_GetICC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_ICC_COLOR_TYPE eIHC, MS_U8* pu8Val);
MS_BOOL Hal_ACE_SetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIBC, MS_U8 u8Val);
MS_BOOL Hal_ACE_GetIBC(void* pInstance, MS_BOOL bScalerWin, XC_ACE_IBC_COLOR_TYPE eIHC, MS_U8* pu8Val);

void Hal_ACE_PatchDTGColorChecker(void* pInstance, MS_U8 u8Mode);
void Hal_ACE_SetSlopValue(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8SlopValue);
void Hal_ACE_SetFCC_En(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Reg, MS_BOOL bEn);
void Hal_ACE_SetFCC_Cb(void* pInstance, MS_U8 u8Reg, MS_U8 u8value);
void Hal_ACE_init_riu_base(MS_VIRT ptr_riu_base);
void Hal_ACE_SetSharpness(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8Sharpness);
void Hal_ACE_SetSkipWaitVsync(void* pInstance, MS_BOOL bIsMainSkipWaitVsyn,MS_BOOL bIsSubSkipWaitVsyn);
MS_BOOL Hal_ACE_GetSkipWaitVsync(void* pInstance, MS_BOOL bScalerWin);

void Hal_ACE_SetBrightness(void* pInstance, MS_BOOL bScalerWin, MS_U8 u8RedBrightness, MS_U8 u8GreenBrightness, MS_U8 u8BlueBrightness );
void Hal_ACE_SetBrightnessPrecise(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedBrightness, MS_U16 u16GreenBrightness, MS_U16 u16BlueBrightness);
void Hal_ACE_GetBrightness(void* pInstance, MS_BOOL bScalerWin, MS_U8* u8RedBrightness, MS_U8* u8GreenBrightness, MS_U8* u8BlueBrightness);
void Hal_ACE_SetPostRGBGain(void* pInstance, MS_BOOL bScalerWin, MS_U16 u8RedGain, MS_U16 u8GreenGain, MS_U16 u8BlueGain);
void Hal_ACE_SetPostRGBOffset(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset);
void Hal_ACE_SetPostRGBGainOffsetTogether(void* pInstance, MS_BOOL bScalerWin,
                                          MS_U16 u16RedGain,   MS_U16 u16GreenGain,   MS_U16 u16BlueGain,
                                          MS_U16 u16RedOffset, MS_U16 u16GreenOffset, MS_U16 u16BlueOffset);
void Hal_ACE_Set_IHC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U8 u8SRAM_Idx, MS_U16 u16Cnt);
void Hal_ACE_Set_ICC_SRAM(void* pInstance, MS_U16 *pBuf, MS_U16 u16Cnt);
void Hal_ACE_Write_Color_Matrix_Burst(void* pInstance, MS_BOOL bWindow, MS_U16* psMatrix );
MS_BOOL Hal_ACE_Is_Support_MLoad(void* pInstance);

void Hal_ACE_MWESetWin(void* pInstance, MS_U16 u16hstart, MS_U16 u16hend, MS_U16 u16vstart, MS_U16 u16vend);
void Hal_ACE_MWEEnable(void* pInstance, MS_BOOL ben, MS_BOOL bLoadFromTable);
void Hal_ACE_MWESetBorder(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16color, MS_U16 u16_l, MS_U16 u16_r, MS_U16 u16_t, MS_U16 u16_d);
void Hal_ACE_MWECloneVisualEffect(void* pInstance);
void Hal_ACE_3DClonePQMap(void* pInstance, XC_ACE_WEAVETYPE enWeaveType);

MS_U8 Hal_ACE_DNR_GetMotion(void* pInstance);
MS_U8 Hal_ACE_DNR_GetCoringThreshold(void* pInstance, MS_BOOL bScalerWin);
MS_U8 Hal_ACE_DNR_GetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin);
MS_U8 Hal_ACE_DNR_GetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin);


void Hal_ACE_DNR_SetCoringThreshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetSharpnessAdjust(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNM_V(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGNR_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGNR_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetCP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetDP(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNM_H_0(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNM_H_1(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGray_Ground_Gain(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetGray_Ground_En(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetSC_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetSpikeNR_0(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetSpikeNR_1(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetSNR_NM(void* pInstance, MS_U16 u16val);
void Hal_ACE_DNR_SetBank_Coring(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetGuassin_SNR_Threshold(void* pInstance, MS_BOOL bScalerWin, MS_U16 u16val);
void Hal_ACE_DNR_SetNRTbl_Y(void* pInstance, MS_U8 u8Idx, MS_U16 u16val);
void Hal_ACE_DNR_SetNRTbl_C(void* pInstance, MS_U8 u8Idx, MS_U16 u16val);
void HAL_ACE_Set_RBChannelOffset(void* pInstance, MS_BOOL bScalerWin, MS_BOOL bEnable);
void Hal_ACE_Set_Device_Bank_Offset(void);

// Function in mdrv_xc_menuload.c
typedef enum
{
    E_MLOAD_UNSUPPORTED = 0,
    E_MLOAD_DISABLED    = 1,
    E_MLOAD_ENABLED     = 2,
}MLOAD_TYPE;

typedef enum
{
    E_MLG_UNSUPPORTED = 0,
    E_MLG_DISABLED    = 1,
    E_MLG_ENABLED     = 2,
}MLG_TYPE;

extern MS_BOOL MApi_XC_MLoad_WriteCmd_NonXC(void *pInstance,MS_U32 u32Bank,MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
extern MS_BOOL MApi_XC_MLoad_Fire(void *pInstance, MS_BOOL bImmediate);
extern MS_BOOL MApi_XC_MLoad_WriteCmd(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
extern MS_BOOL MApi_XC_MLoad_WriteCmds_And_Fire(void *pInstance, MS_U32 *pu32Addr, MS_U16 *pu16Data, MS_U16 *pu16Mask, MS_U16 u16CmdCnt);
extern MS_BOOL MApi_XC_MLoad_WriteCmd_And_Fire(void *pInstance, MS_U32 u32Addr, MS_U16 u16Data, MS_U16 u16Mask);
extern MLOAD_TYPE MApi_XC_MLoad_GetStatus_U2(void *pInstance);

extern MS_S32 _MLOAD_MUTEX;

#define MLOAD_MUTEX
#define MLOAD_MUTEX_DBG 0

#ifdef MLOAD_MUTEX
    #if(MLOAD_MUTEX_DBG)
        #define _MLOAD_ENTRY()                                                         \
                printf("1,==========================\n");                              \
                printf("[%s][%s][%06d]\n",__FILE__,__FUNCTION__,__LINE__);    \
                if(!MsOS_ObtainMutex(_MLOAD_MUTEX, MSOS_WAIT_FOREVER))                 \
                {                                                                       \
                    printf("==========================\n");                             \
                    printf("[%s][%s][%06d] Mutex taking timeout\n",__FILE__,__FUNCTION__,__LINE__);    \
                }
        #define _MLOAD_RETURN()                                                       \
                printf("0,==========================\n");                              \
                printf("[%s][%s][%06d] \n",__FILE__,__FUNCTION__,__LINE__);    \
                MsOS_ReleaseMutex(_MLOAD_MUTEX);
                                    //return _ret;
    #else
        #define _MLOAD_ENTRY()                                               \
                if(!MsOS_ObtainMutex(_MLOAD_MUTEX, MSOS_WAIT_FOREVER))      \
                {                                                            \
                }
        #define _MLOAD_RETURN()    MsOS_ReleaseMutex(_MLOAD_MUTEX);
                                    //return _ret;
    #endif
#else // #if not def MLOAD_MUTEX
    #define _MLOAD_ENTRY()         while(0)
    #define _MLOAD_RETURN()    while(0)
#endif // #ifdef _MLOAD_MUTEX

#endif


