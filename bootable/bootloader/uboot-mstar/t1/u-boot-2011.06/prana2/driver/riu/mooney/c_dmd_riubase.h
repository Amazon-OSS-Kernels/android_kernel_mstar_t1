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
//***                (2 Byte address [23:0])               ***
//************************************************************

#ifndef _C_DMD_RIUBASE_H_
#define _C_DMD_RIUBASE_H_

#define RIUBASE_DMD_RIU_DBG         0x000100    //0x000100 ~ 0x0001ff
#define RIUBASE_DMDMCU51            0x001000    //0x001000 ~ 0x0010ff
#define RIUBASE_DUM0                0x001400    //0x001400 ~ 0x0014ff
#define RIUBASE_DUM1                0x001500    //0x001500 ~ 0x0015ff
#define RIUBASE_DUM2                0x001600    //0x001600 ~ 0x0016ff
#define RIUBASE_DUM3                0x001700    //0x001700 ~ 0x0017ff
#define RIUBASE_DMDMCU51_IRQ        0x001b00    //0x001b00 ~ 0x001bff
#define RIUBASE_DMD_SAWLESS         0x001e00    //0x001e00 ~ 0x001eff
#define RIUBASE_DMD_BACKEND         0x001f00    //0x001f00 ~ 0x001fff
#define RIUBASE_DMD_DEMOD_0         0x002000    //0x002000 ~ 0x0020ff
#define RIUBASE_DMD_DEMOD_1         0x002100    //0x002100 ~ 0x0021ff
#define RIUBASE_DMD_DEMOD_2         0x002200    //0x002200 ~ 0x0022ff
#define RIUBASE_DMD_DEMOD_3         0x002300    //0x002300 ~ 0x0023ff
#define RIUBASE_DMD_DEMOD_4         0x002400    //0x002400 ~ 0x0024ff
#define RIUBASE_DMD_DEMOD_5         0x002500    //0x002500 ~ 0x0025ff
#define RIUBASE_DMD_DEMOD_6         0x002600    //0x002600 ~ 0x0026ff
#define RIUBASE_DMD_DEMOD_7         0x002700    //0x002700 ~ 0x0027ff
#define RIUBASE_DMD_DEMOD_8         0x002800    //0x002800 ~ 0x0028ff
#define RIUBASE_DMD_DEMOD_9         0x002900    //0x002900 ~ 0x0029ff
#define RIUBASE_DMD_DEMOD_A         0x002a00    //0x002a00 ~ 0x002aff
#define RIUBASE_DMD_DEMOD_B         0x002b00    //0x002b00 ~ 0x002bff
#define RIUBASE_DMD_DEMOD_C         0x002c00    //0x002c00 ~ 0x002cff
#define RIUBASE_DMD_DEMOD_D         0x002d00    //0x002d00 ~ 0x002dff
#define RIUBASE_DMD_DEMOD_E         0x002e00    //0x002e00 ~ 0x002eff
#define RIUBASE_DMD_DEMOD_F         0x002f00    //0x002f00 ~ 0x002fff
#define RIUBASE_DEMOD_VIF_RF        0x003100    //0x003100 ~ 0x0031ff
#define RIUBASE_DEMOD_VIF_DBB1      0x003200    //0x003200 ~ 0x0032ff
#define RIUBASE_DEMOD_VIF_DBB2      0x003300    //0x003300 ~ 0x0033ff
#define RIUBASE_DEMOD_VIF_DBB3      0x003400    //0x003400 ~ 0x0034ff
#define RIUBASE_DMD_CLKGEN          0x003500    //0x003500 ~ 0x0035ff

#endif  //_C_DMD_RIUBASE_H_