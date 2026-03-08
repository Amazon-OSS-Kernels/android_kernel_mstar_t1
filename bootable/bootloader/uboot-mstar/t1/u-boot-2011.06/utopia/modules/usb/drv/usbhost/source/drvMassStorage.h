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
///////////////////////////////////////////////////////////////////////////////
//
//	File name: drvMassStorage.H
//	Version: 1.0
//	Date: 2003/7/13
//
//	Author:
//	Email:
//	Phone: (03) 578-7888
//	Company: Faraday Tech. Corp.
///////////////////////////////////////////////////////////////////////////////

#ifndef MassStorage_H

#define MassStorage_H



//=================== 2.Define Data Type  ================================================================
//========================================================================================================
#define HOST20_MSCD_CSW_Status_PASS        0x00
#define HOST20_MSCD_CSW_Status_FAIL        0x01
#define HOST20_MSCD_CSW_Status_PASE_ERROR  0x02
#define HOST20_MSCD_CSW_Status_STALL       0x03
#define HOST20_MSCD_CSW_Status_NOT_VALID   0x04
#define HOST20_MSCD_DEVICE_FATAL_ERROR	0x05
//=================== 3.Structure Definition =============================================================
//========================================================================================================

//=================== 4.Extern Function Definition =======================================================
//========================================================================================================
extern BOOLEAN Host20_MSCD_usb_stor_control_thread(struct us_data *psMassStorage);
extern UINT8 Host20_MSCD_GetMaxLUN(void);
//=================== 5.Extern Variable Definition =======================================================
//========================================================================================================






#endif

