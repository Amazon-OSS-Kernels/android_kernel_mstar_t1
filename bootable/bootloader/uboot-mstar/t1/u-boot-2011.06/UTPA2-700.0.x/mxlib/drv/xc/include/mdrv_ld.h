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
/// file    Mdrv_ld.h
/// @brief  local dimming Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_LD_H_
#define _MDRV_LD_H_


#include "MsCommon.h"
#include "MsOS.h"
#include "mhal_xc_chip_config.h"

// Internal Definition
#include "drvXC_IOPort.h"
#include "apiXC.h"
#include "hwreg_sc.h"
#include "MsCommon.h"
#include "mhal_xc_chip_config.h"
#include "xc_hwreg_utility2.h"
//#include "mdrv_spi.h"

#ifdef _MDRV_LD_C_
#define INTERFACE
#else
#define INTERFACE extern
#endif


#define MST_3xxx_U01        0x21    //Amber5 U01
#define MST_3xxx_U02        0x22    //Amber5 U02
#define MST_3xxx_U03        0x23    //Amber5 U03
#define MST_3xxx_U04        0x24    //Amber5 U04


#define ENABLE_TEST_DRV_LD  1
#define DRV_LD_USE_SWSPI    1
#define MAX_SPI_CMD_NUM     8
#define DRV_LD_SPI_BUF_MAX_SIZE     64//SPI_WRITE_BUF_SIZE

#if(HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
#define MEMORY_MASK 0x07FFFFFF
#else
#define MEMORY_MASK 0x01FFFFFF
#endif


#define LD_MIU_ALIGN  16


typedef enum
{
    LD_BLOCK_10,
    LD_BLOCK_16,
    LD_BLOCK_32,
    LD_BLOCK_72,
    LD_BLOCK_96,
    LD_BLOCK_480,
    LD_BLOCK_TSB,   //Need to check
    LD_BLOCK_DEFALUT,
    LD_BLOCK_NUMS,
}EN_LD_BLOCK_NUM;


typedef enum LD_LSFCONFIG_E
{
    E_LD_LSF_H_SHIFT  = BIT(0),
    E_LD_LSF_V_SHIFT  = BIT(1),
    E_LD_LSF_BILINEAR = BIT(11),
    E_LD_LSF_SIGNBIT  = BIT(14),
}LD_LSFCONFIG_E;
#define LD_LSFCONFIG_VALID (BIT(0)|BIT(1)|BIT(11)|BIT(14))

typedef enum LD_LSFLPF_HTAPS_E
{
    E_LD_LSFLPF_9TAPS     = 0,
    E_LD_LSFLPF_17TAPS    = 1,
    E_LD_LSFLPF_33TAPS    = 2,
}LD_LSFLPF_HTAPS_E;

typedef enum LD_LSF_OUT_MODE_E
{
    E_LD_LSF_OUT_INPUTDATA    = 0,
    E_LD_LSF_OUT_0x3FF        = 1,
    E_LD_LSF_OUT_DIGITALBL    = 2,
}LD_LSF_OUT_MODE_E;

typedef enum LD_COMP_OUT_MODE_E
{
    E_LD_COMP_OUT_NORMAL                      = 0,
    E_LD_COMP_OUT_HIGHLIGHT_OVERFLOW_DATA       = 1,
    E_LD_COMP_OUT_HIGHLIGHT_OVERFLOW_DATA_ONLY   = 2,
    E_LD_COMP_OUT_GRID                        = 3,
}LD_COMP_OUT_MODE_E;

#if(HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
typedef enum LD_DITHER3C_E
{
    E_LD_DITHER_COMPENSATION_OUT  = BIT(10),
    E_LD_DITHER_COMPENSATION  = BIT(13),
}LD_DITHER3C_E;
#define LD_DITHER3C_VALID (BIT(10)|BIT(13))

#else
typedef enum LD_DITHER3C_E
{
    E_LD_DITHER_COMPENSATION_OUT  = BIT(10),
    E_LD_DITHER_MIN_SELECT        = BIT(12),
    E_LD_DITHER_COMPENSATION      = BIT(13),
    E_LD_DITHER_SATURATION        = BIT(15),
}LD_DITHER3C_E;
#define LD_DITHER3C_VALID (BIT(10)|BIT(12)|BIT(13)|BIT(15))

typedef enum LD_DITHER3D_E
{
    E_LD_DITHER_VLPF      = BIT(12),
    E_LD_DITHER_HLPF      = BIT(13),
    E_LD_DITHER_OFFSET    = BIT(15),
}LD_DITHER3D_E;
#define LD_DITHER3D_VALID (BIT(12)|BIT(13)|BIT(15))
#endif

typedef enum LD_CS_MODE_E
{
    E_LD_CS_MODE_SW        = 0,
    E_LD_CS_MODE_HW0       = 1,
    E_LD_CS_MODE_HW1       = 2,
}LD_CS_MODE_E;

typedef enum LD_BL_GLOBAL_GAIN_MODE_E
{
    E_LD_BL_GLOBAL_GAIN_8BIT  = 3,
    E_LD_BL_GLOBAL_GAIN_16BIT = 1,
    E_LD_BL_GLOBAL_GAIN_12BIT = 0,
    E_LD_BL_GLOBAL_GAIN_4BIT  = 5,
    E_LD_BL_GLOBAL_GAIN_10BIT = 7,
}LD_BL_GLOBAL_GAIN_MODE_E;


typedef struct __attribute__((packed))
{
    MS_U16 u16PanelWidth; //panel width
    MS_U16 u16PanelHeight; //panel heigh

    MS_U8 u8LedNumH; // Led number H
    MS_U8 u8LedNumV; // Led number V

    MS_U8 u8LD_BackLightH; //backlight H
    MS_U8 u8LD_BackLightV; //backlight V

    MS_U8* pu8CompTable;  //compensation table
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy0;
#endif

    MS_U8* pu8Edge2DTable; //Edge2D table
#if !defined (__aarch64__)
    MS_U32 u32AlignmentDummy1;
#endif
    //EnuMSPI_CLOCK_SPEED eMSPIClockSpeed;
    MS_U8 u8SpiClockPolarity; // 0 or 1
    MS_U8 u8SpiClockPhase;    // 0 or 1

    MS_U8 u8SpiTime_VSyncWidth; // Unit is us
    MS_U8 u8SpiTime_VSync_To_FirstClock; // Unit is us
    MS_U8 u8SpiTime_Byte_to_Byte; // Unit is us

    MS_U8 u8SpiCmdLen;
    MS_U16 au16SpiCmd[MAX_SPI_CMD_NUM];
    MS_U8 u8CheckSumMode;

} ST_DRV_LD_INFO;


typedef struct __attribute__((packed))
{
    MS_U8 u8MIU;

    //LDF memory frame buffer
    MS_PHY u32LDFReadBuf0Base;
    MS_PHY u32LDFReadBuf1Base;

    //LDB memory frame buffer
    MS_PHY u32LDBWriteBuf0Base;
    MS_PHY u32LDBWriteBuf1Base;

    //SPI memory frame buffer
    MS_PHY u32SPIDataBuf0Base;
    MS_PHY u32SPIDataBuf1Base;

    //edge2 coeff base
    MS_PHY u32Edge2DCoeffBase;
} ST_LD_DRAM_ADDR;

#define LD_BUF_ReadDBuf_0   pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf0Base
#define LD_BUF_ReadDBuf_1   pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDFReadBuf1Base
#define LD_BUF_WriteBuf_0   pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf0Base
#define LD_BUF_WriteBuf_1   pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32LDBWriteBuf1Base
#define LD_BUF_SPIBuf_0     pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf0Base
#define LD_BUF_SPIBuf_1     pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32SPIDataBuf1Base
#define LD_BUF_Edge2DCoe    pXCResourcePrivate->stdrv_LD.gsLDDramBaseAddr.u32Edge2DCoeffBase

//local dimming miu read / write
#if (HW_DESIGN_LD_VER == 2 || HW_DESIGN_LD_VER == 3 || HW_DESIGN_LD_VER == 4)
#define MDrv_LD_MIUReadByte(_BUF_ID, _offset)             (*(volatile unsigned char *) (MsOS_PA2KSEG1(_BUF_ID + _offset) ))
#define MDrv_LD_MIURead2Bytes(_BUF_ID, _offset)           (*(volatile MS_U16 *) (MsOS_PA2KSEG1( _BUF_ID + _offset) ))
#define MDrv_LD_MIUWriteByte(_BUF_ID, _offset, _val)    (*((volatile MS_U8*)(MsOS_PA2KSEG1(_BUF_ID + _offset) ))) = (MS_U8)(_val)
#define MDrv_LD_MIUWrite2Bytes(_BUF_ID, _offset, _val)  (*((volatile MS_U16*)(MsOS_PA2KSEG1(_BUF_ID + _offset) ))) = (MS_U16)(_val)
#else
#define MDrv_LD_MIUReadByte(_BUF_ID, _offset)             (*(volatile unsigned char *) (MsOS_PA2KSEG0(_BUF_ID + _offset) ))
#define MDrv_LD_MIURead2Bytes(_BUF_ID, _offset)           (*(volatile MS_U16 *) (MsOS_PA2KSEG0( _BUF_ID + _offset) ))
#define MDrv_LD_MIUWriteByte(_BUF_ID, _offset, _val)    (*((volatile MS_U8*)(MsOS_PA2KSEG0(_BUF_ID + _offset) ))) = (MS_U8)(_val)
#define MDrv_LD_MIUWrite2Bytes(_BUF_ID, _offset, _val)  (*((volatile MS_U16*)(MsOS_PA2KSEG0(_BUF_ID + _offset) ))) = (MS_U16)(_val)
#endif

#if  (LD_ENABLE==1)
//-------------------------------------------------------------------------------------------------
/// LD initial
/// @param  void                \b IN: none
/// @return true or false
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_LD_Init(void *pInstance);

//-------------------------------------------------------------------------------------------------
/// LD initial, do not confused with MDrv_LD_Init()
/// the call flow is MDrv_XC_LD_Init --> MDrv_LD_Init()
/// @param  enLDPanelType                \b IN: Panel Type
/// @return true or false
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_XC_LD_Init(void *pInstance, EN_LD_PANEL_TYPE enLDPanelType);

//-------------------------------------------------------------------------------------------------
/// LD set memory address for internal use only
/// @param  u32LDFBase0                \b IN: LDF memeory0 address
/// @param  u32LDFBase1                \b IN: LDF memeory1 address
/// @param  u32LDBBase0                \b IN: LDB memeory0 address
/// @param  u32LDBBase1                \b IN: LDB memeory1 address
/// @param  u32EDGE2DBase              \b IN: EDGE2D memeory address
/// @param  u32LEDoffset               \b IN: LED  data offset
/// @return  void
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_XC_LD_SetMemoryAddress(void *pInstance,
                                           MS_PHY u32LDFBase0,
                                           MS_PHY u32LDFBase1,
                                           MS_PHY u32LDBBase0,
                                           MS_PHY u32LDBBase1,
                                           MS_PHY u32EDGE2DBase,
                                           MS_PHY u32LEDOffset);

//-------------------------------------------------------------------------------------------------
/// LD set memory MIU selection for internal use only
/// @param  u8MIU                \b IN: LD's memory miu selection
/// @return void
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_XC_LD_Set_MIUSel(void *pInstance, MS_U8 u8MIU);

//-------------------------------------------------------------------------------------------------
/// LD common initial
/// @param  void                \b IN: none
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_CommonInit(void *pInstance);
//-------------------------------------------------------------------------------------------------
/// Set LocalDimming Data  to spi (not used, set Local dimming data to spi handle in pm code)
/// @param  pu8LocalDimmingData                \b IN: LocalDimming Data
/// @param  u16DataLen                \b IN: LocalDimming Data  size
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_LD_SetLocalDimmingData(void *pInstance, MS_U8* pu8LocalDimmingData, MS_U16 u16DataLen);
//-------------------------------------------------------------------------------------------------
/// LocalDimming test function(Set LocalDimming Data  to spi test )(not used, set Local dimming data to spi handle in pm code)
/// @param  void                \b IN: none
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Test1(void *pInstance);
//-------------------------------------------------------------------------------------------------
/// LocalDimming test function (miu read/write test)
/// @param  void                \b IN: none
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Test2(void *pInstance);
//-------------------------------------------------------------------------------------------------
/// get local dimming info
/// @param  void      \b IN: none
/// @return  local dimming info
//-------------------------------------------------------------------------------------------------
INTERFACE ST_DRV_LD_INFO MDrv_LD_GetLDInfo(void *pInstance);
//-------------------------------------------------------------------------------------------------
/// set OSD enable
/// @param  u8OSDEnable      \b 1:enable ;0;disable
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_SetOSDEnable(void *pInstance, MS_U8 u8OSDEnable);
//-------------------------------------------------------------------------------------------------
/// get panel type
/// @param  void      \b IN: none
/// @return  panel type
//-------------------------------------------------------------------------------------------------
INTERFACE EN_LD_PANEL_TYPE Mdrv_LD_GetPanelType(void *pInstance);
//-------------------------------------------------------------------------------------------------
/// load panel compensation table
/// @param  pu8CompTable      \b IN: panel compensation table
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Load_Compensation_Table(void *pInstance, MS_U8* pu8CompTable,MS_U16 u16CompTableLength);
//-------------------------------------------------------------------------------------------------
/// load  edge2d table
/// @param  u8LEDWidth      \b IN: LED width
/// @param  u8LEDHeight      \b IN: LED heigh
/// @param  u8BLWidth      \b IN: backilight width
/// @param  u8BLHeight      \b IN: backilight heigh
/// @param  pu8Edge2DTable      \b IN: edge2d table
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Load_EDGE2D_Table(void *pInstance, MS_U8 u8LEDWidth, MS_U8 u8LEDHeight, MS_U8 u8BLWidth, MS_U8 u8BLHeight, MS_U8* pu8Edge2DTable);
#if(HW_DESIGN_LD_VER == 1)
//-------------------------------------------------------------------------------------------------
/// load  Hscaling table
/// @param  pu8CoefTable      \b IN: Hscaling table
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Load_Hscaling_Table(void *pInstance, MS_U8* pu8CoefTable);
//-------------------------------------------------------------------------------------------------
/// load  Vscaling table
/// @param  pu8CoefTable      \b IN: Vscaling table
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Load_Vscaling_Table(void *pInstance, MS_U8* pu8CoefTable);
//-------------------------------------------------------------------------------------------------
/// set DMA access x
/// @param  u8DmaIdx      \b IN: DMA access x index
/// @param  u8XStart      \b IN: DMA access x start
/// @param  u8XEnd      \b IN: DMA access x end
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrvl_LD_Set_DmaXOffset(void *pInstance, MS_U8 u8DmaIdx, MS_U8 u8XStart, MS_U8 u8XEnd);
//-------------------------------------------------------------------------------------------------
/// set DMA access y
/// @param  u8DmaIdx      \b IN: DMA access y index
/// @param  u8YStart      \b IN: DMA access y start
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Set_DmaYOffset(void *pInstance, MS_U8 u8DmaIdx, MS_U8 u8YStart);
#endif
//-------------------------------------------------------------------------------------------------
/// set LDF frame buffer
/// @param  u8BufIdx        \b IN: LDF buffer frame index
/// @param  u32LDFBase      \b IN: LDF buffer frame memeory base address
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Set_LDF_FrameBufBaseAddr(void *pInstance, MS_U8 u8BufIdx,MS_PHY u32LDFBase);
//-------------------------------------------------------------------------------------------------
/// get LDF frame buffer
/// @param  u8BufIdx        \b IN: LDF buffer frame index
/// @return   LDF buffer frame memeory base address
//-------------------------------------------------------------------------------------------------
INTERFACE MS_PHY MDrv_LD_Get_LDF_FrameBufBaseAddr(void *pInstance, MS_PHY u8BufIdx);
//-------------------------------------------------------------------------------------------------
/// Set LDB frame buffer
/// @param  u8BufIdx        \b IN: LDB buffer frame index
/// @param  u32LDFBase      \b IN: LDB buffer frame memeory base address
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Set_LDB_FrameBufBaseAddr(void *pInstance, MS_U8 u8BufIdx,MS_PHY u32LDBBase);
//-------------------------------------------------------------------------------------------------
/// get LDB frame buffer
/// @param  u8BufIdx        \b IN: LDB buffer frame index
/// @return   LDB buffer frame memeory base address
//-------------------------------------------------------------------------------------------------
INTERFACE MS_PHY MDrv_LD_Get_LDB_FrameBufBaseAddr(void *pInstance, MS_PHY u8BufIdx);
//-------------------------------------------------------------------------------------------------
/// Set Led  control data offset
/// @param  u32LEDoffset      \b IN: Led  control data offset
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Set_LEDData_BaseOffset(void *pInstance, MS_PHY u32LEDoffset);
//-------------------------------------------------------------------------------------------------
/// Get spi data memeory base address
/// @param  u8BufIdx        \b IN: LDB buffer frame index
/// @return   spi data memeory base address
//-------------------------------------------------------------------------------------------------
INTERFACE MS_PHY MDrv_LD_Get_SPIData_BaseAddr(void *pInstance, MS_PHY u8BufIdx);
//-------------------------------------------------------------------------------------------------
/// set EDGE2D frame buffer
/// @param  u32EDGE2DBase      \b IN: EDGE2D memeory base address
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Set_EDGE2D_BaseAddr(void *pInstance, MS_PHY u32EDGE2DBase);
//-------------------------------------------------------------------------------------------------
/// set EDGE2D frame buffer
/// @param  none
/// @return  EDGE2D memeory base address
//-------------------------------------------------------------------------------------------------
INTERFACE MS_PHY MDrv_LD_Get_EDGE2D_BaseAddr(void *pInstance);
//-------------------------------------------------------------------------------------------------
/// set local dimming enable
/// @param  bEn         \b IN: 1 :enable ;0 :disable
/// @return  none
//-------------------------------------------------------------------------------------------------
INTERFACE void MDrv_LD_Set_LocalDimmingEn(void *pInstance, MS_BOOL bEn);
//-------------------------------------------------------------------------------------------------
/// get local dimming enable
/// @param  none
/// @return  1 :enable ;0 :disable
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_LD_Get_LocalDimmingEn(void *pInstance);
//-------------------------------------------------------------------------------------------------
/// LD set panel type
/// @param  eLdPanelType                \b IN: panel type
/// @return E_APIXC_RET_OK or E_APIXC_RET_FAIL
//-------------------------------------------------------------------------------------------------
INTERFACE MS_BOOL MDrv_XC_LD_SetPanelType(void *pInstance, EN_LD_PANEL_TYPE eLDPanelType );

#endif

#undef INTERFACE

#endif

