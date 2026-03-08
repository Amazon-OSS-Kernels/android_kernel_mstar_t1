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

#ifndef _C_VDMCU_RIUBASE_H_
#define _C_VDMCU_RIUBASE_H_

#define    RIUBASE_M2_RIU_DBG          0x000100    //0x000100 ~ 0x0001ff
#define    RIUBASE_VDMCU51             0x001000    //0x001000 ~ 0x0010ff
#define    RIUBASE_VD0                 0x001100    //0x001100 ~ 0x0011ff
#define    RIUBASE_VD1                 0x001200    //0x001200 ~ 0x0012ff
#define    RIUBASE_VDMCU51_IRQ         0x001b00    //0x001b00 ~ 0x001bff

#endif  //_C_VDMCU_RIUBASE_H_
