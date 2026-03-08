/**
* Copyright (c) 2006 – 2016 MStar Semiconductor, Inc.
* This program is free software. You can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation; either version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

#ifndef __CUSRAWIO_H__
#define __CUSRAWIO_H__


#define UNNECESSARY_PARTITION ""
#define UNNECESSARY_VOLUME ""

#define SPI_DEFAULT_PARTITION UNNECESSARY_PARTITION
#define SPI_DEFAULT_VOLUME UNNECESSARY_VOLUME

#define NAND_DEFAULT_PARTITION "UBILD"
#define NAND_DEFAULT_VOLUME "MPOOL"

#define MMC_DEFAULT_PARTITION "MPOOL"
#define MMC_DEFAULT_VOLUME UNNECESSARY_VOLUME


#if defined (CONFIG_MBOOT_IN_SPI_FLASH)
#define FLASH_MBOOT_TARGET  E_RAW_DATA_IN_SPI
#define FLASH_MBOOT_PARTITION SPI_DEFAULT_PARTITION
#define FLASH_MBOOT_VOLUME SPI_DEFAULT_VOLUME
#elif defined (CONFIG_MBOOT_IN_NAND_FLASH)
#define FLASH_MBOOT_TARGET  E_RAW_DATA_IN_NAND
#define FLASH_MBOOT_PARTITION "MBOOT"
#define FLASH_MBOOT_VOLUME UNNECESSARY_PARTITION
#elif defined  (CONFIG_MBOOT_IN_MMC_FLASH)
#define FLASH_MBOOT_TARGET  E_RAW_DATA_IN_MMC
#define FLASH_MBOOT_PARTITION "MBOOT"
#define FLASH_MBOOT_VOLUME MMC_DEFAULT_VOLUME
#else
#error "please set the correct Mboot storage!!\n"
#endif



#endif //__CUSRAWIO_H__

