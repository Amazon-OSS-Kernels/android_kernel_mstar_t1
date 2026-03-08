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

#ifndef __DRV_SYSTEM_IO_H__
#define __DRV_SYSTEM_IO_H__

#define IO_SYS_GET_RAW_UART
#define IO_SYS_REG_OP

//-------------------------------------------------------------------------------------------------
//  ioctl method
//-------------------------------------------------------------------------------------------------

// Use 'S' as magic number
#define SYS_IOCTL_MAGIC             'S'

#define IOCTL_SYS_INIT                 _IOWR(SYS_IOCTL_MAGIC, 0x00, int)
#define IOCTL_SYS_SET_PANEL_INFO       _IOW (SYS_IOCTL_MAGIC, 0x01, int)
#define IOCTL_SYS_SET_BOARD_INFO       _IOW (SYS_IOCTL_MAGIC, 0x02, int)
#define IOCTL_SYS_GET_PANEL_RES        _IOR (SYS_IOCTL_MAGIC, 0x03, int)
#define IOCTL_SYS_READ_GEN_REGISTER    _IOR (SYS_IOCTL_MAGIC, 0x04, int)
#define IOCTL_SYS_WRITE_GEN_REGISTER   _IOWR(SYS_IOCTL_MAGIC, 0x05, int)
#define IOCTL_SYS_LOAD_AEON            _IOWR(SYS_IOCTL_MAGIC, 0x06, int)
#define IOCTL_SYS_RESET_AEON           _IOWR(SYS_IOCTL_MAGIC, 0x07, int)
#define IOCTL_SYS_ENABLE_AEON          _IO(SYS_IOCTL_MAGIC, 0x08)
#define IOCTL_SYS_DISABLE_AEON         _IO(SYS_IOCTL_MAGIC, 0x09)
#define IOCTL_SYS_SWITCH_UART          _IOR (SYS_IOCTL_MAGIC, 0x0A, int)
#define IOCTL_SYS_DUMP_AEON_MSG        _IOR (SYS_IOCTL_MAGIC, 0x0B, int)
#define IOCTL_SYS_IS_AEON_ENABLE       _IOR (SYS_IOCTL_MAGIC, 0x0C, int)
#define IOCTL_SYS_CHANGE_UART          _IOWR(SYS_IOCTL_MAGIC, 0x0D, int)
#define IOCTL_SYS_DDC2BI               _IOWR(SYS_IOCTL_MAGIC, 0x0E, int) //2008/10/23 Nick

#define IOCTL_SYS_SET_GFX_GOP_INDEX     _IOW (SYS_IOCTL_MAGIC, 0x11,int)
#define IOCTL_SYS_GET_GFX_GOP_INDEX     _IOR (SYS_IOCTL_MAGIC, 0x12,int)

#define IOCTL_SYS_SET_DISPLAY_CTLR_SEPT_INDEX       _IOW (SYS_IOCTL_MAGIC, 0x15,int)
#define IOCTL_SYS_IS_DISPLAY_CTLR_SEPT_INDEX        _IOR (SYS_IOCTL_MAGIC, 0x16,int)

#define IOCTL_SYS_SET_NEXUS     _IOW (SYS_IOCTL_MAGIC, 0x17,int)
#define IOCTL_SYS_HAS_NEXUS     _IOR (SYS_IOCTL_MAGIC, 0x18,int)

#define IOCTL_SYS_PRING_MSG             _IOW (SYS_IOCTL_MAGIC, 0x19,int)
#define IOCTL_SYS_GET_GFX_GOP_PIPELINE_DELAY        _IOWR (SYS_IOCTL_MAGIC, 0x1A,int)
#define IOCTL_SYS_GET_PANEL_H_START     _IOR (SYS_IOCTL_MAGIC, 0x1B,int)

#define IOCTL_SYS_SET_NEXUS_PID     _IOW (SYS_IOCTL_MAGIC, 0x1C,int)
#define IOCTL_SYS_GET_NEXUS_PID     _IOR (SYS_IOCTL_MAGIC, 0x1D,int)

#define IOCTL_SYS_PCMCIA_WRITE      _IOW(SYS_IOCTL_MAGIC, 0x40,int)
#define IOCTL_SYS_PCMCIA_READ       _IOR(SYS_IOCTL_MAGIC, 0x41,int)
#define IOCTL_SYS_PCMCIA_WRITE_DATA       _IOW(SYS_IOCTL_MAGIC, 0x42,int)
#define IOCTL_SYS_PCMCIA_READ_DATA       _IOR(SYS_IOCTL_MAGIC, 0x43,int)

#define IOCTL_SYS_FLUSH_MEMORY         _IO(SYS_IOCTL_MAGIC, 0x50)
#define IOCTL_SYS_READ_MEMORY          _IO(SYS_IOCTL_MAGIC, 0x51)

#ifdef IO_SYS_REG_OP
#   define IOCTL_SYS_REG_OP            _IOWR(SYS_IOCTL_MAGIC, 0x54, int)
#endif

#ifdef IO_SYS_GET_RAW_UART
#   define IOCTL_SYS_GET_RAW_UART      _IOWR(SYS_IOCTL_MAGIC, 0x55, int)
#endif

#define IOCTL_SYS_TIMER                _IOWR(SYS_IOCTL_MAGIC, 0x56, int)
#define IOCTL_SYS_RELOAD_AEON          _IOWR(SYS_IOCTL_MAGIC, 0x57, int)

#if 0
//#define IOCTL_SYS_SWITCH_PAD        _IOWR(SYS_IOCTL_MAGIC, 0x01, DevSys_Switch_Pad)
#define IOCTL_SYS_WDT_ENABLE        _IOW (SYS_IOCTL_MAGIC, 0x02, MS_U32)
#define IOCTL_SYS_WDT_CLEAR         _IO  (SYS_IOCTL_MAGIC, 0x03)
#define IOCTL_SYS_WDT_LASTSTATUS    _IOWR(SYS_IOCTL_MAGIC, 0x04, MS_U32)
#define IOCTL_SYS_WDT_SETTIME       _IOW (SYS_IOCTL_MAGIC, 0x05, MS_U32)
#define IOCTL_SYS_RESET_CHIP        _IO  (SYS_IOCTL_MAGIC, 0x06)
#define IOCTL_SYS_RESET_CPU         _IO  (SYS_IOCTL_MAGIC, 0x07)
#endif

#define IOCTL_SYS_HOTEL_MODE           _IOWR(SYS_IOCTL_MAGIC, 0x58, int)
#define IOCTL_SYS_HOTEL_MODE_PRINTF    _IOWR(SYS_IOCTL_MAGIC, 0x59, int)
#define IOCTL_SYS_POWER_DOWN           _IOWR(SYS_IOCTL_MAGIC, 0x60, int)
#define IOCTL_SYS_GET_MBOX_SHM          _IOWR(SYS_IOCTL_MAGIC, 0x61, int)
#define IOCTL_SYS_GET_MSBIN_INFO          _IOWR(SYS_IOCTL_MAGIC, 0x62, int)

#define IOCTL_SYS_MAXNR    0xFF

#endif // __DRV_SYSTEM_IO_H__

