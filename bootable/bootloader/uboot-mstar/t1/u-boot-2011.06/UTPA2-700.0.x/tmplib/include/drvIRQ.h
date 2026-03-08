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
/// @file   drvIRQ.h
/// @brief  IRQ Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_IRQ IRQ interface
   *  \ingroup  G_PERIPHERAL
     *! \defgroup G_IRQ_INIT Initialization Task relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_EnDecrypt En/Decrypt Task relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_SHA Hash relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_OTHER  other relative
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_ToBeModified IRQ api to be modified
     *  \ingroup  G_IRQ
     *! \defgroup G_IRQ_ToBeRemove IRQ api to be removed
     *  \ingroup  G_IRQ
*/

#ifndef _DRV_IRQ_H_
#define _DRV_IRQ_H_

#include"MsIRQ.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"

//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------
#define MSIF_DRVIRQ_LIB_CODE              {'I','R','Q',' '}    //Lib code
#define MSIF_DRVIRQ_LIBVER                {'0','1'}            //LIB version
#define MSIF_DRVIRQ_BUILDNUM              {'0','3'}            //Build Number
#define MSIF_DRVIRQ_CHANGELIST            {'0','0','5','5','5','6','7','9'} //P4 ChangeList Number

#define DRVIRQ_DRV_VERSION                /* Character String for DRV/API version             */  \
    MSIF_TAG,                           /* 'MSIF'                                           */  \
    MSIF_CLASS,                         /* '00'                                             */  \
    MSIF_CUS,                           /* 0x0000                                           */  \
    MSIF_MOD,                           /* 0x0000                                           */  \
    MSIF_CHIP,                                                                                  \
    MSIF_CPU,                                                                                   \
    MSIF_DRVIRQ_LIB_CODE,                  /* IP__                                             */  \
    MSIF_DRVIRQ_LIBVER,                    /* 0.0 ~ Z.Z                                        */  \
    MSIF_DRVIRQ_BUILDNUM,                  /* 00 ~ 99                                          */  \
    MSIF_DRVIRQ_CHANGELIST,                /* CL#                                              */  \
    MSIF_OS

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
// TICK_TIMER
#define MDRV_RISC32_TIMER   0x80
// PIC
#define MDRV_RISC32_UART    0x81

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
///Define CPU to CPU interrupt type
typedef enum
{
    E_IRQ_CPU0_2_CPU1,      ///< 8051 to AEON
    E_IRQ_CPU0_2_CPU2,      ///< 8051 to BEON
    E_IRQ_CPU1_2_CPU0,      ///< AEON to 8051
    E_IRQ_CPU1_2_CPU2,      ///< AEON to BEON
    E_IRQ_CPU2_2_CPU0,      ///< BEON to 8051
    E_IRQ_CPU2_2_CPU1,      ///< BEON to AEON
}IRQ_CPU_TYPE;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_Detech(MS_U32 u32Vector);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_Attach(MS_U32 u32Vector, void *pIntCb);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_Restore(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_MaskAll(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_UnMaskAll(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_Mask(MS_U32 u32Vector);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_UnMask(MS_U32 u32Vector);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// void MDrv_IRQ_NotifyCpu(IRQ_CPU_TYPE eType);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_IRQ_Init(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// MS_BOOL MDrv_IRQ_InISR(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_IRQ_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
// MS_U32 MDrv_IRQ_SetPowerState(EN_POWER_MODE u16PowerState);


#ifdef __cplusplus
}
#endif

#endif // _DRV_IRQ_H_

