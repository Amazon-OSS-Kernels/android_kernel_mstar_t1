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
/// @file   drvCPU.h
/// @brief  System Control Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_CPU_H_
#define _DRV_CPU_H_

#include "MsTypes.h"
#include "MsDevice.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define CPU_UTOPIA20       (1)

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// #define COPRO_TYPE_DEF
/// Version string.

#define MSIF_COPRO_LIB_CODE                     {'C','O','P','R'}    //Lib code
#define MSIF_COPRO_LIBVER                       {'0','2'}            //LIB version
#define MSIF_COPRO_BUILDNUM                     {'1','0'}            //Build Number
#define MSIF_COPRO_CHANGELIST                   {'0','0','5','5','7','0','9','3'} //P4 ChangeList Number

#define COPRO_DRV_VERSION                       /* Character String for DRV/API version             */  \
    MSIF_TAG,                             /* 'MSIF'                                           */  \
    MSIF_CLASS,                           /* '00'                                             */  \
    MSIF_CUS,                             /* 0x0000                                           */  \
    MSIF_MOD,                             /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_COPRO_LIB_CODE,                        /* IP__                                             */  \
    MSIF_COPRO_LIBVER,                          /* 0.0 ~ Z.Z                                        */  \
    MSIF_COPRO_BUILDNUM,                        /* 00 ~ 99                                          */  \
    MSIF_COPRO_CHANGELIST,                      /* CL#                                              */  \
    MSIF_OS


//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    E_COPRO_OK,
    E_COPRO_FAIL,
} COPRO_Result;

typedef struct _COPRO_DrvInfo
{
    MS_BOOL     Init;
    MS_U32      u32Addr;
} COPRO_DrvInfo;


typedef struct _COPRO_DrvStatus
{
    MS_BOOL     HwBusy;
} COPRO_DrvStatus;

typedef enum _COPRO_DbgLv
{
    E_COPRO_DBGLV_NONE,           //no debug message
    E_COPRO_DBGLV_ERR_ONLY,      //show error only
    E_COPRO_DBGLV_INFO,          //show error & informaiton
    E_COPRO_DBGLV_ALL           //show error, information & funciton name
}COPRO_DbgLv;

typedef enum
{
    Q_PARALLEL,
    Q_STATE,
    Q_MPEG2_FS,
    Q_MPEG2,
    Q_TSPLAYER_FS,
    Q_TSPLAYER,
    Q_VDPLAYER_FS,
    Q_VDPLAYER,
    Q_MBFS,
    Q_IR,
    Q_OBAMA,
    Q_PRINT_MESSAGE,
    Q_CIPLUS,
    Q_DLNA,
    Q_BTPD,
    Q_KTV,
    Q_SHWFS,
    Q_APENGINE,
    Q_MM,
    Q_MHEG5,
    Q_NWDEBUG, //add by Ian
    Q_NUM,
} Q_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
#if (CPU_UTOPIA20)
#include "drvCPU_v2.h"
#endif

    //-------------------------------------------------------------------------------------------------
    /// Reset and disable coprocessor
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
    void MDrv_COPRO_Disable(void);
    //-------------------------------------------------------------------------------------------------
    /// Enable coprocessor
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
//     void MDrv_COPRO_Enable(MS_U32 u32_ADR);
    //void MDrv_BEON_Init(MS_U16 u16BeonBinID, MS_U32 u32BEON_ADR, MS_U32 u32BEON_LEN);
    //void MDrv_BEON_Init_ByAddr(MS_U32 u32BEON_ADR, MS_U32 u32BEON_LEN);
//     void  MDrv_SetBEON_Host(MS_BOOL bHost);
    //-------------------------------------------------------------------------------------------------
    /// Reset and disable coprocessor
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
    void   MDrv_COPRO_Init_Front(void);
    //-------------------------------------------------------------------------------------------------
    /// Reset and disable coprocessor
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
    void   MDrv_COPRO_Init_End(MS_U32 u32_ADR);
    //-------------------------------------------------------------------------------------------------
    /// Get the information of coprocessor
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
    const COPRO_DrvInfo* MDrv_COPRO_GetInfo(void);
    //-------------------------------------------------------------------------------------------------
    /// Get the version of library version
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
//     COPRO_Result MDrv_COPRO_GetLibVer(const MSIF_Version **ppVersion);
    //-------------------------------------------------------------------------------------------------
    /// The the status of coprocessor
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
//     COPRO_Result MDrv_COPRO_GetStatus(COPRO_DrvStatus *pStatus);
    //-------------------------------------------------------------------------------------------------
    /// Setting the debug level
    /// @ingroup DRV_CPU
    void MDrv_COPRO_SetDbgLevel(MS_U8 u8Level);
    //-------------------------------------------------------------------------------------------------
    /// Get IO base for H.K.
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
//     MS_VIRT MDrv_COPRO_GetBase(void);
    //-------------------------------------------------------------------------------------------------
    /// Get the clock setting of CPU clock
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
    MS_U32 MDrv_CPU_QueryClock(void);
    //-------------------------------------------------------------------------------------------------
    /// Set the power status of power
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
//     MS_U32 MDrv_CPU_SetPowerState(EN_POWER_MODE u16PowerState);
    //-------------------------------------------------------------------------------------------------
    /// Get DQ infromation
    /// @ingroup DRV_CPU
    ////-------------------------------------------------------------------------------------------------
    MS_U32 MDrv_CPU_GetDqmemInfo(MS_U32 *pu32_DqmemAddr, MS_U32 *pu32_DqmemSize);

#if !defined(CONFIG_FRC)//frcr2_integration###
void MDrv_FRCR2_Init_Front(void);
void MDrv_FRCR2_Init_End(MS_U32 u32_ADR);
#endif

#ifdef __cplusplus
}
#endif

#endif // _DRV_CPU_H_

