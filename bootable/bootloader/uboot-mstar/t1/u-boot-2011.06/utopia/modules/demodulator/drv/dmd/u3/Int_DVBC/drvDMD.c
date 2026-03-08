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

///////////////////////////////////////////////////////////////////////////////////////////////////
///
/// file    drvDMD.c
/// @brief  DVBC Demodulator Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------

#ifdef MSOS_TYPE_LINUX
    #include <sys/ioctl.h>
    #include <fcntl.h>
    #include <errno.h>
    #include <math.h>
#else // !MSOS_TYPE_LINUX
#endif // MSOS_TYPE_LINUX

#include "MsCommon.h"
#include "drvDMD.h"
#include "devDVBC.h"


#if   (FRONTEND_DEMOD_TYPE == DEMOD_MSDVBC)

//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------
#define DMD_DBG_ENABLE              0


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------

#define DMD_SNR_BIAS                3.5

#ifdef MSOS_TYPE_LINUX_KERNEL
#define DMD_MUTEX_TIMEOUT           MSOS_WAIT_FOREVER
#else
#define DMD_MUTEX_TIMEOUT           2000                                // The maximum timeout by X4CFE_en
#endif

#ifndef MS_NOSAPI
#define DMD_INIT()                 { _s32DmdMutexId = MsOS_CreateMutex(E_MSOS_FIFO, "DMD_Mutex", MSOS_PROCESS_SHARED);   \
                                     if (_s32DmdMutexId < 0)                                        \
                                     {                                                              \
                                         DMD_DBG("%s Failed (%d)\n", __FUNCTION__, __LINE__);       \
                                         return FALSE;                                              \
                                     }                                                              \
                                   }
#define DMD_ENTRY()                if (!MsOS_ObtainMutex(_s32DmdMutexId, DMD_MUTEX_TIMEOUT))        \
                                   {   return FALSE;   }
#define DMD_RETURN(_ret)           { MsOS_ReleaseMutex(_s32DmdMutexId);                             \
                                     return _ret; }// Should be optimized by compiler
#else   // MS_NOSAPI
#define DMD_INIT()                 { }
#define DMD_ENTRY()                { }
#define DMD_RETURN(_ret)           { return _ret; }
#endif  // MS_NOSAPI


#if DMD_DBG_ENABLE
#define DMD_DBG(fmt, args...)       printf(fmt, ##args)
#else
#define DMD_DBG(fmt, args...)       {}
#endif


//-------------------------------------------------------------------------------------------------
//  Local Structurs
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------

static MS_S32                       _s32DmdMutexId;
static MS_BOOL                      _bDmdOpen;

//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------
int DVBC_Init(void);
int DVBC_Open(void);
int DVBC_Release(void);
int DVBC_Ioctl(MS_U32 cmd, ...);
#ifdef MS_NOSAPI
MS_BOOL uDVBC_Restart(DVBC_Param *pParam, DVBC_Mode *pMode);
#endif

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
/// Initialize demodulator driver
/// @return TRUE(Success), FALSE(Failure)
/// DVBC driver is in @ref E_TASK_PRI_HIGH level
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Init(void)
{
    DMD_DBG("%s\n", __FUNCTION__);

    DVBC_Init();

    DMD_INIT()

    DMD_DBG("%s Successful\n", __FUNCTION__);

    return TRUE;

}


//-------------------------------------------------------------------------------------------------
/// Open a demodulator instance
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Open(void)
{
    DMD_DBG("%s\n", __FUNCTION__);

    DMD_ENTRY();

    if (_bDmdOpen)
    {
        DMD_DBG("%s Failed (%d)\n", __FUNCTION__, __LINE__);
        DMD_RETURN(FALSE);
    }

    // Open SMART device
    if (DVBC_Open() < 0)
    {
        DMD_DBG("%s Failed (%d)\n", __FUNCTION__, __LINE__);
        DMD_RETURN(FALSE);
    }

    _bDmdOpen = TRUE;

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Close a demodulator instance
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Close(void)
{
    DMD_DBG("%s\n", __FUNCTION__);

    DMD_ENTRY();
    if (_bDmdOpen == FALSE)
    {
        DMD_RETURN(FALSE);
    }

    _bDmdOpen = FALSE;

    // Close SMART device and free system resource
    DVBC_Release();

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Set demodulator tuning control
/// @param  pMode                   \b IN:  tuning control
/// @return TRUE(Success), FALSE(Failure)
/// @note
/// [NONOS_SUPPORT]
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_SetMode(DMD_Mode *pMode)
{
    //[TODO] configurate DVBC_Mode if it's different than DMD_Mode
    if (DVBC_Ioctl(DEVDVBC_IOC_SETMODE, pMode))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Reset demodulator
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Reset(void)
{
    DMD_ENTRY();

    DMD_DBG("%s\n", __FUNCTION__);

    if (DVBC_Ioctl(DEVDVBC_IOC_RESET))
    {
        DMD_RETURN(FALSE);
    }

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Restart demodulator by tuning parameter
/// @param  pParam                  \b IN:  tuning parameter
/// @return TRUE(Success), FALSE(Failure)
/// @note
/// [NONOS_SUPPORT] TRUE: Lock
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_Restart(DMD_Param *pParam)
{
    DMD_ENTRY();

    DMD_DBG("%s\n", __FUNCTION__);

    if (DVBC_Ioctl(DEVDVBC_IOC_RESET))
    {
        DMD_RETURN(FALSE);
    }

    //[TODO] configurate DVBC_Mode if it's different than DMD_Mode
    if (DVBC_Ioctl(DEVDVBC_IOC_RESTART, pParam))
    {
        DMD_RETURN(FALSE);
    }

    DMD_RETURN(TRUE);
}


//-------------------------------------------------------------------------------------------------
/// Enable/Disable demodulator external TS output
/// @param  bEnable                 \b IN:  Enable/disable
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_TsOut(MS_BOOL bEnable)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_SETTSOUT, bEnable ? 1 : 0))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Demodulator power on/off
/// @param  bPowerOn                \b IN:  Power on/off
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_PowerOnOff(MS_BOOL bPowerOn)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_SETPOWER, bPowerOn ? 1 : 0))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Set carrier frequency bandwidth
/// @param  u32BW                   \b IN:  Frequency bandwidth (KHz)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_SetBW(MS_U32 u32BW)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_SETBW, u32BW))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get carrier frequency bandwid setting
/// @param  pu32BW                  \b OUT: Pointer of return bandwidth
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetBW(MS_U32 *pu32BW)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_GETBW, pu32BW))
    {
        return FALSE;
    }
    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get demodulator lock status
/// @param  pbLock                  \b OUT: Pointer of return lock status
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetLock(MS_BOOL *pbLock)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, pbLock))
    {
        return FALSE;
    }
    return TRUE;
}


#ifndef MSOS_TYPE_LINUX_KERNEL
//-------------------------------------------------------------------------------------------------
/// Get carrier SNR indicator
/// @param  pu32SNR                 \b OUT: Pointer of return SNR (db)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetSNR(MS_U32 *pu32SNR)
{
    MS_S16              s16NoisePower;
    float              vk;

    *pu32SNR = 0;

    MS_BOOL             bLock;
    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }

    if (DVBC_Ioctl(DEVDVBC_IOC_GETNOISEPOWER, &s16NoisePower))
    {
        return FALSE;
    }
    if (s16NoisePower == 0)
    {
        s16NoisePower = 1;
    }
    if (s16NoisePower < 0)// negative
    {
        s16NoisePower = -s16NoisePower;
    }

    vk = 10 * log10(65536.f/((double)s16NoisePower)) - DMD_SNR_BIAS;
    *pu32SNR = (vk > 0.0) ? (MS_U32)(vk) : 0;

    return TRUE;
}

//-------------------------------------------------------------------------------------------------
/// Get carrier BER indicator
/// @param  pfBER                   \b OUT: Pointer of return BER (1/65535)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetBER(float *pfBER)
{
    MS_BOOL             bLock;
    MS_U16              u16WindowLen;
    MS_U32              u32ErrorBit;

    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }

    if (DVBC_Ioctl(DEVDVBC_IOC_GETWINDOWLEN, &u16WindowLen))
    {
        return FALSE;
    }
    if (DVBC_Ioctl(DEVDVBC_IOC_GETERRORBIT, &u32ErrorBit))
    {
        return FALSE;
    }
    // 0 <= fBER <= 1
    *pfBER = (double)(u32ErrorBit)/((double)(u16WindowLen) * 128 * 188 * 8);
    if (*pfBER > 1.0)
    {
        *pfBER = 1.0;
    }
    // .32 format

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Get carrier PWR indicator
/// @param  ps32PWR                 \b OUT: Pointer of return PWR (0-255)
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetPWR(MS_S32 *ps32PWR)
{
    MS_U32              u32RfGain, u32IfGain;
    double              fPower, fTemp;

    *ps32PWR = 0;

#if 0 // no need to be locked
    MS_BOOL             bLock;
    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }
#endif

    if (DVBC_Ioctl(DEVDVBC_IOC_GETRFGAIN, &u32RfGain))
    {
        return FALSE;
    }
    if (DVBC_Ioctl(DEVDVBC_IOC_GETIFGAIN, &u32IfGain))
    {
        return FALSE;
    }
    fTemp = -20 * log10((double)u32RfGain/0x10000) - 20 * log10((double)u32IfGain/0x10000) - 63.4;
    if (fTemp >= -45.1)
    {
        fPower = -30 + ( fTemp + 45.1 ) * 2.5974;
    }
    else if (fTemp >= -47.4)
    {
        fPower = -50 + ( fTemp + 47.4 ) * 8.69565;
    }
    else if (fTemp >= -52.3)
    {
        fPower = -55 + ( fTemp + 52.3 ) * 1.02;
    }
    else //if (fTemp < -52.3)
    {
        fPower = -55 + ( fTemp + 52.3 ) * 3.846154;
    }

    *ps32PWR = (MS_S32)(fPower);


    return TRUE;
}
#endif


//-------------------------------------------------------------------------------------------------
/// Get demodulator tuning parameter
/// @param  pParam                  \b OUT: Pointer of return paramter
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_Dmd_GetParam(DMD_Param *pParam)
{
    MS_BOOL             bLock;

    // Check signal lock
    if (DVBC_Ioctl(DEVDVBC_IOC_GETLOCK, &bLock))
    {
        return FALSE;
    }
    if (bLock == FALSE)
    {
        return FALSE;
    }

    if (DVBC_Ioctl(DEVDVBC_IOC_GETPARAM, pParam))
    {
        return FALSE;
    }

    return TRUE;
}


//-------------------------------------------------------------------------------------------------
/// Config demodulator register parameter
/// @param  pRegParam                \b OUT: Pointer of return paramter
/// @return TRUE(Success), FALSE(Failure)
//-------------------------------------------------------------------------------------------------

MS_BOOL MDrv_Dmd_Config(MS_U8 *pRegParam)
{
    if (DVBC_Ioctl(DEVDVBC_IOC_CONFIG, pRegParam))
    {
        return FALSE;
    }

	return TRUE;
}

#endif // DEMOD_MSDVBC

