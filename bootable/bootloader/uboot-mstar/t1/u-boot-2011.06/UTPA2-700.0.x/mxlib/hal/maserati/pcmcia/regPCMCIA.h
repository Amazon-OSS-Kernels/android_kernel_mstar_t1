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
///////////////////////////////////////////////////////////////////////////////
//
//
//
///////////////////////////////////////////////////////////////////////////////

#ifndef _REG_PCMCIA_H_
#define _REG_PCMCIA_H_

//-----------------------------------------------------------------------------
//  Hardware Capability
//-----------------------------------------------------------------------------
// Base address should be initial.
#define PCMCIA_RIU_MAP u32PCMCIA_RIU_BaseAdd  //obtain in init

//-----------------------------------------------------------------------------
//  Macro and Define
//-----------------------------------------------------------------------------
//hardware spec
#define REG_PCMCIA_PCM_MEM_IO_CMD           0x00UL
#define REG_PCMCIA_ADDR0                    0x02UL
#define REG_PCMCIA_ADDR1                    0x03UL
#define REG_PCMCIA_WRITE_DATA               0x04UL
#define REG_PCMCIA_FIRE_READ_DATA_CLEAR     0x06UL
#define REG_PCMCIA_READ_DATA                0x08UL
#define REG_PCMCIA_READ_DATA_DONE_BUS_IDLE  0x09UL
#define REG_PCMCIA_INT_MASK_CLEAR           0x0AUL
#define REG_PCMCIA_INT_MASK_CLEAR1          0x0BUL
#define REG_PCMCIA_STAT_INT_RAW_INT         0x0EUL
#define REG_PCMCIA_STAT_INT_RAW_INT1        0x0FUL
#define REG_PCMCIA_MODULE_VCC_OOB           0x10UL

#define PCMCIA_ATTRIBMEMORY_READ            0x03UL
#define PCMCIA_ATTRIBMEMORY_WRITE           0x04UL
#define PCMCIA_IO_READ                      0x05UL
#define PCMCIA_IO_WRITE                     0x06UL

#define REG_PCMCIA_BASE                     0x3440UL
#define PCMCIA_BASE_ADDRESS	                ( PCMCIA_RIU_MAP + ( REG_PCMCIA_BASE * 2 ) )

#if defined ( MCU_AEON )
#define PCMCIA_RIU_DEFAULT_BASE_ADDR        0xA0000000UL;
#elif defined ( MCU_MIPS )
#define PCMCIA_RIU_DEFAULT_BASE_ADDR        0xBF200000UL;
#endif

#define PCM_OOB_BIT_MASK                    0x03UL
#define PCM_OOB_BIT_SHFT                    6UL

#define PCM_OOB_CYCLE_EXTEND                0x3UL
                                            // 00:th(CE)=4T (extend 3 active cycle)
                                            // 01:th(CE)=3T (extend 2 active cycle)
                                            // 10:th(CE)=2T (extend 1 active cycle)
                                            // 11:th(CE)=1T

//-----------------------------------------------------------------------------
//  Type and Structure
//-----------------------------------------------------------------------------

#endif // _REG_PCMCIA_H_
