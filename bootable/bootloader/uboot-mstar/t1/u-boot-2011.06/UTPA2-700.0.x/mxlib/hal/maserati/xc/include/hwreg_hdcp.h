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

#ifndef _HWREG_HDCP_H_
#define _HWREG_HDCP_H_

//=============================================================
//HDCP
//#define REG_HDCP_BASE       0x26F0

#define REG_HDCPKEY_00_L    (REG_HDCPKEY_BASE + 0x00)
#define REG_HDCPKEY_00_H    (REG_HDCPKEY_BASE + 0x01)
#define REG_HDCPKEY_01_L    (REG_HDCPKEY_BASE + 0x02)
#define REG_HDCPKEY_01_H    (REG_HDCPKEY_BASE + 0x03)
#define REG_HDCPKEY_02_L    (REG_HDCPKEY_BASE + 0x04)
#define REG_HDCPKEY_02_H    (REG_HDCPKEY_BASE + 0x05)
#define REG_HDCPKEY_03_L    (REG_HDCPKEY_BASE + 0x06)
#define REG_HDCPKEY_03_H    (REG_HDCPKEY_BASE + 0x07)
#define REG_HDCPKEY_04_L    (REG_HDCPKEY_BASE + 0x08)
#define REG_HDCPKEY_04_H    (REG_HDCPKEY_BASE + 0x09)
#define REG_HDCPKEY_05_L    (REG_HDCPKEY_BASE + 0x0A)
#define REG_HDCPKEY_05_H    (REG_HDCPKEY_BASE + 0x0B)
#define REG_HDCPKEY_06_L    (REG_HDCPKEY_BASE + 0x0C)
#define REG_HDCPKEY_06_H    (REG_HDCPKEY_BASE + 0x0D)
#define REG_HDCPKEY_07_L    (REG_HDCPKEY_BASE + 0x0E)
#define REG_HDCPKEY_07_H    (REG_HDCPKEY_BASE + 0x0F)
#define REG_HDCPKEY_08_L    (REG_HDCPKEY_BASE + 0x10)
#define REG_HDCPKEY_08_H    (REG_HDCPKEY_BASE + 0x11)
#define REG_HDCPKEY_09_L    (REG_HDCPKEY_BASE + 0x12)
#define REG_HDCPKEY_09_H    (REG_HDCPKEY_BASE + 0x13)
#define REG_HDCPKEY_0A_L    (REG_HDCPKEY_BASE + 0x14)
#define REG_HDCPKEY_0A_H    (REG_HDCPKEY_BASE + 0x15)
#define REG_HDCPKEY_0B_L    (REG_HDCPKEY_BASE + 0x16)
#define REG_HDCPKEY_0B_H    (REG_HDCPKEY_BASE + 0x17)
#define REG_HDCPKEY_0C_L    (REG_HDCPKEY_BASE + 0x18)
#define REG_HDCPKEY_0C_H    (REG_HDCPKEY_BASE + 0x19)
#define REG_HDCPKEY_0D_L    (REG_HDCPKEY_BASE + 0x1A)
#define REG_HDCPKEY_0D_H    (REG_HDCPKEY_BASE + 0x1B)
#define REG_HDCPKEY_0E_L    (REG_HDCPKEY_BASE + 0x1C)
#define REG_HDCPKEY_0E_H    (REG_HDCPKEY_BASE + 0x1D)
#define REG_HDCPKEY_0F_L    (REG_HDCPKEY_BASE + 0x1E)
#define REG_HDCPKEY_0F_H    (REG_HDCPKEY_BASE + 0x1F)
#define REG_HDCPKEY_10_L    (REG_HDCPKEY_BASE + 0x20)
#define REG_HDCPKEY_10_H    (REG_HDCPKEY_BASE + 0x21)
#define REG_HDCPKEY_11_L    (REG_HDCPKEY_BASE + 0x22)
#define REG_HDCPKEY_11_H    (REG_HDCPKEY_BASE + 0x23)
#define REG_HDCPKEY_12_L    (REG_HDCPKEY_BASE + 0x24)
#define REG_HDCPKEY_12_H    (REG_HDCPKEY_BASE + 0x25)

#define REG_HDCP_00_L       (REG_HDCP_BASE + 0x00)
#define REG_HDCP_00_H       (REG_HDCP_BASE + 0x01)
#define REG_HDCP_01_L       (REG_HDCP_BASE + 0x02)
#define REG_HDCP_01_H       (REG_HDCP_BASE + 0x03)
#define REG_HDCP_02_L       (REG_HDCP_BASE + 0x04)
#define REG_HDCP_02_H       (REG_HDCP_BASE + 0x05)
#define REG_HDCP_03_L       (REG_HDCP_BASE + 0x06)
#define REG_HDCP_03_H       (REG_HDCP_BASE + 0x07)
#define REG_HDCP_09_L       (REG_HDCP_BASE + 0x12)
#define REG_HDCP_09_H       (REG_HDCP_BASE + 0x13)
#define REG_HDCP_15_L       (REG_HDCP_BASE + 0x2A)
#define REG_HDCP_15_H       (REG_HDCP_BASE + 0x2B)
#define REG_HDCP_15_L       (REG_HDCP_BASE + 0x2A)
#define REG_HDCP_15_H       (REG_HDCP_BASE + 0x2B)
#define REG_HDCP_17_L       (REG_HDCP_BASE + 0x2E)
#define REG_HDCP_17_H       (REG_HDCP_BASE + 0x2F)
#define REG_HDCP_18_L       (REG_HDCP_BASE + 0x30)
#define REG_HDCP_18_H       (REG_HDCP_BASE + 0x31)
#define REG_HDCP_19_L       (REG_HDCP_BASE + 0x32)
#define REG_HDCP_19_H       (REG_HDCP_BASE + 0x33)
#define REG_HDCP_1C_L       (REG_HDCP_BASE + 0x38)
#define REG_HDCP_1C_H       (REG_HDCP_BASE + 0x39)

#define REG_HDCP1_1C_L      (REG_HDCP1_BASE + 0x38)
#define REG_HDCP1_1C_H      (REG_HDCP1_BASE + 0x39)

#define REG_HDCP2_1C_L      (REG_HDCP2_BASE + 0x38)
#define REG_HDCP2_1C_H      (REG_HDCP2_BASE + 0x39)

#define REG_HDCP3_09_L      (REG_HDCP3_BASE + 0x12)
#define REG_HDCP3_09_H      (REG_HDCP3_BASE + 0x13)
#define REG_HDCP3_1C_L      (REG_HDCP3_BASE + 0x38)
#define REG_HDCP3_1C_H      (REG_HDCP3_BASE + 0x39)
#endif

