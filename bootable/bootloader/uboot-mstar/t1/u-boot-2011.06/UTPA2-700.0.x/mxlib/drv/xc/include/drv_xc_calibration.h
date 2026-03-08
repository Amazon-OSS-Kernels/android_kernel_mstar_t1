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
#ifndef DRV_XC_CALIBRATION_H
#define DRV_XC_CALIBRATION_H

#ifdef __cplusplus
extern "C"
{
#endif

////////////////////////////////////////////////////////////////////////////////
///Calibration
// the data path is BRI -> GAMMA -> CON_BRI -> DITHER
typedef enum
{
    AFTER_BRI,
    AFTER_CON_BRI,
    AFTER_GAMMA,
    AFTER_DITHER,
    AFTER_ADC,
} MS_IMAGE_CAP_POSITION;

typedef enum
{
    IMAGE_CAPTURE_DISABLE_CSC,
    IMAGE_CAPTURE_ENABLE_CSC,
} MS_IMAGE_CAP_CSC;

typedef enum
{
    CALIBRATION_STEP_INIT,
    CALIBRATION_STEP_CAPTURE,
    CALIBRATION_STEP_READ_MEMORY,
    CALIBRATION_STEP_END,
} MS_CALIBRATION_STEP;

/// ADC setting
typedef struct
{
    MS_U8 RedGain;      ///< ADC red gain
    MS_U8 GreenGain;    ///< ADC green gain
    MS_U8 BlueGain;     ///< ADC blue gain
    MS_U8 RedOffset;    ///< ADC red offset
    MS_U8 GreenOffset;  ///< ADC green offset
    MS_U8 BlueOffset;   ///< ADC blue offset
} MS_POST_CALIBRATION_SETTING;

typedef struct
{
    MS_U8   u8CapPosition;
    MS_U8   u8DIMode, u8F2OutputDataCfg;
    MS_U8   u8NREnable;
    MS_U32  u32DNRBase0, u32DNRBase1, u32DNRWriteLimit;
    MS_U16  u16DNRFetch, u16DNROffset;
    MS_U16  u16VLimit;
    MS_U16  u16INT14, u16INT15;
    MS_U8   u8IPMUX, u8CHIPTOP;
    MS_U8   u8CapSrc, u8Glitch, u8IPDESetting, u8DEEnable;
    MS_U8   u8InterlaceDetect;
    MS_U16  u16HCapStart, u16HCapSize, u16VCapStart, u16VCapSize;
    MS_U8   u8CSC;
    MS_U8   u8PreV, u8PreH;
    MS_U8   u8444_422Filter, u8IP422Format, u8F2InputDataCfg, u8Force10bits;
    MS_U8   u8MemFmt;
    MS_U8   u8MACE;
    MS_U8   u8misc;
} MS_IMAGE_CAP_SETTING_TYPE;

typedef struct
{
    MS_IMAGE_CAP_CSC           eCapEnableCSC;
    MS_IMAGE_STORE_FMT         eStoreFmt;
    MS_IMAGE_CAP_POSITION      eCapPosition;
    MS_U16 u16HCapStart;
    MS_U16 u16VCapStart;
    MS_U16 u16HCapSize;
    MS_U16 u16VCapSize;
    MS_U16 u16CalibrationX0;
    MS_U16 u16CalibrationX1;
    MS_U16 u16CalibrationXLen;
    MS_U16 u16CalibrationY;
    MS_U16 u16CalibrationYLen;
    MS_IMAGE_MEM_INFO stMemInfo;
    MS_IMAGE_CAP_SETTING_TYPE stImageCapSetting;
} MS_IMAGE_CAP_INFO;





#ifdef MSOS_TYPE_LINUX_KERNEL
//extern MS_BOOL g_bDoCalibration;
#else
//MS_BOOL g_bDoCalibration;
#endif


//void MDrv_Calibration_Capture_Image( MS_CALIBRATION_STEP eCalibrationStep, MS_IMAGE_CAP_INFO *ptrCapInfo );
MS_AUTOADC_TYPE MDrv_XC_GetAverageData(void *pInstance, MS_PIXEL_32BIT *sMemBuf, MS_AUTO_CAL_WINDOW_t pCalWin, SCALER_WIN eWindow);
void MApi_XC_ReadRgbData(void *pInstance, MS_IMAGE_MEM_INFO * pMemInfo);
void MApi_XC_ReadVOPData(void *pInstance, MS_IMAGE_MEM_INFO * pMemInfo);
#ifdef __cplusplus
}
#endif

#endif /* DRV_XC_CALIBRATION_H */

