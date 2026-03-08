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
#ifndef _FPGA_DEF_H_
#define _FPGA_DEF_H_

// For PC-FPGA verification:
//		Choose one of following PC-FPGA interface.
#define _UDMA_
//#define _HIF_
//		Below mapping make driver codes transparent to different interfaces.

#if defined(_UDMA_)
	#include "../udma_lib/UDMAapi.h"
#elif defined(_HIF_)
	#include "../hif_lib/HIFapi.h"
#endif

#if defined(_UDMA_)
	//#include "UDMAapi.h"
	#define FPGA_Connect         UDMA_Connect
	#define FPGA_RIURead16       UDMA_RIURead16
	#define FPGA_RIUWrite16      UDMA_RIUWrite16
	#define FPGA_MIURead         UDMAOP_DMAGet
	#define FPGA_MIUWrite        UDMAOP_DMAPut
	#define FPGA_MIUReadFile     UDMAOP_DMAGetFile
	#define FPGA_MIUWriteFile    UDMAOP_DMAPutFile
#elif defined(_HIF_)
	#include "HIFapi.h"
	#define FPGA_Connect         HIF_Connect
	#define FPGA_RIURead16       HIF_RIURead16
	#define FPGA_RIUWrite16      HIF_RIUWrite16
	#define FPGA_MIURead         HIF_MIURead
	#define FPGA_MIUWrite        HIF_MIUWrite
	#define FPGA_MIUReadFile     HIF_DMAGetFile
	#define FPGA_MIUWriteFile    HIF_DMAPutFile
	BOOL HIF_DMAGetFile_slow(char* filename, DWORD addr, int size, int endian, BYTE* pbuf);
#else
	#error "you have to define _UDMA_ or _HIF_!!!"
#endif

#endif // _FPGA_DEF_H_
