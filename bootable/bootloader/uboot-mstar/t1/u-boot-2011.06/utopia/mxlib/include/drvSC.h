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
/// @file   drvSC.h
/// @brief  SmartCard Driver Interface
///////////////////////////////////////////////////////////////////////////////////////////////////

/*! \defgroup G_SC SC interface (drvSC.h)
    \ingroup  G_PERIPHERAL
     <b>Features</b>
     - SC driver init
     - SC driver open
     - Reset the smart card and get the ATR response data
     - Send data to smart card
     - Recv the data from smart card
     - Send and recv data by T0 protocol
     - Send and recv data by T1 protocol
     - SC driver close
     - SC driver exit

     <b> SC Block Diagram: </b> \n
     The SC drver block diagram, all driver API are in user space, INT and data transfer are in kernel driver

     <b> Operation Code Flow: </b> \n
     -# Call sc init
     -# Call sc open
     -# Reset sc and get atr
     -# Call T0 send/recv if inserted card is T0 protocol
     -# Call T1 send/recv if inserted card is T1protocol
     -# Call sc close and then exit if want to exit the SC driver

     \defgroup G_SC_INIT Initialization Task relative
     \ingroup  G_SC
     \defgroup G_SC_COMMON Common Task relative
     \ingroup  G_SC
     \defgroup G_SC_CONTROL Control relative
     \ingroup  G_SC
     \defgroup G_SC_OTHER  other relative
     \ingroup  G_SC
     \defgroup G_SC_ToBeModified SC api to be modified
     \ingroup  G_SC
     \defgroup G_SC_ToBeRemove SC api to be removed
     \ingroup  G_SC
*/

#ifndef __DRV_SC_H__
#define __DRV_SC_H__


#ifdef __cplusplus
extern "C"
{
#endif

#include "MsDevice.h"
#include "UFO.h"
//-------------------------------------------------------------------------------------------------
//  Driver Capability
//-------------------------------------------------------------------------------------------------


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define MSIF_SC_LIB_CODE            {'S','C'}    //Lib code
#define MSIF_SC_LIBVER              {'0','1'}            //LIB version
#define MSIF_SC_BUILDNUM            {'0','1'}            //Build Number
#define MSIF_SC_CHANGELIST          {'0','0','0','0','0','0','0','0'} //P4 ChangeList Number

#define SC_DRV_VERSION              /* Character String for DRV/API version             */  \
    MSIF_TAG,                       /* 'MSIF'                                           */  \
    MSIF_CLASS,                     /* '00'                                             */  \
    MSIF_CUS,                       /* 0x0000                                           */  \
    MSIF_MOD,                       /* 0x0000                                           */  \
    MSIF_CHIP,                                                                              \
    MSIF_CPU,                                                                               \
    MSIF_SC_LIB_CODE,               /* IP__                                             */  \
    MSIF_SC_LIBVER,                 /* 0.0 ~ Z.Z                                        */  \
    MSIF_SC_BUILDNUM,               /* 00 ~ 99                                          */  \
    MSIF_SC_CHANGELIST,             /* CL#                                              */  \
    MSIF_OS

#define SC_ATR_LEN_MAX              33                                  ///< Maximum length of ATR
#define SC_ATR_LEN_MIN              2                                   ///< Minimum length of ATR
#define SC_HIST_LEN_MAX             15                                  ///< Maximum length of ATR history
#define SC_PPS_LEN_MAX              6                                   ///< Maximum length of PPS
#define SC_FIFO_SIZE                512                                 // Rx fifo size

///SC_Param.u8UartMode
#define SC_UART_CHAR_7              (0x02)
#define SC_UART_CHAR_8              (0x03)
#define SC_UART_STOP_1              (0x00)
#define SC_UART_STOP_2              (0x04)
#define SC_UART_PARITY_NO           (0x00)
#define SC_UART_PARITY_ODD          (0x08)
#define SC_UART_PARITY_EVEN         (0x08|0x10)

#define SC_PROC_LOCK                (0xFE66)
#define SC_PROC_UNLOCK              (0x0)

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------

typedef struct _SC_Status
{
    MS_BOOL                         bCardIn;                            ///< Card status

} SC_Status;

typedef enum _SC_DbgLv
{
    E_SC_DBGLV_NONE,                                                    //no debug message
    E_SC_DBGLV_ERR_ONLY,                                                //show error only
    E_SC_DBGLV_REG_DUMP,                                                //show error & reg dump
    E_SC_DBGLV_INFO,                                                    //show error & informaiton
    E_SC_DBGLV_ALL,                                                     //show error, information & funciton name
} SC_DbgLv;

/// SmartCard DDI error code
typedef enum
{
    E_SC_FAIL,                                                          ///< Function fail
    E_SC_OK,                                                            ///< No Error
    E_SC_PPSFAIL,                                                       ///< Do PPS fail
    E_SC_INVCONV,                                                       ///< Inverse convention
    E_SC_CARDIN,                                                        ///< Card in
    E_SC_CARDOUT,                                                       ///< Card out
    E_SC_NODATA,                                                        ///< No data
    E_SC_TIMEOUT,                                                       ///< Timeout
    E_SC_OVERFLOW,                                                      ///< Rx data fifo overflow
    E_SC_CRCERROR,                                                      ///< ATR checksum error
    E_SC_DATAERROR,                                                     ///< Data error
    E_SC_PARMERROR,                                                     ///< Parameter error
} SC_Result;

/// SmartCard event
typedef enum
{
    E_SC_EVENT_DATA,                                                    ///< Rx data valid
    E_SC_EVENT_IN,                                                      ///< Card in
    E_SC_EVENT_OUT,                                                     ///< Card out

} SC_Event;

/// SmartCard CLK setting
typedef enum
{
    E_SC_CLK_3M,                                                        ///< 3 MHz
    E_SC_CLK_4P5M,                                                      ///< 4.5 MHz
    E_SC_CLK_6M,                                                        ///< 6 MHz
    E_SC_CLK_13M,                                                        ///< 6 MHz
    E_SC_CLK_4M,                                                        ///< 4 MHz
} SC_ClkCtrl;

/// SmartCard VCC control mode
typedef enum
{
    E_SC_VCC_CTRL_8024_ON,                                              ///< by external 8024 on
    E_SC_VCC_CTRL_LOW,                                                  ///< by direct Vcc (low active)
    E_SC_VCC_CTRL_HIGH,                                                 ///< by direct Vcc (high active)
    E_SC_OCP_VCC_HIGH,
    E_SC_VCC_VCC_ONCHIP_8024,
} SC_VccCtrl;

// smart card 3V/5V control
typedef enum
{
    E_SC_VOLTAGE_3_POINT_3V,                                              ///<3.3V
    E_SC_VOLTAGE_5V,                                                      ///< 5V
    E_SC_VOLTAGE_MAX
} SC_VoltageCtrl;

typedef void(*P_SC_En5V_Callback)(MS_BOOL bEnable);


typedef enum
{
    E_SC_HIGH_ACTIVE = 0x0,
    E_SC_LOW_ACTIVE  = 0xff3
} SC_CardDetType;

/// SmartCard Timeout config
typedef struct DLL_PACKED
{
    MS_U32 u32T0SendTimeoutMs;
    MS_U32 u32T0RecvTimeoutMs;
    MS_U32 u32T0WaitProByteMs;                                          // Wait procedure byte timeout ms
    MS_U32 u32T1SendTimeoutMs;
    MS_U32 u32T1RecvTimeoutMs;
    MS_U32 u32T14SendTimeoutMs;
    MS_U32 u32T14RecvTimeoutMs;
} SC_TimeoutCfg;

/// SmartCard configuration
typedef struct DLL_PACKED
{
    MS_U8                           u8Protocal;                         ///< T=
    SC_ClkCtrl                      eCardClk;                           ///< Clock
    MS_U8                           u8UartMode;                         ///< Uart Mode
    MS_U16                          u16ClkDiv;                          ///< Div
    SC_VccCtrl                      eVccCtrl;
    MS_U16                          u16Bonding;                         ///Chip Bonding type
    P_SC_En5V_Callback              pfOCPControl;
    MS_U8                           u8Convention;                       ///< Convention
    SC_VoltageCtrl                  eVoltage;
    SC_CardDetType                  u8CardDetInvert;
    SC_TimeoutCfg                   TimeoutCfg;
} SC_Param;

/// SmartCard event callback
typedef void (*P_SC_Callback)(MS_U8 u8SCID, SC_Event eEvent);

typedef struct _Smart_Dev
{

} SC_Dev;


/// SmartCard Info
typedef struct DLL_PACKED
{
    // SmsartCard Protocol
    MS_U8                           u8Protocol;                         ///T= Protocol
    MS_BOOL                         bSpecMode;                          ///Special mode
    MS_U8                           pu8Atr[SC_ATR_LEN_MAX];             ///Atr buffer
    MS_U16                          u16AtrLen;                          ///Atr length
    MS_U8                           pu8Hist[SC_HIST_LEN_MAX];           ///History buffer
    MS_U16                          u16HistLen;                         ///History length
    MS_U8                           u8Fi;                               ///Fi
    MS_U8                           u8Di;                               ///Di
#ifdef UFO_PUBLIC_HEADER_212
    MS_U8                           u8N;                              ////N
#endif
    // Device Setting
    MS_BOOL                         bInited;
    MS_BOOL                         bOpened;                           ///Open
    MS_BOOL                         bCardIn;                            ///Status care in
    MS_BOOL                         blast_cardin;
    SC_ClkCtrl                      eCardClk;                           ///< Clock
    MS_U8                           u8UartMode;                         ///< Uart Mode
    SC_VccCtrl                      eVccCtrl;
    MS_U16                          u16ClkDiv;                         ///< Div
    MS_U16                          u16Bonding; //@TODO: how to take care of bonding?????
    P_SC_En5V_Callback              pfEn5V;
    MS_U8                           u8Convention;

    MS_U8                           u8FifoRx[SC_FIFO_SIZE];
    MS_U16                          u16FifoRxRead;
    MS_U16                          u16FifoRxWrite;

    MS_U8                           u8FifoTx[SC_FIFO_SIZE];
    MS_U16                          u16FifoTxRead;
    MS_U16                          u16FifoTxWrite;
    P_SC_Callback                   pfNotify;                           ///Call back funtcion

    MS_BOOL                         bLastCardIn;
    MS_S32                          s32DevFd;
#ifdef UFO_PUBLIC_HEADER_212
    SC_VoltageCtrl                  eVoltage;
#endif
    SC_TimeoutCfg                   TimeoutCfg;
} SC_Info;


/// SmartCard Caps
typedef struct DLL_PACKED
{
    MS_U8                           u8DevNum;                           ///SmartCard Device Number

} SC_Caps;

///Define SC Command Index
typedef enum
{
    //MIPS-->51 Command Index
    SC_CMDIDX_RAW_EXCHANGE = 0x01,                  /// SC Command Index is Raw Data Exchange
    SC_CMDIDX_GET_ATR      = 0x02,                  /// SC Command Index is Get ATR
    SC_CMDIDX_SEND         = 0x03,                  /// SC Command Index is Send Data
    SC_CMDIDX_RECV         = 0x04,                  /// SC Command Index is Receive Data

    SC_CMDIDX_ACK_51ToMIPS = 0x30,                  /// SC Command Index is ACK 51 To MIPS

    //51->MIPS Command Index
    SC_CMDIDX_ACK_MIPSTo51 = 0xA0,                  /// SC Command Index is ACK MIPS To 51

} SC_CmdIndex;


///Define SC Acknowledge Flags
typedef enum
{
    E_SC_ACKFLG_NULL                = 0,            /// Ack flag for NULL
    E_SC_ACKFLG_WAIT_RAW_EXCHANGE   = (1<<0),       /// Ack flag for Raw Exchange
    E_SC_ACKFLG_WAIT_GET_ATR        = (1<<1),       /// Ack flag for ATR
    E_SC_ACKFLG_WAIT_SEND           = (1<<2),       /// Ack flag for Sending
    E_SC_ACKFLG_WAIT_RECV           = (1<<3),       /// Ack flag for Receiving

} SC_AckFlags;

typedef struct __attribute__ ((packed, aligned (4096)))
{
    MS_U8  u8SCID;
    MS_U16 u16DataLen;       /* Send and reply data length*/
    MS_U16 u16ReplyMaxLen;   /* Maximum length of reply data */
    MS_U8  u8Data[259];      /* T=0, 256 data bytes plus the two status words SW1 and SW2
                                                T=1, 254 INF bytes plus NAD, PCB, LEN, and two EDC bytes
                                                Send and reply data buffer*/
} SC_DataCfg;

/// SmartCard Caps
typedef struct
{
    MS_PHY   u32DataBuffAddr;                            ///SmartCard Data Buffer Address, 4K alignment
    MS_PHY   u32FwBuffAddr;                              ///SmartCard Firmware Buffer Address, 64K alignment
} SC_BuffAddr;

//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_INIT
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Init(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Open(MS_U8 u8SCID, MS_U8 u8Protocol, SC_Param* pParam, P_SC_Callback pfSmartNotify);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Config(MS_U8 u8SCID, SC_Param* pParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Close(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Reset(MS_U8 u8SCID, SC_Param* pParam);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Activate(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Deactivate(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Reset_ATR(MS_U8 u8SCID, SC_Param* pParam, MS_U8 *pu8Atr, MS_U16 *pu16AtrLen, MS_U8 *pu8His, MS_U16 *pu16HisLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_PPS(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Send(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendDataLen, MS_U32 u32TimeoutMs);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Recv(MS_U8 u8SCID, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen, MS_U32 u32TimeoutMs);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_T0_SendRecv(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendLen, MS_U8 *pu8RecvData, MS_U16 *pu16RecvLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_T1_SendRecv(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_T14_SendRecv(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 u16SendLen, MS_U8 *pu8RecvData, MS_U16 *pu16RecvLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Exit(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_GetATR(MS_U8 u8SCID, MS_U32 u32TimeOut,MS_U8 *pu8Atr, MS_U16 *pu16AtrLen, MS_U8 *pu8His, MS_U16 *pu16HisLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
const SC_Info*  MDrv_SC_GetInfo(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_GetLibVer(const MSIF_Version **ppVersion);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_GetStatus(MS_U8 u8SCID, SC_Status *pStatus);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SC_SetDbgLevel(SC_DbgLv eLevel);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_SetPPS(MS_U8 u8SCID, MS_U8 u8SCProtocol, MS_U8 u8Di,MS_U8 u8Fi);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_ClearState(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_PowerOff(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_CONTROL
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_SetGuardTime(MS_U8 u8SCID, MS_U8 u8GuardTime);

//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_Task_Proc(void);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SC_ISR_Proc(MS_U8 u8SCID);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_RawExchange(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_RawExchangeTimeout(MS_U8 u8SCID, MS_U8 *pu8SendData, MS_U16 *u16SendDataLen, MS_U8 *pu8ReadData, MS_U16 *u16ReadDataLen, MS_U32 u32TimeoutMs);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
SC_Result       MDrv_SC_SetBuffAddr(SC_BuffAddr *pScBuffAddr);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_BOOL         MDrv_SC_CardVoltage_Config(MS_U8 u8SCID, SC_VoltageCtrl eVoltage);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_ToBeRemove
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
void            MDrv_SC_EnableTimeout(MS_BOOL bTimeout);
//-------------------------------------------------------------------------------------------------
/// MOBF Encrypt
/// @ingroup G_SC_COMMON
/// @param u32Key \b IN: Key
/// @param bEnable \b IN: TRUE/FLASE
/// @return DRVAESDMA_OK : Success
/// @return Others : Fail
//-------------------------------------------------------------------------------------------------
MS_U32          MDrv_SC_SetPowerState(EN_POWER_MODE u16PowerState);

#ifdef __cplusplus
}
#endif

#endif // __DRV_SC_H__
