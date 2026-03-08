/**
* Copyright (c) 2006 - 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/
//******************************************************************************
//******************************************************************************
//************************************************************
//***             RIU Base address of each IP              ***
//***                (Byte address [23:0])                 ***
//************************************************************

#ifndef _C_PM_RIUBASE_H_
#define _C_PM_RIUBASE_H_

#define    RIUBASE_PM_RIU_DBG           0x000100    //0x000100 ~ 0x0001ff
#define    RIUBASE_MENULOAD             0x000200    //0x000200 ~ 0x00021f
#define    RIUBASE_GDMA                 0x000300    //0x000300 ~ 0x0003ff
#define    RIUBASE_DDC                  0x000400    //0x000400 ~ 0x0004ff
#define    RIUBASE_ISP                  0x000800    //0x000800 ~ 0x0008bf
#define    RIUBASE_FSP                  0x0008c0    //0x0008c0 ~ 0x0008df
#define    RIUBASE_QSPI                 0x0008e0    //0x0008e0 ~ 0x0008ff
#define    RIUBASE_PM_SLEEP             0x000e00    //0x000e00 ~ 0x000eff
#define    RIUBASE_MCU                  0x001000    //0x001000 ~ 0x0010ff
#define    RIUBASE_PM_CEC               0x001100    //0x001100 ~ 0x0011ff
#define    RIUBASE_PM_RTC               0x001200    //0x001200 ~ 0x0012ff
#define    RIUBASE_PM_RTC2              0x001300    //0x001300 ~ 0x0013ff
#define    RIUBASE_PM_SAR               0x001400    //0x001400 ~ 0x0014ff
#define    RIUBASE_PM_AV_LNK            0x001500    //0x001500 ~ 0x0015ff
#define    RIUBASE_PM_TOP               0x001e00    //0x001e00 ~ 0x001eff
#define    RIUBASE_EFUSE                0x002000    //0x002000 ~ 0x0020ff
#define    RIUBASE_IRQ                  0x002b00    //0x002b00 ~ 0x002b7f
#define    RIUBASE_CACHE                0x002b80    //0x002b80 ~ 0x002bbf
#define    RIUBASE_XDMIU                0x002bc0    //0x002bc0 ~ 0x002bff
#define    RIUBASE_WDT                  0x003000    //0x003000 ~ 0x00301f
#define    RIUBASE_TIMER0               0x003020    //0x003020 ~ 0x00303f
#define    RIUBASE_TIMER1               0x003040    //0x003040 ~ 0x00305f
#define    RIUBASE_SEC_KEY              0x003100    //0x003100 ~ 0x0031ff
#define    RIUBASE_DID_KEY              0x003800    //0x003800 ~ 0x0038ff
#define    RIUBASE_REG_PIU_MISC_0       0x003c00    //0x003c00 ~ 0x003cff
#define    RIUBASE_IR                   0x003d00    //0x003d00 ~ 0x003dff

#endif  //_C_PM_RIUBASE_H_
