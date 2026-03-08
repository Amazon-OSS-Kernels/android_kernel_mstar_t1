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

#ifndef _CPE_AMBA_H_
#define _CPE_AMBA_H_

#include <MsTypes.h>

// CPE device's data Sturcture
struct cpe_dev {
  struct device_s dev;
  MS_U32  devid;  //device id
  MS_U32 uhcbase;         //device base address of memory mapping I/O
  MS_U32 utmibase; // new
  MS_U32 usbcbase; // new
  cyg_uint64 dma_mask;
  MS_U8 intNum;
  char *bus_name;
  char *product_desc;
  struct list_head *pHubEvent;
};

#endif /* _CPE_AMBA_H_ */
