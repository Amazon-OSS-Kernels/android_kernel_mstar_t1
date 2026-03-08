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

#ifndef _CPE_AMBA_H_
#define _CPE_AMBA_H_

#include "MsTypes.h"

// CPE device's data Sturcture
struct cpe_dev {  
  struct device_s dev;
  U32  devid;  //device id
  U32 mapbase;         //device base address of memory mapping I/O
  U32 utmibase; // new
  U32 usbcbase; // new
  //U32  irq[6];
  cyg_uint64 dma_mask;
  U8 intNum;
}; 


struct cpe_driver {
  struct device_driver  drv;
  U32    devid;
};

//extern int AMBA_match(struct device_s * dev, struct device_driver * drv);
extern struct bus_type CPE_AMBA_bus_type;
extern struct bus_type CPE_AMBA_bus_type_Port2;
extern struct bus_type CPE_AMBA_bus_type_EX;

#endif /* _CPE_AMBA_H_ */
