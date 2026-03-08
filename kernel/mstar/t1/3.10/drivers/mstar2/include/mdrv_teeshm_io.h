/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
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
///
/// @file   mdrv_mpool_io.h
/// @brief  Memory Pool  Driver IO Interface
/// @author MStar Semiconductor Inc.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MDRV_TEESHM_IO_H_
#define _MDRV_TEESHM_IO_H_

/* Use 'S' as magic number(0x53) */
#define TEESHM_IOC_MAGIC                'S'

/* <--- 8_bit --->  <--- 8_bit --->  <--- 8_bit --->  <--- 8_bit ---> */
/*     DIR(R/W)         PARA_SIZE        TYPE(0x53)       NR(0x00~0x05) */
#define TEESHM_IOC_RESOURCE_CREATE			_IOWR(TEESHM_IOC_MAGIC, 0x00, DrvTeeShmResourceCreate)			// having input and output, using IOWR,		TEESHM_IOC_RESOURCE_CREATE is0xC0105300
#define TEESHM_IOC_RESOURCE_DESTROY			_IOW(TEESHM_IOC_MAGIC, 0x01, DrvTeeShmResourceDestroy)			// only input, using _IOW
#define TEESHM_IOC_RESOURCE_OBTAIN			_IOW(TEESHM_IOC_MAGIC, 0x02, DrvTeeShmResourceObtain)
#define TEESHM_IOC_RESOURCE_RELEASE			_IOW(TEESHM_IOC_MAGIC, 0x03, DrvTeeShmResourceRelease)
#define TEESHM_IOC_RESOURCE_MAP				_IOWR(TEESHM_IOC_MAGIC, 0x04, DrvTeeShmResourceMap)				//											TEESHM_IOC_RESOURCE_MAP is 0xC0105304
#define TEESHM_IOC_RESOURCE_UNMAP			_IOW(TEESHM_IOC_MAGIC, 0x05, DrvTeeShmResourceUnmap)			//											TEESHM_IOC_RESOURCE_UNMAP is 0x40105305

/* compat ioctl */
#ifdef CONFIG_COMPAT
#define COMPAT_TEESHM_IOC_RESOURCE_CREATE		_IOWR(TEESHM_IOC_MAGIC, 0x00, DrvTeeShmResourceCreate_32)
#define COMPAT_TEESHM_IOC_RESOURCE_MAP			_IOWR(TEESHM_IOC_MAGIC, 0x04, DrvTeeShmResourceMap_32)
#define COMPAT_TEESHM_IOC_RESOURCE_UNMAP		_IOW(TEESHM_IOC_MAGIC, 0x05, DrvTeeShmResourceUnmap_32)
#endif

#endif
