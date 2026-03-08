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

#ifndef _HAL_HWI2C_H_
#define _HAL_HWI2C_H_

////////////////////////////////////////////////////////////////////////////////
/// @file halHWI2C.h
/// @brief MIIC control functions
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Define & data type
////////////////////////////////////////////////////////////////////////////////
//v: value n: shift n bits
//v: value n: shift n bits
#define _LShift(v, n)       ((v) << (n))
#define _RShift(v, n)       ((v) >> (n))

#define HIGH_BYTE(val)      (MS_U8)_RShift((val), 8)
#define LOW_BYTE(val)       ((MS_U8)((val) & 0xFF))

#define __BIT(x)    ((MS_U8)_LShift(1, x))
#define __BIT0       __BIT(0)
#define __BIT1       __BIT(1)
#define __BIT2       __BIT(2)
#define __BIT3       __BIT(3)
#define __BIT4       __BIT(4)
#define __BIT5       __BIT(5)
#define __BIT6       __BIT(6)
#define __BIT7       __BIT(7)

#define HWI2C_SET_RW_BIT(bRead, val) ((bRead) ? ((val) | __BIT0) : ((val) & ~__BIT0))

#define HAL_HWI2C_PORTS         6
#define HAL_HWI2C_PORT0         0
#define HAL_HWI2C_PORT1         1
#define HAL_HWI2C_PORT2         2
#define HAL_HWI2C_PORT3         3
#define HAL_HWI2C_PORT4         4
#define HAL_HWI2C_PORT5         5



typedef enum _HAL_HWI2C_STATE
{
	E_HAL_HWI2C_STATE_IDEL = 0,
	E_HAL_HWI2C_STATE_START,
	E_HAL_HWI2C_STATE_WRITE,
	E_HAL_HWI2C_STATE_READ,
	E_HAL_HWI2C_STATE_INT,
	E_HAL_HWI2C_STATE_WAIT,
	E_HAL_HWI2C_STATE_STOP
} HAL_HWI2C_STATE;


typedef enum _HAL_HWI2C_PORT
{
    E_HAL_HWI2C_PORT0_0 = 0, //disable port 0
    E_HAL_HWI2C_PORT0_1,
    E_HAL_HWI2C_PORT0_2,
    E_HAL_HWI2C_PORT0_3,
    E_HAL_HWI2C_PORT0_4,
    E_HAL_HWI2C_PORT0_5,
    E_HAL_HWI2C_PORT0_6,
    E_HAL_HWI2C_PORT0_7,

    E_HAL_HWI2C_PORT1_0,  //disable port 1
    E_HAL_HWI2C_PORT1_1,
    E_HAL_HWI2C_PORT1_2,
    E_HAL_HWI2C_PORT1_3,
    E_HAL_HWI2C_PORT1_4,
    E_HAL_HWI2C_PORT1_5,
    E_HAL_HWI2C_PORT1_6,
    E_HAL_HWI2C_PORT1_7,

    E_HAL_HWI2C_PORT2_0,  //disable port 2
    E_HAL_HWI2C_PORT2_1,
    E_HAL_HWI2C_PORT2_2,
    E_HAL_HWI2C_PORT2_3,
    E_HAL_HWI2C_PORT2_4,
    E_HAL_HWI2C_PORT2_5,
    E_HAL_HWI2C_PORT2_6,
    E_HAL_HWI2C_PORT2_7,

    E_HAL_HWI2C_PORT3_0, //disable port 3
    E_HAL_HWI2C_PORT3_1,
    E_HAL_HWI2C_PORT3_2,
    E_HAL_HWI2C_PORT3_3,
    E_HAL_HWI2C_PORT3_4,
    E_HAL_HWI2C_PORT3_5,
    E_HAL_HWI2C_PORT3_6,
    E_HAL_HWI2C_PORT3_7,

	E_HAL_HWI2C_PORT4_0, //disable port 4
    E_HAL_HWI2C_PORT4_1,
    E_HAL_HWI2C_PORT4_2,
    E_HAL_HWI2C_PORT4_3,
    E_HAL_HWI2C_PORT4_4,
    E_HAL_HWI2C_PORT4_5,
    E_HAL_HWI2C_PORT4_6,
    E_HAL_HWI2C_PORT4_7,

	E_HAL_HWI2C_PORT5_0, //disable port 5
    E_HAL_HWI2C_PORT5_1,
    E_HAL_HWI2C_PORT5_2,
    E_HAL_HWI2C_PORT5_3,
    E_HAL_HWI2C_PORT5_4,
    E_HAL_HWI2C_PORT5_5,
    E_HAL_HWI2C_PORT5_6,
    E_HAL_HWI2C_PORT5_7,

    E_HAL_HWI2C_PORT_NOSUP
}HAL_HWI2C_PORT;

typedef enum _HAL_HWI2C_CLKSEL
{
    E_HAL_HWI2C_CLKSEL_HIGH = 0,
    E_HAL_HWI2C_CLKSEL_NORMAL,
    E_HAL_HWI2C_CLKSEL_SLOW,
    E_HAL_HWI2C_CLKSEL_VSLOW,
    E_HAL_HWI2C_CLKSEL_USLOW,
    E_HAL_HWI2C_CLKSEL_UVSLOW,
    E_HAL_HWI2C_CLKSEL_NOSUP
}HAL_HWI2C_CLKSEL;

typedef enum _HAL_HWI2C_CLK
{
    E_HAL_HWI2C_CLK_DIV4 = 1, //750K@12MHz
    E_HAL_HWI2C_CLK_DIV8,     //375K@12MHz
    E_HAL_HWI2C_CLK_DIV16,    //187.5K@12MHz
    E_HAL_HWI2C_CLK_DIV32,    //93.75K@12MHz
    E_HAL_HWI2C_CLK_DIV64,    //46.875K@12MHz
    E_HAL_HWI2C_CLK_DIV128,   //23.4375K@12MHz
    E_HAL_HWI2C_CLK_DIV256,   //11.71875K@12MHz
    E_HAL_HWI2C_CLK_DIV512,   //5.859375K@12MHz
    E_HAL_HWI2C_CLK_DIV1024,  //2.9296875K@12MHz
    E_HAL_HWI2C_CLK_NOSUP
}HAL_HWI2C_CLK;

typedef enum {
    E_HAL_HWI2C_READ_MODE_DIRECT,                       ///< first transmit slave address + reg address and then start receive the data */
    E_HAL_HWI2C_READ_MODE_DIRECTION_CHANGE,             ///< slave address + reg address in write mode, direction change to read mode, repeat start slave address in read mode, data from device
    E_HAL_HWI2C_READ_MODE_DIRECTION_CHANGE_STOP_START,  ///< slave address + reg address in write mode + stop, direction change to read mode, repeat start slave address in read mode, data from device
    E_HAL_HWI2C_READ_MODE_MAX
} HAL_HWI2C_ReadMode;

typedef enum _HAL_HWI2C_DMA_ADDRMODE
{
    E_HAL_HWI2C_DMA_ADDR_NORMAL = 0,
    E_HAL_HWI2C_DMA_ADDR_10BIT,
    E_HAL_HWI2C_DMA_ADDR_MAX,
}HAL_HWI2C_DMA_ADDRMODE;

typedef enum _HAL_HWI2C_DMA_MIUPRI
{
    E_HAL_HWI2C_DMA_PRI_LOW = 0,
    E_HAL_HWI2C_DMA_PRI_HIGH,
    E_HAL_HWI2C_DMA_PRI_MAX,
}HAL_HWI2C_DMA_MIUPRI;

typedef enum _HAL_HWI2C_DMA_MIUCH
{
    E_HAL_HWI2C_DMA_MIU_CH0 = 0,
    E_HAL_HWI2C_DMA_MIU_CH1,
    E_HAL_HWI2C_DMA_MIU_MAX,
}HAL_HWI2C_DMA_MIUCH;

typedef struct _HAL_HWI2C_PinCfg
{
    MS_U32  u32Reg;    /// register
    MS_U8   u8BitPos;  /// bit position
    MS_BOOL bEnable;   /// enable or disable
}HAL_HWI2C_PinCfg;

typedef struct _HAL_HWI2C_PortCfg //Synchronize with drvHWI2C.h
{
    MS_U32                  u32DmaPhyAddr;  /// DMA physical address
    HAL_HWI2C_DMA_ADDRMODE  eDmaAddrMode;   /// DMA address mode
    HAL_HWI2C_DMA_MIUPRI    eDmaMiuPri;     /// DMA miu priroity
    HAL_HWI2C_DMA_MIUCH     eDmaMiuCh;      /// DMA miu channel
    MS_BOOL                 bDmaEnable;     /// DMA enable

    HAL_HWI2C_PORT          ePort;          /// number
    HAL_HWI2C_CLKSEL        eSpeed;         /// clock speed
    HAL_HWI2C_ReadMode      eReadMode;      /// read mode
    MS_BOOL                 bEnable;        /// enable

}HAL_HWI2C_PortCfg;

/// I2C Configuration for initialization
typedef struct _HAL_HWI2C_CfgInit //Synchronize with drvHWI2C.h
{
    HAL_HWI2C_PortCfg   sCfgPort[8];    /// port cfg info
    HAL_HWI2C_PinCfg    sI2CPin;        /// pin info
    HAL_HWI2C_CLKSEL    eSpeed;         /// speed
    HAL_HWI2C_PORT      ePort;          /// port
    HAL_HWI2C_ReadMode  eReadMode;      /// read mode
    
}HAL_HWI2C_CfgInit;

////////////////////////////////////////////////////////////////////////////////
// Extern function
////////////////////////////////////////////////////////////////////////////////
void HAL_HWI2C_ExtraDelay(MS_U32 u32Us);
void HAL_HWI2C_SetIOMapBase(MS_VIRT u32Base);
MS_U8 HAL_HWI2C_ReadByte(MS_VIRT u32RegAddr);
MS_U16 HAL_HWI2C_Read2Byte(MS_U32 u32RegAddr);
MS_U32 HAL_HWI2C_Read4Byte(MS_U32 u32RegAddr);
MS_BOOL HAL_HWI2C_WriteByte(MS_U32 u32RegAddr, MS_U8 u8Val);
MS_BOOL HAL_HWI2C_Write2Byte(MS_U32 u32RegAddr, MS_U16 u16Val);
MS_BOOL HAL_HWI2C_Write4Byte(MS_U32 u32RegAddr, MS_U32 u32Val);
MS_BOOL HAL_HWI2C_WriteRegBit(MS_U32 u32RegAddr, MS_U8 u8Mask, MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_WriteByteMask(MS_U32 u32RegAddr, MS_U8 u8Val, MS_U8 u8Mask);

MS_BOOL HAL_HWI2C_Init_Chip(void);
MS_BOOL HAL_HWI2C_IsMaster(void);
MS_BOOL HAL_HWI2C_Master_Enable(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_SetPortRegOffset(HAL_HWI2C_PORT ePort, MS_U32* pu32PortOffset);
MS_BOOL HAL_HWI2C_GetPortIdxByOffset(MS_U32 u32PortOffset, MS_U8* pu8Port);
MS_BOOL HAL_HWI2C_GetPortIdxByPort(HAL_HWI2C_PORT ePort, MS_U8* pu8Port);
MS_BOOL HAL_HWI2C_SelectPort(HAL_HWI2C_PORT ePort);
MS_BOOL HAL_HWI2C_SetClk(MS_U32 u32PortOffset, HAL_HWI2C_CLKSEL eClkSel);

MS_BOOL HAL_HWI2C_Start(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_Stop(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_ReadRdy(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_SendData(MS_U32 u32PortOffset, MS_U8 u8Data);
MS_U8 HAL_HWI2C_RecvData(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_Get_SendAck(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_NoAck(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_Ack(MS_U32 u32PortOffset);
MS_U8 HAL_HWI2C_GetState(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_Is_Idle(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_Is_INT(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_Clear_INT(MS_U32 u32PortOffset);
MS_BOOL HAL_HWI2C_Reset(MS_U32 u32PortOffset, MS_BOOL bReset);
MS_BOOL HAL_HWI2C_Send_Byte(MS_U32 u32PortOffset, MS_U8 u8Data);
MS_BOOL HAL_HWI2C_Recv_Byte(MS_U32 u32PortOffset, MS_U8 *pData);

MS_BOOL HAL_HWI2C_DMA_Init(MS_U32 u32PortOffset, HAL_HWI2C_PortCfg* pstPortCfg);
MS_BOOL HAL_HWI2C_DMA_Enable(MS_U32 u32PortOffset, MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_DMA_ReadBytes(MS_U32 u32PortOffset, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
MS_BOOL HAL_HWI2C_DMA_WriteBytes(MS_U32 u32PortOffset, MS_U16 u16SlaveCfg, MS_U32 uAddrCnt, MS_U8 *pRegAddr, MS_U32 uSize, MS_U8 *pData);
#ifdef CONFIG_HWIIC_INTERRUPT_MODE_ENABLE
void HAL_HWI2C_Resume_Flag(MS_U8 u8Resume);
#endif
void HAL_HWI2C_Init_ExtraProc(void);

#if 0
MS_BOOL HAL_HWI2C_EnINT(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnDMA(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnClkStretch(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnTimeoutINT(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnFilter(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_EnPushSda(MS_BOOL bEnable);

MS_BOOL HAL_HWI2C_DMA_SetINT(MS_BOOL bEnable);
MS_BOOL HAL_HWI2C_DMA_Reset(MS_BOOL bReset);
MS_BOOL HAL_HWI2C_DMA_MiuReset(MS_BOOL bReset);
MS_BOOL HAL_HWI2C_DMA_SetMiuPri(HAL_HWI2C_DMA_MIUPRI eMiuPri);
MS_BOOL HAL_HWI2C_DMA_SetMiuAddr(MS_U32 u32MiuAddr);
MS_BOOL HAL_HWI2C_DMA_Trigger(void);
MS_BOOL HAL_HWI2C_DMA_ReTrigger(void);
MS_BOOL HAL_HWI2C_DMA_SetReadMode(HAL_HWI2C_ReadMode eReadMode);
MS_BOOL HAL_HWI2C_DMA_SetRdWrt(MS_BOOL bRdWrt);
MS_BOOL HAL_HWI2C_DMA_SetMiuChannel(HAL_HWI2C_DMA_MIUCH eMiuCh);
MS_BOOL HAL_HWI2C_DMA_TxfrDone(void);
MS_BOOL HAL_HWI2C_DMA_IsTxfrDone(void);
MS_BOOL HAL_HWI2C_DMA_SetTxfrCmd(MS_U8 u8CmdLen, MS_U8* pu8CmdBuf);
MS_BOOL HAL_HWI2C_DMA_SetCmdLen(MS_U8 u8CmdLen);
MS_BOOL HAL_HWI2C_DMA_SetDataLen(MS_U32 u32DataLen);
MS_U32 HAL_HWI2C_DMA_GetTxfrCnt(void);
MS_BOOL HAL_HWI2C_DMA_SetSlaveAddr(MS_U16 u16SlaveAddr);
HAL_HWI2C_DMA_ADDRMODE HAL_HWI2C_DMA_GetAddrMode(void);
MS_BOOL HAL_HWI2C_DMA_SetAddrMode(HAL_HWI2C_DMA_ADDRMODE eAddrMode);
#endif

#endif  //_MHAL_HWI2C_H_

