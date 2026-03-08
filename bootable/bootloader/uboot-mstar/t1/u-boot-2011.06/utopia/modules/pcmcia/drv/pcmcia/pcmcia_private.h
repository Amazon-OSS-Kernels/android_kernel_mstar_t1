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
// ********************************************************
//

///////////////////////////////////////////////////////////////////////////////
///
/// file    pcmcia_private.h
/// @brief  PCMCIA Driver Utopia2.0 private
///////////////////////////////////////////////////////////////////////////////

#ifndef __PCMCIA_PRIVATE__
#define __PCMCIA_PRIVATE__
#include "drvPCMCIA.h"

#define MAGIC_NUM 0x87654321

enum {
    E_PCMCIA_RESOURCE,
} ePcmResourceId;

typedef struct _PCMCIA_RESOURCE_PRIVATE
{
//    MS_BOOL         bPCMCIA_Irq[E_PCMCIA_MODULE_MAX];
//    MS_BOOL         bPCMCIA_IrqStatus[E_PCMCIA_MODULE_MAX];
//    MS_U8           u8PCMCIA_Command[E_PCMCIA_MODULE_MAX];
//    MS_U8           u8HW_ResetDuration;

//    MS_BOOL         bHighActive;
//    MS_BOOL         bCardInside[E_PCMCIA_MODULE_MAX];
//    MS_BOOL         bPCMCIA_Detect_Enable;
//    MS_U32          u32PCMCIA_CD_To_HWRST_Timer[E_PCMCIA_MODULE_MAX];
    MS_U8           u8PCMCIACurModule;
    
    // APP callback function
//    IsrCallback     fnIsrCallback[E_PCMCIA_MODULE_MAX];
    
    MS_BOOL         bInited_Drv;   
    MS_U32          u32Magic;
} PCMCIA_RESOURCE_PRIVATE;

typedef struct _PCMCIA_INSTANT_PRIVATE
{
} PCMCIA_INSTANT_PRIVATE;


void        _MDrv_PCMCIA_InitSW( MS_BOOL bCD_Reverse );
void        _MDrv_PCMCIA_InitHW( MS_BOOL bResume );
MS_U32      _MDrv_PCMCIA_SetPowerState(EN_POWER_MODE u16PowerState);
MS_BOOL     _MDrv_PCMCIA_DetectV2( PCMCIA_MODULE eModule );
MS_BOOL     _MDrv_PCMCIA_PollingV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_Set_HW_ResetDuration( MS_U8 u8HW_ResetDuration );
void        _MDrv_PCMCIA_ResetHW_V2( PCMCIA_MODULE eModule);
void        _MDrv_PCMCIA_WriteAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value);
void        _MDrv_PCMCIA_ReadAttribMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 *pDest );
void        _MDrv_PCMCIA_WriteIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value);
void        _MDrv_PCMCIA_WriteIOMemLongV2( PCMCIA_MODULE eModule, MS_U16 u16Addr, MS_U8 u8Value, MS_U16 u16DataLen, MS_U8* u8pWriteBuffer);
MS_U8       _MDrv_PCMCIA_ReadIOMemV2( PCMCIA_MODULE eModule, MS_U16 u16Addr );
MS_BOOL     _MDrv_PCMCIA_IsModuleStillPluggedV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_Set_Detect_Trigger( MS_BOOL bHighActive );
void        _MDrv_PCMCIA_Set_Detect_Enable( MS_BOOL bEnable );
MS_U32      _MDrv_PCMCIA_Get_CD_IntervalV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_ParseAttribMem( MS_U8 *pAttribMem, MS_U16 dwLen, PCMCIA_INFO *pInfo );
void        _MDrv_PCMCIA_SetCommandBitV2( PCMCIA_MODULE eModule, MS_U8 u8CommandBit, MS_BOOL bValue );
MS_BOOL     _MDrv_PCMCIA_ResetInterfaceV2( PCMCIA_MODULE eModule );
MS_BOOL     _MDrv_PCMCIA_IsDataAvailableV2( PCMCIA_MODULE eModule );
MS_U16      _MDrv_PCMCIA_ReadDataV2( PCMCIA_MODULE eModule, MS_U8* u8pReadBuffer, MS_U16 u16ReadBufferSize );
MS_BOOL     _MDrv_PCMCIA_WriteDataV2( PCMCIA_MODULE eModule, MS_U8* u8pWriteBuffer, MS_U16 u16DataLen );
MS_BOOL     _MDrv_PCMCIA_SwitchToIOmodeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo );
MS_U16      _MDrv_PCMCIA_NegotiateBufferSizeV2( PCMCIA_MODULE eModule, PCMCIA_INFO *pInfo );
void        _MDrv_PCMCIA_WriteBufferSizeV2( PCMCIA_MODULE eModule, MS_U16 u16BufferSize );
MS_BOOL     _MDrv_PCMCIA_GetLibVer( const MSIF_Version **ppVersion );
void        _MDrv_PCMCIA_Enable_InterruptV2( PCMCIA_MODULE eModule, MS_BOOL bEnable );
void        _MDrv_PCMCIA_Set_InterruptStatusV2( PCMCIA_MODULE eModule, MS_BOOL Status );
MS_BOOL     _MDrv_PCMCIA_Get_InterruptStatusV2( PCMCIA_MODULE eModule );
void        _MDrv_PCMCIA_InstarllIsrCallbackV2( PCMCIA_MODULE eModule, IsrCallback fnIsrCallback );
MS_BOOL     _MDrv_PCMCIA_WaitForStatusBitV2( PCMCIA_MODULE eModule, MS_U8 u8StatusBit );
void        _MDrv_PCMCIA_Exit( MS_BOOL bSuspend );
#endif //__PCMCIA_PRIVATE__
