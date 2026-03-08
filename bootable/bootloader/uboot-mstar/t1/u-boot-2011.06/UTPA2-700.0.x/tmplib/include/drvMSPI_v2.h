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

//////////////////////////////////////////////////////////////////////////////////////////////////
///
/// @file   drvMSPI_v2.h
/// @brief  MSPI Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _DRV_MSPI_V2_H_
#define _DRV_MSPI_V2_H_

#ifdef __cplusplus
extern "C"
{
#endif

//-------------------------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------------------------
// below for utopia20 structure


typedef enum {
    MDrv_CMD_MSPI_Init_Ext,
    MDrv_CMD_MSPI_Init,
    MDrv_CMD_MSPI_Read,
    MDrv_CMD_MSPI_Write,
    MDrv_CMD_MSPI_DCConfig,
    MDrv_CMD_MSPI_CLKConfig,
    MDrv_CMD_MSPI_FRAMEConfig,
    MDrv_CMD_MSPI_SlaveEnable,
    MDrv_CMD_MSPI_SetPowerState,
    MDrv_CMD_MSPI_DbgLEvel,
    MDrv_CMD_MasterSPI_Init_Ext,
    MDrv_CMD_MasterSPI_Init,
    MDrv_CMD_MasterSPI_Read,
    MDrv_CMD_MasterSPI_Write,
    MDrv_CMD_MasterSPI_DCConfig,
    MDrv_CMD_MasterSPI_CLKConfig,
    MDrv_CMD_MasterSPI_FRAMEConfig,
    MDrv_CMD_MasterSPI_SlaveEnable,
    MDrv_CMD_MasterSPI_CSPadConfig,
    MDrv_CMD_MasterSPI_MaxClkConfig,
} eMSPIIoctlOpt;

//-------------------------------------------------------------------------------------------------
// Macros
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
// Type and Structure Declaration
//-------------------------------------------------------------------------------------------------

typedef struct _MPSI_INIT_EXT
{
    MS_U8 u8HWNum;
}MSPI_INIT_EXT, *PMSPI_INIT_EXT;

typedef struct _MSPI_INIT
{
    MSPI_config *ptMSPIConfig;
    MS_U8 u8HWNum;
}MSPI_INIT,     *PMSPI_INIT;

typedef struct _MSPI_READ
{
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MSPI_READ,    *PMSPI_READ;

typedef struct _MSPI_WRITE
{
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MSPI_WRITE,   *PMSPI_WRITE;

typedef struct _MSPI_DCCONFIG
{
    MSPI_DCConfig *ptDCConfig;
}MSPI_DCCONFIG, *PMSPI_DCCONFIG;

typedef struct _MSPI_CLKCONFIG
{
    MSPI_CLKConfig *ptCLKConfig;
}MSPI_CLKCONFIG, *PMSPI_CLKCONFIG;

typedef struct _MSPI_FRAMECONFIG
{
    MSPI_FrameConfig *ptFrameConfig;
}MSPI_FRAMECONFIG, *PMSPI_FRAMECONFIG;

typedef struct _MSPI_SLAVE_ENABLE
{
    MS_BOOL bEnable;
}MSPI_SLAVE_ENABLE, *PMSPI_SLAVE_ENABLE;

typedef struct _MSPI_SETPOWERSTATE
{
    EN_POWER_MODE enPowerState;
}MSPISETPOWERSTATE, *PMSPISETPOWERSTATE;

typedef struct _MASTERSPI_READ
{
    MS_U32 u32DevID;
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MASTERSPI_READ,    *PMASTERSPI_READ;

typedef struct _MASTERSPI_WRITE
{
    MS_U32 u32DevID;
    MS_U8 *pu8Data;
    MS_U16 u16Size;
}MASTERSPI_WRITE,   *PMASTERSPI_WRITE;

typedef struct _MASTERSPI_DCCONFIG
{
    MS_U32 u32DevID;
    MSPI_DCConfig *ptDCConfig;
}MASTERSPI_DCCONFIG, *PMASTERSPI_DCCONFIG;

typedef struct _MASTERSPI_CLKCONFIG
{
    MS_U32 u32DevID;
    MSPI_CLKConfig *ptCLKConfig;
}MASTERSPI_CLKCONFIG, *PMASTERSPI_CLKCONFIG;

typedef struct _MASTERSPI_FRAMECONFIG
{
    MS_U32 u32DevID;
    MSPI_FrameConfig *ptFrameConfig;
}MASTERSPI_FRAMECONFIG, *PMASTERSPI_FRAMECONFIG;

typedef struct _MASTERSPI_SLAVE_ENABLE
{
    MS_U32 u32DevID;
    MS_BOOL bEnable;
}MASTERSPI_SLAVE_ENABLE, *PMASTERSPI_SLAVE_ENABLE;

typedef struct _MASTERSPI_CSPADCONFIG
{
    MS_U32 u32DevID;
    MS_U32 u32CsPad;
}MASTERSPI_CS_PADCONFIG, *PMASTERSPI_CSPADCONFIG;

typedef struct _MASTERSPI_MAXCLKCONFIG
{
    MS_U32 u32DevID;
    MS_U32 u32MaxClk;
}MASTERSPI_MAXCLKCONFIG, *PMASTERSPI_MAXCLKCONFIG;

typedef struct _MSPI_SETDBGLEVEL
{
    MS_U8 u8DbgLevel;
}MSPI_SETDBGLEVEL,   *PMSPI_SETDBGLEVEL;

//-------------------------------------------------------------------------------------------------
// Extern Global Variabls
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
// Extern Functions
//-------------------------------------------------------------------------------------------------

//////////////////////////////////////////////////////////////

#ifdef __cplusplus
}
#endif

#endif // _DRV_MSPI_V2_H_

