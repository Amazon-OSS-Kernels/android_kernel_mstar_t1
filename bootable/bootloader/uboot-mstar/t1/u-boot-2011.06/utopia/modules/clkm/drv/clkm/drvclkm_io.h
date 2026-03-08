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

#ifndef _DRVCLKM_IO_H_
#define _DRVCLKM_IO_H_

////////////////////////////////////////////////////////////////////////////////
/// @file drvBDMA.h
/// @brief Byte DMA control driver
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Header Files
////////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
extern "C"
{
#endif

#include "MsTypes.h"
#include "drvCLKM.h"

#define CLKM_MAGIC                ('6')
#define CMD_CLKM_GET_HANDLE                             _IOWR(CLKM_MAGIC, 0x00, CLKM_GetHandle_PARAM)
#define CMD_CLKM_SET_CLK_SOURCE  			                  _IOWR(CLKM_MAGIC, 0x01, int)
#define CMD_CLKM_CLK_GATE_DISABLE  			                  _IOWR(CLKM_MAGIC, 0x02, int)


#ifdef __cplusplus
}
#endif
#endif 
