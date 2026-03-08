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


//-------------------------------------------------------------------------------------------------
//  Include Files
//-------------------------------------------------------------------------------------------------
#include "MsCommon.h"
#include "regCHIP.h"
#include "halCHIP.h"
#include "halDMD_VD_MBX.h"
#include "drvMMIO.h"
#include "ULog.h"
//-------------------------------------------------------------------------------------------------
//  Driver Compiler Options
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Defines
//-------------------------------------------------------------------------------------------------
#ifdef MCU_AEON                             // Non-OS
    #define BASEADDR_RIU 0xA0000000UL
#else                                       // ecos
    #define BASEADDR_RIU 0xBF800000UL
#endif

#define RIU_MACRO_START             do {
#define RIU_MACRO_END               } while (0)

// Address bus of RIU is 16 bits.

#define RIU_READ_BYTE(addr)         ( READ_BYTE( _hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr) ) )
#define RIU_READ_2BYTE(addr)        ( READ_WORD( _hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr) ) )
#define RIU_WRITE_BYTE(addr, val)   { WRITE_BYTE( _hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr), val) }
#define RIU_WRITE_2BYTE(addr, val)  { WRITE_WORD( _hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr + (addr), val) }

// Standard Form

#define RIU_ReadByte( u32Reg )      RIU_READ_BYTE(((u32Reg) << 1) - ((u32Reg) & 1))

#define RIU_Read2Byte( u32Reg )    (RIU_READ_2BYTE((u32Reg)<<1))

#define RIU_ReadRegBit( u32Reg, u8Mask )   (RIU_READ_BYTE(((u32Reg)<<1) - ((u32Reg) & 1)) & (u8Mask))

#define RIU_WriteRegBit( u32Reg, bEnable, u8Mask )                                      \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) , (bEnable) ? (RIU_READ_BYTE(  (((u32Reg) <<1) - ((u32Reg) & 1))  ) |  (u8Mask)) :                           \
                                (RIU_READ_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)) ) & ~(u8Mask)));                            \
    RIU_MACRO_END

#define RIU_WriteByte( u32Reg, u8Val )                                                  \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE(((u32Reg) << 1) - ((u32Reg) & 1), u8Val);                           \
    RIU_MACRO_END

#define RIU_Write2Byte( u32Reg, u16Val )                                                \
    RIU_MACRO_START                                                                     \
    if ( ((u32Reg) & 0x01) )                                                                            \
    {                                                                                                                \
        RIU_WRITE_BYTE(((u32Reg) << 1) - 1, (MS_U8)((u16Val)));                                     \
        RIU_WRITE_BYTE(((u32Reg) + 1) << 1, (MS_U8)((u16Val) >> 8));                             \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        RIU_WRITE_2BYTE( ((u32Reg)<<1) ,  u16Val);                                     \
    }                                                                                   \
    RIU_MACRO_END

#define RIU_WriteByteMask( u32Reg, u8Val, u8Msk )                                       \
    RIU_MACRO_START                                                                     \
    RIU_WRITE_BYTE( (((u32Reg) <<1) - ((u32Reg) & 1)), (RIU_READ_BYTE((((u32Reg) <<1) - ((u32Reg) & 1))) & ~(u8Msk)) | ((u8Val) & (u8Msk)));                   \
    RIU_MACRO_END

#define DMD_MBX_TIMEOUT 200
//-------------------------------------------------------------------------------------------------
//  Local Structures
//-------------------------------------------------------------------------------------------------
typedef struct
{
    MS_PHYADDR  u32DMD_VD_MBX_BaseAddr;
    MS_BOOL     bBaseAddrInitialized;
    MS_U8       u8DMD_VD_MBX_Type;
} hal_DMD_VD_MBX_t;

//-------------------------------------------------------------------------------------------------
//  Global Variables
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Local Variables
//-------------------------------------------------------------------------------------------------
static hal_DMD_VD_MBX_t _hal_DMD_VD_MBX =
{
    .u32DMD_VD_MBX_BaseAddr = BASEADDR_RIU,
    .bBaseAddrInitialized = 0,
    .u8DMD_VD_MBX_Type = -1,
};
static MS_U8 MBX_SetType;
//-------------------------------------------------------------------------------------------------
//  Debug Functions
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Local Functions
//-------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------
//  Global Functions
//-------------------------------------------------------------------------------------------------
MS_BOOL HAL_SYS_DMD_VD_MBX_Init(void)
{
    MS_VIRT u32NonPMBank;
    MS_PHY u32NonPMBankSize;
    if (!MDrv_MMIO_GetBASE( &u32NonPMBank, &u32NonPMBankSize, MS_MODULE_AVD))
    {
        ULOGD("DEMOD","IOMap failure to get MAP_NONPM_BANK\n");
        return FALSE;
    }

    _hal_DMD_VD_MBX.u32DMD_VD_MBX_BaseAddr=u32NonPMBank;
    _hal_DMD_VD_MBX.bBaseAddrInitialized = 1;
    _hal_DMD_VD_MBX.u8DMD_VD_MBX_Type = -1;
    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_SYS_DMD_VD_MBX_Init %lx\n",u32NonPMBank);
    #endif
    return TRUE;
}

MS_U8 HAL_SYS_DMD_VD_MBX_GetType(void)
{
    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_SYS_DMD_VD_MBX_GetType %d\n",MBX_SetType);
    #endif
    //return RIU_ReadByte(0x1E3E);
    return MBX_SetType;
}

void HAL_SYS_DMD_VD_MBX_SetType(MS_U8 u8Value)
{
    MBX_SetType =  u8Value;
    //RIU_WriteByte(0x1E3E, u8Value);
}

#if (DMD_VD_MBX_CHIP_VERSION == DMD_VD_MBX_CHIP_T3)
// ATV
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WaitReady(void)
{
    MS_U32 u32StartTime=MsOS_GetSystemTime();
    while (MsOS_GetSystemTime()-u32StartTime < 10)
    {
        if (!RIU_ReadByte(DemodCmd)) return TRUE;
        MsOS_DelayTaskUs(500);
    }
    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_SYS_DMD_VD_MBX_ATV_WaitReady Fail\n");
    #endif
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_ReadByte(MS_U32 u32Reg, MS_U8 *u8Value)
{
    if (HAL_SYS_DMD_VD_MBX_ATV_WaitReady())
    {
        RIU_WriteByte(DemodAdrL, u32Reg&0xFF);
        RIU_WriteByte(DemodAdrH, (u32Reg>>8)&0xFF);
        RIU_WriteByte(DemodCmd, DemodCmdRdReg);

        if (HAL_SYS_DMD_VD_MBX_ATV_WaitReady())
        {
            *u8Value = RIU_ReadByte(DemodData);
            return TRUE;
        }
    }
    #ifdef MS_DEBUG
    ULOGD("DEMOD","HAL_SYS_DMD_VD_MBX_ATV_ReadByte Fail\n");
    #endif
    *u8Value = 0;
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WriteByte(MS_U32 u32Reg, MS_U8 u8Val)
{
    if (HAL_SYS_DMD_VD_MBX_ATV_WaitReady())
    {
        RIU_WriteByte(DemodAdrL, u32Reg&0xFF);
        RIU_WriteByte(DemodAdrH, (u32Reg>>8)&0xFF);
        RIU_WriteByte(DemodData, u8Val);
        RIU_WriteByte(DemodCmd, DemodCmdWrReg);
        return TRUE;
    }
    return FALSE;
}
#else
MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WaitReady(void)
{
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_ReadByte(MS_U32 u32Reg, MS_U8 *u8Value)
{
    return FALSE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_ATV_WriteByte(MS_U32 u32Reg, MS_U8 u8Val)
{
    return FALSE;
}
#endif

// DVBT & DVBC
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WaitReady(void)
{
    MS_U32 u32StartTime=MsOS_GetSystemTime();
    while (RIU_ReadByte(MBRegBase + 0x00)) // wait VDMCU ready
    {
        if (MsOS_Timer_DiffTimeFromNow(u32StartTime)>DMD_MBX_TIMEOUT)
        {
            ULOGD("DEMOD","HAL_SYS_DMD_VD_MBX_DVB_WaitReady Timeout\n");
            return FALSE;
        }
    }
    return TRUE;
}

void HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake(void)
{
    MS_U32 u32StartTime=MsOS_GetSystemTime();
    while(RIU_ReadByte(MBRegBase + 0x00) != 0xFF)           // wait MB_CNTL set done
    {
        if (MsOS_Timer_DiffTimeFromNow(u32StartTime)>DMD_MBX_TIMEOUT)
        {
            ULOGD("DEMOD","HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake Timeout\n");
            break;
        }
    }
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadByte(MS_U16 u16Addr, MS_U8 *u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x00, 0x01);                 // MB_CNTL set read mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                         // assert interrupt to VD MCU51
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                         // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    *u8Value = RIU_ReadByte(MBRegBase + 0x03);             // REG_DATA get
    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteByte(MS_U32 u16Addr, MS_U8 u8Data)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x03, u8Data);               // REG_DATA
    RIU_WriteByte(MBRegBase + 0x00, 0x02);                 // MB_CNTL set write mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                         // assert interrupt to VD MCU51
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                         // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear
    return TRUE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_ReadDspReg(MS_U32 u16Addr, MS_U8 *u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));    // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);           // ADDR_L
    RIU_WriteByte(MBRegBase + 0x00, 0x03);                  // MB_CNTL set read mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                 // assert interrupt to DMD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                 // de-assert interrupt to DMD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    *u8Value = RIU_ReadByte(MBRegBase + 0x03);              // REG_DATA get
    RIU_WriteByte(MBRegBase + 0x00, 0x00);                  // MB_CNTL clear

    return TRUE;
}


MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_WriteDspReg(MS_U32 u16Addr, MS_U8 u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));        // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);               // ADDR_L
    RIU_WriteByte(MBRegBase + 0x03, u8Value);                    // REG_DATA
    RIU_WriteByte(MBRegBase + 0x00, 0x04);                      // MB_CNTL set write mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                     // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                     // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    RIU_WriteByte(MBRegBase + 0x00, 0x00);                      // MB_CNTL clear

    return TRUE;
}

MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_ReadReg(MS_U16 u16Addr, MS_U8 *u8Value)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x00, 0x05);                 // MB_CNTL set read mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                // assert interrupt to DMD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                // de-assert interrupt to DMD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    *u8Value = RIU_ReadByte(MBRegBase + 0x03);             // REG_DATA get
    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

//-----------------------------------------------------------------------
MS_BOOL HAL_SYS_DMD_VD_MBX_DVB_DBG_WriteReg(MS_U32 u16Addr, MS_U8 u8Data)
{
    if (!HAL_SYS_DMD_VD_MBX_DVB_WaitReady()) return FALSE;

    RIU_WriteByte(MBRegBase + 0x02, (MS_U8)(u16Addr >> 8));   // ADDR_H
    RIU_WriteByte(MBRegBase + 0x01, (MS_U8)u16Addr);          // ADDR_L
    RIU_WriteByte(MBRegBase + 0x03, u8Data);               // REG_DATA
    RIU_WriteByte(MBRegBase + 0x00, 0x06);                 // MB_CNTL set write mode

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                // de-assert interrupt to VD MCU51

    HAL_SYS_DMD_VD_MBX_DVB_WaitHandShake();

    RIU_WriteByte(MBRegBase + 0x00, 0x00);                 // MB_CNTL clear

    return TRUE;
}

// ATSC
void HAL_SYS_DMD_VD_MBX_ATSC_WriteByte(MS_U16 u16Addr, MS_U8 u8Data)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag;

    RIU_WriteByte(MBRegBase + 0x00, (u16Addr&0xff));
    RIU_WriteByte(MBRegBase + 0x01, (u16Addr>>8));
    RIU_WriteByte(MBRegBase + 0x10, u8Data);
    RIU_WriteByte(MBRegBase + 0x1E, 0x01);

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                     // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                     // de-assert interrupt to VD MCU51

    for ( u8CheckCount=0; u8CheckCount < 10 ; u8CheckCount++ )
    {
        u8CheckFlag = RIU_ReadByte(MBRegBase + 0x1E);
        if ((u8CheckFlag&0x01)==0)
             break;
        MsOS_DelayTask(1);
    }
}

MS_U8 HAL_SYS_DMD_VD_MBX_ATSC_ReadByte(MS_U16 u16Addr)
{
    MS_U8 u8CheckCount;
    MS_U8 u8CheckFlag;
    MS_U8 u8Value;

    RIU_WriteByte(MBRegBase + 0x00, (u16Addr&0xff));
    RIU_WriteByte(MBRegBase + 0x01, (u16Addr>>8));
    RIU_WriteByte(MBRegBase + 0x1E, 0x02);

    RIU_WriteByte(DMDMcuBase + 0x03, 0x02);                     // assert interrupt to VD MCU51 //mick
    RIU_WriteByte(DMDMcuBase + 0x03, 0x00);                     // de-assert interrupt to VD MCU51

    for ( u8CheckCount=0; u8CheckCount < 10 ; u8CheckCount++ )
    {
        u8CheckFlag = RIU_ReadByte(MBRegBase + 0x1E);
        if ((u8CheckFlag&0x02)==0)
        {
            u8Value = RIU_ReadByte(MBRegBase + 0x10);
            return u8Value;
        }
        MsOS_DelayTask(1);
    }

    return 0;
}
