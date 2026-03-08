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

#ifndef _HAL_AVD_H_
#define _HAL_AVD_H_

#ifndef UNUSED
#define UNUSED(x) ((x)=(x))
#endif

#define AVD_PAULO_1_A               0
#define AVD_PAULO_1_B               1   // Paulo1, Paulo2
#define AVD_PAULO_2_A               2
#define AVD_SATURN_A                10  // S1
#define AVD_LOLA_A                  20  // Lola, Lomeo, S2, S3, Neptune
#define AVD_LATTE_A                 21  // Latte, Laser
#define AVD_MOSES_A                 22  // Moses
#define AVD_ERIS_A                  23  // Eris,Pluto,T1,T2
#define AVD_EUCLID_A                24  // Euclid(13K code size with SPL2/DPL2)
// For New Version VD MCU below
#define AVD_LOPEZ_A                 30  // Lopez,Metis,Martina
#define AVD_MUSE_A                  31  // Maria5
#define AVD_RAPHAEL_A               32  // Rapfael
#define AVD_TITANIA3_A              33  // T3
#define AVD_METIS_A                 34  // Metis,Martina
#define AVD_TITANIA4_A              35  // T4,T7,Janus
#define AVD_AMBER1_A                36  // Amber1,Amber5
#define AVD_AMBER7_A                37//Amber7,Agate,Amethyst,Eagle

#define VD_CHIP_VERSION             AVD_AMBER7_A

// 20090812 turn off MID function temporarily because of COMB bug, 1
//#define AVD_COMB_3D_MID            1
#if((VD_CHIP_VERSION>=AVD_TITANIA3_A))
#define NEW_VD_MCU              1
#else
#define NEW_VD_MCU              0
#endif
#if((VD_CHIP_VERSION==AVD_AMBER1_A))
#define TWO_VD_DSP_CODE              1
#else
#define TWO_VD_DSP_CODE              0
#endif
#if ((VD_CHIP_VERSION==AVD_TITANIA4_A))
#define LOAD_CODE_BYTE_WRITE_ONLY 1
#else
#define LOAD_CODE_BYTE_WRITE_ONLY 0
#endif
#if ((VD_CHIP_VERSION==AVD_TITANIA3_A))
#define T3_LOAD_CODE 1
#else
#define T3_LOAD_CODE 0
#endif
//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define FSC_AUTO_DET_ENABLE                     0x00
#define FSC_AUTO_DET_DISABLE                    0x01

#define FSC_MODE_PAL                            0x00
#define FSC_MODE_SECAM                          0x01
#define FSC_MODE_NTSC                           0x02
#define FSC_MODE_NTSC_443                       0x03
#define FSC_MODE_PAL_M                          0x04
#define FSC_MODE_PAL_60                         0x05
#define FSC_MODE_PAL_N                          0x06

#define AVD_AGC_ENABLE                              0x00
#define AVD_AGC_DISABLE                             0x03

#define AVD_DSP_CODE_TYPE_VIF 0
#define AVD_DSP_CODE_TYPE_ADC 1

// Coarse Gain Define
#if (VD_CHIP_VERSION==AVD_AMBER1_A)      // Brian 20110428  A1 & A5 ADC gain mapping are different
#define VD_AGC_COARSE_GAIN  0xE // 4'b:1110    0.857
#else
#define VD_AGC_COARSE_GAIN  0xA // 4'b:1010    0.867
#endif

//#define VD_AGC_COARSE_GAIN_X_0_5        0   // x0.5
//#define VD_AGC_COARSE_GAIN_X_1          1   // x1
//#define VD_AGC_COARSE_GAIN_X_2          2   // x2
//#define VD_AGC_COARSE_GAIN_X_4          3   // x4

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
//CLK_VDMCU clock setting
//[0]: disable clock
//[1]: invert clock
//[4:2]:
//     000: 170MHz (MPLL_DIV_BUF)
//     001: 160HMz
//     010: 144MHz
//     011: 123MHz
//     100: 108MHz
//     101: mem_clock
//     110: mem_clock div 2
//     111: select XTAL

typedef enum
{
    AVD_VDMCU_CLOCK_170Mhz,
    AVD_VDMCU_CLOCK_160Mhz,
    AVD_VDMCU_CLOCK_144Mhz,
    AVD_VDMCU_CLOCK_123Mhz,
    AVD_VDMCU_CLOCK_108Mhz,
    AVD_VDMCU_CLOCK_MEM,
    AVD_VDMCU_CLOCK_MEM_DIV2,
    AVD_VDMCU_CLOCK_XTAL,
    AVD_VDMCU_CLOCK_54Mhz, // for drvavd.c compatibility, will set to 108Mhz
    AVD_VDMCU_CLOCK_86Mhz, // for drvavd.c compatibility, will set to 108Mhz
} AVD_VDMCUClockSpeed;

typedef enum
{
    AVD_VDMCU_CLOCK_NORMAL,
    AVD_VDMCU_CLOCK_INV,
} AVD_VDMCUClockInverse;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
extern const MS_U8 _u8VdDecInitialize[];
extern void HAL_AVD_ADC_SetGMC(MS_U8 u8Value);
extern void HAL_AVD_VDMCU_SoftStop (void);
extern void HAL_AVD_VDMCU_SetFreeze (MS_BOOL bEnable);
extern void HAL_AVD_VDMCU_SetClock (AVD_VDMCUClockSpeed eClock, AVD_VDMCUClockInverse eInverse);
extern void HAL_AVD_AFEC_SetClockSource(MS_BOOL bSource);
extern void HAL_AVD_VDMCU_LoadDSP(const  MS_U8 *pu8VD_DSP, MS_U32 len);
extern void HAL_AVD_RegInit (void);
extern void HAL_AVD_RegInitExt(MS_U8 *_u8VdDecInitializeExt);
extern MS_U16 HAL_AVD_AFEC_GetStatus(void);
extern MS_BOOL HAL_AVD_AFEC_GetHWHsync(void);
extern MS_BOOL HAL_AVD_AFEC_GetBurstOn(void);
extern MS_BOOL HAL_AVD_AFEC_GetCoChannelOn(void);
extern MS_U16 HAL_AVD_AFEC_GetVTotal(void);
extern MS_U16 HAL_AVD_AFEC_GetHTotal(void);
extern MS_U8 HAL_AVD_AFEC_GetNoiseMag(void);
extern void HAL_AVD_AFEC_McuReset(void);
extern void HAL_AVD_AFEC_SetClock(MS_BOOL bEnable);
extern void HAL_AVD_AFEC_SetPatchFlag(MS_U32 u32VDPatchFlag);
extern void HAL_AVD_AFEC_SetInput(AVD_InputSourceType eSource, MS_U8 u8ScartFB, AVD_DemodType eDemodType, MS_U32 u32XTAL_Clock);
extern void HAL_AVD_AFEC_SetRegFromDSP (void);
extern void HAL_AVD_AFEC_SetHTotal (MS_U32 u32HTotal);
extern void HAL_AVD_AFEC_SetVtotal (MS_U8 u8Mode);
extern void HAL_AVD_AFEC_SetBT656Width (MS_U8 u8BT656Width);
extern void HAL_AVD_AFEC_EnableForceMode (MS_BOOL bEnable);
extern void HAL_AVD_AFEC_SetFSCMode (MS_U8 u8FSCMode);
extern void HAL_AVD_AFEC_EnableCVBSLPF(MS_BOOL bEnable);
extern void HAL_AVD_AFEC_EnableBottomAverage(MS_BOOL bEnable);
extern void HAL_AVD_AFEC_EnableVBIDPLSpeedup(MS_BOOL bEnable);
extern void HAL_AVD_AFEC_AGCSetMode (MS_U8 u8AgcMode);
extern void HAL_AVD_AFEC_AGCSetCoarseGain (MS_U8 u8AgcCoarseGain);
extern void HAL_AVD_AFEC_AGCSetFineGain (MS_U8 u8AgcFineGain);
extern void HAL_AVD_AFEC_SetColorKillLevel (MS_U8 u8ColorKillLevel);
extern void HAL_AVD_AFEC_SetHsyncSensitivity(VD_HSYNC_SENSITIVITY eVDHsyncSensitivityTuning);
extern void HAL_AVD_AFEC_SetSwingLimit(MS_U8 u8Limit);
extern void HAL_AVD_AFEC_SetChannelChange (void);
extern void HAL_AVD_AFEC_SetColorStripe(MS_U8 u8Value);
extern MS_BOOL HAL_AVD_COMB_Get3dCombTimingCheck(void);
extern void HAL_AVD_COMB_SetMemoryProtect (MS_PHY u32COMB_3D_Addr, MS_U32 u32COMB_3D_Len);
#ifdef AVD_COMB_3D_MID
extern void HAL_AVD_COMB_Set3dCombMid(MS_BOOL bEnable);
#endif
extern void HAL_AVD_COMB_Set3dComb(MS_BOOL bEnable);
extern void HAL_AVD_COMB_Set3dCombSpeed(MS_U8 u8COMB57, MS_U8 u8COMB58, MS_U8 u8COMB5F);
extern void HAL_AVD_COMB_Set3dDetectionTolerance(MS_U8 u8Threshold);
extern void HAL_AVD_COMB_Set3dFineTune(MS_BOOL bEnable);
extern void HAL_AVD_COMB_SetF2(MS_U8 u8Value);
extern void HAL_AVD_COMB_SetNonStandardFSC(MS_BOOL bIsPAL,MS_BOOL bIsNonstandard);
extern void HAL_AVD_COMB_SetYCPipe(MS_U8 u8YCPipe);
extern void HAL_AVD_COMB_SetCbCrInverse(MS_U8 u8CbCrInverse);
extern void HAL_AVD_COMB_SetVerticalTimingDetectMode(MS_U8 u8Mode);
extern void HAL_AVD_COMB_SetLineBufferMode(MS_U8 u8Mode);
extern void HAL_AVD_COMB_SetNonStandardHtotal(MS_BOOL bEnable);
extern void HAL_AVD_COMB_SetHtotal(MS_U16 u16Htotal);
extern void HAL_AVD_COMB_SetHsyncTolerance(MS_U8 u8Tolerance);
extern void HAL_AVD_COMB_SetMemoryRequest(MS_BOOL bEnable);
extern void HAL_AVD_VBI_SetTTSigDetSel(MS_BOOL bEnable);
extern void HAL_AVD_VBI_SetVPSPhaseAcc(MS_U16 u16Parameter);
extern MS_U8 HAL_AVD_GetReg(MS_U16 u16Addr);
extern void HAL_AVD_SetReg(MS_U16 u16Addr, MS_U8 u8Value);
extern void HAL_AVD_SetPQFineTune(void);
extern MS_U8 HAL_AVD_GetHsyncEdge(void);
extern void HAL_AVD_Set2D3DPatchOnOff(MS_BOOL bEnable);
extern void HAL_AVD_AFEC_BackPorchWindowPosition(MS_BOOL bEnable ,MS_U8 u8Value);
extern void HAL_AVD_SetDSPCodeType(MS_BOOL bEnable);
extern void HAL_AVD_ShiftClk(AVD_ATV_CLK_TYPE eShiftMode, AVD_VideoStandardType eStandard,MS_U32 u32XTAL_Clock);
extern MS_BOOL HAL_AVD_AFEC_GetMacroVisionDetect(void);
extern MS_BOOL HAL_AVD_VBI_GetCGMSDetect(void);
#endif // _HAL_AVD_H_

