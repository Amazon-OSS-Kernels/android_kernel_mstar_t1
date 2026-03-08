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
/// @file   drvPARFLASH.h
/// @brief  Parallel Flash Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_PARFLASH_H_
#define _DRV_PARFLASH_H_

#include "MsCommon.h"

#ifdef __cplusplus
extern "C"
{
#endif


//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_NORF_TAG                          {'M','S','I','F'}    //Version ID header
#define MSIF_NORF_CLASS                        {'1','0'}            //info Class code
#define MSIF_NORF_CUS                          0x6666               //Customer ID
#define MSIF_NORF_MOD                          0x0001               //Module ID
#define MSIF_NORF_CHIP                         0x000F               //CHIP ID: T3
#define MSIF_NORF_CPU                          '1'                  //CPU
#define MSIF_NORF_LIB_CODE                     {'N','O','R','F'}    //Lib code
#define MSIF_NORF_LIBVER                       {'0','1'}            //LIB version
#define MSIF_NORF_BUILDNUM                     {'0','1'}            //Build Number
#define MSIF_NORF_CHANGELIST                   {'0','0','1','1','4','5','3','4'} //P4 ChangeList Number
#define MSIF_NORF_OS                           '0'                  //OS

#define          NORF_DRV_VERSION                  /* Character String for DRV/API version             */  \
            MSIF_NORF_TAG,                         /* 'MSIF'                                           */  \
            MSIF_NORF_CLASS,                       /* '00'                                             */  \
            MSIF_NORF_CUS,                         /* 0x0000                                           */  \
            MSIF_NORF_MOD,                         /* 0x0000                                           */  \
            MSIF_NORF_CHIP,                                                                              \
            MSIF_NORF_CPU,                                                                               \
            MSIF_NORF_LIB_CODE,                    /* IP__                                             */  \
            MSIF_NORF_LIBVER,                      /* 0.0 ~ Z.Z                                        */  \
            MSIF_NORF_BUILDNUM,                    /* 00 ~ 99                                          */  \
            MSIF_NORF_CHANGELIST,                  /* CL#                                              */  \
            MSIF_NORF_OS

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

//flash support list, should sync to flash content table
typedef enum
{
    enPF_MX29LV400CT = 0,
    enPF_MX29LV400CB,
    enPF_MX29LV800CT,
    enPF_MX29LV800CB,
    enPF_MX29LV160CT,
    enPF_MX29LV160CB,
    enPF_MX29LV640BT,
    enPF_MX29LV640BB,
    enPF_MX29GL256EH,
    enPF_MX29GA128EH,
    enPF_MX29GA128EL,
    enPF_MX29GA256EH,
    enPF_MX29GA256EL,

    enPF_SPAN_S29GL128P,
    enPF_SPAN_S29GL256P,
    enPF_SPAN_S29GL512P,
    enPF_SPAN_S29GL01GP,

    enPF_SPAN_S29GL128P_S,
    enPF_SPAN_S29GL256P_S,
    enPF_SPAN_S29GL512P_S,

    evPF_NUM,
}PARFLASH_Device;

typedef struct
{
    MS_BOOL bTopBoot;
    MS_BOOL bUniformSecTbl;
    MS_U16 u16SecNum;   //sector num
    MS_U32 u32TotBytes; //flash size
    MS_U8* pu8SecTbl;   //Un-uniform sector table. if uniform sector table, pu8SecTbl = 0.
}PARFLASH_Content;

/// Parallel Flash information structure
typedef struct
{
    PARFLASH_Device enDevice;
    MS_BOOL bbytemode;
    MS_U16 u16SecNum;
    MS_U32 u32TotBytes;
}PARFLASH_Info;

typedef struct
{
    MS_BOOL bIsBusy;
}PARFLASH_DevSts;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
/// Get the information of Parallel Flash
/// @return the pointer to the driver information
//-------------------------------------------------------------------------------------------------
const PARFLASH_Info *MDrv_PARFLASH_GetInfo(void);

//------------------------------------------------------------------------------
/// Description: Show the NORF driver version
/// @param  ppVersion \b OUT: output NORF driver version
/// @return TRUE : succeed
/// @return FALSE : failed
//------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_GetLibVer(const MSIF_Version **ppVersion);

//------------------------------------------------------------------------------
/// Get Parallel Flash driver status
/// @param  pDrvStatus \b OUT: poniter to store the returning driver status
/// @return TRUE : succeed
/// @return FALSE : failed to get the driver status
//------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_GetStatus(PARFLASH_DevSts* pDrvStatus);

//------------------------------------------------------------------------------
/// Set detailed level of Parallel Flash driver debug message
/// @param u8DbgLevel  \b IN  debug level for Parallel Flash driver
/// @return TRUE : succeed
/// @return FALSE : failed to set the debug level
//------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_SetDbgLevel(MS_U8 u8DbgLevel);

//-------------------------------------------------------------------------------------------------
/// Initialize Parallel Flash
/// @return None
/// @note   called only once at system initialization
//-------------------------------------------------------------------------------------------------
void MDrv_PARFLASH_Init(void) ;

//-------------------------------------------------------------------------------------------------
/// Select chip
/// @param  u8cs    \b IN: Flash index. value: 0~1
/// @return FALSE : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_ChipSelect(MS_U8 u8cs);

//-------------------------------------------------------------------------------------------------
/// Reset Parallel Flash
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_Reset(void);

//-------------------------------------------------------------------------------------------------
/// Read ID in Parallel Flash
/// @param  pu16Data    \b IN: ID data, max length is 4 words
/// @param  pu8r_len    \b IN: ID data count
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_ReadID(MS_U16 *pu16Data, MS_U8 *pu8r_len);

//-------------------------------------------------------------------------------------------------
/// Read Sector protect condition
/// @param  u16sec      \b IN: Sector index
/// @param  pbProtect    \b Out: BOOL point to store protect condition.
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_SectorProtectVerify(MS_U16 u16sec, MS_BOOL* pbProtect);

//-------------------------------------------------------------------------------------------------
/// Read Sector protect condition
/// @param  u8addr      \b IN: CFI read address
/// @param  pu16data    \b IN: pointer to store CFI read data
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_CFI_Read(MS_U8 u8addr, MS_U16* pu16data);

//-------------------------------------------------------------------------------------------------
/// Read data from Parallel Flash
/// @param  u32Addr \b IN: start address (4-B aligned)
/// @param  pu8Data \b OUT: data ptr to store the read data
/// @param  u32Size \b IN: size in Bytes (4-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_Read(MS_U32 u32Addr, MS_U8 *pu8Data, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// Erase all sectors in Parallel Flash
/// @return TRUE : succeed
/// @return FALSE : fail before timeout
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_EraseChip(void);

//-------------------------------------------------------------------------------------------------
/// Erase certain sectors in Parallel Flash
/// @param  u16StartSec    \b IN: start sector
/// @param  u16EndSec      \b IN: end sector
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_EraseSec(MS_U16 u16StartSec, MS_U16 u16EndSec);

//-------------------------------------------------------------------------------------------------
/// Write data to Parallel Flash
/// @param  u32Addr \b IN: start address (4-B aligned)
/// @param  pu8Data \b IN: data to be written
/// @param  u32Size \b IN: size in Bytes (4-B aligned)
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_Write(MS_U32 u32Addr, MS_U8 *pu8Data, MS_U32 u32Size);

//-------------------------------------------------------------------------------------------------
/// Write write commend directly
/// @param  u8cycles   \b IN: Bus cycles of commend. Maximun value is 8.
/// @param  pu32Addrs  \b IN: Commend Address array to be written.
/// @param  pu16Data   \b IN: Commend Data array to be written.
/// @return TRUE : succeed
/// @return FALSE : fail before timeout or illegal parameters
/// @note   Not allowed in interrupt context
//-------------------------------------------------------------------------------------------------
MS_BOOL MDrv_PARFLASH_WriteCmdCycle(MS_U8 u8cycles, MS_U32 *pu32Addrs, MS_U16 *pu16Data);

//-------------------------------------------------------------------------------------------------
/// Getting Sector Offset address in byte unit
/// @param  u16sector   \b IN: Sector index
/// @return unsinged long value: Offset address
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PARFLASH_GetSectorOffset(MS_U16 u16sector);

//-------------------------------------------------------------------------------------------------
/// Getting Sector size in byte unit
/// @param  u16startsec   \b IN: Start Sector index
/// @param  u16endsec     \b IN: End Sector index
/// @return unsinged long value: sector size
//-------------------------------------------------------------------------------------------------
MS_U32 MDrv_PARFLASH_GetSectorSize(MS_U16 u16startsec, MS_U16 u16endsec);

#ifdef __cplusplus
}
#endif

#endif // _DRV_PARFLASH_H_
