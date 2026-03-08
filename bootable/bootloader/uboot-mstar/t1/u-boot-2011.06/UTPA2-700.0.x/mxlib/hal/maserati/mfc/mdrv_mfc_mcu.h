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
/******************************************************************************

 [Module Name]: MsMcu.h
 [Date]:        17-Aug-2005
 [Comment]:
   Mcu control definition.
 [Reversion History]:
*******************************************************************************/
#ifndef _MDRV_MFC_MCU_H_
#define _MDRV_MFC_MCU_H_

#define ISR_EXT_TIMER0_INT		1
#define ISR_EXT_TIMER1_INT		1

void MDrv_MFC_McuWatchDogClear(void);
void MDrv_MFC_SetInterrupt(BOOL bCtrl);
void MDrv_MFC_McuInitialize(void);

#endif
