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
/// file    pcmcia.h
/// @brief  PCMCIA Driver Utopia2.0 Interface header
///////////////////////////////////////////////////////////////////////////////
#ifndef __PCMCIA_H__
#define __PCMCIA_H__

/*****************************************************************************/
/*                       Header Files                                        */
/*****************************************************************************/
// Common Definition
#include "MsTypes.h"
#include "drvPCMCIA.h"

typedef enum {
    E_MDRV_CMD_PCM_Init,
    E_MDRV_CMD_PCM_Detect,
    E_MDRV_CMD_PCM_Enable_Interrupt,
    E_MDRV_CMD_PCM_Exit,
    E_MDRV_CMD_PCM_Get_CD_Interval,
    E_MDRV_CMD_PCM_Get_InterruptStatus,
    E_MDRV_CMD_PCM_GetLibVer,
    E_MDRV_CMD_PCM_InstallIsrCallback,
    E_MDRV_CMD_PCM_IsDataAvailable,
    E_MDRV_CMD_PCM_IsModuleStillPlugged,
    E_MDRV_CMD_PCM_NegotiateBufferSize,
    E_MDRV_CMD_PCM_ParseAttribMem,
    E_MDRV_CMD_PCM_Polling,
    E_MDRV_CMD_PCM_ReadAttribMem, 
    E_MDRV_CMD_PCM_ReadData,
    E_MDRV_CMD_PCM_ReadIOMem,
    E_MDRV_CMD_PCM_ResetHW,
    E_MDRV_CMD_PCM_ResetInterface,
    E_MDRV_CMD_PCM_Set_Detect_Enable,
    E_MDRV_CMD_PCM_Set_Detect_Trigger,
    E_MDRV_CMD_PCM_Set_HW_ResetDuration,
    E_MDRV_CMD_PCM_Set_InterruptStatus,
    E_MDRV_CMD_PCM_SetCommandBit,
    E_MDRV_CMD_PCM_SwitchToIOmode,
    E_MDRV_CMD_PCM_WaitForStatusBit,
    E_MDRV_CMD_PCM_WriteAttribMem,
    E_MDRV_CMD_PCM_WriteBufferSize,
    E_MDRV_CMD_PCM_WriteData,
    E_MDRV_CMD_PCM_WriteIOMem,
    E_MDRV_CMD_PCM_WriteIOMemLong,
    E_MDRV_CMD_PCM_SetPowerState,
    E_MDRV_CMD_PCM_MAX,
} ePcmIoctlOpt;

// E_MDRV_CMD_PCM_Detect
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bDetected;
} PCM_DETECT;

// E_MDRV_CMD_PCM_Enable_Interrupt
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bEnable;
} PCM_ENABLE_INT;

// E_MDRV_CMD_PCM_InstallIsrCallback
typedef struct {
    PCMCIA_MODULE   eModule;
    IsrCallback     pfcb;
} PCM_INSTCB;

// E_MDRV_CMD_PCM_Get_CD_Interval
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U32          u32Interval;
} PCM_GET_CD_INTERVAL;

// E_MDRV_CMD_PCM_Get_InterruptStatus
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bIntStatus;
} PCM_GET_INT_STATUS;

// E_MDRV_CMD_PCM_IsDataAvailable
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bDA; // data available
} PCM_DATA_AVAILABLE;

// E_MDRV_CMD_PCM_IsModuleStillPlugged
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bPlugIn;
} PCM_MODULE_PLUGIN;

// E_MDRV_CMD_PCM_NegotiateBufferSize
typedef struct {
    PCMCIA_MODULE   eModule;
    PCMCIA_INFO*    pInfo;
    MS_U16          u16BufSize;
} PCM_NETOTIAGE_BUFSIZE;

// E_MDRV_CMD_PCM_ParseAttribMem
typedef struct {
    MS_U8*          pu8AttribMem;
    MS_U16          u16DwLen;
    PCMCIA_INFO*    pInfo;
} PCM_PARSE_ATTRIBUTE;

// E_MDRV_CMD_PCM_Polling
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bChanged; // card insertion status change
} PCM_POLL;

// E_MDRV_CMD_PCM_ReadAttribMem
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Data;
} PCM_READ_ATTRIB_MEM;

// E_MDRV_CMD_PCM_ReadIOMem
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Data;
} PCM_READ_IO_MEM;

// E_MDRV_CMD_PCM_ResetInterface
typedef struct {
    PCMCIA_MODULE   eModule;
} PCM_RESET_INTERFACE;

// E_MDRV_CMD_PCM_ReadData
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8*          pu8ReadBuffer;
    MS_U16          u16ReadBufferSize;
    MS_U16          u16SizeRead;
} PCM_READ_DATA;

// E_MDRV_CMD_PCM_Set_InterruptStatus
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_BOOL         bStatus;
} PCM_SET_INTR_STATUS;

// E_MDRV_CMD_PCM_SetCommandBit
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8           u8CmdBit;
    MS_BOOL         bValue;
} PCM_SETCOMMANDBIT;

// E_MDRV_CMD_PCM_SwitchToIOmode
typedef struct {
    PCMCIA_MODULE   eModule;
    PCMCIA_INFO*    pInfo;
} PCM_SWITCH_TO_IOMODE;

// E_MDRV_CMD_PCM_WaitForStatusBit
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8           u8StatusBit;
} PCM_WAIT_STATUS;

// E_MDRV_CMD_PCM_WriteAttribMem
// E_MDRV_CMD_PCM_WriteIOMem
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Value;
} PCM_WRITE_ATTRIB_MEM, PCM_WRITE_IO_MEM;

// E_MDRV_CMD_PCM_WriteBufferSize
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16BufferSize;
} PCM_WRITE_BUF_SIZE;

// E_MDRV_CMD_PCM_WriteData
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U8*          pu8WriteBuffer;
    MS_U16          u16DataLen;
} PCM_WRITE_DATA;

// E_MDRV_CMD_PCM_WriteIOMemLong
typedef struct {
    PCMCIA_MODULE   eModule;
    MS_U16          u16Addr;
    MS_U8           u8Value;
    MS_U16          u16DataLen;
    MS_U8*          pu8WriteBuffer;
} PCM_WRITE_IO_MEM_LONG;

void CIRegisterToUtopia(FUtopiaOpen ModuleType);
MS_U32 PCMCIAOpen(void** pInstantTmp, const void* const pAttribute);
MS_U32 PCMCIAClose(void* pInstantTmp);
MS_U32 PCMCIAIoctl(void* pInstance, MS_U32 u32Cmd, void* const pArgs);

#endif //__PCMCIA_H__
