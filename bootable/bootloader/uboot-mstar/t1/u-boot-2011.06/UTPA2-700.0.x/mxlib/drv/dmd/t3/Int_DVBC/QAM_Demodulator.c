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

#include "Board.h"
#include "QAM_Demodulator.h"
#include "GPIO.h"
#include "MsTypes.h"
#include "drvBDMA.h"
//#include "SWI2C.h"
#include"drvIIC.h"
#include "drvUartDebug.h" //For OTHER_TUNER_DEBUG
#include "msAPI_CI.h"
#include "msAPI_Timer.h"
#include "apiXC.h"
#include "apiXC_Adc.h"

#include "MsCommon.h"
#include "MsIRQ.h"
#include "MsOS.h"

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
#include "MApp_GlobalFunction.h"
#include "INTERN_DVBC.c"
#include "drvVIF.h"
#endif

#define QAM_DBG(x)  //x
#if ENABLE_AUTOTEST
extern BOOLEAN g_bAutobuildDebug;
#endif
extern U16 u16ScanDtvChNum;

void devQAM_Init(void)
{
    // hard-wire reset pin
    Demodulator_OFF();
    MsOS_DelayTask(20);
    Demodulator_ON();
    MsOS_DelayTask(30);

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    INTERN_DVBC_Power_On_Initialization();
#endif

    devDigitalTuner_Init();
}

void devQAM_ControlPowerOnOff(BOOLEAN bPowerOn)
{

#if (FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF || FRONTEND_IF_DEMODE_TYPE == MSTAR_VIF_MSB1210 || FRONTEND_IF_DEMODE_TYPE == MSTAR_INTERN_VIF)
    if (bPowerOn == ENABLE) // DTV mode
    {
        EXT_RF_AGC_OFF();
        printf("KKK: (Temp)EXT_RF_AGC_OFF...for DTV\n");
    }
    else
    {
        EXT_RF_AGC_ON();
        printf("KKK: (Temp)EXT_RF_AGC_On...for ATV\n");
    }
#endif

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    INTERN_DVBC_Power_ON_OFF(bPowerOn);
#endif
}

FUNCTION_RESULT devQAM_PowerSave(void)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    if (INTERN_DVBC_Power_Save() == TRUE)
        return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    return E_RESULT_SUCCESS;
#endif
}

void devQAM_TunerProgramming(U8 *cTunerData)
{
    *cTunerData=*cTunerData;
}

#if DTV_SCAN_AUTO_FINE_TUNE_ENABLE
BOOLEAN devQAM_GetFrequencyOffset(float *pFreqOff, RF_CHANNEL_BANDWIDTH u8BW)
{
    *pFreqOff = 0;  //Temp Solution;;
    u8BW = u8BW; //To avoid compiler warning
    return 1;
}
BOOLEAN devQAM_SetAfcEnable(BOOLEAN bEnable)
{
    bEnable = bEnable; //To avoid compiler warning
    return 1;
}
#endif

void devQAM_SetFrequency(DWORD dwFrequency, U8 u8QamMode, U16 u16SymbolRate, BOOLEAN bSpecInv)
{
    //dwFrequency = 562000;                   // << Ken 20090629
    //eBandWidth = E_RF_CH_BAND_8MHz;         // << Ken 20090629

    //printf("====dwFrequency = %lu \n", dwFrequency/1000);
    //printf("====eBandWidth = %bx\n", eBandWidth);
    #if ENABLE_AUTOTEST
    if(g_bAutobuildDebug && (dwFrequency/1000 == 850))
    {
        printf("31_DTV_Tuning_End_%d\n", u16ScanDtvChNum);
    }
    #endif

    #if (ENABLE_AUTOTEST || ENABLE_BOOTTIME)
      #if (ENABLE_BOOTTIME==DISABLE)
        if (g_bAutobuildDebug == TRUE)
      #endif
        {
            gU32TmpTime = msAPI_Timer_DiffTimeFromNow(gU32BootTime);
            printf("[boot time]QAM set RF = %ld\n", gU32TmpTime);
        }
    #endif

    devDigitalTuner_SetFreq((double)(dwFrequency/1000.0), E_RF_CH_BAND_8MHz);

#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    //MsOS_DelayTask(10); //waiting for tuner write

    #if ENABLE_CI
    if ( msAPI_CI_CardDetect())
        INTERN_DVBC_Config (u16SymbolRate, u8QamMode, 36167, bSpecInv, FALSE);
    else
    #endif
    INTERN_DVBC_Config (u16SymbolRate, u8QamMode, 36167, bSpecInv, TRUE);

    INTERN_DVBC_Active(ENABLE);
#else
    UNUSED(dwFrequency);
    UNUSED(u8QamMode);
    UNUSED(u16SymbolRate);
    UNUSED(bSpecInv);
#endif

}

BOOLEAN devQAM_GetLockStatus(QAM_LOCK_STATUS eStatus)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return INTERN_DVBC_Lock(eStatus);
#else
    UNUSED(eStatus);
    return TRUE;
#endif
}

BYTE devQAM_GetSignalToNoiseRatio(void)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return (BYTE)INTERN_DVBC_GetSNR();
#else
    return 1;
#endif
}

void devQAM_PassThroughI2C(BOOLEAN bEnable)
{
    bEnable=bEnable;
}

void devQAM_SwitchTransportSteamInferface(QAM_TS_INTERFACE interface)
{
    BOOLEAN bEnable;

    if ( interface == E_QAM_SERIAL_INTERFACE )
    {
        bEnable = TRUE;
    }
    else
    {
        bEnable = FALSE;
    }
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    INTERN_DVBC_Serial_Control(bEnable);
#endif

}

FUNCTION_RESULT devQAM_GetSignal_Vit( DWORD *vit )
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    float ber;
    *vit = *vit;
    if (INTERN_DVBC_GetPostViterbiBer(&ber)==TRUE)
       return E_RESULT_SUCCESS;
    else
        return E_RESULT_FAILURE;
#else
    *vit = *vit;
    return E_RESULT_FAILURE;
#endif
}

FUNCTION_RESULT devQAM_GetSignalStrength(WORD *strength)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return INTERN_DVBC_GetSignalStrength(strength);
#else
    *strength = 80;
    return E_RESULT_SUCCESS;
#endif

}


FUNCTION_RESULT devQAM_GetSignalQuality(WORD *quality)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return INTERN_DVBC_GetSignalQuality(quality);
#else
    *quality = 80;
    return E_RESULT_SUCCESS;
#endif
}

BOOLEAN devQAM_PassThroughI2C_ReadBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
    UNUSED(u8SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
}

BOOLEAN devQAM_PassThroughI2C_WriteBytes(U8 u8SlaveID, U8 u8AddrNum, U8* paddr, U16 u16size, U8* pu8data)
{
    UNUSED(u8SlaveID);
    UNUSED(u8AddrNum);
    UNUSED(paddr);
    UNUSED(u16size);
    UNUSED(pu8data);
    return TRUE;
}
BOOLEAN devQAM_GetSpecInv(void)
{
#if ((FRONTEND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD) || (FRONTEND_SECOND_DEMOD_TYPE == EMBEDDED_DVBC_DEMOD))
    return TRUE;
#endif
    return TRUE;
}
