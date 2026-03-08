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

#ifndef _HAL_PARFLASH_H_
#define _HAL_PARFLASH_H_

#include "MsCommon.h"


//-------------------------------------------------------------------------------------------------
//  Macro and Define
//-------------------------------------------------------------------------------------------------
#define DEBUG_PAR_FLASH(debug_level, x)     do { if (_u8ParFlashDbgLevel >= (debug_level)) (x); } while(0)

#define PFSH_MAX_CMDRUN                  8

//-------------------------------------------------------------------------------------------------
//  Type and Structure
//-------------------------------------------------------------------------------------------------
typedef enum
{
    PARFLASH_DBGLV_NONE,    ///< disable all the debug message
    PARFLASH_DBGLV_INFO,    ///< information
    PARFLASH_DBGLV_NOTICE,  ///< normal but significant condition
    PARFLASH_DBGLV_WARNING, ///< warning conditions
    PARFLASH_DBGLV_ERR,     ///< error conditions
    PARFLASH_DBGLV_CRIT,    ///< critical conditions
    PARFLASH_DBGLV_ALERT,   ///< action must be taken immediately
    PARFLASH_DBGLV_EMERG,   ///< system is unusable
    PARFLASH_DBGLV_DEBUG,   ///< debug-level messages
}PARFLASH_DebugLevel;


//-------------------------------------------------------------------------------------------------
//  Function and Variable
//-------------------------------------------------------------------------------------------------

extern void HAL_PARFLASH_Config(MS_U32 u32PMRegBase, MS_U32 u32RegBaseAddr);
extern MS_BOOL HAL_PARFLASH_SelectChip(MS_U8 u8cs);
extern void HAL_PARFLASH_Init(MS_BOOL *pbMode);
extern MS_BOOL HAL_PARFLASH_PrepareCmdWrite(MS_U8 u8cmdnum, MS_U32 *pu32cmdadr, MS_U16* pu16cmddata);
extern MS_BOOL HAL_PARFLASH_LastCmdTrig(MS_U8 u8runs, MS_U32 u32lastaddr, MS_U16 u16lastdata);
extern MS_BOOL HAL_PARFLASH_Cmd_Write(MS_U8 u8runs, MS_U32 *pu32cmdadr, MS_U16* pu16cmddata);
extern MS_BOOL HAL_PARFLASH_Read(MS_U32 u32addr, MS_U16* pu16data);

// DON'T USE THESE DIRECTLY
extern MS_U8 _u8ParFlashDbgLevel;

#endif // _HAL_PARFLASH_H_
